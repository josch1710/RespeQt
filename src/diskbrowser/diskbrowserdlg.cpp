/*
 * diskbrowserdlg.cpp - implementation of RespeQt's DiskBrowserDlg class.
 *
 * Copyright 2023 D.Caputi
 */
#include "diskbrowser/diskbrowserdlg.h"
#include "diskimages/diskimage.h"
#include "filesystems/ataridirentry.h"
#include "filesystems/atarifilesystem.h"
#include "filesystems/dos10filesystem.h"
#include "filesystems/dos20filesystem.h"
#include "filesystems/dos25filesystem.h"
#include "filesystems/mydosfilesystem.h"
#include "filesystems/spartadosfilesystem.h"
#include "diskbrowser/folderdisks.h"
#include "diskbrowser/dbini.h"
#include "diskbrowser/dbjson.h"
#include "mainwindow.h"
#include "miscutils.h"
#include "respeqtsettings.h"
#include "ui_diskbrowserdlg.h"
#include <QFileDialog>
#include <QObject>
#include <QImageReader>
#include <QMenu>
#include <QStandardPaths>
#include <QInputDialog>

DiskBrowserDlg::DiskBrowserDlg(SioWorkerPtr pSio, QWidget *parent)
    : QDialog(parent), ui(new Ui::DiskBrowserDlg)
{
    sio = std::move(pSio);

    ui->setupUi(this);

    ui->splitTopDirBotPng->setOther(ui->splitLeftAtrRightDirPng);
    ui->splitLeftAtrRightDirPng->setOther(ui->splitTopDirBotPng);

    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowseFolder()));
    connect(ui->treeDisks, &QTreeWidget::itemSelectionChanged, this, &DiskBrowserDlg::itemSelectionChanged);
    connect(ui->treeDisks, &QTreeWidget::itemDoubleClicked, this, &DiskBrowserDlg::itemDoubleClicked);
    connect(ui->cboFolderPath, SIGNAL(currentTextChanged(QString)), this, SLOT(onFolderChanged(QString)));
    connect(ui->picPreview, &PicPreview::sigPopupMenuReq, this, &DiskBrowserDlg::popupMenuReq);
    connect(ui->picPreview, &PicPreview::sigTitleChanged, this, &DiskBrowserDlg::titleChanged);
    connect(ui->picPreview, &PicPreview::sigIndexChanged, this, &DiskBrowserDlg::indexChanged);

    if (RespeqtSettings::instance()->dbDataSource() == DbData_appSettings)
        _dbSettings = new DbIni();
    else
        _dbSettings = new DbJson();

    refreshFoldersCombobox();
    onFolderChanged(""); // reload the current item 0 in the combo
}

DiskBrowserDlg::~DiskBrowserDlg()
{
    delete ui;
    delete _dbSettings;
}

void DiskBrowserDlg::clear()
{
    ui->picPreview->clear();
    ui->lblFileList->clear();

    ui->treeDisks->blockSignals(true);
    ui->treeDisks->clear();
    ui->treeDisks->setColumnCount(1);
    ui->treeDisks->setHeaderHidden(true);
    ui->treeDisks->setRootIsDecorated(false);
    ui->treeDisks->blockSignals(false);
}

QString DiskBrowserDlg::getMostRecentFolder()
{
    QString fileName = RespeqtSettings::instance()->mostRecentBrowserFolder();
    QFileInfo fileInfo = QFileInfo(fileName);

    if (fileInfo.isFile())
        return fileInfo.path();

    return fileName;
}

QString DiskBrowserDlg::getMostRecentDisk()
{
    QString fileName = RespeqtSettings::instance()->mostRecentBrowserFolder();
    QFileInfo fileInfo = QFileInfo(fileName);

    if (fileInfo.isFile())
        return fileInfo.fileName();

    return fileName;
}

void DiskBrowserDlg::onBrowseFolder()
{
    QString lastDir = getMostRecentFolder();
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open folder containing disk images"), lastDir);

    dirName = QDir::fromNativeSeparators(dirName);
    if (dirName.isEmpty())
        return;

    onFolderChanged(dirName);
}

void DiskBrowserDlg::onFolderChanged(QString folder)
{
    if (folder.isEmpty())
        folder = ui->cboFolderPath->currentText();
    if (folder.isEmpty())
        return;

    if (!QFileInfo::exists(folder))
    {
        RespeqtSettings::instance()->delMostRecentBrowserFolder(folder);
        refreshFoldersCombobox();
        return;
    }

    if (RespeqtSettings::instance()->dbDataSource() == DbData_subDir)
        _dbSettings->setDataDir(folder);

    QString disk = getRecentDisk(folder);
    QString path = disk.isEmpty() ? folder : folder + "/" + disk;
    // NOTE: MRU scheme uses "path" for both folder *and* files (if one is selected)

    RespeqtSettings::instance()->setMostRecentBrowserFolder(path);

    refreshFoldersCombobox();

    _folderDisks.load(folder);

    clear();// clear disk collection browser contents

    // fill in any sub-directories
    auto folders = _folderDisks.folders();
    foreach (const QString &subdir, folders)
    {
        if (subdir.startsWith('.') && (subdir != ".."))     // hide .folders on Windows
            continue;

        auto item = new QTreeWidgetItem(ui->treeDisks);
        auto icon = QIcon{":/icons/silk-icons/icons/folder_explore.png"};
        item->setText(0, subdir);
        item->setIcon(0, icon);
        setItemIsFolder(item, true);
    }

    // fill in all disk image files
    auto disks = _folderDisks.disks();
    foreach (const QString &disk, disks)
    {
        auto item = new QTreeWidgetItem(ui->treeDisks);
        auto icon = QIcon{":/icons/other-icons/floppy.png"};
        item->setText(0, disk);
        item->setIcon(0, icon);
        setItemIsFolder(item, false);
    }

    if (disk.isEmpty())
    {
        // select the currently mounted disk if present in the list
        auto imageSet = RespeqtSettings::instance()->mountedImageSetting(0);
        QString pathName = imageSet.fileName;

        disk = QFileInfo(pathName).fileName();
    }

    if (!disk.isEmpty() && disks.contains(disk))
    {
        auto items = ui->treeDisks->findItems(disk, Qt::MatchExactly);
        QTreeWidgetItem *item = (items.length() > 0) ? items[0] : nullptr;
        ui->treeDisks->setCurrentItem(item);
    }
}

void DiskBrowserDlg::refreshFoldersCombobox()
{
    ui->cboFolderPath->blockSignals(true);
    ui->cboFolderPath->clear();

    QStringList folders;    // build a list of MRU folders for the dropdown list

    foreach (const QString& name, RespeqtSettings::instance()->recentBrowserFolders())
    {
        auto fileInf = QFileInfo(name);
        if (fileInf.exists())
        {
            QString path = fileInf.isFile() ? fileInf.path() : name;// don't want file names in dropdown
            folders += path;
        }
        else if (isDiskImage(name)) // MRU missing. First check if a disk is selected
        {
            QString path = getParentDir(name);
            if (QFileInfo::exists(path))
            {
                folders += path;    // Keep parent folder of bad disk
            }
            else
            {
                qDebug() << "!w" << tr("Disk collection Browser most recent list updated. '%1' not found.").arg(name);
                RespeqtSettings::instance()->delMostRecentBrowserFolder(name);
            }
        }
        else    // Simple case of missing folder
        {
            qDebug() << "!w" << tr("Disk collection Browser most recent list updated. Folder '%1' not found.").arg(name);
            RespeqtSettings::instance()->delMostRecentBrowserFolder(name);
        }
    }
    ui->cboFolderPath->addItems(folders);
    ui->cboFolderPath->setCurrentIndex(0);
    ui->cboFolderPath->blockSignals(false);
}

void DiskBrowserDlg::itemSelectionChanged()
{
    update();
}

void DiskBrowserDlg::update()
{
    if (!ui->treeDisks->topLevelItemCount() || !ui->treeDisks->currentItem())
        return;

    auto currentItem = ui->treeDisks->currentItem();

    if (itemIsFolder(currentItem))
    {
        ui->picPreview->clear();
        ui->lblFileList->clear();
        return;
    }

    QString diskName = currentItem->text(0);
    QString pathName = ui->cboFolderPath->currentText();
    QString fullName = pathName + QString("/") + diskName;
    QFileInfo fiDisk = QFileInfo(fullName);

    if (!fiDisk.exists())
    {
        // TBD: remove item
        Q_ASSERT(0);
        return;
    }

    _diskFullName = fullName;
    _diskFileName = diskName;
    _currentDir = pathName;

    RespeqtSettings::instance()->setMostRecentBrowserFolder(fullName);
    MainWindow::instance()->mountFileWithDefaultProtection(0, fullName);

    QString fileList;   // get a list of files to show in the file list pane

    if (sio)
    {
        const int deviceNo = 0;
        auto img = qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(deviceNo + DISK_BASE_CDEVIC));
        int fs = img->defaultFileSystem();
        Filesystems::AtariFileSystem *atariFs = nullptr;

        switch (fs)
        {
            case 1: atariFs = new Filesystems::Dos10FileSystem(img); break;
            case 2: atariFs = new Filesystems::Dos20FileSystem(img); break;
            case 3: atariFs = new Filesystems::Dos25FileSystem(img); break;
            case 4: atariFs = new Filesystems::MyDosFileSystem(img); break;
            case 5: atariFs = new Filesystems::SpartaDosFileSystem(img); break;
            default: break;
        }

        if (atariFs)
        {
            int dir = atariFs->rootDir();
            auto entries = atariFs->getEntries(dir);

            foreach (const Filesystems::AtariDirEntry& entry, entries)
                fileList += entry.name() + "\n";

            if (fileList.isEmpty())
                fileList = tr("!Root dir empty:\nNo Files");
        }
        else
        {
            fileList = tr("!File system not recognized:\nNo Files");
        }
    }
    else
    {
        fileList = tr("!SIO device not available:\nNo Files");
    }

    if (fileList[0] == '!') // detected error/annomally parsing filesystem above
    {
        qDebug() << fileList;
        fileList[0] = '\n';
    }

    double oldRatio = ui->picPreview->ratio();

    ui->lblFileList->setText(fileList);
    ui->picPreview->clear();
    _picInfo.clear();

    bool favorJson = RespeqtSettings::instance()->dbFavorJson();
    bool fileNames = RespeqtSettings::instance()->dbUseFileNames();

    // find a custom or built-in pic

    auto fileInfo = QFileInfo {_diskFullName};
    QDir dir {fileInfo.absolutePath()};

    if (fileNames)
    {
        _picInfo.label = parsePicLabel();
        _picInfo.pic   = findPicFile();
    }
    if (_picInfo.pic.isEmpty() || favorJson)
        _picInfo.pic = _dbSettings->getPicture(dir, _diskFileName, _picSource);
    if (_picInfo.label.isEmpty() || favorJson)
        _picInfo.label = _dbSettings->getLabel(dir, _diskFileName);

    if (_picInfo.pic.isEmpty())
    {
        _picInfo.pic = getFloppyPic();
        if (!_picInfo.pic.isEmpty())
        {
            _picSource = PicSource_floppy;
            if (_picInfo.label.isEmpty())
                _picInfo.label.title = fileInfo.completeBaseName();
            ui->picPreview->setLabel(_picInfo.label);
        }
    }

    ui->picPreview->setFileName(_picInfo.pic);

    double newRatio = ui->picPreview->ratio();

    if (oldRatio != newRatio)
    {
        if (newRatio == 0.0)
            newRatio = 1.0;

        ui->splitLeftAtrRightDirPng->setRatio(newRatio);
        ui->splitTopDirBotPng->setRatio(newRatio, true);
    }
}

QString DiskBrowserDlg::getRecentDisk(QString folder)
{
    foreach (const QString &text, RespeqtSettings::instance()->recentBrowserFolders())
    {
        auto fi = QFileInfo(text);
        if (fi.isFile() && (fi.path() == folder))
            return fi.fileName();
    }

    return QString();
}

QString DiskBrowserDlg::getParentDir(QString fileFolder)
{
    int lastSlash = fileFolder.lastIndexOf('/');    // TBD: what if '\'?
    if (lastSlash >= 0)
        fileFolder.truncate(lastSlash);

    return fileFolder;
}

bool DiskBrowserDlg::isDiskImage(const QString &name)
{
    foreach (const QString &fileType, FileTypes::getDiskImageTypes())
    {
        QString ext = fileType.right(4);
        if (name.endsWith(ext, osCaseSensitivity()))
            return true;
    }
    return false;
}

int DiskBrowserDlg::getHorzSplitPos()
{
    return ui->splitLeftAtrRightDirPng->sizes().at(0);
}

int DiskBrowserDlg::getVertSplitPos()
{
    return ui->splitTopDirBotPng->sizes().at(0);
}

void DiskBrowserDlg::setHorzSplitPos(int pos)
{
    if (pos == -1)
        return;

    QList<int> sizes = ui->splitLeftAtrRightDirPng->sizes();
    int total = sizes.at(0) + sizes.at(1);
    sizes.clear();
    sizes << pos << (total - pos);
    ui->splitLeftAtrRightDirPng->setSizes(sizes);
}

void DiskBrowserDlg::setVertSplitPos(int pos)
{
    if (pos == -1)
        return;

    QList<int> sizes = ui->splitTopDirBotPng->sizes();
    int total = sizes.at(0) + sizes.at(1);
    sizes.clear();
    sizes << pos << (total - pos);
    ui->splitTopDirBotPng->setSizes(sizes);
}

void DiskBrowserDlg::itemDoubleClicked(QTreeWidgetItem *item, int)
{
    QString text = item->text(0);
    auto path = ui->cboFolderPath->currentText();

    if (text == "..")
    {
        QDir dir(path);

        if (dir.cdUp())
            onFolderChanged(dir.absolutePath());
    }
    else if (itemIsFolder(item))
    {
        path = path + "/" + text;
        onFolderChanged(path);
    }
}

void DiskBrowserDlg::setItemIsFolder(QTreeWidgetItem *item, bool isFolder)
{
    item->setData(0, Qt::UserRole, isFolder);
}

bool DiskBrowserDlg::itemIsFolder(QTreeWidgetItem *item)
{
    return item->data(0, Qt::UserRole).toBool();
}

void DiskBrowserDlg::closeEvent(QCloseEvent *event)
{
    if (!event->spontaneous()) // linux fires 2 close events (2nd one is !visible)...
    {
        RespeqtSettings::instance()->saveWidgetGeometry(this);
        RespeqtSettings::instance()->setShowDiskBrowser(isVisible());
        RespeqtSettings::instance()->setDiskBrowserHorzSplitPos(getHorzSplitPos());
        RespeqtSettings::instance()->setDiskBrowserVertSplitPos(getVertSplitPos());
    }
    event->accept();
}

void DiskBrowserDlg::showEvent(QShowEvent *event)
{
    static int count = 0;

    QDialog::showEvent(event);

    if (event->type() != QEvent::Show)
        return;

    if (!count && RespeqtSettings::instance()->saveWindowsPos())
    {
        // Restore last widget geometry
        RespeqtSettings::instance()->restoreWidgetGeometry(this);
        setHorzSplitPos(RespeqtSettings::instance()->diskBrowserHorzSplitPos());
        setVertSplitPos(RespeqtSettings::instance()->diskBrowserVertSplitPos());
    }

    if (count++)
        update();
}

DiskLabel DiskBrowserDlg::parsePicLabel()
{
    DiskLabel label;

    auto fileInfo = QFileInfo {_diskFullName};

    Q_ASSERT(fileInfo.exists());    // validated prior to this call

    QString baseName = fileInfo.completeBaseName();

    static QRegularExpression re("(^\\d+)([b|B]?)(\\.?)(.*)");
    auto rem = re.match(baseName);

    if (rem.hasMatch())
    {
        QString index = rem.captured(1);
        QString title  = rem.captured(4);
        bool isSideB = rem.captured(2).toUpper() == "B";

        label = DiskLabel {title, index, isSideB};
    }
    else
    {
        label = DiskLabel(baseName);
    }

    return label;
}

// findPicFile (formally findImage) is my second pass at a scheme for preview pics. The pics/images are placed in the folder along side disk images.
//  (note: an "image file" in this context is a digital picture, not an ATR disk!)
// A complex, yet flexible naming convention allows the app to grab corresponding pics and labeling data (for rendering on a blank/built in floppy pic)
// and is organized and prioritized as follows:
// 1. Basename with viable image extension.
//    ex: DiskBaseName.png or *.jpg, etc... any Qt supported format can be used.
// 2. Matching index prefixed image file in the disk folder. Note index B designates the reverse side of a floppy disk.
//    ex: disk name = 12b.Title of Disk.ATR ("Title of Disk" appears on the mock label if no PNG present)
//        image file = 12b.Menu Screen.PNG (screen shot of a disk that boots to a game menu)
//    or: image file = 12b.PNG
// 3. Use the hard-coded/generic name respeqt_db.* for default thumbnail.
//    ex: respeqt_db.png
// 4. TBD: check INI file scheme for an image to load
//
// formerly step 5 was: Default to loading a built-in image of a 5 1/2-inch floppy disk (moved to getFloppyPic)
//
QString DiskBrowserDlg::findPicFile()
{
    auto fileInfo = QFileInfo {_diskFullName};
    auto diskBase = fileInfo.completeBaseName();
    QDir dir {fileInfo.absolutePath()};
    QDir subdir {fileInfo.absolutePath() + "/.respeqt_db"};
    auto formats = QImageReader::supportedImageFormats();
    auto fmtlist = toStringList(formats);
    auto entries = dir.entryInfoList(fmtlist);
    auto bsidexp = _picInfo.label.sideB ? QString("[b|B]") : QString();
    auto sregexp = QString("^(%1)(%2)(\\.)(.*)").arg(_picInfo.label.index).arg(bsidexp);
    auto qregexp = QRegularExpression {sregexp};

    if (subdir.exists())
        entries += subdir.entryInfoList(fmtlist);   // also scan ./.respeqt_db sub-dir for pics

    foreach (const QFileInfo& entry, entries)
    {
        // 1. check for basename with viable image extension

        const QString entryName {entry.completeBaseName()};
        if (entryName == diskBase)
        {
            setToolTip(entryName);
            _picSource = PicFromFname_base;
            return entry.absoluteFilePath();
        }

        // 2. check for matching indexing filename prefix

        if (!_picInfo.label.index.isEmpty())     // check if current disk has index prefix NN. or NNb.
        {
            auto basename = entry.completeBaseName();
            auto matcher = qregexp.match(basename);
            if (matcher.hasMatch())
            {
                QString tip = matcher.captured(4);  // use mid string as tooltip
                setToolTip(tip);
                _picSource = PicFromFname_index;
                return entry.absoluteFilePath();
            }
        }

        // 3. use generic name (i.e. "respeqt_db.png") for default thumbnail

        auto imagePath = fileInfo.path() + "/respeqt_db." + entry.suffix();
        if (!QFileInfo::exists(imagePath))
            imagePath = fileInfo.path() + "/.respeqt_db/respeqt_db." + entry.suffix();
        if (QFileInfo::exists(imagePath))
        {
            setToolTip("");
            _picSource = PicFromFname_dir;
            return imagePath;
        }
    }
    return QString();
}

QString DiskBrowserDlg::getFloppyPic()  // 5. load built-in image of a 5 1/2-inch floppy disk
{

    if (_picInfo.label.sideB)
        return FLOPPY_BACKSIDE_PNG; // same 2 labels as default, but flip-side of double-sided floppy

    return FLOPPY_INDEXED_PNG;      // default has 2 labels: (small) disk no./index & (large) title/content
}

void DiskBrowserDlg::popupMenuReq(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(QIcon(":/icons/silk-icons/icons/image.png"), "Set Default Preview...", this, &DiskBrowserDlg::actionSetDefault);
    menu.addAction(QIcon(":/icons/silk-icons/icons/folder_image.png"), "Set Folder Preview Pic...", this, &DiskBrowserDlg::actionSetDirPic);
    menu.addAction(QIcon(":/icons/silk-icons/icons/image_add.png"), "Set Disk Preview Pic...", this, &DiskBrowserDlg::actionSetPic);
    menu.addAction(QIcon(":/icons/silk-icons/icons/image_delete.png"), "Clear Preview", this, &DiskBrowserDlg::actionClearPic);
    if (_picSource == PicSource_floppy)
    {
        menu.addSeparator();
        menu.addAction(QIcon(":/icons/silk-icons/icons/font.png"), "Set Disk Title", this, &DiskBrowserDlg::actionSetTitle);
        menu.addAction(QIcon(":/icons/silk-icons/icons/text_list_numbers.png"), "Set Disk Index", this, &DiskBrowserDlg::actionSetIndex);
        QString text = QString("Label Side %1").arg(_picInfo.label.sideB ? "A" : "B");
        menu.addAction(QIcon(":/icons/other-icons/sideb.png"), text, this, &DiskBrowserDlg::actionBackSide);
    }
    menu.exec(pos);
}

void DiskBrowserDlg::actionBackSide()
{
    if (_picSource == PicSource_floppy)
    {
        _picInfo.label.sideB = !_picInfo.label.sideB;
        _dbSettings->setSideB(_picInfo.label.sideB, _currentDir, _diskFileName);
        update();
    }
}

void DiskBrowserDlg::titleChanged(QString title)
{
    _picInfo.label.title = title;
    _dbSettings->setTitle(title, _currentDir, _diskFileName);
}

void DiskBrowserDlg::indexChanged(QString index)
{
    _picInfo.label.index = index;
    _dbSettings->setIndex(index, _currentDir, _diskFileName);
}

QString DiskBrowserDlg::browseForPic(const QString& start)
{
    auto formats = QImageReader::supportedImageFormats();
    auto fmtList = toStringList(formats);
    auto fmtStrs = fmtList.join(' ');
    auto filters = QString("Images (%1)").arg(fmtStrs);

    return QFileDialog::getOpenFileName(this, "Choose Default Pic", start, filters);
}

void DiskBrowserDlg::actionSetDefault()
{
    QString fname = browseForPic(_currentDir);
    if (fname.isEmpty())
        return;

    _dbSettings->setPicture(fname);
    update();
}

void DiskBrowserDlg::actionSetDirPic()
{
    QString fname = browseForPic(_currentDir);
    if (fname.isEmpty())
        return;

    _dbSettings->setPicture(fname, _currentDir);
    update();
}

void DiskBrowserDlg::actionSetPic()
{
    QString fname = browseForPic(_currentDir);
    if (fname.isEmpty())
        return;

    _dbSettings->setPicture(fname, _currentDir, _diskFileName);
    update();
}

void DiskBrowserDlg::actionSetTitle()
{
    if (_picSource == PicSource_floppy)
    {
        ui->picPreview->editTitle();
    }
    else
    {
        bool ok;
        QString text = QInputDialog::getText(this, "Input Text",
                                             "Disk Title:", QLineEdit::Normal,
                                             _picInfo.label.title, &ok);
        if (ok)
            _dbSettings->setTitle(text, _currentDir, _diskFileName);
    }
}

void DiskBrowserDlg::actionSetIndex()
{
    if (_picSource == PicSource_floppy)
    {
        ui->picPreview->editIndex();
    }
    else
    {
        bool ok;
        QString text = QInputDialog::getText(this, "Input Text",
                                             "Disk Index:", QLineEdit::Normal,
                                             _picInfo.label.index, &ok);
        if (ok)
            _dbSettings->setIndex(text, _currentDir, _diskFileName);
    }
}

void DiskBrowserDlg::actionClearPic()
{
    switch (_picSource)
    {
    case PicFromJson_dir:
        _dbSettings->setPicture("", _currentDir, "");
        break;
    case PicFromJson_global:
        _dbSettings->setPicture("");
        break;
    case PicFromJson_disk:
        _dbSettings->setPicture("", _currentDir, _diskFileName);
        break;
    default:
        break;
    }

    update();
}
