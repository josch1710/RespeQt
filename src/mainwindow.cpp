/*
 * mainwindow.cpp
 *
 * Copyright 2015, 2017 Joseph Zatarski
 * Copyright 2016 TheMontezuma
 * Copyright 2017, 2020 josch1710
 * Copyright 2017 blind
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "drivewidget.h"
#include "folderimage.h"
#include "pclink.h"
#include "rcl.h"
#include "smartdevice.h"
#include "autobootdialog.h"
#include "autoboot.h"
#include "cassettedialog.h"
#include "bootoptionsdialog.h"
#include "logdisplaydialog.h"
#include "infowidget.h"
#include "printerwidget.h"
#include "printers/printerfactory.h"
//#include "printers/outputfactory.h"
#include "printers/printers.h"
//#include "printers/outputs.h"
#include "respeqtsettings.h"

#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QScrollBar>
#include <QTranslator>
#include <QMessageBox>
#include <QWidget>
#include <QDrag>
#include <QtDebug>
#include <QDesktopWidget>
#include <QFont>
#include <QPrintDialog>
#include <QPrinter>
#include <QToolButton>
#include <QHBoxLayout>
#include <QPrinterInfo>
#include <typeinfo>
#include <memory>

#include "atarifilesystem.h"
#include "miscutils.h"

#include <QFontDatabase>

#ifndef QT_NO_QDEBUG
#include "siorecorder.h"
#endif

static QFile *logFile;
static QMutex *logMutex;

QString g_exefileName;
static QString g_rclFileName;
QString g_respeQtAppPath;
static QRect g_savedGeometry;
bool g_disablePicoHiSpeed;
static bool g_D9DOVisible = true;
bool g_miniMode = false;
static bool g_shadeMode = false;
SimpleDiskImage *g_translator = nullptr;
//static int g_savedWidth;

MainWindow* MainWindow::sInstance{nullptr};

// ****************************** END OF GLOBALS ************************************//

// Displayed only in debug mode    "!d"
// Unimportant     (gray)          "!u"
// Normal          (black)         "!n"
// Important       (blue)          "!i"
// Warning         (brown)         "!w"
// Error           (red)           "!e"
void MainWindow::logMessageOutput(QtMsgType type, const QMessageLogContext &/*context*/, const QString &msg)
{
    logMutex->lock();
    logFile->write(QString::number((quint64)QThread::currentThreadId(), 16).toLatin1());
    switch (type) {
#if QT_VERSION >= 0x050500
        case QtInfoMsg:
            logFile->write(": [Info]    ");
            break;
#endif
        case QtDebugMsg:
            logFile->write(": [Debug]    ");
            break;
        case QtWarningMsg:
            logFile->write(": [Warning]  ");
            break;
        case QtCriticalMsg:
            logFile->write(": [Critical] ");
            break;
        case QtFatalMsg:
            logFile->write(": [Fatal]    ");
            break;
    }
    QByteArray localMsg = msg.toLocal8Bit();
    QByteArray displayMsg = localMsg.mid(3);
    logFile->write(displayMsg);
    logFile->write("\n");
    if (type == QtFatalMsg) {
        logFile->close();
        abort();
    }
    logMutex->unlock();

    if (msg[0] == '!') {
#ifdef QT_NO_DEBUG
        if (msg[1] == 'd') {
            return;
        }
#endif
        // TODO Should be signal-slot
        sInstance->doLogMessage(localMsg.at(1), displayMsg);
    }
}

void MainWindow::doLogMessage(int type, const QString &msg)
{
    emit logMessage(type, msg);
}

MainWindow::MainWindow()
    : QMainWindow(nullptr), ui(new Ui::MainWindow),
      isClosing(false)
 {
    // Make the main window delete when closed.
    setAttribute(Qt::WA_DeleteOnClose, true);

    /* Setup the logging system */
    sInstance = this;
    g_respeQtAppPath = QCoreApplication::applicationDirPath();
    g_disablePicoHiSpeed = false;
    logFile = new QFile(QDir::temp().absoluteFilePath("respeqt.log"));
    logFile->open(QFile::WriteOnly | QFile::Truncate | QFile::Unbuffered | QFile::Text);
    logMutex = new QMutex();
    connect(this, &MainWindow::logMessage, this, &MainWindow::uiMessage, Qt::QueuedConnection);
    qInstallMessageHandler(MainWindow::logMessageOutput);
    qDebug() << "!d" << tr("RespeQt started at %1.").arg(QDateTime::currentDateTime().toString());

    logWindow_ = nullptr;

    /* Remove old temporaries */
    QDir tempDir = QDir::temp();
    QStringList filters;
    filters << "respeqt-*";
    QFileInfoList list = tempDir.entryInfoList(filters, QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    foreach(QFileInfo file, list) {
        deltree(file.absoluteFilePath());
    }

    QCoreApplication::setOrganizationName("ZeeSoft");
    QCoreApplication::setOrganizationDomain("https://github.com/jzatarski/RespeQt");
    QCoreApplication::setApplicationName("RespeQt");

    /* Load translators */
    loadTranslators();

#ifdef Q_OS_MAC
    /// This HAS to be executed before setupUi
    auto noNative = !(RespeqtSettings::instance()->nativeMenu());
    QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, noNative);
#endif

    /* Setup UI */
    ui->setupUi(this);

    connectUISignal();

    /* Setup the printer factory */
    auto pfactory = Printers::PrinterFactory::instance();
    pfactory->registerPrinter<Printers::Atari1020>(Printers::Atari1020::typeName());
    pfactory->registerPrinter<Printers::Atari1025>(Printers::Atari1025::typeName());
    pfactory->registerPrinter<Printers::Atari1027>(Printers::Atari1027::typeName());
    pfactory->registerPrinter<Printers::Atari1029>(Printers::Atari1029::typeName());
    //pfactory->registerPrinter<Printers::Passthrough>(Printers::Passthrough::typeName());

    /* Add QActions for most recent */
    for( int i = 0; i < NUM_RECENT_FILES; ++i ) {
        auto recentAction = new QAction(this);
        connect(recentAction, &QAction::triggered, this, &MainWindow::openRecent);
        recentFilesActions_.append(recentAction);
    }

    auto diskMenu = (QWidget*)menuBar()->children().at(1);

    diskMenu->addActions( recentFilesActions_ );

    /* Initialize diskWidgets array and tool button actions */
    createDeviceWidgets();

    /* Parse command line arguments:
      arg(1): session file (xxxxxxxx.respeqt)   */

    QStringList RespeQtArgs = QCoreApplication::arguments();
    g_sessionFile = g_sessionFilePath = "";
    if (RespeQtArgs.size() > 1) {
       QFile sess;
       QString s = QDir::separator();             //
       int i = RespeQtArgs.at(1).lastIndexOf(s);   //
       if (i != -1) {
           i++;
           g_sessionFile = RespeQtArgs.at(1).right(RespeQtArgs.at(1).size() - i);
           g_sessionFilePath = RespeQtArgs.at(1).left(i);
           g_sessionFilePath = QDir::fromNativeSeparators(g_sessionFilePath);
           sess.setFileName(g_sessionFilePath+g_sessionFile);
           if (!sess.exists()) {
               QMessageBox::question(this, tr("Session file error"),
               tr("Requested session file not found in the given directory path or the path is incorrect. RespeQt will continue with default session configuration."), QMessageBox::Ok);
               g_sessionFile = g_sessionFilePath = "";
           }
       } else {
           if (RespeQtArgs.at(1) != "") {
                g_sessionFile = RespeQtArgs.at(1);
                g_sessionFilePath = QDir::currentPath();
                sess.setFileName(g_sessionFile);
                if (!sess.exists()) {
                    QMessageBox::question(this, tr("Session file error"),
                    tr("Requested session file not found in the application's current directory path\n (No path was specified). RespeQt will continue with default session configuration."), QMessageBox::Ok);
                    g_sessionFile = g_sessionFilePath = "";
                }
            }
        }
    }
    // Pass Session file name, path and MainWindow title to RespeQtSettings //
    RespeqtSettings::instance()->setSessionFile(g_sessionFile, g_sessionFilePath);
    RespeqtSettings::instance()->setMainWindowTitle(g_mainWindowTitle);

    // Display Session name, and restore session parameters if session file was specified //
    g_mainWindowTitle = tr("RespeQt - Atari Serial Peripheral Emulator for Qt");
    if (g_sessionFile != "") {
        setWindowTitle(g_mainWindowTitle + tr(" -- Session: ") + g_sessionFile);
        RespeqtSettings::instance()->loadSessionFromFile(g_sessionFilePath+g_sessionFile);
    } else {
        setWindowTitle(g_mainWindowTitle);
    }
    setGeometry(RespeqtSettings::instance()->lastHorizontalPos(), RespeqtSettings::instance()->lastVerticalPos(), RespeqtSettings::instance()->lastWidth(), RespeqtSettings::instance()->lastHeight());

    /* Setup status bar */
    speedLabel = new QLabel(this);
    onOffLabel = new QLabel(this);
    prtOnOffLabel = new QLabel(this);
    netLabel = new QLabel(this);
    clearMessagesLabel = new QLabel(this);
    speedLabel->setText(tr("19200 bits/sec"));
    onOffLabel->setMinimumWidth(21);
    prtOnOffLabel->setMinimumWidth(18);
    netLabel->setMinimumWidth(18);

    clearMessagesLabel->setMinimumWidth(21);
    clearMessagesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/page_white_c.png").pixmap(16, 16, QIcon::Normal));
    clearMessagesLabel->setToolTip(tr("Clear messages"));
    clearMessagesLabel->setStatusTip(clearMessagesLabel->toolTip());

    speedLabel->setMinimumWidth(80);

    limitEntriesLabel = new QLabel(this);
    limitEntriesLabel->setMinimumWidth(21);
    limitEntriesLabel->setToolTip(tr("Should the file entry limit be 64."));
    limitEntriesLabel->setStatusTip(limitEntriesLabel->toolTip());
    if (RespeqtSettings::instance()->limitFileEntries())
        limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock.png").pixmap(16, 16, QIcon::Normal));
    else
        limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock_open.png").pixmap(16, 16, QIcon::Normal));

    ui->statusBar->addPermanentWidget(speedLabel);
    ui->statusBar->addPermanentWidget(onOffLabel);
    ui->statusBar->addPermanentWidget(prtOnOffLabel);
    ui->statusBar->addPermanentWidget(netLabel);
    ui->statusBar->addPermanentWidget(clearMessagesLabel);
    ui->statusBar->addPermanentWidget(limitEntriesLabel);

    ui->textEdit->installEventFilter(this);
    changeFonts();
    g_D9DOVisible =  RespeqtSettings::instance()->D9DOVisible();
    showHideDrives();

    /* Connect to the network */
    QString netInterface;
    auto oNet = new Network();
    if (oNet->openConnection(netInterface)) {
        netLabel->setPixmap(QIcon(":/icons/oxygen-icons/16x16/actions/network_connect.png").pixmap(16, 16, QIcon::Normal));
        netLabel->setToolTip(tr("Connected to the network via: ") +  netInterface);
        netLabel->setStatusTip(netLabel->toolTip());
    } else {
        netLabel->setPixmap(QIcon(":/icons/oxygen-icons/16x16/actions/network_disconnect.png").pixmap(16, 16, QIcon::Normal));
        netLabel->setToolTip(tr("No network connection"));
        netLabel->setStatusTip(netLabel->toolTip());

        //        QMessageBox::information(this,tr("Network connection cannot be opened"), tr("No network interface was found!"));
    }

    /* Connect SioWorker signals */
    sio = SioWorkerPtr::create();
    connect(sio.data(), &SioWorker::started, this, &MainWindow::sioStarted);
    connect(sio.data(), &SioWorker::finished, this, &MainWindow::sioFinished);
    connect(sio.data(), &SioWorker::statusChanged, this, &MainWindow::sioStatusChanged);
    shownFirstTime = true;
    sio->setAutoReconnect(RespeqtSettings::instance()->sioAutoReconnect());

    PCLINK* pclink = new PCLINK(sio);
    sio->installDevice(PCLINK_CDEVIC, pclink);

    /* Restore application state */
    for (int i = 0; i < DISK_COUNT; i++) {
        RespeqtSettings::ImageSettings is;
        is = RespeqtSettings::instance()->mountedImageSetting(i);
        mountFile(i, is.fileName, is.isWriteProtected);
    }
    updateRecentFileActions();

    setAcceptDrops(true);

    // SmartDevice (ApeTime + URL submit)
    SmartDevice *smart = new SmartDevice(sio);
    sio->installDevice(SMART_CDEVIC, smart);

    // RespeQt Client  //
    RCl *rcl = new RCl(sio);
    sio->installDevice(RESPEQT_CLIENT_CDEVIC, rcl);

    // Documentation Display
    docDisplayWindow = new DocDisplayWindow();

    connect(docDisplayWindow, &DocDisplayWindow::closed, this, &MainWindow::docDisplayWindowClosed);

    setUpPrinterEmulationWidgets(RespeqtSettings::instance()->printerEmulation());

    untitledName = 0;

#ifndef Q_OS_MACX
    connect(&trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    trayIcon.setIcon(windowIcon());
#endif

    // TODO Testing.
    // Connections needed for remotely mounting a disk image & Toggle Auto-Commit //
    connect (rcl, &RCl::findNewSlot, this, &MainWindow::firstEmptyDiskSlot);
    connect (this, &MainWindow::newSlot, rcl, &RCl::gotNewSlot);
    connect (rcl, &RCl::mountFile, this, &MainWindow::mountFileWithDefaultProtection);
    connect (this, &MainWindow::fileMounted, rcl, &RCl::fileMounted);
    connect (rcl, &RCl::toggleAutoCommit, this, &MainWindow::autoCommit);
    connect (rcl, &RCl::toggleHappy, this, &MainWindow::happy);
    connect (rcl, &RCl::toggleChip, this, &MainWindow::chip);
    connect (rcl, &RCl::bootExe, this, &MainWindow::bootExeTriggered);
}

MainWindow::~MainWindow()
{
    if (ui->actionStartEmulation->isChecked()) {
        ui->actionStartEmulation->trigger();
    }

    qDebug() << "!d" << tr("RespeQt stopped at %1.").arg(QDateTime::currentDateTime().toString());
    qInstallMessageHandler(0);
    delete logMutex;
    delete logFile;
    delete ui;

}

void MainWindow::createDeviceWidgets()
{

    ui->rightColumn->setAlignment(Qt::AlignTop);

    for (int i = 0; i < DISK_COUNT; i++) {      //
        auto driveWidget = new DriveWidget(i);

        if (i<8) {
            ui->leftColumn->addWidget( driveWidget );
        } else {
            ui->rightColumn->addWidget( driveWidget );
        }

        driveWidget->setup(RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
        diskWidgets[i] = driveWidget;

        // connect signals to slots
        connect(driveWidget, &DriveWidget::actionMountDisk, this, &MainWindow::mountDiskTriggered);
        connect(driveWidget, &DriveWidget::actionMountFolder, this, &MainWindow::mountFolderTriggered);
        connect(driveWidget, &DriveWidget::actionAutoSave, this, &MainWindow::autoSaveTriggered);
        connect(driveWidget, &DriveWidget::actionEject, this, &MainWindow::ejectTriggered);
        connect(driveWidget, &DriveWidget::actionNextSide, this, &MainWindow::nextSideTriggered);
        connect(driveWidget, &DriveWidget::actionToggleHappy, this, &MainWindow::happyToggled);
        connect(driveWidget, &DriveWidget::actionToggleChip, this, &MainWindow::chipToggled);
        connect(driveWidget, &DriveWidget::actionToggleOSB, this, &MainWindow::OSBToggled);
        connect(driveWidget, &DriveWidget::actionToolDisk, this, &MainWindow::toolDiskTriggered);
        connect(driveWidget, &DriveWidget::actionWriteProtect, this, &MainWindow::protectTriggered);
        connect(driveWidget, &DriveWidget::actionEditDisk, this, &MainWindow::editDiskTriggered);
        connect(driveWidget, &DriveWidget::actionSave, this, &MainWindow::saveTriggered);
        connect(driveWidget, &DriveWidget::actionRevert, this, &MainWindow::revertTriggered);
        connect(driveWidget, &DriveWidget::actionSaveAs, this, &MainWindow::saveAsTriggered);
        connect(driveWidget, &DriveWidget::actionBootOptions, this, &MainWindow::bootOptionTriggered);
        connect(this, &MainWindow::setFont, driveWidget, &DriveWidget::setFont);
    }

    for (int i = 0; i < PRINTER_COUNT; i++) {      //
        auto printerWidget = new PrinterWidget(i);
        if (i<2) {
            ui->leftColumn2->addWidget( printerWidget );
        } else {
            ui->rightColumn2->addWidget( printerWidget );
        }
        printerWidgets[i] = printerWidget;
   }

   changeFonts();
}

 void MainWindow::mousePressEvent(QMouseEvent *event)
 {
     auto slot{containingDiskSlot(event->pos())};

     if (event->button() == Qt::LeftButton
         && slot >= 0) {

         auto drag = new QDrag((QWidget*)this);
         auto mimeData = new QMimeData;

         mimeData->setData("application/x-respeqt-disk-image", QByteArray(1, slot));
         drag->setMimeData(mimeData);

         drag->exec();
     }

     if (event->button() == Qt::LeftButton && onOffLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
         ui->actionStartEmulation->trigger();
     }

     if (event->button() == Qt::LeftButton && prtOnOffLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
         ui->actionPrinterEmulation->trigger();     //
     }
     if (event->button() == Qt::LeftButton && clearMessagesLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
         ui->textEdit->clear();
         emit sendLogText("");
     }
     if (event->button() == Qt::LeftButton && !speedLabel->isHidden() && speedLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
        ui->actionOptions->trigger();
     }
     if (event->button() == Qt::LeftButton && limitEntriesLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
        ui->actionLimitFileEntries->trigger();
     }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    auto i{containingDiskSlot(event->pos())};
    if (i >= 0 && (event->mimeData()->hasUrls() ||
            event->mimeData()->hasFormat("application/x-respeqt-disk-image"))
    )
        event->setDropAction(event->proposedAction());
    else
        event->setDropAction(Qt::IgnoreAction);

    event->accept();
    for (int j = 0; j < DISK_COUNT; j++) { //
        if (i == j) {
            diskWidgets[j]->setFrameShadow(QFrame::Sunken);
        } else {
            diskWidgets[j]->setFrameShadow(QFrame::Raised);
        }
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    auto i{containingDiskSlot(event->pos())};
    if (i >= 0 && (event->mimeData()->hasUrls() ||
            event->mimeData()->hasFormat("application/x-respeqt-disk-image"))
    )
        event->setDropAction(event->proposedAction());
    else
        event->setDropAction(Qt::IgnoreAction);

    event->accept();

    for (int j = 0; j < DISK_COUNT; j++) { //
        if (i == j) {
            diskWidgets[j]->setFrameShadow(QFrame::Sunken);
        } else {
            diskWidgets[j]->setFrameShadow(QFrame::Raised);
        }
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    for (int j = 0; j < DISK_COUNT; j++) { //
        diskWidgets[j]->setFrameShadow(QFrame::Raised);
    }
    auto slot{containingDiskSlot(event->pos())};
    if (!(event->mimeData()->hasUrls() ||
          event->mimeData()->hasFormat("application/x-respeqt-disk-image")) ||
          slot < 0) {
        return;
    }

    if (event->mimeData()->hasFormat("application/x-respeqt-disk-image")) {
        int source = event->mimeData()->data("application/x-respeqt-disk-image").at(0);
        if (slot != source) {
            sio->swapDevices(slot + DISK_BASE_CDEVIC, source + DISK_BASE_CDEVIC);
            RespeqtSettings::instance()->swapImages(slot, source);

            auto pclink = reinterpret_cast<PCLINK*>(sio->getDevice(PCLINK_CDEVIC));
            if(pclink->hasLink(slot+1) || pclink->hasLink(source+1))
            {
                sio->uninstallDevice(PCLINK_CDEVIC);
                pclink->swapLinks(slot+1,source+1);
                sio->installDevice(PCLINK_CDEVIC,pclink);
            }
            qDebug() << "!n" << tr("Swapped disk %1 with disk %2.").arg(slot + 1).arg(source + 1);
        }
        return;
    }

    QStringList files;
    foreach (QUrl url, event->mimeData()->urls()) {
        if (!url.toLocalFile().isEmpty()) {
            files.append(url.toLocalFile());
        }
    }
    if (files.isEmpty()) {
        return;
    }

    FileTypes::FileType type = FileTypes::getFileType(files.at(0));

    if (type == FileTypes::Xex) {
        g_exefileName = files.at(0);  //
        bootExe(files.at(0));
        return;
    }

    if (type == FileTypes::Cas) {
        bool restart;
        restart = ui->actionStartEmulation->isChecked();
        if (restart) {
            ui->actionStartEmulation->trigger();
            sio->wait();
            qApp->processEvents();
        }

        auto dlg = new CassetteDialog(this, files.at(0));
        dlg->exec();
        delete dlg;

        if (restart) {
            ui->actionStartEmulation->trigger();
        }
        return;
    }

    mountFileWithDefaultProtection(slot, files[0]);
    files.removeAt(0);
    while (!files.isEmpty() && (slot = firstEmptyDiskSlot(slot, false)) >= 0) {
        mountFileWithDefaultProtection(slot, files[0]);
        files.removeAt(0);
    }
    slot = 0;
    while (!files.isEmpty() && (slot = firstEmptyDiskSlot(slot, false)) >= 0) {
        mountFileWithDefaultProtection(slot, files[0]);
        files.removeAt(0);
    }
    foreach(QString file, files) {
        qCritical() << "!e" << tr("Cannot mount '%1': No empty disk slots.").arg(file);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isClosing)
        return;
    isClosing = true;

    // Save various session settings  //
    if (RespeqtSettings::instance()->saveWindowsPos()) {
        if (g_miniMode) {
            saveMiniWindowGeometry();
        } else {
            saveWindowGeometry();
        }
    }
    if (g_sessionFile != "")
        RespeqtSettings::instance()->saveSessionToFile(g_sessionFilePath + "/" + g_sessionFile);
    RespeqtSettings::instance()->setD9DOVisible(g_D9DOVisible);
    bool wasRunning = ui->actionStartEmulation->isChecked();
    QMessageBox::StandardButton answer = QMessageBox::No;

    if (wasRunning) {
        ui->actionStartEmulation->trigger();
    }

    int toBeSaved = 0;

    for (int i = 0; i < DISK_COUNT; i++) {      //
        auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(i + DISK_BASE_CDEVIC));
        if (img && img->isModified()) {
            toBeSaved++;
        }
    }

    for (int i = 0; i < DISK_COUNT; i++) {      //
        auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(i + DISK_BASE_CDEVIC));
        if (img && img->isModified()) {
            toBeSaved--;
            answer = saveImageWhenClosing(i, answer, toBeSaved);
            if (answer == QMessageBox::NoToAll) {
                break;
            }
            if (answer == QMessageBox::Cancel) {
                if (wasRunning) {
                    ui->actionStartEmulation->trigger();
                }
                event->ignore();
                return;
            }
        }
    }

    //close any disk edit dialogs we have open
    for (int i = 0; i < DISK_COUNT; i++) {
        auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(i + DISK_BASE_CDEVIC));
        if (img && img->editDialog()) img->editDialog()->close();
    }

    //
    delete docDisplayWindow;
    docDisplayWindow = nullptr;

    for (int i = DISK_BASE_CDEVIC; i < (DISK_BASE_CDEVIC+DISK_COUNT); i++) {
        auto s = qobject_cast <SimpleDiskImage*> (sio->getDevice(i));
        if (s) {
            s->close();
        }
    }

    for(int i = 0; i < PRINTER_COUNT; i++)
    {
        auto printerWidget = printerWidgets[i];
        if (printerWidget)
            printerWidget->disconnectPrinter();
    }

    event->accept();
}

void MainWindow::hideEvent(QHideEvent *event)
{
#ifndef Q_OS_MACX
    if (RespeqtSettings::instance()->minimizeToTray()) {
        trayIcon.show();
        oldWindowFlags = windowFlags();
        oldWindowStates = windowState();
        setWindowFlags(Qt::Widget);
        hide();
        event->ignore();
        return;
    }
#endif
    QMainWindow::hideEvent(event);
}

void MainWindow::show()
{
    QMainWindow::show();
    if (shownFirstTime) {
        shownFirstTime = false; // Reset the flag

        /* Open options dialog if it's the first time */
        if (RespeqtSettings::instance()->isFirstTime()) {
            if (QMessageBox::Yes == QMessageBox::question(this, tr("First run"),
                    tr("You are running RespeQt for the first time.\n\nDo you want to open the options dialog?"),
                    QMessageBox::Yes, QMessageBox::No)) {
                ui->actionOptions->trigger();
            }
        }
        qDebug() << "!d" << "Starting emulation";

        ui->actionStartEmulation->trigger();
    }
}

void MainWindow::enterEvent(QEvent *)
{
    if (g_miniMode && g_shadeMode) {
       setWindowOpacity(1.0);
    }
}

void MainWindow::leaveEvent(QEvent *)
{
    if (g_miniMode && g_shadeMode) {
       setWindowOpacity(0.25);
    }
}

bool MainWindow::eventFilter(QObject * /*obj*/, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        showLogWindowTriggered();
    }
    return false;
}

void MainWindow::showLogWindowTriggered()
{
    if (logWindow_ == nullptr)
    {
        logWindow_ = new LogDisplayDialog(this);
        int x, y, w, h;
        x = geometry().x();
        y = geometry().y();
        w = geometry().width();
        h = geometry().height();
        if (!g_miniMode) {
            logWindow_->setGeometry(x+w/1.9, y+30, logWindow_->geometry().width(), geometry().height());
        } else {
            logWindow_->setGeometry(x+20, y+60, w, h*2);
        }
        connect(this, &MainWindow::sendLogText, logWindow_, &LogDisplayDialog::getLogText);
        connect(this, &MainWindow::sendLogTextChange, logWindow_, &LogDisplayDialog::getLogTextChange);
        emit sendLogText(ui->textEdit->toHtml());
    }

    logWindow_->show();
}

void MainWindow::logChanged(QString text)
{
    emit sendLogTextChange(text);
}

void MainWindow::saveWindowGeometry()
{
    RespeqtSettings::instance()->setLastHorizontalPos(geometry().x());
    RespeqtSettings::instance()->setLastVerticalPos(geometry().y());
    RespeqtSettings::instance()->setLastWidth(geometry().width());
    RespeqtSettings::instance()->setLastHeight(geometry().height());
}

void MainWindow::saveMiniWindowGeometry()
{
    RespeqtSettings::instance()->setLastMiniHorizontalPos(geometry().x());
    RespeqtSettings::instance()->setLastMiniVerticalPos(geometry().y());
}

void MainWindow::toggleShadeTriggered()
{
    if (g_shadeMode) {
        setWindowFlags(Qt::WindowSystemMenuHint);
        setWindowOpacity(1.0);
        g_shadeMode = false;
        QMainWindow::show();
    } else {
        setWindowFlags(Qt::FramelessWindowHint);
        setWindowOpacity(0.25);
        g_shadeMode = true;
        QMainWindow::show();
    }
}

// Toggle Mini Mode //
void MainWindow::toggleMiniModeTriggered()
{
    g_miniMode = !g_miniMode;

    int i;
    for( i = 1; i < 8; ++i ) {
        diskWidgets[i]->setVisible( !g_miniMode );
    }

    for( ; i < DISK_COUNT; ++i ) {
        diskWidgets[i]->setVisible( !g_miniMode && g_D9DOVisible );
    }

    if(!g_miniMode) {
        /*if (g_D9DOVisible) {
            setMinimumWidth(688);
        } else
        {
            setMinimumWidth(344);
        }*/

        setMinimumHeight(426);
        setMaximumHeight(QWIDGETSIZE_MAX);
        ui->textEdit->setVisible(true);
        ui->actionHideShowDrives->setEnabled(true);
        saveMiniWindowGeometry();
        setGeometry(g_savedGeometry);
        setWindowOpacity(1.0);
        setWindowFlags(Qt::WindowSystemMenuHint);
        ui->actionToggleShade->setDisabled(true);
        QMainWindow::show();
        g_shadeMode = false;
    } else {
        g_savedGeometry = geometry();
        ui->textEdit->setVisible(false);
        setMinimumWidth(400);
        setMinimumHeight(100);
        setMaximumHeight(100);
        setGeometry(RespeqtSettings::instance()->lastMiniHorizontalPos(), RespeqtSettings::instance()->lastMiniVerticalPos(),
                    minimumWidth(), minimumHeight());
        ui->actionHideShowDrives->setDisabled(true);
        ui->actionToggleShade->setEnabled(true);
        if (RespeqtSettings::instance()->enableShade()) {
            setWindowOpacity(0.25);
            setWindowFlags(Qt::FramelessWindowHint);
            g_shadeMode = true;
        } else {
            g_shadeMode = false;
        }
        QMainWindow::show();
    }
}

void MainWindow::showHideDrives()
{
    for( int i = 8; i < DISK_COUNT; ++i ) {
        diskWidgets[i]->setVisible(g_D9DOVisible);
    }
    for (int i = 2; i < PRINTER_COUNT; ++i ) {
        printerWidgets[i]->setVisible(g_D9DOVisible);
    }

    // infoWidget->setVisible(g_D9DOVisible);

    if( g_D9DOVisible ) {
        ui->actionHideShowDrives->setText(QApplication::translate("MainWindow", "Hide drives D9-DO", 0));
        ui->actionHideShowDrives->setStatusTip(QApplication::translate("MainWindow", "Hide drives D9-DO", 0));
        ui->actionHideShowDrives->setIcon(QIcon(":/icons/silk-icons/icons/drive_add.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
        //setMinimumWidth(688);
    } else {
        ui->actionHideShowDrives->setText(QApplication::translate("MainWindow", "Show drives D9-DO", 0));
        ui->actionHideShowDrives->setStatusTip(QApplication::translate("MainWindow", "Show drives D9-DO", 0));
        ui->actionHideShowDrives->setIcon(QIcon(":/icons/silk-icons/icons/drive_delete.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
        //setMinimumWidth(344);
    }
}

// Toggle Hide/Show drives D9-DO  //
void MainWindow::hideShowTriggered()
{
    g_D9DOVisible = !g_D9DOVisible;
    g_miniMode = false;

    showHideDrives();

    //setGeometry(geometry().x(), geometry().y(), 0, geometry().height());
    saveWindowGeometry();
}

// Toggle printer Emulation ON/OFF //
void MainWindow::printerEmulationTriggered()
{
    if (RespeqtSettings::instance()->printerEmulation()) {
        setUpPrinterEmulationWidgets(false);
        RespeqtSettings::instance()->setPrinterEmulation(false);
        qWarning() << "!i" << tr("Printer emulation stopped.");
    } else {
        setUpPrinterEmulationWidgets(true);
        RespeqtSettings::instance()->setPrinterEmulation(true);
        qWarning() << "!i" << tr("Printer emulation started.");
    }
}

void MainWindow::setUpPrinterEmulationWidgets(bool enable)
{
    if (enable) {
        ui->actionPrinterEmulation->setText(QApplication::translate("MainWindow", "Stop printer emulation", 0));
        ui->actionPrinterEmulation->setStatusTip(QApplication::translate("MainWindow", "Stop printer emulation", 0));
        ui->actionPrinterEmulation->setIcon(QIcon(":/icons/silk-icons/icons/printer.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
        prtOnOffLabel->setPixmap(QIcon(":/icons/silk-icons/icons/printer.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
    } else {
        ui->actionPrinterEmulation->setText(QApplication::translate("MainWindow", "Start printer emulation", 0));
        ui->actionPrinterEmulation->setStatusTip(QApplication::translate("MainWindow", "Start printer emulation", 0));
        ui->actionPrinterEmulation->setIcon(QIcon(":/icons/silk-icons/icons/printer_delete.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
        prtOnOffLabel->setPixmap(QIcon(":/icons/silk-icons/icons/printer_delete.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
    }
    prtOnOffLabel->setToolTip(ui->actionPrinterEmulation->toolTip());
    prtOnOffLabel->setStatusTip(ui->actionPrinterEmulation->statusTip());
}

void MainWindow::startEmulationTriggered()
{
    if (ui->actionStartEmulation->isChecked()) {
        sio->start(QThread::TimeCriticalPriority);
    } else {
        sio->setPriority(QThread::NormalPriority);
        sio->wait();
        qApp->processEvents();
    }
}

void MainWindow::sioStarted()
{
    ui->actionStartEmulation->setText(tr("&Stop emulation"));
    ui->actionStartEmulation->setToolTip(tr("Stop SIO peripheral emulation"));
    ui->actionStartEmulation->setStatusTip(tr("Stop SIO peripheral emulation"));
    onOffLabel->setPixmap(ui->actionStartEmulation->icon().pixmap(16, QIcon::Normal, QIcon::On));
    onOffLabel->setToolTip(ui->actionStartEmulation->toolTip());
    onOffLabel->setStatusTip(ui->actionStartEmulation->statusTip());
    for (int i = 0; i < PRINTER_COUNT; i++) {
        printerWidgets[i]->setSioWorker(sio);
    }
}

void MainWindow::sioFinished()
{
    ui->actionStartEmulation->setText(tr("&Start emulation"));
    ui->actionStartEmulation->setToolTip(tr("Start SIO peripheral emulation"));
    ui->actionStartEmulation->setStatusTip(tr("Start SIO peripheral emulation"));
    ui->actionStartEmulation->setChecked(false);
    onOffLabel->setPixmap(ui->actionStartEmulation->icon().pixmap(16, QIcon::Normal, QIcon::Off));
    onOffLabel->setToolTip(ui->actionStartEmulation->toolTip());
    onOffLabel->setStatusTip(ui->actionStartEmulation->statusTip());
    speedLabel->hide();
    speedLabel->clear();
    qWarning() << "!i" << tr("Emulation stopped.");
}

void MainWindow::sioStatusChanged(QString status)
{
    speedLabel->setText(status);
    speedLabel->show();
    sio->setDisplayCommandName(RespeqtSettings::instance()->isCommandName());
}

void MainWindow::deviceStatusChanged(int deviceNo)
{
    if (deviceNo >= DISK_BASE_CDEVIC && deviceNo < (DISK_BASE_CDEVIC+DISK_COUNT)) { // 0x31 - 0x3E
        auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(deviceNo));

        DriveWidget *diskWidget = diskWidgets[deviceNo - DISK_BASE_CDEVIC];

        if (img) {

            // Show file name without the path and set toolTip & statusTip to show the path separately //
            QString filenamelabel;
            int i = -1;

            if (img->description() == tr("Folder image")) {
                i = img->originalFileName().lastIndexOf("\\"); // NOTE: This expects folder separators to be '\\'
            } else {
                i = img->originalFileName().lastIndexOf("/");
            }
            if (i == -1) {
                i = img->originalFileName().lastIndexOf("/");
            }
            if ((i != -1) || (img->originalFileName().mid(0, 14) == "Untitled image")) {
                filenamelabel = img->originalFileName().right(img->originalFileName().size() - ++i);
            } else {
                filenamelabel = "!!!!!!!!.!!!";
            }

            diskWidget->setLabelToolTips(img->originalFileName().left(i - 1),
                                         img->originalFileName(),
                                         img->description());

            bool enableEdit = img->editDialog() != 0;

            if (img->description() == tr("Folder image")) {
                diskWidget->showAsFolderMounted(filenamelabel, img->description(), enableEdit);
            } else {
                bool enableSave = false;

                if (img->isModified()) {
                    if (!diskWidget->isAutoSaveEnabled()) {    //
                        enableSave = true;
                    } else {
                        // Image is modified and autosave is checked, so save the image (no need to lock it)  //
                        bool saved;
                        saved = img->save();
                        if (!saved) {
                            int response = QMessageBox::question(this, tr("Save failed"),
                                            tr("'%1' cannot be saved, do you want to save the image with another name?").arg(img->originalFileName()),
                                            QMessageBox::Yes, QMessageBox::No);
                            if (response == QMessageBox::Yes) {
                                saveDiskAs(deviceNo);
                            }
                        }
                    }
                }
                diskWidget->showAsImageMounted(filenamelabel, img->description(), enableEdit, enableSave, img->isLeverOpen(), img->isHappyEnabled(), img->isChipOpen(),
                                               img->isTranslatorActive(), img->isToolDiskActive(), img->hasSeveralSides(), RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(),
                                               RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
            }

            img->setDisplayTransmission(RespeqtSettings::instance()->displayTransmission());
            img->setSpyMode(RespeqtSettings::instance()->isSpyMode());
            img->setDisassembleUploadedCode(RespeqtSettings::instance()->disassembleUploadedCode());
            img->setTranslatorAutomaticDetection(RespeqtSettings::instance()->translatorAutomaticDetection());
            img->setTranslatorDiskImagePath(RespeqtSettings::instance()->translatorDiskImagePath());
            img->setToolDiskImagePath(RespeqtSettings::instance()->toolDiskImagePath());
            img->setActivateChipModeWithTool(RespeqtSettings::instance()->activateChipModeWithTool());
            img->setActivateHappyModeWithTool(RespeqtSettings::instance()->activateHappyModeWithTool());
        } else {
            diskWidget->showAsEmpty(RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
        }
    }
    updateHighSpeed();
}

void MainWindow::updateHighSpeed()
{
    if (sio->port() != nullptr) {
        int nbChip = 0;
        for(int i = 0 ; i < DISK_COUNT; ++i ) {
            auto disk = qobject_cast <SimpleDiskImage*> (sio->getDevice(i + DISK_BASE_CDEVIC));
            Board *board = disk != nullptr ? disk->getBoardInfo() : nullptr;
            if ((board != nullptr) && (board->isChipOpen())) {
                nbChip++;
            }
        }
        if (nbChip > 0) {
            sio->port()->forceHighSpeed(52400);
        } else {
            sio->port()->forceHighSpeed(0);
        }
    }
}

void MainWindow::uiMessage(int t, QString message)
{
    if (message.at(0) == '"') {
        message.remove(0, 1);
    }
    if (message.at(message.count() - 1) == '"') {
        message.resize(message.count() - 1);
    }
    if (message.at(message.count() - 1) == '"') {
        message.resize(message.count() - 1);
    }

    // paragraph symbol is used to display the next characters, up to the end of line, with a fixed font
    int indexParagraph = message.indexOf("§");
    if (indexParagraph != -1) {
        message.remove(indexParagraph, 1);
    }

    if (message == lastMessage) {
        lastMessageRepeat++;
        message = QString("%1 [x%2]").arg(message).arg(lastMessageRepeat);
        ui->textEdit->moveCursor(QTextCursor::End);
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
    } else {
        lastMessage = message;
        lastMessageRepeat = 1;
    }

    ui->statusBar->showMessage(message, 3000);

    switch (t) {
        case 'd':
            message = QString("<span style='color:green'>%1</span>").arg(message);
            break;
        case 'u':
            if (indexParagraph != -1) {
                QString header = message.left(indexParagraph);
                QString fixed = message.right(message.count() - indexParagraph);
                message = QString("<span style='color:gray'>%1 <span style='font-family:courier,monospace'>%2</span></span>").arg(header).arg(fixed);
            }
            else {
                message = QString("<span style='color:gray'>%1</span>").arg(message);
            }
            break;
        case 'n':
            message = QString("<span style='color:black'>%1</span>").arg(message);
            break;
        case 'i':
            message = QString("<span style='color:blue'>%1</span>").arg(message);
            break;
        case 'w':
            message = QString("<span style='color:brown'>%1</span>").arg(message);
            break;
        case 'e':
            message = QString("<span style='color:red'>%1</span>").arg(message);
            break;
        default:
            message = QString("<span style='color:purple'>%1</span>").arg(message);
            break;
    }

    ui->textEdit->append(message);
    ui->textEdit->verticalScrollBar()->setSliderPosition(ui->textEdit->verticalScrollBar()->maximum());
    ui->textEdit->horizontalScrollBar()->setSliderPosition(ui->textEdit->horizontalScrollBar()->minimum());
    logChanged(message);
}

void MainWindow::showOptionsTriggered()
{
    auto restart = ui->actionStartEmulation->isChecked();
    if (restart) {
        ui->actionStartEmulation->trigger();
        sio->wait();
        qApp->processEvents();
    }
    OptionsDialog optionsDialog(this);
    optionsDialog.exec();

// Change drive slot description fonts
    changeFonts();

// load translators and retranslate
    loadTranslators();

// retranslate Designer Form
    ui->retranslateUi(this);

    setupDebugItems();

    if (RespeqtSettings::instance()->limitFileEntries()) {
        limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock.png").pixmap(16, 16, QIcon::Normal));
    }
    else {
        limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock_open.png").pixmap(16, 16, QIcon::Normal));
    }

    for (int i = DISK_BASE_CDEVIC; i < (DISK_BASE_CDEVIC+DISK_COUNT); i++) {    // 0x31 - 0x3E
        deviceStatusChanged(i);
    }
    sio->setAutoReconnect(RespeqtSettings::instance()->sioAutoReconnect());

    ui->actionStartEmulation->trigger();
}

void MainWindow::changeFonts()
{
    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    if (RespeqtSettings::instance()->useLargeFont()) {
        //QFont font("Arial Black", 9, QFont::Normal);
        font.setPointSize(9);
        emit setFont(font);
    } else {
        font.setPointSize(8);
        //QFont font("MS Shell Dlg 2,8", 8, QFont::Normal);
        emit setFont(font);
    }
 }

void MainWindow::showAboutTriggered()
{
    AboutDialog aboutDialog(this, VERSION);
    aboutDialog.exec();
}
//
void MainWindow::showDocumentationTriggered()
{
    if (ui->actionDocumentation->isChecked()) {
        docDisplayWindow->show();
    } else {
        docDisplayWindow->hide();
    }
}
//
void MainWindow::docDisplayWindowClosed()
{
     ui->actionDocumentation->setChecked(false);
}
// Restart emulation and re-translate following a session load //
void MainWindow::setSession()
{
    bool restart;
    restart = ui->actionStartEmulation->isChecked();
    if (restart) {
        ui->actionStartEmulation->trigger();
       sio->wait();
        qApp->processEvents();
    }

    // load translators and retranslate
    loadTranslators();
    ui->retranslateUi(this);
    for (int i = DISK_BASE_CDEVIC; i < (DISK_BASE_CDEVIC+DISK_COUNT); i++) {
        deviceStatusChanged(i);
    }

    ui->actionStartEmulation->trigger();
}



void MainWindow::openRecent()
{
    qDebug("open recent");
    auto action = qobject_cast<QAction*>(sender());
    if(action)
    {
        mountFileWithDefaultProtection(firstEmptyDiskSlot(), action->text());
    }
}

void MainWindow::updateRecentFileActions()
{
    for(int i = 0; i < NUM_RECENT_FILES; ++i)
    {
        QAction* action = this->recentFilesActions_[i];
        const RespeqtSettings::ImageSettings& image = RespeqtSettings::instance()->recentImageSetting(i);

        if(image.fileName != "" )
        {
            action->setVisible(true);
            action->setText(image.fileName);
        }
        else
        {
            action->setVisible(false);
        }
    }
}


bool MainWindow::ejectImage(int no, bool ask)
{
    auto pclink = reinterpret_cast<PCLINK*>(sio->getDevice(PCLINK_CDEVIC));
    if(pclink->hasLink(no+1))
    {
        sio->uninstallDevice(PCLINK_CDEVIC);
        pclink->resetLink(no+1);
        sio->installDevice(PCLINK_CDEVIC,pclink);
    }

    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));

    if (ask && img && img->isModified()) {
        QMessageBox::StandardButton answer;
        answer = saveImageWhenClosing(no, QMessageBox::No, 0);
        if (answer == QMessageBox::Cancel) {
            return false;
        }
    }

    sio->uninstallDevice(no + DISK_BASE_CDEVIC);
    if (img) {
        delete img;
        diskWidgets[no]->showAsEmpty(RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
        RespeqtSettings::instance()->unmountImage(no);
        updateRecentFileActions();
        deviceStatusChanged(no + DISK_BASE_CDEVIC);
        qDebug() << "!n" << tr("Unmounted disk %1").arg(no + 1);
    }
    return true;
}

int MainWindow::containingDiskSlot(const QPoint &point)
{
    int i;
    QPoint distance = centralWidget()->geometry().topLeft();
    for (i=0; i < DISK_COUNT; i++) {    //
        QRect rect = diskWidgets[i]->geometry().translated(distance);
        if (rect.contains(point)) {
            break;
        }
    }
    if (i > DISK_COUNT-1) {   //
        i = -1;
    }
    return i;
}

int MainWindow::firstEmptyDiskSlot(int startFrom, bool createOne)
{
    int i;
    for (i = startFrom; i < DISK_COUNT; i++) {  //
        if (!sio->getDevice(DISK_BASE_CDEVIC + i)) {
            break;
        }
    }
    if (i > DISK_COUNT-1) {   //
        if (createOne) {
            i = DISK_COUNT-1;
        } else {
            i = -1;
        }
    }
    emit newSlot(i);        //
    return i;
}

void MainWindow::bootExe(const QString &fileName)
{
    SioDevice *old = sio->getDevice(DISK_BASE_CDEVIC);
    AutoBoot loader(sio, old);
    AutoBootDialog dlg(this);

    bool highSpeed = RespeqtSettings::instance()->useHighSpeedExeLoader() &&
        (RespeqtSettings::instance()->serialPortHandshakingMethod() != HANDSHAKE_SOFTWARE);

    if (!loader.open(fileName, highSpeed)) {
        return;
    }

    // TODO Test
    sio->uninstallDevice(DISK_BASE_CDEVIC);
    sio->installDevice(DISK_BASE_CDEVIC, &loader);
    connect(&loader, &AutoBoot::booterStarted, &dlg, &AutoBootDialog::booterStarted);
    connect(&loader, &AutoBoot::booterLoaded, &dlg, &AutoBootDialog::booterLoaded);
    connect(&loader, &AutoBoot::blockRead, &dlg, &AutoBootDialog::blockRead);
    connect(&loader, &AutoBoot::loaderDone, &dlg, &AutoBootDialog::loaderDone);
    // TODO keepOpen MIA connect(&dlg, &AutoBootDialog::keepOpen()), this, &MainWindow::keepBootExeOpen);

    dlg.exec();

    sio->uninstallDevice(DISK_BASE_CDEVIC);
    if (old) {
        sio->installDevice(DISK_BASE_CDEVIC, old);
        sio->getDevice(DISK_BASE_CDEVIC);
    }
    if(!g_exefileName.isEmpty()) bootExe(g_exefileName);
}

// Make boot executable dialog persistant until it's manually closed //
void MainWindow::keepBootExeOpen()
{
    bootExe(g_exefileName);
}

void MainWindow::bootExeTriggered(const QString &fileName)
{
    QString path = RespeqtSettings::instance()->lastRclDir();
    g_exefileName = path + "/" + fileName;
    if (!g_exefileName.isEmpty()) {
        RespeqtSettings::instance()->setLastExeDir(QFileInfo(g_exefileName).absolutePath());
        bootExe(g_exefileName);
    }
}

void MainWindow::selectBootExeTriggered()
{
    auto dir = RespeqtSettings::instance()->lastExeDir();
    g_exefileName = QFileDialog::getOpenFileName(this, tr("Open executable"),
        dir,
        tr(
           "Atari executables (*.xex *.com *.exe);;"
           "All files (*)")
    );

    if (!g_exefileName.isEmpty()) {
        RespeqtSettings::instance()->setLastExeDir(QFileInfo(g_exefileName).absolutePath());
        bootExe(g_exefileName);
    }
}

void MainWindow::mountFileWithDefaultProtection(int no, const QString &fileName)
{
    // If fileName was passed from RCL it is an 8.1 name, so we need to find
    // the full PC name in order to validate it.  //
    QString atariFileName, atariLongName, path;

    g_rclFileName = fileName;
    atariFileName = fileName;

    if(atariFileName.left(1) == "*") {
        atariLongName = atariFileName.mid(1);
        QString path = RespeqtSettings::instance()->lastRclDir();
        if(atariLongName == "") {
            sio->port()->writeDataNak();
            return;
        } else {
            atariFileName =  path + "/" + atariLongName;
        }
    }

    const RespeqtSettings::ImageSettings* imgSetting = RespeqtSettings::instance()->getImageSettingsFromName(atariFileName);
    bool prot = (imgSetting!=nullptr) && imgSetting->isWriteProtected;
    mountFile(no, atariFileName, prot);
}

void MainWindow::mountFile(int no, const QString &fileName, bool /*prot*/)
{
    SimpleDiskImage *disk = nullptr;
    bool isDir = false;
    bool ask   = true;

    if (fileName.isEmpty()) {
        if(g_rclFileName.left(1) == "*") emit fileMounted(false);
        return;
    }

    FileTypes::FileType type = FileTypes::getFileType(fileName);

    if (type == FileTypes::Dir) {
        disk = new FolderImage(sio, RespeqtSettings::instance()->limitFileEntries() ? 64 : -1);
        isDir = true;
    } else {
        disk = installDiskImage();
    }

    if (disk) {
        auto oldDisk = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
        Board *board = oldDisk != nullptr ? oldDisk->getBoardInfo() : nullptr;
        if(g_rclFileName.left(1) == "*") ask = false;
        if (!disk->open(fileName, type) || !ejectImage(no, ask) ) {
            RespeqtSettings::instance()->unmountImage(no);
            delete disk;
            if (board != nullptr) {
                delete board;
            }
            if(g_rclFileName.left(1) == "*") emit fileMounted(false);  //
            return;
        }
        else if (board != nullptr) {
            disk->setBoardInfo(board);
            delete board;
        }

        sio->installDevice(DISK_BASE_CDEVIC + no, disk);

        try {
            auto pclink = dynamic_cast<PCLINK*>(sio->getDevice(PCLINK_CDEVIC));
            if(pclink != nullptr && (isDir || pclink->hasLink(no+1)))
            {
                sio->uninstallDevice(PCLINK_CDEVIC);
                if(isDir)
                {
                    pclink->setLink(no+1,QDir::toNativeSeparators(fileName).toLatin1());
                }
                else
                {
                    pclink->resetLink(no+1);
                }
                sio->installDevice(PCLINK_CDEVIC,pclink);
            }
        } catch(std::bad_cast& e)
        {
            qDebug() << "!e " << tr("Bad cast for PCLINK");
        }

        diskWidgets[no]->updateFromImage(disk, RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());

        RespeqtSettings::instance()->mountImage(no, fileName, disk->isReadOnly());
        updateRecentFileActions();
        // TODO Test
        connect(disk, &SimpleDiskImage::statusChanged, this, &MainWindow::deviceStatusChanged, Qt::QueuedConnection);
        deviceStatusChanged(DISK_BASE_CDEVIC + no);

        // Extract the file name without the path //
        QString filenamelabel;
        int i = fileName.lastIndexOf("/");
        if (i != -1) {
            i++;
            filenamelabel = fileName.right(fileName.size() - i);
        }

        qDebug() << "!n" << tr("[%1] Mounted '%2' as '%3'.")
                .arg(disk->deviceName())
                .arg(filenamelabel)
                .arg(disk->description());

        if(g_rclFileName.left(1) == "*") emit fileMounted(true);  //
    }
}

SimpleDiskImage *MainWindow::installDiskImage()
{
    SimpleDiskImage *disk = new SimpleDiskImage(sio);
    disk->setDisplayTransmission(RespeqtSettings::instance()->displayTransmission());
    disk->setSpyMode(RespeqtSettings::instance()->isSpyMode());
    disk->setTrackLayout(RespeqtSettings::instance()->isTrackLayout());
    disk->setDisassembleUploadedCode(RespeqtSettings::instance()->disassembleUploadedCode());
    disk->setTranslatorAutomaticDetection(RespeqtSettings::instance()->translatorAutomaticDetection());
    disk->setTranslatorDiskImagePath(RespeqtSettings::instance()->translatorDiskImagePath());
    disk->setToolDiskImagePath(RespeqtSettings::instance()->toolDiskImagePath());
    disk->setActivateChipModeWithTool(RespeqtSettings::instance()->activateChipModeWithTool());
    disk->setActivateHappyModeWithTool(RespeqtSettings::instance()->activateHappyModeWithTool());
    return disk;
}

void MainWindow::mountDiskImage(int no)
{
    QString dir;

    if (no < 0)
    {
        no = firstEmptyDiskSlot(0, true);
    }
// Always mount from "last image dir" //
//    if (diskWidgets[no].fileNameLabel->text().isEmpty()) {
        dir = RespeqtSettings::instance()->lastDiskImageDir();
//    } else {
//        dir = QFileInfo(diskWidgets[no].fileNameLabel->text()).absolutePath();
//    }
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open a disk image"),
                                                    dir,
                                                    tr(
                                                    "All Atari disk images (*.atr *.xfd *.atx *.pro);;"
                                                    "SIO2PC ATR images (*.atr);;"
                                                    "XFormer XFD images (*.xfd);;"
                                                    "ATX images (*.atx);;"
                                                    "Pro images (*.pro);;"
                                                    "All files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    RespeqtSettings::instance()->setLastDiskImageDir(QFileInfo(fileName).absolutePath());
    mountFileWithDefaultProtection(no, fileName);
}

void MainWindow::mountFolderImage(int no)
{
    QString dir;

    if (no < 0)
    {
        no = firstEmptyDiskSlot(0, true);
    }
    // Always mount from "last folder dir" //
    dir = RespeqtSettings::instance()->lastFolderImageDir();
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open a folder image"), dir);
    fileName = QDir::fromNativeSeparators(fileName);    //
    if (fileName.isEmpty()) {
        return;
    }
    RespeqtSettings::instance()->setLastFolderImageDir(fileName);
    mountFileWithDefaultProtection(no, fileName);
}

void MainWindow::loadNextSide(int no)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
    mountFileWithDefaultProtection(no, img->getNextSideFilename());
}

void MainWindow::toggleHappy(int no, bool enabled)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
    img->setHappyMode(enabled);
}

void MainWindow::toggleChip(int no, bool open)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
    img->setChipMode(open);
    updateHighSpeed();
}

void MainWindow::toggleOSB(int no, bool open)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
    img->setOSBMode(open);
}

void MainWindow::toggleToolDisk(int no, bool enabled)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
    img->setToolDiskMode(enabled);
    updateHighSpeed();
}

void MainWindow::toggleWriteProtection(int no, bool protectionEnabled)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
    img->setReadOnly(protectionEnabled);
    RespeqtSettings::instance()->setMountedImageProtection(no, protectionEnabled);
}

void MainWindow::openEditor(int no)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
    if (img->editDialog()) {
        img->editDialog()->close();
    } else {
        auto dlg = new DiskEditDialog();
        dlg->go(img);
        dlg->show();
    }
}

QMessageBox::StandardButton MainWindow::saveImageWhenClosing(int no, QMessageBox::StandardButton previousAnswer, int number)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));

    if (previousAnswer != QMessageBox::YesToAll) {
        QMessageBox::StandardButtons buttons;
        if (number) {
            buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll | QMessageBox::Cancel;
        } else {
            buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
        }
        previousAnswer = QMessageBox::question(this, tr("Image file unsaved"), tr("'%1' has unsaved changes, do you want to save it?")
                                       .arg(img->originalFileName()), buttons);
    }
    if (previousAnswer == QMessageBox::Yes || previousAnswer == QMessageBox::YesToAll) {
        saveDisk(no);
    }
    if (previousAnswer == QMessageBox::Close) {
        previousAnswer = QMessageBox::Cancel;
    }
    return previousAnswer;
}

void MainWindow::loadTranslators()
{
    qApp->removeTranslator(&respeqt_qt_translator);
    qApp->removeTranslator(&respeqt_translator);
    if (RespeqtSettings::instance()->i18nLanguage().compare("auto") == 0) {
        QString locale = QLocale::system().name();
        respeqt_translator.load(":/translations/i18n/respeqt_" + locale);
        respeqt_qt_translator.load(":/translations/i18n/qt_" + locale);
        qApp->installTranslator(&respeqt_qt_translator);
        qApp->installTranslator(&respeqt_translator);
    } else if (RespeqtSettings::instance()->i18nLanguage().compare("en") != 0) {
        respeqt_translator.load(":/translations/i18n/respeqt_" + RespeqtSettings::instance()->i18nLanguage());
        respeqt_qt_translator.load(":/translations/i18n/qt_" + RespeqtSettings::instance()->i18nLanguage());
        qApp->installTranslator(&respeqt_qt_translator);
        qApp->installTranslator(&respeqt_translator);
    }
}

void MainWindow::saveDisk(int no)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));

    if (img->isUnnamed()) {
        saveDiskAs(no);
    } else {
        img->lock();
        bool saved = img->save();
        img->unlock();
        if (!saved) {
            if (QMessageBox::question(this, tr("Save failed"), tr("'%1' cannot be saved, do you want to save the image with another name?")
                .arg(img->originalFileName()), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
                saveDiskAs(no);
            }
        }
    }
}
//
void MainWindow::autoCommit(int no, bool st)
{
    if( no < DISK_COUNT )
    {
        if ( (diskWidgets[no]->isAutoSaveEnabled() && st) || (!diskWidgets[no]->isAutoSaveEnabled() && !st) )
                          diskWidgets[no]->triggerAutoSaveClickIfEnabled();
    }
}


void MainWindow::happy(int no, bool st)
{
    if( no < DISK_COUNT )
    {
        if ( (diskWidgets[no]->isHappyEnabled() && st) || (!diskWidgets[no]->isHappyEnabled() && !st) )
                          diskWidgets[no]->triggerHappyClickIfEnabled();
    }
}


void MainWindow::chip(int no, bool st)
{
    if( no < DISK_COUNT )
    {
        if ( (diskWidgets[no]->isChipEnabled() && st) || (!diskWidgets[no]->isChipEnabled() && !st) )
                          diskWidgets[no]->triggerChipClickIfEnabled();
    }
}

void MainWindow::autoSaveDisk(int no)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));

    DriveWidget* widget = diskWidgets[no];

    if (img->isUnnamed()) {
        saveDiskAs(no);
        widget->updateFromImage(img, RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(),  RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
        return;
    }

    bool autoSaveEnabled = widget->isAutoSaveEnabled();

    if (autoSaveEnabled) {
        qDebug() << "!n" << tr("[Disk %1] Auto-commit ON.").arg(no+1);
    } else {
        qDebug() << "!n" << tr("[Disk %1] Auto-commit OFF.").arg(no+1);
    }

    bool saved;

    img->lock();
    saved = img->save();
    img->unlock();
    if (!saved) {
        if (QMessageBox::question(this, tr("Save failed"), tr("'%1' cannot be saved, do you want to save the image with another name?")
            .arg(img->originalFileName()), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
            saveDiskAs(no);
        }
    }
    widget->updateFromImage(img, RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
}

//
void MainWindow::saveDiskAs(int no)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
    QString dir, fileName;
    bool saved = false;

    if (img->isUnnamed()) {
        dir = RespeqtSettings::instance()->lastDiskImageDir();
    } else {
        dir = QFileInfo(img->originalFileName()).absolutePath();
    }

    do {
        fileName = QFileDialog::getSaveFileName(this, tr("Save image as"),
                                 dir,
                                 tr(
                                                    "All Atari disk images (*.atr *.xfd *.atx *.pro);;"
                                                    "SIO2PC ATR images (*.atr);;"
                                                    "XFormer XFD images (*.xfd);;"
                                                    "ATX images (*.atx);;"
                                                    "Pro images (*.pro);;"
                                                    "All files (*)"));
        if (fileName.isEmpty()) {
            return;
        }

        img->lock();
        saved = img->saveAs(fileName);
        img->unlock();

        if (!saved) {
            if (QMessageBox::question(this, tr("Save failed"), tr("'%1' cannot be saved, do you want to save the image with another name?")
                .arg(fileName), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) {
                break;
            }
        }

    } while (!saved);

    if (saved) {
        RespeqtSettings::instance()->setLastDiskImageDir(QFileInfo(fileName).absolutePath());
    }
    RespeqtSettings::instance()->unmountImage(no);
    RespeqtSettings::instance()->mountImage(no, fileName, img->isReadOnly());
}

void MainWindow::revertDisk(int no)
{
    auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(no + DISK_BASE_CDEVIC));
    if (QMessageBox::question(this, tr("Revert to last saved"),
            tr("Do you really want to revert '%1' to its last saved state? You will lose the changes that has been made.")
            .arg(img->originalFileName()), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        img->lock();
        img->reopen();
        img->unlock();
        deviceStatusChanged(DISK_BASE_CDEVIC + no);
    }
}

// Slots for handling actions for devices.
void MainWindow::mountDiskTriggered(int deviceId) { mountDiskImage(deviceId); }
void MainWindow::mountFolderTriggered(int deviceId) { mountFolderImage(deviceId); }
void MainWindow::ejectTriggered(int deviceId) { ejectImage(deviceId); }
void MainWindow::nextSideTriggered(int deviceId) { loadNextSide(deviceId); }
void MainWindow::happyToggled(int deviceId, bool open) { toggleHappy(deviceId, open) ;}
void MainWindow::chipToggled(int deviceId, bool open) { toggleChip(deviceId, open); }
void MainWindow::OSBToggled(int deviceId, bool open) { toggleOSB(deviceId, open); }
void MainWindow::toolDiskTriggered(int deviceId, bool open) { toggleToolDisk(deviceId, open); }
void MainWindow::protectTriggered(int deviceId, bool writeProtectEnabled) { toggleWriteProtection(deviceId, writeProtectEnabled); }
void MainWindow::editDiskTriggered(int deviceId) { openEditor(deviceId); }
void MainWindow::saveTriggered(int deviceId) { saveDisk(deviceId); }
void MainWindow::autoSaveTriggered(int deviceId) { autoSaveDisk(deviceId); }
void MainWindow::saveAsTriggered(int deviceId) { saveDiskAs(deviceId); }
void MainWindow::revertTriggered(int deviceId) { revertDisk(deviceId); }

void MainWindow::ejectAllTriggered()
{
    QMessageBox::StandardButton answer = QMessageBox::No;

    int toBeSaved = 0;

    for (int i = 0; i < DISK_COUNT; i++) {  //
        auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(i + DISK_BASE_CDEVIC));
        if (img && img->isModified()) {
            toBeSaved++;
        }
    }

    if (!toBeSaved) {
        for (int i = DISK_COUNT-1; i >= 0; i--) {
            ejectImage(i);
        }
        return;
    }

    bool wasRunning = ui->actionStartEmulation->isChecked();
    if (wasRunning) {
        ui->actionStartEmulation->trigger();
    }

    for (int i = DISK_COUNT-1; i >= 0; i--) {
        auto img = qobject_cast <SimpleDiskImage*> (sio->getDevice(i + DISK_BASE_CDEVIC));
        if (img && img->isModified()) {
            toBeSaved--;
            answer = saveImageWhenClosing(i, answer, toBeSaved);
            if (answer == QMessageBox::NoToAll) {
                break;
            }
            if (answer == QMessageBox::Cancel) {
                if (wasRunning) {
                    ui->actionStartEmulation->trigger();
                }
                return;
            }
        }
    }
    for (int i = DISK_COUNT-1; i >= 0; i--) {
        ejectImage(i, false);
    }
    if (wasRunning) {
        ui->actionStartEmulation->trigger();
    }
}

void MainWindow::newImageTriggered()
{
    CreateImageDialog dlg(this);
    if (!dlg.exec()) {
        return;
    };

    int no = firstEmptyDiskSlot(0, true);

    auto disk = installDiskImage();
    // TODO Test
    connect(disk, &SimpleDiskImage::statusChanged, this, &MainWindow::deviceStatusChanged, Qt::QueuedConnection);

    if (!disk->create(++untitledName)) {
        delete disk;
        return;
    }

    DiskGeometry g;
    uint size = dlg.sectorCount() * dlg.sectorSize();
    if (dlg.sectorSize() == 256) {
        if (dlg.sectorCount() >= 3) {
            size -= 384;
        } else {
            size -= dlg.sectorCount() * 128;
        }
    }
    g.initialize(size, dlg.sectorSize());

    if (!disk->format(g)) {
        delete disk;
        return;
    }

    if (!ejectImage(no)) {
        delete disk;
        return;
    }

    sio->installDevice(DISK_BASE_CDEVIC + no, disk);
    deviceStatusChanged(DISK_BASE_CDEVIC + no);
    qDebug() << "!n" << tr("[%1] Mounted '%2' as '%3'.")
            .arg(disk->deviceName())
            .arg(disk->originalFileName())
            .arg(disk->description());
}

void MainWindow::openSessionTriggered()
{
    auto dir = RespeqtSettings::instance()->lastSessionDir();
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open session"),
         dir,
         tr(
            "RespeQt sessions (*.respeqt);;"
            "All files (*)")
    );
    if (fileName.isEmpty()) {
        return;
    }
// First eject existing images, then mount session images and restore mainwindow position and size //
    MainWindow::ejectAllTriggered();

    RespeqtSettings::instance()->setLastSessionDir(QFileInfo(fileName).absolutePath());
    g_sessionFile = QFileInfo(fileName).fileName();
    g_sessionFilePath = QFileInfo(fileName).absolutePath();

// Pass Session file name, path and MainWindow title to RespeQtSettings //
    RespeqtSettings::instance()->setSessionFile(g_sessionFile, g_sessionFilePath);
    RespeqtSettings::instance()->setMainWindowTitle(g_mainWindowTitle);

    RespeqtSettings::instance()->loadSessionFromFile(fileName);

    setWindowTitle(g_mainWindowTitle + tr(" -- Session: ") + g_sessionFile);
    setGeometry(RespeqtSettings::instance()->lastHorizontalPos(), RespeqtSettings::instance()->lastVerticalPos(), RespeqtSettings::instance()->lastWidth() , RespeqtSettings::instance()->lastHeight());

    for (int i = 0; i < DISK_COUNT; i++) {  //
        RespeqtSettings::ImageSettings is;
        is = RespeqtSettings::instance()->mountedImageSetting(i);
        mountFile(i, is.fileName, is.isWriteProtected);
    }
    g_D9DOVisible = RespeqtSettings::instance()->D9DOVisible();
    hideShowTriggered();
    setSession();
}
void MainWindow::saveSessionTriggered()
{
    auto dir = RespeqtSettings::instance()->lastSessionDir();
    auto fileName = QFileDialog::getSaveFileName(this, tr("Save session as"),
         dir,
         tr(
            "RespeQt sessions (*.respeqt);;"
            "All files (*)")
    );
    if (fileName.isEmpty()) {
        return;
    }
    RespeqtSettings::instance()->setLastSessionDir(QFileInfo(fileName).absolutePath());

// Save mainwindow position and size to session file //
    if (RespeqtSettings::instance()->saveWindowsPos()) {
        RespeqtSettings::instance()->setLastHorizontalPos(geometry().x());
        RespeqtSettings::instance()->setLastVerticalPos(geometry().y());
        RespeqtSettings::instance()->setLastWidth(geometry().width());
        RespeqtSettings::instance()->setLastHeight(geometry().height());
    }
    RespeqtSettings::instance()->saveSessionToFile(fileName);
}

void MainWindow::textPrinterWindowClosed()
{
    ui->actionShowPrinterTextOutput->setChecked(false);
}

void MainWindow::cassettePlaybackTriggered()
{
    auto fileName = QFileDialog::getOpenFileName(this,
            tr("Open a cassette image"),
            RespeqtSettings::instance()->lastCasDir(),
            tr("CAS images (*.cas);;All files (*)")
    );

    if (fileName.isEmpty()) {
        return;
    }
    RespeqtSettings::instance()->setLastCasDir(QFileInfo(fileName).absolutePath());

    bool restart;
    restart = ui->actionStartEmulation->isChecked();
    if (restart) {
        ui->actionStartEmulation->trigger();
        sio->wait();
        qApp->processEvents();
    }

    auto dlg = new CassetteDialog(this, fileName);
    dlg->exec();
    delete dlg;

    if (restart) {
        ui->actionStartEmulation->trigger();
    }
}

void MainWindow::quitApplicationTriggered()
{
    close();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
#ifndef Q_OS_MACX
    if (reason == QSystemTrayIcon::DoubleClick) {
        setWindowFlags(oldWindowFlags);
        setWindowState(oldWindowStates);
        show();
        activateWindow();
        raise();
        trayIcon.hide();
    }
#else
    (void)reason; // Suppress the unused parameter warning.
#endif
}

void MainWindow::bootOptionTriggered()
{
    QString folderPath = RespeqtSettings::instance()->mountedImageSetting(0).fileName;
    BootOptionsDialog bod(folderPath, this);
    bod.exec();
}

/*void MainWindow::closeTextPrinterWindow(const Printers::TextPrinterWindow *window)
{
    for(auto i = 0; i < PRINTER_COUNT; i++)
    {
        if (printerWidgets[i]->connected() && printerWidgets[i]->device() == window)
        {
            printerWidgets[i]->disconnectPrinter();
            break;
        }
    }
}*/

// This connect the signal from UI to slots
void MainWindow::connectUISignal()
{
    connect(ui->actionPlaybackCassette, &QAction::triggered, this, &MainWindow::cassettePlaybackTriggered);
    connect(ui->actionBootExe, &QAction::triggered, this, &MainWindow::selectBootExeTriggered);
    connect(ui->actionSaveSession, &QAction::triggered, this, &MainWindow::saveSessionTriggered);
    connect(ui->actionOpenSession, &QAction::triggered, this, &MainWindow::openSessionTriggered);
    connect(ui->actionNewImage, &QAction::triggered, this, &MainWindow::newImageTriggered);
    connect(ui->actionEjectAll, &QAction::triggered, this, &MainWindow::ejectAllTriggered);
    connect(ui->actionOptions, &QAction::triggered, this, &MainWindow::showOptionsTriggered);
    connect(ui->actionStartEmulation, &QAction::triggered, this, &MainWindow::startEmulationTriggered);
    connect(ui->actionLimitFileEntries, &QAction::triggered, this, &MainWindow::toggleLimitEntriesTriggered);
    connect(ui->actionPrinterEmulation, &QAction::triggered, this, &MainWindow::printerEmulationTriggered);
    connect(ui->actionHideShowDrives, &QAction::triggered, this, &MainWindow::hideShowTriggered);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quitApplicationTriggered);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::showAboutTriggered);
    connect(ui->actionDocumentation, &QAction::triggered, this, &MainWindow::showDocumentationTriggered);
    connect(ui->actionToggleMiniMode, &QAction::triggered, this, &MainWindow::toggleMiniModeTriggered);
    connect(ui->actionToggleShade, &QAction::triggered, this, &MainWindow::toggleShadeTriggered);
    connect(ui->actionLogWindow, &QAction::triggered, this, &MainWindow::showLogWindowTriggered);
    connect(ui->actionCaptureSnapshot, &QAction::toggled, this, &MainWindow::toggleSnapshotCapture);
    connect(ui->actionReplaySnapshot, &QAction::triggered, this, &MainWindow::replaySnapshot);
}

void MainWindow::toggleSnapshotCapture(bool toggle)
{
    if (sio) {
        auto recorder = Tests::SioRecorder::instance();

        if (!recorder->isSnapshotRunning())
        {
           recorder->startSIOSnapshot();
        } else {
           auto snapshot = recorder->stopSIOSnapshot();
           auto fileName = QFileDialog::getSaveFileName(MainWindow::instance(),
               tr("Save test Json File"), QString(), tr("Json Files (*.json)"));
           QFile file{fileName};
           file.open(QFile::WriteOnly|QFile::Truncate);
           file.write(snapshot);
           file.close();
        }
    }
    ui->actionCaptureSnapshot->setChecked(toggle);
}

void MainWindow::replaySnapshot()
{
    if (sio->isRunning() && RespeqtSettings::instance()->backend() != SerialBackend::TEST)
    {
    // TODO Ask for permission to cut the serial connection, if open.
        auto answer = QMessageBox::question(this, tr("Disconnect serial"), tr("If you proceed, the standard serial port will be disconnected. Would you like to proceed?"));
        if (answer == QMessageBox::No)
            return;
    }

    // Open the snapshot and load it into SioRecorder
    auto fileName = QFileDialog::getOpenFileName(MainWindow::instance(),
        tr("Save test Json File"), QString(), tr("Json Files (*.json)"));
    QFile file{fileName};
    file.open(QFile::ReadOnly);
    auto recorder = Tests::SioRecorder::instance();
    recorder->prepareReplaySnapshot(&file, RespeqtSettings::instance()->backend());
    file.close();
    // Now set SioRecorder to be the serial port and start the connection again.
    ui->actionStartEmulation->trigger();
    RespeqtSettings::instance()->setBackend(SerialBackend::TEST);
    ui->actionStartEmulation->trigger();
}

void MainWindow::setupDebugItems()
{
    ui->menuDebug->menuAction()->setVisible(RespeqtSettings::instance()->debugMenuVisible());
}

void MainWindow::toggleLimitEntriesTriggered()
{
    if (RespeqtSettings::instance()->limitFileEntries()) {
        limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock_open.png").pixmap(16, 16, QIcon::Normal));
    }
    else {
        limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock.png").pixmap(16, 16, QIcon::Normal));
    }
    RespeqtSettings::instance()->setlimitFileEntries(!RespeqtSettings::instance()->limitFileEntries());
}
