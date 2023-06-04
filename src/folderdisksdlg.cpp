#include "folderdisksdlg.h"
#include "ui_folderdisksdlg.h"
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

FolderDisksDlg::FolderDisksDlg(SioWorkerPtr pSio, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FolderDisksDlg)
{
    sio = std::move(pSio);

    ui->setupUi(this);

    connect(ui->btnBrowse, SIGNAL(clicked()), this, SLOT(onBrowseFolder()));
    connect(ui->listDisks, SIGNAL(itemSelectionChanged()), this, SLOT(onDiskChanged()));
    connect(ui->cboFolderPath, SIGNAL(currentTextChanged(QString)), this, SLOT(onFolderChanged(QString)));

    refreshFoldersCombobox();
    onFolderChanged("");
}

FolderDisksDlg::~FolderDisksDlg()
{
    delete ui;
}

QString FolderDisksDlg::getMostRecentFolder()
{
    QString   fileName = RespeqtSettings::instance()->mostRecentFolderDisks();
    QFileInfo fileInfo = QFileInfo(fileName);

    if (fileInfo.isFile())
        return fileInfo.path();

    return fileName;
}

QString FolderDisksDlg::getMostRecentDisk()
{
    QString   fileName = RespeqtSettings::instance()->mostRecentFolderDisks();
    QFileInfo fileInfo = QFileInfo(fileName);

    if (fileInfo.isFile())
        return fileInfo.fileName();

    return fileName;
}

void FolderDisksDlg::onBrowseFolder()
{
    QString lastDir = getMostRecentFolder();
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open folder containing disk images"), lastDir);

    dirName = QDir::fromNativeSeparators(dirName);
    if (dirName.isEmpty())
        return;

    onFolderChanged(dirName);
}

void FolderDisksDlg::onFolderChanged(QString folder)
{
    if (folder.isEmpty())
        folder = ui->cboFolderPath->currentText();
    if (folder.isEmpty())
        return;

    QString disk = getRecentDisk(folder);
    QString path = disk.isEmpty() ? folder : folder + "/" + disk;

    RespeqtSettings::instance()->setMostRecentFolderDisks(path);

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

void FolderDisksDlg::refreshFoldersCombobox()
{
    ui->cboFolderPath->blockSignals(true);
    ui->cboFolderPath->clear();

    foreach (QString name, RespeqtSettings::instance()->recentFolderDisks())
    {
        auto fileInf = QFileInfo(name);
        QString path = fileInf.isFile() ? fileInf.path() : name;
        ui->cboFolderPath->addItem(path);
    }

    ui->cboFolderPath->setCurrentIndex(0);
    ui->cboFolderPath->blockSignals(false);
}

void FolderDisksDlg::onDiskChanged()
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

    RespeqtSettings::instance()->setMostRecentFolderDisks(fullName);

    MainWindow::instance()->mountFileWithDefaultProtection(0, fullName);

    // look for a preview/thumbnail with diskname.png
    QString   imagePath = fiDisk.absolutePath() + "/" + fiDisk.completeBaseName() + ".png";
    QFileInfo fiPreview = QFileInfo(imagePath);

    if (fiPreview.exists())
    {
        QPixmap* pixMap = new QPixmap(imagePath);
        ui->lblPreview->setPixmap(*pixMap);
        delete pixMap;
    }
    else
    {
        // no preview: get a list of files

        ui->lblPreview->clear();

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
                QString list;

                foreach (const Filesystems::AtariDirEntry& entry, entries)
                    list += entry.name() + "\n";

                ui->lblPreview->setText(list);
            }
        }
    }
}

QString FolderDisksDlg::getRecentDisk(QString folder)
{
    foreach (QString text, RespeqtSettings::instance()->recentFolderDisks())
    {
        auto fi = QFileInfo(text);
        if (fi.isFile() && (fi.path() == folder))
            return fi.fileName();
    }

    return QString();
}

int FolderDisksDlg::getSplitPos()
{
    return ui->splitter->sizes().at(0);
}

void FolderDisksDlg::setSplitPos(int pos)
{
    QList<int> sizes = ui->splitter->sizes();
    int total = sizes.at(0) + sizes.at(1);
    sizes.clear();
    sizes << pos << (total - pos);
    ui->splitter->setSizes(sizes);
}
