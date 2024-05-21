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

/*
 * TODO Fix startup setting for Debug menu, Lock icon
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "autoboot.h"
#include "autobootdialog.h"
#include "bootoptionsdialog.h"
#include "cassettedialog.h"
#include "drivewidget.h"
#include "include/diskimages/folderimage.h"
#include "logdisplaydialog.h"
#include "pclink.h"
#include "printers/printerfactory.h"
#include "printerwidget.h"
#include "rcl.h"
#include "smartdevice.h"
//#include "printers/outputfactory.h"
#include "printers/printers.h"
//#include "printers/outputs.h"
#include "respeqtsettings.h"

//#include <QDesktopWidget> deprecated in Qt 5. Use QScreen:
#include <QScreen>
#include <QDrag>
#include <QDragEnterEvent>
//#include <QDropEvent>
#include <QEvent>
#include <QFont>
#include <QHBoxLayout>
#include <QMessageBox>
//#include <QPrintDialog>
//#include <QPrinter>
//#include <QPrinterInfo>
#include <QScrollBar>
#include <QToolButton>
#include <QTranslator>
#include <QUrl>
#include <QWidget>
#include <QtDebug>
#include <memory>
#include <typeinfo>

#include "filesystems/atarifilesystem.h"
#include "miscutils.h"

#include <QFontDatabase>
#include <utility>

#ifndef QT_NO_QDEBUG
#include "siorecorder.h"
#endif

static QFile *logFile;
static QMutex *logMutex;

QString g_exefileName;
static QString g_rclFileName;
QString g_respeQtAppPath;
bool g_disablePicoHiSpeed;
//SimpleDiskImage *g_translator = nullptr;
//static int g_savedWidth;

MainWindow *MainWindow::sInstance{nullptr};

// ****************************** END OF GLOBALS ************************************//

// DEV NOTE: using qDebug() requires one of the following prefix strings:
//
// Displayed only in debug mode    "!d"
// Unimportant     (gray)          "!u"
// Normal          (black)         "!n"
// Important       (blue)          "!i"
// Warning         (brown)         "!w"
// Error           (red)           "!e"
// (???)           (purple)        "!"      (not used?)
//
void MainWindow::logMessageOutput(QtMsgType type, const QMessageLogContext & /*context*/, const QString &msg) {
  logMutex->lock();
  logFile->write(QString::number((quint64) QThread::currentThreadId(), 16).toLatin1());
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

  QString nonConstMsg(msg);
  if (!msg.startsWith("!"))     // Qt/system log message?
    nonConstMsg = "123" + msg;  // "123" gets removed below

  QByteArray localMsg = nonConstMsg.toLocal8Bit();
  QByteArray displayMsg = localMsg.mid(3);

  logFile->write(displayMsg);
  logFile->write("\n");

  if (type == QtFatalMsg) {
    logFile->close();
#if defined(QT_NO_DEBUG) // allow debugger to survive fatal error
    abort();
#elif defined (Q_OS_WIN) // TBD: non-windows? (QtCreator 10.x on Mojave is OK)
    __debugbreak();
#endif
  }
  logMutex->unlock();   // __debugbreak() (line above) breaks *here* on fatal error, crash or assert.

#ifdef QT_NO_DEBUG
  // release build: filter Qt/system messages (all app generated log messages follow above DEV NOTE)
  if ((msg[0] != '!') || (msg[1] == 'd')) {
    return;
  }
#endif
  sInstance->doLogMessage(localMsg.at(1), displayMsg);
}

void MainWindow::doLogMessage(int type, const QString &msg) {
  emit logMessage(type, msg);
}

MainWindow::MainWindow()
    : QMainWindow(nullptr), ui(new Ui::MainWindow),
      isClosing(false), lastMessageRepeat(0),
      printerWidgets{}, diskWidgets{} {
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
  qDebug() << "!d" << tr("RespeQt started @ %1.").arg(QDateTime::currentDateTime().toString());

  logWindow_ = nullptr;
  diskBrowserDlg = nullptr;

  /* Remove old temporaries */
  QDir tempDir = QDir::temp();
  QStringList filters;
  filters << "respeqt-*";
  QFileInfoList list = tempDir.entryInfoList(filters, QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
  foreach (QFileInfo file, list) {
    deltree(file.absoluteFilePath());
  }

  QCoreApplication::setOrganizationName("ZeeSoft");
  QCoreApplication::setOrganizationDomain("org.respeqt");
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
  for (int i = 0; i < NUM_RECENT_FILES; ++i) {
    auto recentAction = new QAction(this);
    connect(recentAction, &QAction::triggered, this, &MainWindow::openRecent);
    recentFilesActions_.append(recentAction);
  }

  auto diskMenu = (QWidget *) menuBar()->children().at(1);

  diskMenu->addActions(recentFilesActions_);

  /* Initialize diskWidgets array and tool button actions */
  createDeviceWidgets();
  // Configure debug menu
  setupDebugItems();

  /* Parse command line arguments:
      arg(1): session file (xxxxxxxx.respeqt)   */

  QStringList RespeQtArgs = QCoreApplication::arguments();
  g_sessionFile = g_sessionFilePath = "";
  if (RespeQtArgs.size() > 1) {
    QFile sess;
    QString s = QDir::separator();           //
    int i = RespeQtArgs.at(1).lastIndexOf(s);//
    if (i != -1) {
      i++;
      g_sessionFile = RespeQtArgs.at(1).right(RespeQtArgs.at(1).size() - i);
      g_sessionFilePath = RespeQtArgs.at(1).left(i);
      g_sessionFilePath = QDir::fromNativeSeparators(g_sessionFilePath);
      sess.setFileName(g_sessionFilePath + g_sessionFile);
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
    RespeqtSettings::instance()->loadSessionFromFile(g_sessionFilePath + g_sessionFile);
  } else {
    setWindowTitle(g_mainWindowTitle);
  }

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

  netLabel->setPixmap(QIcon(":/icons/oxygen-icons/16x16/actions/network_disconnect.png").pixmap(16, 16, QIcon::Normal));
  netLabel->setToolTip(tr("No TNFS connection"));
  netLabel->setStatusTip(netLabel->toolTip());

  connect(&tnfs, &Network::Tnfs::allSessionsDisconnected, this, &MainWindow::allSessionsDisconnected);
  connect(&tnfs, &Network::Tnfs::sessionConnected, this, &MainWindow::sessionConnected);

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
  ui->textEdit->document()->setMaximumBlockCount(MAXBLOCK);
  changeFonts();

  /* Connect SioWorker signals */
  sio = SioWorkerPtr::create();
  connect(sio.data(), &SioWorker::started, this, &MainWindow::sioStarted);
  connect(sio.data(), &SioWorker::finished, this, &MainWindow::sioFinished);
  connect(sio.data(), &SioWorker::statusChanged, this, &MainWindow::sioStatusChanged);
  shownFirstTime = true;
  sio->setAutoReconnect(RespeqtSettings::instance()->sioAutoReconnect());

  auto pclink = new PCLINK(sio);
  sio->installDevice(PCLINK_CDEVIC, pclink);

  /* Restore application state */
  for (char i = 0; i < DISK_COUNT; i++) {
    RespeqtSettings::ImageSettings is { RespeqtSettings::instance()->mountedImageSetting(i) };
    mountFile(i, is.fileName, is.isWriteProtected);
  }
  updateRecentFileActions();

  setAcceptDrops(true);

  // SmartDevice (ApeTime + URL submit)
  auto smart = new SmartDevice(sio);
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
  connect(rcl, &RCl::findNewSlot, this, &MainWindow::firstEmptyDiskSlot);
  connect(this, &MainWindow::newSlot, rcl, &RCl::gotNewSlot);
  connect(rcl, &RCl::mountFile, this, &MainWindow::mountFileWithDefaultProtection);
  connect(this, &MainWindow::fileMounted, rcl, &RCl::fileMounted);
  connect(rcl, &RCl::toggleAutoCommit, this, &MainWindow::autoCommit);
  connect(rcl, &RCl::toggleHappy, this, &MainWindow::happy);
  connect(rcl, &RCl::toggleChip, this, &MainWindow::chip);
  connect(rcl, &RCl::bootExe, this, &MainWindow::bootExeTriggered);
}

MainWindow::~MainWindow() {
  if (ui->actionStartEmulation->isChecked()) {
    ui->actionStartEmulation->trigger();
  }

  qDebug() << "!d" << tr("RespeQt stopped at %1.").arg(QDateTime::currentDateTime().toString());
  qInstallMessageHandler(nullptr);
  delete logMutex;
  delete logFile;
  delete ui;
}

void MainWindow::createDeviceWidgets() {

  ui->rightColumn->setAlignment(Qt::AlignTop);

  for (int i = 0; i < DISK_COUNT; i++) {//
    auto driveWidget = new DriveWidget(i);

    if (i < 8) {
      ui->leftColumn->addWidget(driveWidget);
    } else {
      ui->rightColumn->addWidget(driveWidget);
    }

    driveWidget->setup(RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
    diskWidgets.push_back(driveWidget);

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
    connect(this, &MainWindow::fontChanged, driveWidget, &DriveWidget::setLabelFont);
  }

  for(int i = 0; i < PRINTER_COUNT; i++) {//
    auto printerWidget = new PrinterWidget(i);
    if (i < 2) {
      ui->leftColumn2->addWidget(printerWidget);
    } else {
      ui->rightColumn2->addWidget(printerWidget);
    }
    printerWidgets.push_back(printerWidget);
  }

  changeFonts();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
  if (isMiniMode && isShadeMode) {
    auto delta = QPoint(event->globalPos() - savedPosition);
    move(x() + delta.x(), y() + delta.y());
    savedPosition = event->globalPos();
  }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {

  if (event->button() != Qt::LeftButton)    // check for LEFT mouse button
    return;                                // EXIT NOW if NOT!

  if (isMiniMode && isShadeMode) {
    savedPosition = event->globalPos();
  } else {
    // auto slot {containingDiskSlot(event->pos())};  do not use
    // Mingw 4.9.2 converts initialization braces to std::initializer_list, when auto is used
    auto slot = containingDiskSlot(event->pos());   // should be OK with all compilers
    if (slot >= 0) {
      auto drag = new QDrag((QWidget *) this);
      auto mimeData = new QMimeData;

      mimeData->setData("application/x-respeqt-disk-image", QByteArray(1, slot));
      drag->setMimeData(mimeData);
      drag->exec();
    }
  }
  if (onOffLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
    ui->actionStartEmulation->trigger();
  }
  if (prtOnOffLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
    ui->actionPrinterEmulation->trigger();//
  }
  if (clearMessagesLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
    ui->textEdit->clear();
    emit sendLogText("");
  }
  if (!speedLabel->isHidden() && speedLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
    ui->actionOptions->trigger();
  }
  if (limitEntriesLabel->geometry().translated(ui->statusBar->geometry().topLeft()).contains(event->pos())) {
    ui->actionLimitFileEntries->trigger();
  }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
  /// not using initialization braces here (Mingw 4.9.2 treats them as std::initializer_list when auto is used)
  auto i = containingDiskSlot(event->pos());
  if (i >= 0 && (event->mimeData()->hasUrls() ||
                 event->mimeData()->hasFormat("application/x-respeqt-disk-image")))
    event->setDropAction(event->proposedAction());
  else
    event->setDropAction(Qt::IgnoreAction);

  event->accept();
  for (int j = 0; j < DISK_COUNT; j++) {
    diskWidgets[j]->setDropTarget(i == j);
  }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event) {
  event->accept();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event) {
  /// not using initialization braces here (Mingw 4.9.2 treats them as std::initializer_list when auto is used)
  auto i = containingDiskSlot(event->pos());
  if (i >= 0 && (event->mimeData()->hasUrls() ||
                 event->mimeData()->hasFormat("application/x-respeqt-disk-image")))
    event->setDropAction(event->proposedAction());
  else
    event->setDropAction(Qt::IgnoreAction);

  event->accept();

  for (int j = 0; j < DISK_COUNT; j++) {
      diskWidgets[j]->setDropTarget(i == j);
  }
}

void MainWindow::dropEvent(QDropEvent *event) {
  for (int j = 0; j < DISK_COUNT; j++) {
    diskWidgets[j]->setDropTarget(false);
  }
  /// not using initialization braces here (Mingw 4.9.2 treats them as std::initializer_list when auto is used)
  auto slot = containingDiskSlot(event->pos());
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

      auto pclink = reinterpret_cast<PCLINK *>(sio->getDevice(PCLINK_CDEVIC));
      if (pclink->hasLink(slot + 1) || pclink->hasLink(source + 1)) {
        sio->uninstallDevice(PCLINK_CDEVIC);
        pclink->swapLinks(slot + 1, source + 1);
        sio->installDevice(PCLINK_CDEVIC, pclink);
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
    g_exefileName = files.at(0);//
    bootExe(files.at(0));
    return;
  }

  if (type == FileTypes::Cas) {
    bool restart;
    restart = ui->actionStartEmulation->isChecked();
    if (restart) {
      ui->actionStartEmulation->trigger();
      sio->waitOnPort();
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
  foreach (QString file, files) {
    qCritical() << "!e" << tr("Cannot mount '%1': No empty disk slots.").arg(file);
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (isClosing)
    return;
  isClosing = true;

  // Save various session settings  //

  if (RespeqtSettings::instance()->saveWindowsPos()) {

    RespeqtSettings::instance()->saveMainWinGeometry(this, isMiniMode);

    if (diskBrowserDlg) {
      diskBrowserDlg->close();
    } else {
      RespeqtSettings::instance()->setShowDiskBrowser(false);
    }

    if (logWindow_) {
      RespeqtSettings::instance()->setShowLogWindow(logWindow_->isVisible());
      RespeqtSettings::instance()->saveWidgetGeometry(logWindow_);
    } else {
      RespeqtSettings::instance()->setShowLogWindow(false);
    }
  }

  if (RespeqtSettings::instance()->saveDiskVis()) {
    RespeqtSettings::instance()->setD9DOVisible(isD9DOVisible);
  }

  if (g_sessionFile != "")
    RespeqtSettings::instance()->saveSessionToFile(g_sessionFilePath + "/" + g_sessionFile);

  bool wasRunning = ui->actionStartEmulation->isChecked();
  QMessageBox::StandardButton answer = QMessageBox::No;

  if (wasRunning) {
    ui->actionStartEmulation->trigger();
  }

  int toBeSaved = 0;

  for (int i = 0; i < DISK_COUNT; i++) {//
    auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(i + DISK_BASE_CDEVIC));
    if (img && img->isModified()) {
      toBeSaved++;
    }
  }

  for (char i = 0; i < DISK_COUNT; i++) {//
    auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(i + DISK_BASE_CDEVIC));
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
    auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(i + DISK_BASE_CDEVIC));
    if (img && img->editDialog()) img->editDialog()->close();
  }

  //
  delete docDisplayWindow;
  docDisplayWindow = nullptr;

  for (int i = DISK_BASE_CDEVIC; i < (DISK_BASE_CDEVIC + DISK_COUNT); i++) {
    auto s = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(i));
    if (s) {
      s->close();
    }
  }

  for (int i = 0; i < PRINTER_COUNT; i++) {
    auto printerWidget = printerWidgets[i];
    if (printerWidget)
      printerWidget->disconnectPrinter();
  }

  event->accept();
}

void MainWindow::hideEvent(QHideEvent *event) {
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

void MainWindow::showEvent(QShowEvent *event) {
  static bool shownThisTime = true;

  if (event->type() == QEvent::Show && shownFirstTime) {
    shownFirstTime = false;// Reset the flag

    /* Open options dialog if it's the first time */
    if (RespeqtSettings::instance()->isFirstTime()) {
      if (QMessageBox::Yes == QMessageBox::question(this, tr("First run"),
          tr("You are running RespeQt for the first time.\n\nDo you want to open the options dialog?"),
          QMessageBox::Yes, QMessageBox::No)) {
        ui->actionOptions->trigger();
      }
    }
    qDebug() << "!d"
             << "Starting emulation";

    ui->actionStartEmulation->trigger();
  }
  if (event->type() == QEvent::Show && shownThisTime) {
    shownThisTime = false;
    restoreLayout();
  }
  QMainWindow::showEvent(event);
}

#if (QT_VERSION_MAJOR < 6)
void MainWindow::enterEvent(QEvent *) {
#else
void MainWindow::enterEvent(QEnterEvent *) {
#endif
  if (isMiniMode && isShadeMode) {
    setWindowOpacity(1.0);
  }
}

void MainWindow::leaveEvent(QEvent *) {
  if (isMiniMode && isShadeMode) {
    setWindowOpacity(0.25);
  }
}

bool MainWindow::eventFilter(QObject * /*obj*/, QEvent *event) {
  if (event->type() == QEvent::MouseButtonDblClick) {
    showLogWindowTriggered();
  }
  return false;
}

void MainWindow::showLogWindowTriggered() {
  if (logWindow_ == nullptr) {
    logWindow_ = new LogDisplayDialog(this);
    bool restored = false;
    if (RespeqtSettings::instance()->saveWindowsPos()) {
      restored = RespeqtSettings::instance()->restoreWidgetGeometry(logWindow_);
    }
    if (!restored) {
      int x, y, w, h;
      x = geometry().x();
      y = geometry().y();
      w = geometry().width();
      h = geometry().height();
      if (!isMiniMode) {
        logWindow_->setGeometry(static_cast<int>(x + w / 1.9), y + 30, logWindow_->geometry().width(), geometry().height());
      } else {
        logWindow_->setGeometry(x + 20, y + 60, w, h * 2);
      }
    }
    connect(this, &MainWindow::sendLogText, logWindow_, &LogDisplayDialog::setLogText);
    connect(this, &MainWindow::sendLogTextChange, logWindow_, &LogDisplayDialog::addLogTextChange);
    emit sendLogText(ui->textEdit->toHtml());
  }
  logWindow_->show();
}

void MainWindow::logChanged(QString text) {
  emit sendLogTextChange(std::move(text));
}

void MainWindow::toggleShadeTriggered() {
  if (isShadeMode) {
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowOpacity(1.0);
    isShadeMode = false;
    QMainWindow::show();
  } else {
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.25);
    isShadeMode = true;
    QMainWindow::show();
  }
}

// Toggle Mini Mode //
void MainWindow::toggleMiniModeTriggered() {
  isMiniMode = !isMiniMode;

  // set asside current geometry before doing anything
  auto geometryBeforeToggle = geometry();

  showHideDrives();

  ui->line->setVisible(!isMiniMode);

  if (!isMiniMode) {  // Full Window Mode:

    setMinimumHeight(RespeqtSettings::instance()->DefaultFullModeSize.height());
    setMaximumHeight(QWIDGETSIZE_MAX);
    ui->textEdit->setVisible(true);
    ui->actionHideShowDrives->setEnabled(true);
    if (savedGeometry.isEmpty()) {
      // on first toggle, pull values from our last persisted setting or defaults
      RespeqtSettings::instance()->restoreMainWinGeometry(this,isMiniMode);
    } else {
      // use saved geometry for both mini and full
      setGeometry(savedGeometry);
    }
    setWindowOpacity(1.0);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    ui->actionToggleShade->setDisabled(true);
    isShadeMode = false;

  } else {  // Mini-mode Window:

    ui->textEdit->setVisible(false);
    int height = diskWidgets[0]->sizeHint().height() + ui->menuBar->height() + ui->statusBar->height();
    setMinimumWidth(RespeqtSettings::instance()->DefaultMiniModeSize.width());
    setMinimumHeight(height);
    setMaximumHeight(height);
    if (savedGeometry.isEmpty()) {
      // on first toggle, pull values from our last persisted setting or defaults
      RespeqtSettings::instance()->restoreMainWinGeometry(this, isMiniMode);
    } else {
      // use saved geometry for both mini and full
      setGeometry(savedGeometry);
    }
    ui->actionHideShowDrives->setDisabled(true);
    ui->actionToggleShade->setEnabled(true);
    if (RespeqtSettings::instance()->enableShade()) {
      setWindowOpacity(0.25);
      setWindowFlags(Qt::FramelessWindowHint);
      isShadeMode = true;
    } else {
      isShadeMode = false;
    }
  }
  savedGeometry = geometryBeforeToggle;

  QMainWindow::show();
}

void MainWindow::showHideDrives() {
  for (int i = 1; i < DISK_COUNT; ++i) {
    diskWidgets[i]->setVisible(!isMiniMode && (isD9DOVisible || (i < 8)));
  }
  for (int i = 0; i < PRINTER_COUNT; ++i) {
    printerWidgets[i]->setVisible(!isMiniMode && (isD9DOVisible || (i < 2)));
  }

  if (isD9DOVisible) {
    ui->actionHideShowDrives->setText(QApplication::translate("MainWindow", "Hide drives D9-DO", nullptr));
    ui->actionHideShowDrives->setStatusTip(QApplication::translate("MainWindow", "Hide drives D9-DO", nullptr));
    ui->actionHideShowDrives->setIcon(QIcon(":/icons/silk-icons/icons/drive_add.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
    //setMinimumWidth(688);
  } else {
    ui->actionHideShowDrives->setText(QApplication::translate("MainWindow", "Show drives D9-DO", nullptr));
    ui->actionHideShowDrives->setStatusTip(QApplication::translate("MainWindow", "Show drives D9-DO", nullptr));
    ui->actionHideShowDrives->setIcon(QIcon(":/icons/silk-icons/icons/drive_delete.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
    //setMinimumWidth(344);
  }
}

// Toggle Hide/Show drives D9-DO  //
void MainWindow::hideShowTriggered() {
  isD9DOVisible = !isD9DOVisible;
  isMiniMode = isShadeMode = false;

  showHideDrives();

  //setGeometry(geometry().x(), geometry().y(), 0, geometry().height());
}

// Toggle printer Emulation ON/OFF //
void MainWindow::printerEmulationTriggered() {
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

void MainWindow::setUpPrinterEmulationWidgets(bool enable) {
  if (enable) {
    ui->actionPrinterEmulation->setText(QApplication::translate("MainWindow", "Stop printer emulation", nullptr));
    ui->actionPrinterEmulation->setStatusTip(QApplication::translate("MainWindow", "Stop printer emulation", nullptr));
    ui->actionPrinterEmulation->setIcon(QIcon(":/icons/silk-icons/icons/printer.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
    prtOnOffLabel->setPixmap(QIcon(":/icons/silk-icons/icons/printer.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
  } else {
    ui->actionPrinterEmulation->setText(QApplication::translate("MainWindow", "Start printer emulation", nullptr));
    ui->actionPrinterEmulation->setStatusTip(QApplication::translate("MainWindow", "Start printer emulation", nullptr));
    ui->actionPrinterEmulation->setIcon(QIcon(":/icons/silk-icons/icons/printer_delete.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
    prtOnOffLabel->setPixmap(QIcon(":/icons/silk-icons/icons/printer_delete.png").pixmap(16, 16, QIcon::Normal, QIcon::On));
  }
  prtOnOffLabel->setToolTip(ui->actionPrinterEmulation->toolTip());
  prtOnOffLabel->setStatusTip(ui->actionPrinterEmulation->statusTip());
}

void MainWindow::startEmulationTriggered() {
  if (ui->actionStartEmulation->isChecked()) {
    sio->start(QThread::TimeCriticalPriority);
  } else {
    sio->setPriority(QThread::NormalPriority);
    sio->waitOnPort();
    qApp->processEvents();
  }
}

void MainWindow::sioStarted() {
  ui->actionStartEmulation->setText(tr("&Stop emulation"));
  ui->actionStartEmulation->setToolTip(tr("Stop SIO peripheral emulation"));
  ui->actionStartEmulation->setStatusTip(tr("Stop SIO peripheral emulation"));
  onOffLabel->setPixmap(ui->actionStartEmulation->icon().pixmap(16, QIcon::Normal, QIcon::On));
  onOffLabel->setToolTip(ui->actionStartEmulation->toolTip());
  onOffLabel->setStatusTip(ui->actionStartEmulation->statusTip());
  for (auto pwidget: printerWidgets) {
    pwidget->setSioWorker(sio);
  }
}

void MainWindow::sioFinished() {
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

void MainWindow::sioStatusChanged(const QString &status) {
  speedLabel->setText(status);
  speedLabel->show();
  sio->setDisplayCommandName(RespeqtSettings::instance()->isCommandName());
}

void MainWindow::deviceStatusChanged(unsigned char deviceNo) {
  if (deviceNo >= DISK_BASE_CDEVIC && deviceNo < (DISK_BASE_CDEVIC + DISK_COUNT)) {// 0x31 - 0x3E
    auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(deviceNo));

    DriveWidget *diskWidget = diskWidgets[deviceNo - DISK_BASE_CDEVIC];

    if (img) {

      // Show file name without the path and set toolTip & statusTip to show the path separately //
      QString filenamelabel;
      int i = -1;

      if (img->description() == tr("Folder image")) {
        i = img->originalFileName().lastIndexOf("\\");// NOTE: This expects folder separators to be '\\'
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

      bool enableEdit = img->editDialog() != nullptr;

      if (img->description() == tr("Folder image")) {
        diskWidget->showAsFolderMounted(filenamelabel, img->description(), enableEdit);
      } else {
        bool enableSave = false;

        if (img->isModified()) {
          if (!diskWidget->isAutoSaveEnabled()) {//
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
                saveDiskAs(static_cast<char>(deviceNo - DISK_BASE_CDEVIC));
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

void MainWindow::updateHighSpeed() {
  if (sio->port() != nullptr) {
    int nbChip = 0;
    for (int i = 0; i < DISK_COUNT; ++i) {
      auto disk = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(i + DISK_BASE_CDEVIC));
      DiskImages::Board *board = disk != nullptr ? disk->getBoardInfo() : nullptr;
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

void MainWindow::uiMessage(int t, QString message) {
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
        message = QString("<span style='color:gray'>%1 <span style='font-family:courier,monospace'>%2</span></span>")
                          .arg(header, fixed);
      } else {
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

void MainWindow::showOptionsTriggered() {
  auto restart = ui->actionStartEmulation->isChecked();
  if (restart) {
    ui->actionStartEmulation->trigger();
    sio->waitOnPort();
    qApp->processEvents();
  }

  OptionsDialog optionsDialog(this);
  if (optionsDialog.exec() != QDialog::Accepted)
    return;

  // Change drive slot description fonts
  changeFonts();

  // load translators and retranslate
  loadTranslators();

  // retranslate Designer Form
  ui->retranslateUi(this);

  // fix Hide/Show D9-DO menu (retranslate sets menu text to 'Hide...')
  showHideDrives();

  setupDebugItems();

  if (RespeqtSettings::instance()->limitFileEntries()) {
    limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock.png").pixmap(16, 16, QIcon::Normal));
  } else {
    limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock_open.png").pixmap(16, 16, QIcon::Normal));
  }

  for (char i = DISK_BASE_CDEVIC; i < (DISK_BASE_CDEVIC + DISK_COUNT); i++) {// 0x31 - 0x3E
    deviceStatusChanged(i);
  }
  sio->setAutoReconnect(RespeqtSettings::instance()->sioAutoReconnect());

  ui->actionStartEmulation->trigger();
}

void MainWindow::changeFonts() {
  QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
  if (RespeqtSettings::instance()->useLargeFont()) {
    font.setPointSize(10);
    emit fontChanged(font);
  } else {
    font.setPointSize(8);
  }
  emit fontChanged(font);
}

void MainWindow::showAboutTriggered() {
  AboutDialog aboutDialog(this, VERSION);
  aboutDialog.exec();
}
//
void MainWindow::showDocumentationTriggered() {
  if (ui->actionDocumentation->isChecked()) {
    docDisplayWindow->show();
  } else {
    docDisplayWindow->hide();
  }
}
//
void MainWindow::docDisplayWindowClosed() {
  ui->actionDocumentation->setChecked(false);
}
// Restart emulation and re-translate following a session load //
void MainWindow::setSession() {
  bool restart;
  restart = ui->actionStartEmulation->isChecked();
  if (restart) {
    ui->actionStartEmulation->trigger();
    sio->waitOnPort();
    qApp->processEvents();
  }

  // load translators and retranslate
  loadTranslators();
  ui->retranslateUi(this);
  for (unsigned char i = DISK_BASE_CDEVIC; i < (DISK_BASE_CDEVIC + DISK_COUNT); i++) {
    deviceStatusChanged(i);
  }

  ui->actionStartEmulation->trigger();
}


void MainWindow::openRecent() {
  qDebug("open recent");
  auto action = qobject_cast<QAction *>(sender());
  if (action) {
    mountFileWithDefaultProtection(firstEmptyDiskSlot(), action->text());
  }
}

void MainWindow::updateRecentFileActions() {
  for (int i = 0; i < NUM_RECENT_FILES; ++i) {
    QAction *action = this->recentFilesActions_[i];
    const RespeqtSettings::ImageSettings &image = RespeqtSettings::instance()->recentImageSetting(i);

    if (image.fileName != "") {
      action->setVisible(true);
      action->setText(image.fileName);
    } else {
      action->setVisible(false);
    }
  }
}


bool MainWindow::ejectImage(char no, bool ask) {
  auto pclink = reinterpret_cast<PCLINK *>(sio->getDevice(PCLINK_CDEVIC));
  if (pclink->hasLink(no + 1)) {
    sio->uninstallDevice(PCLINK_CDEVIC);
    pclink->resetLink(no + 1);
    sio->installDevice(PCLINK_CDEVIC, pclink);
  }

  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));

  if (ask && img && img->isModified()) {
    QMessageBox::StandardButton answer;
    answer = saveImageWhenClosing(no, QMessageBox::No, 0);
    if (answer == QMessageBox::Cancel) {
      return false;
    }
  }

  sio->uninstallDevice(no + DISK_BASE_CDEVIC);
  if (img) {
    if (typeid(img) == typeid(DiskImages::FolderImage)) {
        tnfs.removeMountPoint(QDir(img->originalFileName()));
    }
    delete img;
    diskWidgets[no]->showAsEmpty(RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
    RespeqtSettings::instance()->unmountImage(no);
    updateRecentFileActions();
    deviceStatusChanged(no + DISK_BASE_CDEVIC);
    qDebug() << "!n" << tr("Unmounted disk %1").arg(no + 1);
  }
  return true;
}

char MainWindow::containingDiskSlot(const QPoint &point) {
  char i;
  QPoint distance = centralWidget()->geometry().topLeft();
  for (i = 0; i < DISK_COUNT; i++) {//
    QRect rect = diskWidgets[i]->geometry().translated(distance);
    if (rect.contains(point)) {
      break;
    }
  }
  if (i > DISK_COUNT - 1) {//
    i = -1;
  }
  return i;
}

char MainWindow::firstEmptyDiskSlot(char startFrom, bool createOne) {
  char i;
  for (i = startFrom; i < DISK_COUNT; i++) {//
    if (!sio->getDevice(DISK_BASE_CDEVIC + i)) {
      break;
    }
  }
  if (i > DISK_COUNT - 1) {//
    if (createOne) {
      i = DISK_COUNT - 1;
    } else {
      i = -1;
    }
  }
  emit newSlot(i);//
  return i;
}

void MainWindow::bootExe(const QString &fileName) {
  auto old = sio->getDevice(DISK_BASE_CDEVIC);
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
  if (!g_exefileName.isEmpty())
    bootExe(g_exefileName);
}

// Make boot executable dialog persistant until it's manually closed //
/*void MainWindow::keepBootExeOpen() {
  bootExe(g_exefileName);
}*/

void MainWindow::bootExeTriggered(const QString &fileName) {
  QString path = RespeqtSettings::instance()->lastRclDir();
  g_exefileName = path + "/" + fileName;
  if (!g_exefileName.isEmpty()) {
    RespeqtSettings::instance()->setLastExeDir(QFileInfo(g_exefileName).absolutePath());
    bootExe(g_exefileName);
  }
}

void MainWindow::selectBootExeTriggered() {
  auto dir = RespeqtSettings::instance()->lastExeDir();
  g_exefileName = QFileDialog::getOpenFileName(this, tr("Open executable"),
                                               dir,
                                               tr(
                                                       "Atari executables (*.xex *.com *.exe);;"
                                                       "All files (*)"));

  if (!g_exefileName.isEmpty()) {
    RespeqtSettings::instance()->setLastExeDir(QFileInfo(g_exefileName).absolutePath());
    bootExe(g_exefileName);
  }
}

void MainWindow::mountFileWithDefaultProtection(char no, const QString &fileName) {
  // If fileName was passed from RCL it is an 8.1 name, so we need to find
  // the full PC name in order to validate it.  //
  QString atariFileName, atariLongName;

  g_rclFileName = fileName;
  atariFileName = fileName;

  if (atariFileName.left(1) == "*") {
    atariLongName = atariFileName.mid(1);
    auto path = RespeqtSettings::instance()->lastRclDir();
    if (atariLongName == "") {
      sio->port()->writeDataNak();
      return;
    } else {
      atariFileName = path + "/" + atariLongName;
    }
  }

  /// not using initialization braces here (Mingw 4.9.2 treats them as std::initializer_list when auto is used)
  const auto imgSetting = RespeqtSettings::instance()->getImageSettingsFromName(atariFileName);
  auto prot = (!imgSetting.fileName.isEmpty()) && imgSetting.isWriteProtected;
  mountFile(no, atariFileName, prot);
}

void MainWindow::mountFile(char no, const QString &fileName, bool /*prot*/) {
  DiskImages::SimpleDiskImage *disk = nullptr;
  bool isDir = false;
  bool ask = true;

  if (fileName.isEmpty()) {
    if (g_rclFileName.left(1) == "*") emit fileMounted(false);
    return;
  }

  FileTypes::FileType type = FileTypes::getFileType(fileName);

  if (type == FileTypes::Dir) {
    disk = new DiskImages::FolderImage(sio, RespeqtSettings::instance()->limitFileEntries() ? 64 : -1);
    isDir = true;
    // TNFS informieren.
    tnfs.addMountPoint(fileName);
  } else {
    disk = installDiskImage();
  }

  if (disk) {
    auto oldDisk = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
    DiskImages::Board *board = oldDisk != nullptr ? oldDisk->getBoardInfo() : nullptr;
    if (g_rclFileName.left(1) == "*")
      ask = false;
    if (!disk->open(fileName, type) || !ejectImage(no, ask)) {
      RespeqtSettings::instance()->unmountImage(no);
      delete disk;
      delete board;

      if (g_rclFileName.left(1) == "*")
        emit fileMounted(false);
      return;
    } else if (board != nullptr) {
      disk->setBoardInfo(board);
      delete board;
    }

    sio->installDevice(DISK_BASE_CDEVIC + no, disk);

    try {
      auto pclink = dynamic_cast<PCLINK *>(sio->getDevice(PCLINK_CDEVIC));
      if (pclink != nullptr && (isDir || pclink->hasLink(no + 1))) {
        sio->uninstallDevice(PCLINK_CDEVIC);
        if (isDir) {
          pclink->setLink(no + 1, QDir::toNativeSeparators(fileName).toLatin1());
        } else {
          pclink->resetLink(no + 1);
        }
        sio->installDevice(PCLINK_CDEVIC, pclink);
      }
    } catch (std::bad_cast &e) {
      qDebug() << "!e " << tr("Bad cast for PCLINK");
    }

    diskWidgets[no]->updateFromImage(disk, RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());

    RespeqtSettings::instance()->mountImage(no, fileName, disk->isReadOnly());
    updateRecentFileActions();
    // TODO Test
    connect(disk, &DiskImages::SimpleDiskImage::statusChanged, this, &MainWindow::deviceStatusChanged, Qt::QueuedConnection);
    deviceStatusChanged(DISK_BASE_CDEVIC + no);

    // Extract the file name without the path //
    QString filenamelabel;
    int i = fileName.lastIndexOf("/");
    if (i != -1) {
      i++;
      filenamelabel = fileName.right(fileName.size() - i);
    }

    qDebug() << "!n" << tr("[%1] Mounted '%2' as '%3'.")
                                .arg(disk->deviceName(), filenamelabel, disk->description());

    if (g_rclFileName.left(1) == "*") emit fileMounted(true);//
  }
}

DiskImages::SimpleDiskImage *MainWindow::installDiskImage() {
  auto disk = new DiskImages::SimpleDiskImage(sio);
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

void MainWindow::mountDiskImage(char no) {
  QString dir;

  if (no < 0) {
    no = firstEmptyDiskSlot(0, true);
  }
  dir = RespeqtSettings::instance()->lastDiskImageDir();
  QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open a disk image"),
    dir,
    tr(
            "All Atari disk images (*.atr *.xfd *.atx *.pro);;"
            "SIO2PC ATR images (*.atr);;"
            "XFormer XFD images (*.xfd);;"
            "ATX images (*.atx);;"
            "Pro images (*.pro);;"
            "All files (*)"
        )
    );
  if (fileName.isEmpty()) {
    return;
  }
  RespeqtSettings::instance()->setLastDiskImageDir(QFileInfo(fileName).absolutePath());
  mountFileWithDefaultProtection(no, fileName);
}

void MainWindow::mountFolderImage(char no) {
  QString dir;

  if (no < 0) {
    no = firstEmptyDiskSlot(0, true);
  }
  // Always mount from "last folder dir" //
  dir = RespeqtSettings::instance()->lastFolderImageDir();
  QString fileName = QFileDialog::getExistingDirectory(this, tr("Open a folder image"), dir);
  fileName = QDir::fromNativeSeparators(fileName);//
  if (fileName.isEmpty()) {
    return;
  }
  RespeqtSettings::instance()->setLastFolderImageDir(fileName);
  mountFileWithDefaultProtection(no, fileName);
  //tnfs.addMountPoint(fileName);
}

void MainWindow::loadNextSide(char no) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
  mountFileWithDefaultProtection(no, img->getNextSideFilename());
}

void MainWindow::toggleHappy(char no, bool enabled) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
  img->setHappyMode(enabled);
}

void MainWindow::toggleChip(char no, bool open) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
  img->setChipMode(open);
  updateHighSpeed();
}

void MainWindow::toggleOSB(char no, bool open) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
  img->setOSBMode(open);
}

void MainWindow::toggleToolDisk(char no, bool enabled) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
  img->setToolDiskMode(enabled);
  updateHighSpeed();
}

void MainWindow::toggleWriteProtection(char no, bool protectionEnabled) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
  img->setReadOnly(protectionEnabled);
  RespeqtSettings::instance()->setMountedImageProtection(no, protectionEnabled);
}

void MainWindow::openEditor(char no) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
  if (img->editDialog()) {
    img->editDialog()->close();
  } else {
    auto dlg = new DiskEditDialog();
    dlg->go(img);
    dlg->show();
  }
}

QMessageBox::StandardButton MainWindow::saveImageWhenClosing(char no, QMessageBox::StandardButton previousAnswer, int number) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));

  if (previousAnswer != QMessageBox::YesToAll) {
    QMessageBox::StandardButtons buttons;
    if (number) {
      buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::NoToAll | QMessageBox::Cancel;
    } else {
      buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
    }
    previousAnswer = QMessageBox::question(this, tr("Image file unsaved"), tr("'%1' has unsaved changes, do you want to save it?").arg(img->originalFileName()), buttons);
  }
  if (previousAnswer == QMessageBox::Yes || previousAnswer == QMessageBox::YesToAll) {
    saveDisk(no);
  }
  if (previousAnswer == QMessageBox::Close) {
    previousAnswer = QMessageBox::Cancel;
  }
  return previousAnswer;
}

void MainWindow::loadTranslators() {
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

void MainWindow::saveDisk(char no) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));

  if (img->isUnnamed()) {
    saveDiskAs(no);
  } else {
    img->lock();
    bool saved = img->save();
    img->unlock();
    if (!saved) {
      if (QMessageBox::question(this, tr("Save failed"), tr("'%1' cannot be saved, do you want to save the image with another name?").arg(img->originalFileName()), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        saveDiskAs(no);
      }
    }
  }
}
//
void MainWindow::autoCommit(char no, bool st) {
  if (no < DISK_COUNT) {
    if ((diskWidgets[no]->isAutoSaveEnabled() && st) || (!diskWidgets[no]->isAutoSaveEnabled() && !st))
      diskWidgets[no]->triggerAutoSaveClickIfEnabled();
  }
}


void MainWindow::happy(char no, bool st) {
  if (no < DISK_COUNT) {
    if ((diskWidgets[no]->isHappyEnabled() && st) || (!diskWidgets[no]->isHappyEnabled() && !st))
      diskWidgets[no]->triggerHappyClickIfEnabled();
  }
}


void MainWindow::chip(char no, bool st) {
  if (no < DISK_COUNT) {
    if ((diskWidgets[no]->isChipEnabled() && st) || (!diskWidgets[no]->isChipEnabled() && !st))
      diskWidgets[no]->triggerChipClickIfEnabled();
  }
}

void MainWindow::autoSaveDisk(char no) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));

  DriveWidget *widget = diskWidgets[no];

  if (img->isUnnamed()) {
    saveDiskAs(no);
    widget->updateFromImage(img, RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
    return;
  }

  bool autoSaveEnabled = widget->isAutoSaveEnabled();

  if (autoSaveEnabled) {
    qDebug() << "!n" << tr("[Disk %1] Auto-commit ON.").arg(no + 1);
  } else {
    qDebug() << "!n" << tr("[Disk %1] Auto-commit OFF.").arg(no + 1);
  }

  bool saved;

  img->lock();
  saved = img->save();
  img->unlock();
  if (!saved) {
    if (QMessageBox::question(this, tr("Save failed"), tr("'%1' cannot be saved, do you want to save the image with another name?").arg(img->originalFileName()), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
      saveDiskAs(no);
    }
  }
  widget->updateFromImage(img, RespeqtSettings::instance()->hideHappyMode(), RespeqtSettings::instance()->hideChipMode(), RespeqtSettings::instance()->hideNextImage(), RespeqtSettings::instance()->hideOSBMode(), RespeqtSettings::instance()->hideToolDisk());
}

//
void MainWindow::saveDiskAs(char no) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
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
      if (QMessageBox::question(this, tr("Save failed"), tr("'%1' cannot be saved, do you want to save the image with another name?").arg(fileName), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) {
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

void MainWindow::revertDisk(char no) {
  auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(no + DISK_BASE_CDEVIC));
  if (QMessageBox::question(this, tr("Revert to last saved"),
                            tr("Do you really want to revert '%1' to its last saved state? You will lose the changes that has been made.")
                                    .arg(img->originalFileName()),
                            QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
    img->lock();
    img->reopen();
    img->unlock();
    deviceStatusChanged(DISK_BASE_CDEVIC + no);
  }
}

// Slots for handling actions for devices.
void MainWindow::mountDiskTriggered(char no) { mountDiskImage(no); }
void MainWindow::mountFolderTriggered(char no) { mountFolderImage(no); }
void MainWindow::ejectTriggered(char no) { ejectImage(no); }
void MainWindow::nextSideTriggered(char no) { loadNextSide(no); }
void MainWindow::happyToggled(char no, bool open) { toggleHappy(no, open); }
void MainWindow::chipToggled(char no, bool open) { toggleChip(no, open); }
void MainWindow::OSBToggled(char no, bool open) { toggleOSB(no, open); }
void MainWindow::toolDiskTriggered(char no, bool open) { toggleToolDisk(no, open); }
void MainWindow::protectTriggered(char no, bool writeProtectEnabled) { toggleWriteProtection(no, writeProtectEnabled); }
void MainWindow::editDiskTriggered(char no) { openEditor(no); }
void MainWindow::saveTriggered(char no) { saveDisk(no); }
void MainWindow::autoSaveTriggered(char no) { autoSaveDisk(no); }
void MainWindow::saveAsTriggered(char no) { saveDiskAs(no); }
void MainWindow::revertTriggered(char no) { revertDisk(no); }

void MainWindow::ejectAllTriggered() {
  QMessageBox::StandardButton answer = QMessageBox::No;

  int toBeSaved = 0;

  for (int i = 0; i < DISK_COUNT; i++) {//
    auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(i + DISK_BASE_CDEVIC));
    if (img && img->isModified()) {
      toBeSaved++;
    }
  }

  if (!toBeSaved) {
    for (char i = DISK_COUNT - 1; i >= 0; i--) {
      ejectImage(i);
    }
    return;
  }

  bool wasRunning = ui->actionStartEmulation->isChecked();
  if (wasRunning) {
    ui->actionStartEmulation->trigger();
  }

  for (char i = DISK_COUNT - 1; i >= 0; i--) {
    auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(i + DISK_BASE_CDEVIC));
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
  for (char i = DISK_COUNT - 1; i >= 0; i--) {
    ejectImage(i, false);
  }
  if (wasRunning) {
    ui->actionStartEmulation->trigger();
  }
}

void MainWindow::newImageTriggered() {
  CreateImageDialog dlg(this);
  if (!dlg.exec()) {
    return;
  }

  auto no = firstEmptyDiskSlot(0, true);

  auto disk = installDiskImage();
  // TODO Test
  connect(disk, &DiskImages::SimpleDiskImage::statusChanged, this, &MainWindow::deviceStatusChanged, Qt::QueuedConnection);

  if (!disk->create(++untitledName)) {
    delete disk;
    return;
  }

  DiskImages::DiskGeometry g;
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
                              .arg(disk->deviceName(), disk->originalFileName(), disk->description());
}

void MainWindow::openSessionTriggered() {
  auto dir = RespeqtSettings::instance()->lastSessionDir();
  auto fileName = QFileDialog::getOpenFileName(this, tr("Open session"),
                                               dir,
                                               tr(
                                                       "RespeQt sessions (*.respeqt);;"
                                                       "All files (*)"));
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

  for (char i = 0; i < DISK_COUNT; i++) {
    RespeqtSettings::ImageSettings is;
    is = RespeqtSettings::instance()->mountedImageSetting(i);
    mountFile(i, is.fileName, is.isWriteProtected);
  }

  restoreLayout();

  setSession();
}

void MainWindow::saveSessionTriggered() {
  auto dir = RespeqtSettings::instance()->lastSessionDir();
  auto fileName = QFileDialog::getSaveFileName(this, tr("Save session as"),
                                               dir,
                                               tr(
                                                       "RespeQt sessions (*.respeqt);;"
                                                       "All files (*)"));
  if (fileName.isEmpty()) {
    return;
  }
  RespeqtSettings::instance()->setLastSessionDir(QFileInfo(fileName).absolutePath());

  // Save mainwindow position and size to session file //
  RespeqtSettings::instance()->saveMainWinGeometry(this, isMiniMode);

  if (diskBrowserDlg) {
    RespeqtSettings::instance()->setShowDiskBrowser(diskBrowserDlg->isVisible());
    RespeqtSettings::instance()->saveWidgetGeometry(diskBrowserDlg);
    RespeqtSettings::instance()->setDiskBrowserHorzSplitPos(diskBrowserDlg->getHorzSplitPos());
    RespeqtSettings::instance()->setDiskBrowserVertSplitPos(diskBrowserDlg->getVertSplitPos());
  } else {
    RespeqtSettings::instance()->setShowDiskBrowser(false);
  }

  if (logWindow_) {
    RespeqtSettings::instance()->setShowLogWindow(logWindow_->isVisible());
    RespeqtSettings::instance()->saveWidgetGeometry(logWindow_);
  } else {
    RespeqtSettings::instance()->setShowLogWindow(false);
  }

  RespeqtSettings::instance()->saveSessionToFile(fileName);
}

/*void MainWindow::textPrinterWindowClosed() {
  ui->actionShowPrinterTextOutput->setChecked(false);
}*/

void MainWindow::cassettePlaybackTriggered() {
  auto fileName = QFileDialog::getOpenFileName(this,
                                               tr("Open a cassette image"),
                                               RespeqtSettings::instance()->lastCasDir(),
                                               tr("CAS images (*.cas);;All files (*)"));
  if (fileName.isEmpty()) {
    return;
  }
  RespeqtSettings::instance()->setLastCasDir(QFileInfo(fileName).absolutePath());

  bool restart;
  restart = ui->actionStartEmulation->isChecked();
  if (restart) {
    ui->actionStartEmulation->trigger();
    sio->waitOnPort();
    qApp->processEvents();
  }

  auto dlg = new CassetteDialog(this, fileName);
  dlg->exec();
  delete dlg;

  if (restart) {
    ui->actionStartEmulation->trigger();
  }
}

void MainWindow::quitApplicationTriggered() {
  close();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
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
  (void) reason;// Suppress the unused parameter warning.
#endif
}

void MainWindow::bootOptionTriggered() {
  QString folderPath = RespeqtSettings::instance()->mountedImageSetting(0).fileName;
  BootOptionsDialog bod(folderPath, this);
  bod.exec();
}

void MainWindow::diskBrowserTriggered() {
  if (!diskBrowserDlg) {
    diskBrowserDlg = new DiskBrowserDlg(sio, this);
  }
  diskBrowserDlg->showNormal();
}

// This connect the signal from UI to slots
void MainWindow::connectUISignal() {
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
  connect(ui->actionDiskBrowser, &QAction::triggered, this, &MainWindow::diskBrowserTriggered);
  connect(ui->actionCaptureSnapshot, &QAction::toggled, this, &MainWindow::toggleSnapshotCapture);
  connect(ui->actionReplaySnapshot, &QAction::triggered, this, &MainWindow::replaySnapshot);
}

void MainWindow::toggleSnapshotCapture(bool toggle) {
  if (sio) {
    auto recorder = SioRecorder::instance();

    if (!recorder->isSnapshotRunning()) {
      recorder->startSIOSnapshot();
    } else {
      auto snapshot = recorder->stopSIOSnapshot();
      auto fileName = QFileDialog::getSaveFileName(MainWindow::instance(),
                                                   tr("Save test Json File"), QString(), tr("Json Files (*.json)"));
      QFile file{fileName};
      file.open(QFile::WriteOnly | QFile::Truncate);
      file.write(snapshot);
      file.close();
    }
  }
  ui->actionCaptureSnapshot->setChecked(toggle);
}

void MainWindow::replaySnapshot() {
  if (sio->isRunning() && RespeqtSettings::instance()->backend() != SerialBackend::TEST) {
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
  auto recorder = SioRecorder::instance();
  recorder->prepareReplaySnapshot(&file, RespeqtSettings::instance()->backend());
  file.close();
  // If SIO is connected aka running, then first disconnect
  if (sio->isRunning())
    ui->actionStartEmulation->trigger();
  // Now set SioRecorder to be the serial port and start the connection again.
  RespeqtSettings::instance()->setBackend(SerialBackend::TEST);
  ui->actionStartEmulation->trigger();
}

void MainWindow::setupDebugItems() {
  ui->menuDebug->menuAction()->setVisible(RespeqtSettings::instance()->debugMenuVisible());
}

void MainWindow::toggleLimitEntriesTriggered() {
  if (RespeqtSettings::instance()->limitFileEntries()) {
    limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock_open.png").pixmap(16, 16, QIcon::Normal));
  } else {
    limitEntriesLabel->setPixmap(QIcon(":/icons/silk-icons/icons/lock.png").pixmap(16, 16, QIcon::Normal));
  }
  RespeqtSettings::instance()->setlimitFileEntries(!RespeqtSettings::instance()->limitFileEntries());
}

void MainWindow::restoreLayout()
{
    if (RespeqtSettings::instance()->saveDiskVis())
        isD9DOVisible = RespeqtSettings::instance()->D9DOVisible();

    if (RespeqtSettings::instance()->saveWindowsPos())
    {
        isMiniMode = RespeqtSettings::instance()->miniMode();
        if (isMiniMode)
        {                                           // check if mini-mode was last used
            isMiniMode = false;                     // reset now so we can toggle it ON
            ui->actionToggleMiniMode->trigger();    // trigger mini-mode toggle action
        }
        else
        {
            showHideDrives();
        }
        RespeqtSettings::instance()->restoreMainWinGeometry(this, isMiniMode);

        if (RespeqtSettings::instance()->showDiskBrowser() && !(diskBrowserDlg && diskBrowserDlg->isVisible()))
            diskBrowserTriggered();

        if (RespeqtSettings::instance()->showLogWindow() && !(logWindow_ && logWindow_->isVisible()))
            showLogWindowTriggered();
    }
}

bool MainWindow::checkChangeDbSource(DbDataSource dbSourceNew)
{
    DbDataSource dbSourceNow = RespeqtSettings::instance()->dbDataSource();
    if (dbSourceNew == dbSourceNow)
        return false;

    auto &sDbSettings = RespeqtSettings::dbSettings();
    if (!sDbSettings || sDbSettings->isEmpty())
        return true;

    if (!diskBrowserDlg)
        return true;

    if (diskBrowserDlg->isVisible())
    {
        QMessageBox::warning(diskBrowserDlg, tr("Data Format Conversion Required"),
               tr("The Disk Collection Browser Window is currently open.\n"
                  "To convert DCB data format, please close and try again."));
        return false;
    }

    diskBrowserDlg->close();
    diskBrowserDlg = nullptr;
    return true;
}

void MainWindow::allSessionsDisconnected()
{
    netLabel->setPixmap(QIcon(":/icons/oxygen-icons/16x16/actions/network_disconnect.png").pixmap(16, 16, QIcon::Normal));
    netLabel->setToolTip(tr("No TNFS connections"));
    netLabel->setStatusTip(netLabel->toolTip());
}

void MainWindow::sessionConnected()
{
    netLabel->setPixmap(QIcon(":/icons/oxygen-icons/16x16/actions/network_connect.png").pixmap(16, 16, QIcon::Normal));
    netLabel->setToolTip(tr("TNFS connected"));
    netLabel->setStatusTip(netLabel->toolTip());
}
