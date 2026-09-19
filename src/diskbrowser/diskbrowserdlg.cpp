/*
 * diskbrowserdlg.cpp - implementation of RespeQt's DiskBrowserDlg class.
 *
 * Copyright 2023 D.Caputi
 */
// ReSharper disable CppMemberFunctionMayBeStatic
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
#include <QFontDatabase>
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

    // The listing is column aligned Atari directory output and needs a fixed
    // pitch font. The form named Courier10 BT, which exists on Windows only --
    // everywhere else Qt silently substituted a proportional font and the
    // columns fell apart. Ask the platform for its own monospace font.
    ui->lblFileList->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    ui->treeDisks->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    ui->treeDisks->setSortingEnabled(true);
    ui->treeDisks->sortByColumn(0,Qt::SortOrder::AscendingOrder);

    ui->splitTopDirBotPng->setOther(ui->splitLeftAtrRightDirPng);
    ui->splitLeftAtrRightDirPng->setOther(ui->splitTopDirBotPng);

    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowseFolder()));
    connect(ui->treeDisks, &QTreeWidget::itemSelectionChanged, this, &DiskBrowserDlg::itemSelectionChanged);
    connect(ui->treeDisks, &QTreeWidget::itemDoubleClicked, this, &DiskBrowserDlg::itemDoubleClicked);
    connect(ui->treeDisks, &QTreeWidget::customContextMenuRequested, this, &DiskBrowserDlg::popupMenuReq);
    connect(ui->cboFolderPath, SIGNAL(currentTextChanged(QString)), this, SLOT(onFolderChanged(QString)));
    connect(ui->picPreview, &PicPreview::sigPopupMenuReq, this, &DiskBrowserDlg::popupMenuReq);
    connect(ui->picPreview, &PicPreview::sigTitleChanged, this, &DiskBrowserDlg::titleChanged);
    connect(ui->picPreview, &PicPreview::sigIndexChanged, this, &DiskBrowserDlg::indexChanged);

    refreshFoldersCombobox();
    onFolderChanged(""); // reload the current item 0 in the combo
}

DiskBrowserDlg::~DiskBrowserDlg()
{
    delete ui;
}

void DiskBrowserDlg::clear() const
{
    ui->picPreview->clearAll();
    ui->lblFileList->clear();

    ui->treeDisks->blockSignals(true);
    ui->treeDisks->clear();
    ui->treeDisks->setColumnCount(2);
    ui->treeDisks->setHeaderHidden(true);
    ui->treeDisks->setRootIsDecorated(false);
    ui->treeDisks->blockSignals(false);
}

QString DiskBrowserDlg::getMostRecentFolder() // NOLINT(*-convert-member-functions-to-static)
{
    QString fileName = RespeqtSettings::instance()->mostRecentBrowserFolder();

    if (const auto fileInfo = QFileInfo(fileName); fileInfo.isFile())
        return fileInfo.path();

    return fileName;
}

QString DiskBrowserDlg::getMostRecentDisk() // NOLINT(*-convert-member-functions-to-static)
{
    QString fileName = RespeqtSettings::instance()->mostRecentBrowserFolder();

    if (const auto fileInfo = QFileInfo(fileName); fileInfo.isFile())
        return fileInfo.fileName();

    return fileName;
}

void DiskBrowserDlg::onBrowseFolder()
{
    const QString lastDir {getMostRecentFolder()};
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open folder containing disk images"), lastDir);

    dirName = QDir::fromNativeSeparators(dirName);
    if (dirName.isEmpty())
        return;

    onFolderChanged(dirName);
}

void DiskBrowserDlg::onFolderChanged(QString lastFolder)
{
    if (lastFolder.isEmpty())
        lastFolder = ui->cboFolderPath->currentText();
    if (lastFolder.isEmpty())
        return;

    if (!QFileInfo::exists(lastFolder))
    {
        RespeqtSettings::instance()->delMostRecentBrowserFolder(lastFolder);
        refreshFoldersCombobox();
        return;
    }

    if (RespeqtSettings::instance()->dbDataSource() == DbData_subDirJson)
        RespeqtSettings::dbSettings()->setDataDir(lastFolder);

    QString disk = getRecentDisk(lastFolder);
    const QString path = disk.isEmpty() ? lastFolder : lastFolder + "/" + disk;
    // NOTE: MRU scheme uses "path" for both folder *and* files (if one is selected)

    RespeqtSettings::instance()->setMostRecentBrowserFolder(path);

    refreshFoldersCombobox();

    _folderDisks.load(lastFolder);

    clear();    // clear disk collection browser contents

    // fill in any sub-directories
    auto folders {_folderDisks.folders()};
    foreach (const QString &subdir, folders)
    {
        if (subdir.startsWith('.') && subdir != "..")     // hide .folders on Windows
            continue;

        const auto item {new DbItem(ui->treeDisks)};
        auto icon {QIcon::fromTheme("folder")};
        item->setIcon(1, icon);
        item->setText(1, subdir);
        item->setFolder(true);
    }

    // fill in all disk image files
    auto disks {_folderDisks.disks()};
    foreach (const QString &disk_, disks)
    {
        auto index {diskIndex(lastFolder, disk_)};
        const auto item {new DbItem(ui->treeDisks)};
        item->setText(0, index);
        if (!index.isEmpty() && disk_.startsWith(index))
        {
            item->setText(1, disk_.mid(index.length() + 1));
            item->setData(1, Qt::UserRole, index);
        }
        else
            item->setText(1, disk_);
    }

    if (disk.isEmpty())
    {
        // select the currently mounted disk if present in the list
        auto [fileName, isWriteProtected] {RespeqtSettings::instance()->mountedImageSetting(0)};
        const QString pathName {fileName};

        disk = QFileInfo(pathName).fileName();
    }

    if (!disk.isEmpty() && disks.contains(disk))
    {
        QString col1text = disk;
        if (const auto index {diskIndex(lastFolder, disk)}; !index.isEmpty() && disk.startsWith(index))
            col1text = disk.mid(index.length() + 1);
        auto items {ui->treeDisks->findItems(col1text, Qt::MatchExactly, 1)};
        QTreeWidgetItem* item = !items.empty() ? items[0] : nullptr;
        ui->treeDisks->setCurrentItem(item);
    }

    ui->treeDisks->resizeColumnToContents(0);
}

void DiskBrowserDlg::refreshFoldersCombobox() const
{
    ui->cboFolderPath->blockSignals(true);
    ui->cboFolderPath->clear();

    const QStringList folders = RespeqtSettings::instance()->buildBrowserFolders();

    ui->cboFolderPath->addItems(folders);
    ui->cboFolderPath->setCurrentIndex(0);
    ui->cboFolderPath->blockSignals(false);
}

void DiskBrowserDlg::itemSelectionChanged()
{
    updateDiskBrowser();
}

void DiskBrowserDlg::updateDiskBrowser()
{
    if (!ui->treeDisks->topLevelItemCount() || !ui->treeDisks->currentItem())
        return;

    const auto currentItem {ui->treeDisks->currentItem()};

    if (itemIsFolder(currentItem))
    {
        ui->picPreview->clearAll();
        ui->lblFileList->clear();
        return;
    }

    QString diskName = currentItem->text(1);
    if (const QString strIndex = currentItem->data(1, Qt::UserRole).toString(); !strIndex.isEmpty())
        diskName = strIndex + "." + diskName;
    const QString pathName = ui->cboFolderPath->currentText();
    const QString fullName = pathName + QString("/") + diskName;
    const auto fiDisk = QFileInfo(fullName);

    if (!fiDisk.exists())
    {
        // TBD: remove item
        Q_ASSERT(0);
        return;
    }

    _diskFullName = fullName;
    _diskFileName = diskName;
    _currentDir = pathName;
    _diskTitle  = fiDisk.completeBaseName();

    RespeqtSettings::instance()->setMostRecentBrowserFolder(fullName);
    MainWindow::instance()->mountFileWithDefaultProtection(0, fullName);

    QString fileList;   // get a list of files to show in the file list pane

    if (sio)
    {
        constexpr int deviceNo {0};
        const auto img {qobject_cast<DiskImages::SimpleDiskImage *>(sio->getDevice(deviceNo + DISK_BASE_CDEVIC))};
        const int fs = img->defaultFileSystem();
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
            auto entries {atariFs->getEntries(atariFs->rootDir())};

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

    const double oldRatio = ui->picPreview->ratio();

    ui->lblFileList->setText(fileList);
    ui->picPreview->clearAll();
    _picInfo.clear();

    const bool favorJson = RespeqtSettings::instance()->dbFavorJson();
    const bool fileNames = RespeqtSettings::instance()->dbUseFileNames();

    // find a custom or built-in pic

    const QDir dir {fiDisk.absolutePath()};

    if (fileNames)
    {
        _picInfo.label = parsePicLabel();
        _picInfo.pic   = findPicFile();
    }
    if (_picInfo.pic.isEmpty() || favorJson)
    {
        if (auto jsonPic = RespeqtSettings::dbSettings()->getPicture(dir, _diskFileName, _picSource); _picInfo.pic.isEmpty() || !jsonPic.isEmpty())
            _picInfo.pic = std::move(jsonPic);
    }
    if (_picInfo.label.isEmpty() || favorJson)
    {
        if (auto jsonLabel {RespeqtSettings::dbSettings()->getLabel(dir, _diskFileName)}; _picInfo.label.isEmpty() || !jsonLabel.isEmpty())
            _picInfo.label = std::move(jsonLabel);
    }
    if (_picInfo.pic.isEmpty())
    {
        _picInfo.pic = getFloppyPic();
        if (!_picInfo.pic.isEmpty())
        {
            _picSource = PicSource_floppy;
            if (_picInfo.label.title.isEmpty())
                _picInfo.label.title = _diskTitle;
            ui->picPreview->setLabel(_picInfo.label);
        }
    }

    // make sure the pic is valid
    if (!_picInfo.pic.isEmpty() && !QFile::exists(_picInfo.pic))
        ui->picPreview->clearAll();    // TBD: remove entry, log error?
    else
        ui->picPreview->setFileName(_picInfo.pic);

    if (double newRatio = ui->picPreview->ratio(); oldRatio != newRatio)
    {
        if (newRatio == 0.0)
            newRatio = 1.0;

        ui->splitLeftAtrRightDirPng->setRatio(newRatio);
        ui->splitTopDirBotPng->setRatio(newRatio, true);
    }
}

QString DiskBrowserDlg::getRecentDisk(const QString& folder) // NOLINT(*-convert-member-functions-to-static)
{
    foreach (const QString &text, RespeqtSettings::instance()->recentBrowserFolders())
    {
        if (auto fi {QFileInfo(text)}; fi.isFile() && fi.path() == folder)
            return fi.fileName();
    }

    return {};
}

int DiskBrowserDlg::getHorzSplitPos() const
{
    return ui->splitLeftAtrRightDirPng->sizes().at(0);
}

int DiskBrowserDlg::getVertSplitPos() const
{
    return ui->splitTopDirBotPng->sizes().at(0);
}

void DiskBrowserDlg::setHorzSplitPos(const int pos) const
{
    if (pos == -1)
        return;

    QList<int> sizes = ui->splitLeftAtrRightDirPng->sizes();
    const int total = sizes.at(0) + sizes.at(1);
    sizes.clear();
    sizes << pos << (total - pos);
    ui->splitLeftAtrRightDirPng->setSizes(sizes);
}

void DiskBrowserDlg::setVertSplitPos(const int pos) const
{
    if (pos == -1)
        return;

    QList<int> sizes = ui->splitTopDirBotPng->sizes();
    const int total = sizes.at(0) + sizes.at(1);
    sizes.clear();
    sizes << pos << (total - pos);
    ui->splitTopDirBotPng->setSizes(sizes);
}

void DiskBrowserDlg::itemDoubleClicked(QTreeWidgetItem *item, int)
{
    const QString text {item->text(1)};
    auto path {ui->cboFolderPath->currentText()};

    if (text == "..")
    {
        if (QDir dir(path); dir.cdUp())
            onFolderChanged(dir.absolutePath());
    }
    else if (itemIsFolder(item))
    {
        path = path + "/" + text;
        onFolderChanged(path);
    }
}

bool DiskBrowserDlg::itemIsFolder(QTreeWidgetItem *item) // NOLINT(*-convert-member-functions-to-static)
{
    const auto dbItem = dynamic_cast<DbItem*>(item);
    return dbItem && dbItem->isFolder();
}

void DiskBrowserDlg::closeEvent(QCloseEvent *event)
{
    if (!event->spontaneous()) // linux fires 2 close events (2nd one is !visible)...
    {
        if (!RespeqtSettings::instance()->saveWidgetGeometry(this))
        {
            qDebug() << "!n" << "Widget geometry couldn't be saved!";
        }
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
        updateDiskBrowser();
}

DiskLabel DiskBrowserDlg::parsePicLabel(const QString& diskName) const
{
    QString baseName {diskName};

    if (baseName.isEmpty())
    {
        const QFileInfo fileInfo {_diskFullName};
        Q_ASSERT(fileInfo.exists());                // validated prior to this call
        baseName = fileInfo.completeBaseName();
    }

    DiskLabel label;
    static QRegularExpression re{"(^\\d+)([b|B]?)(\\.?)(.*)"};

    if (const auto rem {re.match(baseName)}; rem.hasMatch())
    {
        const QString index = rem.captured(1);
        const QString title  = rem.captured(4);
        const bool isSideB = rem.captured(2).toUpper() == "B";

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
    const QFileInfo fileInfo {_diskFullName};
    const auto diskBase {fileInfo.completeBaseName()};
    const QDir dir {fileInfo.absolutePath()};
    const QDir subdir {fileInfo.absolutePath() + "/.respeqt_db"};
    const auto formats {QImageReader::supportedImageFormats()};
    const auto fmtlist {DbUtils::toStringList(formats)};
    auto entries {dir.entryInfoList(fmtlist)};
    auto bsidexp {_picInfo.label.sideB ? QString("[b|B]") : QString()};
    const auto sregexp {QString("^(%1)(%2)(\\.)(.*)").arg(_picInfo.label.index, bsidexp)};
    static const QRegularExpression qregexp {sregexp};

    if (subdir.exists())
        entries += subdir.entryInfoList(fmtlist);   // also scan ./.respeqt_db sub-dir for pics

    foreach (const QFileInfo& entry, entries)
    {
        // 1. check for basename with viable image extension

        if (const QString entryName {entry.completeBaseName()}; entryName == diskBase)
        {
            setToolTip(entryName);
            _picSource = PicFromFname_base;
            return entry.absoluteFilePath();
        }

        // 2. check for matching indexing filename prefix

        if (!_picInfo.label.index.isEmpty())     // check if current disk has index prefix NN. or NNb.
        {
            auto basename {entry.completeBaseName()};
            if (auto matcher {qregexp.match(basename)}; matcher.hasMatch())
            {
                const QString tip = matcher.captured(4);  // use mid string as tooltip
                setToolTip(tip);
                _picSource = PicFromFname_index;
                return entry.absoluteFilePath();
            }
        }

        // 3. use generic name (i.e. "respeqt_db.png") for default thumbnail

        auto imagePath {fileInfo.path() + "/respeqt_db." + entry.suffix()};
        if (!QFileInfo::exists(imagePath))
            imagePath = fileInfo.path() + "/.respeqt_db/respeqt_db." + entry.suffix();
        if (QFileInfo::exists(imagePath))
        {
            setToolTip("");
            _picSource = PicFromFname_dir;
            return imagePath;
        }
    }
    return {};
}

QString DiskBrowserDlg::getFloppyPic() const
// 5. load built-in image of a 5 1/2-inch floppy disk
{

    if (_picInfo.label.sideB)
        return FLOPPY_BACKSIDE_PNG; // same 2 labels as default, but flip-side of double-sided floppy

    return FLOPPY_INDEXED_PNG;      // default has 2 labels: (small) disk no./index & (large) title/content
}

void DiskBrowserDlg::popupMenuReq(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(QIcon::fromTheme("image"), "Set Default Preview...", this, &DiskBrowserDlg::actionSetDefault);
    menu.addAction(QIcon::fromTheme("folder_image"), "Set Folder Preview Pic...", this, &DiskBrowserDlg::actionSetDirPic);
    if (_picSource != PicSource_none)
    {
        menu.addAction(QIcon::fromTheme("image_add"), "Set Disk Preview Pic...", this, &DiskBrowserDlg::actionSetPic);
        menu.addAction(QIcon::fromTheme("image_delete"), "Clear Preview", this, &DiskBrowserDlg::actionClearPic);
    }
    if (_picSource == PicSource_floppy)
    {
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme("font"), "Set Disk Title", this, &DiskBrowserDlg::actionSetTitle);
        menu.addAction(QIcon::fromTheme("text_list_numbers"), "Set Disk Index", this, &DiskBrowserDlg::actionSetIndex);
        const QString text = QString("Label Side %1").arg(_picInfo.label.sideB ? "A" : "B");
        menu.addAction(QIcon::fromTheme("sideb"), text, this, &DiskBrowserDlg::actionBackSide);
    }

    if (sender() == ui->treeDisks)
    {
        const QPoint globalPt = ui->treeDisks->mapToGlobal(pos);
        menu.exec(globalPt);
    }
    else
    {
        menu.exec(pos);
    }
}

void DiskBrowserDlg::actionBackSide()
{
    if (_picSource == PicSource_floppy)
    {
        _picInfo.label.sideB = !_picInfo.label.sideB;
        RespeqtSettings::dbSettings()->setSideB(_picInfo.label.sideB, _currentDir, _diskFileName);
        updateDiskBrowser();
    }
}

void DiskBrowserDlg::titleChanged(const QString& title)
{
    _picInfo.label.title = title;
    RespeqtSettings::dbSettings()->setTitle(title, _currentDir, _diskFileName);
}

void DiskBrowserDlg::indexChanged(const QString& index)
{
    _picInfo.label.index = index;
    RespeqtSettings::dbSettings()->setIndex(index, _currentDir, _diskFileName);
    ui->treeDisks->currentItem()->setText(0, index);
    ui->treeDisks->resizeColumnToContents(0);
    ui->treeDisks->sortByColumn(0, Qt::AscendingOrder); // BUG in custom sort (shouldn't need this!)
}

QString DiskBrowserDlg::browseForPic(const QString& start, const QString& action)
{
    const auto formats {QImageReader::supportedImageFormats()};
    const auto fmtList {DbUtils::toStringList(formats)};
    const auto fmtStrs {fmtList.join(' ')};
    const auto filters {QString("Images (%1)").arg(fmtStrs)};

#ifdef Q_OS_LINUX
    // Quirks on linux: getOpenFileName will use case sensitive filters (i.e. it won't find all caps *.JPG)
    // The non-native/Qt version is better, but resizes rediculously wide to fit the long filter string! (TBD: fix)
    auto fname {QFileDialog::getOpenFileName(this, action, start, filters, nullptr, QFileDialog::DontUseNativeDialog)};
#else
    auto fname {QFileDialog::getOpenFileName(this, action, start, filters, nullptr)};
#endif
    return fname;
}

QString DiskBrowserDlg::checkCopyPic(const QString& fname)
{
    if (!RespeqtSettings::instance()->dbCopyPics())
        return fname;

    QString fileName = QFileInfo(fname).fileName();
    QString newPath;

    switch (RespeqtSettings::instance()->dbDataSource())
    {
    case DbData_appSettings:
        newPath = _currentDir;
        break;
    case DbData_subDirJson:
        newPath = _currentDir + "/.respeqt_db";
        break;
    case DbData_appFolderJson:
        newPath = RespeqtSettings::instance()->appDataFolder() + "/.respeqt_db/" + QDir(_currentDir).dirName();
        // ReSharper disable once CppExpressionWithoutSideEffects
        QDir(newPath).mkpath(".");
        break;
    case DbData_none:
        break;
    }
    QString newName = newPath + "/" + fileName;

    if (QFile::exists(newName))
    {
        const QString text = QString("Pic %1 already exists: Replace and use for this?\n"
                               "(choosing 'no' will use pic from current location)").arg(fileName,newName);
        if (QMessageBox::question(this,"Confirm Overwrite", text) == QMessageBox::Yes)
            QFile::remove(newName);
        else
            return fname;
    }
    if (QFile::copy(fname, newName))
    {
        qDebug() << "!i" << "Disk Collection Browser pic " << fileName << " copied to " << newPath;
        return newName;
    }

    // error copying the file TBD: popup?
    qDebug() << "!e" << "Disk Collection Browser could not copy to " << newPath;
    return {};
}

void DiskBrowserDlg::actionSetDefault()
{
    QString pic = browseForPic(_currentDir, "Choose a Default Pic");
    if (pic.isEmpty())
        return;

    if (RespeqtSettings::instance()->dbDataSource() != DbData_appSettings)
        pic = checkCopyPic(pic);
    else if (RespeqtSettings::instance()->dbCopyPics())
        qDebug() << "!w" << "Updating default Pic file for all collections - file not copied";

    RespeqtSettings::dbSettings()->setPicture(pic);
    updateDiskBrowser();
}

void DiskBrowserDlg::actionSetDirPic()
{
    const QString pic = browseForPic(_currentDir, "Choose Pic for this Collection");
    if (pic.isEmpty())
        return;

    RespeqtSettings::dbSettings()->setPicture(checkCopyPic(pic), _currentDir);
    updateDiskBrowser();
}

void DiskBrowserDlg::actionSetPic()
{
    const QString pic = browseForPic(_currentDir, "Choose Pic for this Disk");
    if (pic.isEmpty())
        return;

    RespeqtSettings::dbSettings()->setPicture(checkCopyPic(pic), _currentDir, _diskFileName);
    updateDiskBrowser();
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
        const QString text = QInputDialog::getText(this, "Input Text",
                                             "Disk Title:", QLineEdit::Normal,
                                             _picInfo.label.title, &ok);
        if (ok)
            RespeqtSettings::dbSettings()->setTitle(text, _currentDir, _diskFileName);
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
        const QString text = QInputDialog::getText(this, "Input Text",
                                             "Disk Index:", QLineEdit::Normal,
                                             _picInfo.label.index, &ok);
        if (ok)
            RespeqtSettings::dbSettings()->setIndex(text, _currentDir, _diskFileName);
    }
}

void DiskBrowserDlg::actionClearPic()
{
    switch (_picSource)
    {
    case PicFromJson_dir:
        RespeqtSettings::dbSettings()->setPicture("", _currentDir, "");
        break;
    case PicFromJson_global:
        RespeqtSettings::dbSettings()->setPicture("");
        break;
    case PicFromJson_disk:
        RespeqtSettings::dbSettings()->setPicture("", _currentDir, _diskFileName);
        break;
    default:
        break;
    }

    updateDiskBrowser();
}

QString DiskBrowserDlg::diskIndex(const QString& folder, const QString& disk) const
{
    QString index;

    if (RespeqtSettings::instance()->dbUseFileNames())
    {
        const auto label {parsePicLabel(disk)};
        index = label.index;
        if (label.sideB)
            index += 'b';
    }
    if (index.isEmpty() || RespeqtSettings::instance()->dbFavorJson())
    {
        if (const auto label {RespeqtSettings::dbSettings()->getLabel(folder, disk)}; index.isEmpty() || !label.isEmpty())
            index = label.index;
    }

    return index;
}


// DbItem class - mostly just so I can sort the TreeDisks by the index column
//
bool DbItem::operator<(const QTreeWidgetItem& other) const
{
    const auto& dbOther = dynamic_cast<const DbItem&>(other);
    if (!isFolder() && dbOther.isFolder())
        return false;

    bool comp = QTreeWidgetItem::operator<(other);

    if (isFolder() && dbOther.isFolder())
        return comp;

    if (text(0).isEmpty() && other.text(0).isEmpty())
        return comp;

    if (compNumberVal(text(0), other.text(0), comp))
        return comp;

    if (text(0).isEmpty() || other.text(0).isEmpty())
        return !comp;

    return comp;
}

bool DbItem::compNumberVal(const QString& index, const QString& other, bool& comp) const // NOLINT(*-convert-member-functions-to-static)
{
    static QRegularExpression re("^(\\d+)([b|B]?)$");
    const auto remIndex {re.match(index)};
    const auto remOther {re.match(other)};
    const bool bothNums {remIndex.hasMatch() && remOther.hasMatch()};

    if (bothNums)
    {
        const int nIndex {remIndex.captured(1).toInt()};

        if (const int nOther {remOther.captured(1).toInt()}; nIndex == nOther)
        {
            const bool bIndex {!remIndex.captured(2).isEmpty()};
            const bool bOther {!remOther.captured(2).isEmpty()};

            comp = !bIndex && bOther;
        }
        else
        {
            comp = nIndex < nOther;
        }
    }
    return bothNums;
}
