#include "diskbrowserdlg.h"
#include "ui_diskbrowserdlg.h"
#include "respeqtsettings.h"
#include "filesystems/atarifilesystem.h"
#include "filesystems/dos10filesystem.h"
#include "filesystems/dos20filesystem.h"
#include "filesystems/dos25filesystem.h"
#include "filesystems/mydosfilesystem.h"
#include "filesystems/spartadosfilesystem.h"
#include "filesystems/ataridirentry.h"
#include "diskimages/diskimage.h"
#include "folderdisks.h"
#include "mainwindow.h"
#include "miscutils.h"
#include <QFileDialog>
#include <QObject>

DiskBrowserDlg::DiskBrowserDlg(SioWorkerPtr pSio, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiskBrowserDlg)
{
    sio = std::move(pSio);

    ui->setupUi(this);

    ui->splitTopDirBotPng->setOther(ui->splitLeftAtrRightDirPng);
    ui->splitLeftAtrRightDirPng->setOther(ui->splitTopDirBotPng);

    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowseFolder()));
    connect(ui->treeDisks, &QTreeWidget::itemSelectionChanged, this, &DiskBrowserDlg::itemSelectionChanged);
    connect(ui->treeDisks, &QTreeWidget::itemDoubleClicked, this, &DiskBrowserDlg::itemDoubleClicked);
    connect(ui->cboFolderPath, SIGNAL(currentTextChanged(QString)), this, SLOT(onFolderChanged(QString)));

    refreshFoldersCombobox();
    onFolderChanged("");        // reload the current item 0 in the combo
}

DiskBrowserDlg::~DiskBrowserDlg()
{
    delete ui;
}

void DiskBrowserDlg::clear()
{
    ui->lblPreview->clear();
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
    QString   fileName = RespeqtSettings::instance()->mostRecentBrowserFolder();
    QFileInfo fileInfo = QFileInfo(fileName);

    if (fileInfo.isFile())
        return fileInfo.path();

    return fileName;
}

QString DiskBrowserDlg::getMostRecentDisk()
{
    QString   fileName = RespeqtSettings::instance()->mostRecentBrowserFolder();
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

    QString disk = getRecentDisk(folder);
    QString path = disk.isEmpty() ? folder : folder + "/" + disk;
    // NOTE: MRU scheme uses "path" for both folder *and* files (if one is selected)

    RespeqtSettings::instance()->setMostRecentBrowserFolder(path);

    refreshFoldersCombobox();

    _folderDisks.load(folder);

    clear();    // clear disk browser contents

    // fill in any sub-directories
    auto folders = _folderDisks.folders();
    foreach (const QString& subdir, folders)
    {
        auto item = new QTreeWidgetItem(ui->treeDisks);
        auto icon = QIcon {":/icons/silk-icons/icons/folder_explore.png"};
        item->setText(0, subdir);
        item->setIcon(0, icon);
        setItemIsFolder(item, true);
    }

    // fill in all disk image files
    auto disks = _folderDisks.disks();
    foreach (const QString& disk, disks)
    {
        auto item = new QTreeWidgetItem(ui->treeDisks);
        auto icon = QIcon {":/icons/other-icons/floppy.png"};
        item->setText(0, disk);
        item->setIcon(0, icon);
        setItemIsFolder(item, false);
    }

    if (disk.isEmpty())
    {
        // select the currently mounted disk if present in the list
        auto    imageSet = RespeqtSettings::instance()->mountedImageSetting(0);
        QString pathName = imageSet.fileName;

        disk = QFileInfo(pathName).fileName();
    }

    if (!disk.isEmpty() && disks.contains(disk))
    {
        auto items = ui->treeDisks->findItems(disk, Qt::MatchExactly);
        QTreeWidgetItem* item = (items.length() > 0) ? items[0] : nullptr;
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
            QString path = fileInf.isFile() ? fileInf.path() : name;    // don't want file names in dropdown
            folders += path;
        }
        else if (isDiskImage(name))  // MRU missing. First check if a disk is selected
        {
            QString path = getParentDir(name);
            if (QFileInfo::exists(path))
            {
                folders += path;    // Keep parent folder of bad disk
            }
            else
            {
                qDebug() << "!w" << tr("Disk Browser most recent list updated. '%1' not found.").arg(name);
                RespeqtSettings::instance()->delMostRecentBrowserFolder(name);
            }
        }
        else    // Simple case of missing folder
        {
            qDebug() << "!w" << tr("Disk Browser most recent list updated. Folder '%1' not found.").arg(name);
            RespeqtSettings::instance()->delMostRecentBrowserFolder(name);
        }
    }
    ui->cboFolderPath->addItems(folders);
    ui->cboFolderPath->setCurrentIndex(0);
    ui->cboFolderPath->blockSignals(false);
}

void DiskBrowserDlg::itemSelectionChanged()
{
    if (!ui->treeDisks->topLevelItemCount() || !ui->treeDisks->currentItem())
        return;

    auto currentItem = ui->treeDisks->currentItem();

    if (itemIsFolder(currentItem))
    {
        ui->lblPreview->clear();
        ui->lblFileList->clear();
        return;
    }

    QString diskName = currentItem->text(0);
    QString pathName = ui->cboFolderPath->currentText();
    QString fullName = pathName + QString("/") + diskName;
    QFileInfo fiDisk = QFileInfo(fullName);

    if (!fiDisk.exists())
    {
        Q_ASSERT(0);
        return;
    }

    RespeqtSettings::instance()->setMostRecentBrowserFolder(fullName);
    MainWindow::instance()->mountFileWithDefaultProtection(0, fullName);

    QString fileList;   // get a list of files to show in the file list pane

    if (sio)
    {
        const int deviceNo = 0;
        auto img = qobject_cast <DiskImages::SimpleDiskImage*>(sio->getDevice(deviceNo + DISK_BASE_CDEVIC));
        int fs = img->defaultFileSystem();
        Filesystems::AtariFileSystem* atariFs = nullptr;

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
            int     dir     = atariFs->rootDir();
            auto    entries = atariFs->getEntries(dir);

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

    double oldRatio = ui->lblPreview->ratio();

    ui->lblFileList->setText(fileList);
    ui->lblPreview->setDiskName(fullName);

    double newRatio = ui->lblPreview->ratio();

    if (oldRatio != newRatio)
    {
        if (newRatio == 0.0)
            newRatio = 1.0;     // TBD

        ui->splitLeftAtrRightDirPng->setRatio(newRatio);
        ui->splitTopDirBotPng->setRatio(newRatio, true);
    }
    // TBD 1. allow drop target or paste image, show hint text
    // TBD 2. interop with emulator code, boot it and show screen
}

QString DiskBrowserDlg::getRecentDisk(QString folder)
{
    foreach (const QString& text, RespeqtSettings::instance()->recentBrowserFolders())
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

bool DiskBrowserDlg::isDiskImage(const QString& name)
{
    foreach (const QString& fileType, FileTypes::getDiskImageTypes())
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

void DiskBrowserDlg::itemDoubleClicked(QTreeWidgetItem* item, int)
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

void DiskBrowserDlg::setItemIsFolder(QTreeWidgetItem* item, bool isFolder)
{
    item->setData(0, Qt::UserRole, isFolder);
}

bool DiskBrowserDlg::itemIsFolder(QTreeWidgetItem* item)
{
    return item->data(0, Qt::UserRole).toBool();
}

void DiskBrowserDlg::closeEvent(QCloseEvent *event)
{
    RespeqtSettings::instance()->setDiskBrowserHorzSplitPos(getHorzSplitPos());
    RespeqtSettings::instance()->setDiskBrowserVertSplitPos(getVertSplitPos());
    QDialog::closeEvent(event);
}

void DiskBrowserDlg::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    setHorzSplitPos(RespeqtSettings::instance()->diskBrowserHorzSplitPos());
    setVertSplitPos(RespeqtSettings::instance()->diskBrowserVertSplitPos());
}
