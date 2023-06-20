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
#include <QFileDialog>

DiskBrowserDlg::DiskBrowserDlg(SioWorkerPtr pSio, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiskBrowserDlg)
{
    sio = std::move(pSio);

    ui->setupUi(this);

    ui->splitTopDirBotPng->setOther(ui->splitLeftAtrRightDirPng);
    ui->splitLeftAtrRightDirPng->setOther(ui->splitTopDirBotPng);

    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowseFolder()));
    connect(ui->listDisks, SIGNAL(itemSelectionChanged()), this, SLOT(onDiskChanged()));
    connect(ui->cboFolderPath, SIGNAL(currentTextChanged(QString)), this, SLOT(onFolderChanged(QString)));

    refreshFoldersCombobox();
    onFolderChanged("");
}

DiskBrowserDlg::~DiskBrowserDlg()
{
    delete ui;
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

    QString disk = getRecentDisk(folder);
    QString path = disk.isEmpty() ? folder : folder + "/" + disk;
    // TBD: comment explaining my scheme that uses "path" for both folder and files.

    RespeqtSettings::instance()->setMostRecentBrowserFolder(path);

    refreshFoldersCombobox();

    folderDisks.load(folder);

    ui->listDisks->blockSignals(true);
    ui->listDisks->clear();
    ui->lblPreview->clear();
    ui->listDisks->blockSignals(false);

    QStringList disks = folderDisks.getList();
    ui->listDisks->addItems(disks);

    if (disk.isEmpty())
    {
        // select the currently mounted disk if present in the list
        auto    imageSet = RespeqtSettings::instance()->mountedImageSetting(0);
        QString pathName = imageSet.fileName;

        disk = QFileInfo(pathName).fileName();
    }

    if (!disk.isEmpty() && disks.contains(disk))
    {
        int index = disks.indexOf(disk);
        ui->listDisks->setCurrentRow(index);
    }
}

void DiskBrowserDlg::refreshFoldersCombobox()
{
    ui->cboFolderPath->blockSignals(true);
    ui->cboFolderPath->clear();

    foreach (const QString& name, RespeqtSettings::instance()->recentBrowserFolders())
    {
        auto fileInf = QFileInfo(name);
        QString path = fileInf.isFile() ? fileInf.path() : name;
        ui->cboFolderPath->addItem(path);
    }

    ui->cboFolderPath->setCurrentIndex(0);
    ui->cboFolderPath->blockSignals(false);
}

void DiskBrowserDlg::onDiskChanged()
{
    if (ui->listDisks->count() == 0)
        return;

    QString diskName = ui->listDisks->currentItem()->text();
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
                fileList = "!Root dir empty:\nNo Files";
        }
        else
        {
            fileList = "!File system not recognized:\nNo Files";
        }
    }
    else
    {
        fileList = "!SIO device not available:\nNo Files";
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
