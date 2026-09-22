/*
 * diskeditdialog.cpp
 *
 * Copyright 2015 Joseph Zatarski
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "ui/diskeditdialog.h"
#include "ui_diskeditdialog.h"

#include <QDebug>
#include <QIcon>
#include <QPrintDialog>

#include <algorithm>

#include "miscutils.h"
#include "respeqtsettings.h"
#include "filesystems/atarifilesystem.h"
#include "filesystems/dos10filesystem.h"
#include "filesystems/dos20filesystem.h"
#include "filesystems/dos25filesystem.h"
#include "filesystems/mydosfilesystem.h"
#include "filesystems/spartadosfilesystem.h"

namespace UI {

/* MyModel */

FileModel::FileModel(QObject *parent)
    : QAbstractTableModel(parent) {
  fileSystem = nullptr;
  tempDirs = new QStringList();
}

FileModel::~FileModel() {
  while (!tempDirs->isEmpty()) {
    deltree(QDir::temp().absoluteFilePath(tempDirs->last()));
    tempDirs->removeLast();
  }
  delete tempDirs;
  delete fileSystem;
}

Qt::ItemFlags FileModel::flags(const QModelIndex &index) const {
  auto flags {
    Qt::ItemIsSelectable
    | Qt::ItemIsEnabled
    | Qt::ItemIsDragEnabled
    | Qt::ItemIsDropEnabled
  };

  if (index.column() == 1 || index.column() == 2
      || (index.column() == 4 && typeid(*fileSystem) == typeid(Filesystems::SpartaDosFileSystem)))
    flags |= Qt::ItemIsEditable;

  return flags;
}

QVariant FileModel::data(const QModelIndex &index, const int role) const {
  if (index.row() >= rowCount()) {
    return {};
  }
  if (index.column() == 1 && role == Qt::DecorationRole) {
    if ((entries.at(index.row()).attributes & Filesystems::AtariDirEntry::Directory) != 0) {
      return QIcon::fromTheme("folder");
    }
    return QIcon::fromTheme("page_white");
  }

  if (role == Qt::TextAlignmentRole && (index.column() == 3 || index.column() == 0)) {
    return {Qt::AlignRight | Qt::AlignVCenter};
  }

  QDateTime time = entries.at(index.row()).dateTime;

  if (index.column() == 4 && !time.isValid()) {
    if (role == Qt::TextAlignmentRole) {
      return Qt::AlignCenter;
    }
  }

  if (role != Qt::DisplayRole && role != Qt::EditRole) {
    QVariant x;
    return x;
  }

  switch (index.column()) {
    default:
      return {};

    case 0:
      return entries.at(index.row()).no;

    case 1:
      return entries.at(index.row()).baseName();

    case 2:
      return entries.at(index.row()).suffix();

    case 3:
      {
        if (entries.at(index.row()).size >= 0) {
          return entries.at(index.row()).size;
        }
        return {};
      }

    case 4:
      if (time.isValid()) {
        return time;
      }
      if (role == Qt::DisplayRole) {
        return QString("n/a");
      }
      return {};

    case 5:
      return entries.at(index.row()).attributeNames();
  }

  return {};
}

bool FileModel::setData(const QModelIndex &index, const QVariant &value, const int role) {
  if (role != Qt::EditRole) {
    return false;
  }
  switch (index.column()) {
    default:
      return false;

    case 1: {
      QString s = value.toString();
      if (s.isEmpty()) {
        return false;
      }
      s = s.toUpper();
      if (s.count() > 8) {
        s.resize(8);
      }
      foreach (QChar c, s) {
        if (static_cast<uint8_t>(c.toLatin1()) > 0x7f || (!c.isLetterOrNumber() && c != '_')) {
          return false;
        }
      }
      while (s.count() < 8) {
        s.append(' ');
      }
      s.append(entries.at(index.row()).suffix());
      while (s.count() < 11) {
        s.append(' ');
      }
      if (s.toLatin1() == entries.at(index.row()).atariName) {
        return false;
      }
      if (fileSystem->rename(entries.at(index.row()), s.toLatin1())) {
        Filesystems::AtariDirEntry entry = entries.at(index.row());
        entry.atariName = s.toLatin1();
        entries[index.row()] = std::move(entry);
        emit dataChanged(index, index);
        return true;
      }
      return false;
    }

    case 2: {
      QString s = value.toString();
      if (s.isEmpty()) {
        return false;
      }
      s = s.toUpper();
      if (s.count() > 3) {
        s.resize(3);
      }
      foreach (QChar c, s) {
        if (static_cast<uint8_t>(c.toLatin1()) > 0x7f || (!c.isLetterOrNumber() && c != '_')) {
          return false;
        }
      }
      while (s.count() < 3) {
        s.append(' ');
      }
      QString b = entries.at(index.row()).baseName();
      while (b.count() < 8) {
        b.append(' ');
      }
      s = b + s;
      if (s.toLatin1() == entries.at(index.row()).atariName) {
        return false;
      }
      if (fileSystem->rename(entries.at(index.row()), s.toLatin1())) {
        Filesystems::AtariDirEntry entry = entries.at(index.row());
        entry.atariName = s.toLatin1();
        entries[index.row()] = std::move(entry);
        emit dataChanged(index, index);
        return true;
      }
      return false;
    }

    case 4:
      return false;
  }

  return false;
}

void FileModel::deleteFiles(const QModelIndexList& indexes) {
  QList<int> l;

  QList<Filesystems::AtariDirEntry> selectedEntries;
  foreach (QModelIndex i, indexes) {
    if (i.isValid() && i.column() == 0) {
      selectedEntries.append(entries.at(i.row()));
      l.append(i.row());
    }
  }

  fileSystem->deleteRecursive(selectedEntries);

  emit layoutAboutToBeChanged();
  while (!l.isEmpty()) {
    const int i = l.last();
    entries.removeAt(i);
    l.removeLast();
  }

  emit layoutChanged();
}

QVariant FileModel::headerData(const int section, const Qt::Orientation orientation, const int role) const {
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
    return {};
  }
  switch (section) {
    default:
      return {};

    case 0:
      return tr("No");

    case 1:
      return tr("Name");

    case 2:
      return tr("Extension");

    case 3:
      return tr("Size");

    case 4:
      return tr("Time");

    case 5:
      return tr("Notes");
  }

  return {};
}

int FileModel::rowCount(const QModelIndex & /*parent*/) const {
  return entries.count();
}

int FileModel::columnCount(const QModelIndex & /*parent*/) const {
  return 6;
}

void FileModel::sort(int column, Qt::SortOrder order) {
  if (column < 0 || column > 5) {
    return;
  }
  emit layoutAboutToBeChanged();

  auto sorting {
    [column, order](const Filesystems::AtariDirEntry &e1, const Filesystems::AtariDirEntry &e2) {
      switch (column) {
        default:
          return false;

        case 0:
          {
            if (order == Qt::AscendingOrder) {
              return e1.no < e2.no;
            }
            return e1.no > e2.no;
          }

        case 1:
          if (order == Qt::AscendingOrder) {
            return e1.baseName() < e2.baseName();
          }
          return e1.baseName() > e2.baseName();

        case 2:
          {
            if (order == Qt::AscendingOrder) {
              return e1.suffix() < e2.suffix();
            }
            return e1.suffix() > e2.suffix();
          }

        case 3:
          {
            if (order == Qt::AscendingOrder) {
              return e1.size < e2.size;
            }
            return e1.size > e2.size;
          }

        case 4:
          {
            if (order == Qt::AscendingOrder) {
              return e1.dateTime < e2.dateTime;
            }
            return e1.dateTime > e2.dateTime;
          }

        case 5:
          {
            if (order == Qt::AscendingOrder) {
              return e1.attributeNames() < e2.attributeNames();
            }
            return e1.attributeNames() > e2.attributeNames();
          }
      }

      return false;
    }
  };

  std::stable_sort(entries.begin(), entries.end(), sorting);

  emit layoutChanged();
}

void FileModel::setFileSystem(Filesystems::AtariFileSystem *aFileSystem) {
  emit layoutAboutToBeChanged();

  delete fileSystem;

  fileSystem = aFileSystem;
  if (fileSystem) {
    setRoot();
  } else {
    entries.clear();
  }
  emit layoutChanged();
}

void FileModel::setDirectory(const int row) {
  emit layoutAboutToBeChanged();
  paths.append(entries.at(row).name());
  dirs.append(entries.at(row).firstSector);
  m_currentPath.append(entries.at(row).name() + ">");
  entries = fileSystem->getEntries(entries.at(row).firstSector);
  emit layoutChanged();
}

void FileModel::toParent() {
  paths.removeLast();
  dirs.removeLast();
  m_currentPath = QString("D%1:").arg(fileSystem->image()->deviceNo() - 0x30);
  foreach (QString s, paths) {
    m_currentPath.append(s + ">");
  }
  emit layoutAboutToBeChanged();
  entries = fileSystem->getEntries(dirs.last());
  emit layoutChanged();
}

void FileModel::setRoot() {
  emit layoutAboutToBeChanged();
  paths.clear();
  dirs.clear();
  const auto dir = fileSystem->rootDir();
  entries = fileSystem->getEntries(dir);
  dirs.append(dir);
  m_currentPath = QString("D%1:").arg(fileSystem->image()->deviceNo() - 0x30);
  emit layoutChanged();
}

void FileModel::insertFiles(const QStringList& names) {
  if (names.isEmpty()) {
    return;
  }

  const auto dir = dirs.last();

  emit layoutAboutToBeChanged();
  entries.append(fileSystem->insertRecursive(dir, names));
  emit layoutChanged();
}

bool FileModel::dropMimeData(const QMimeData *data, Qt::DropAction /*action*/, int /*row*/, int /*column*/, const QModelIndex & /*parent*/) {
  if (data->hasUrls()) {
    QStringList names;
    foreach(QUrl url, data->urls()) {
      if (QString name = url.toLocalFile(); !name.isEmpty()) {
        names.append(name);
      }
    }
    insertFiles(names);
    return true;
  }
  return false;
}

QStringList FileModel::mimeTypes() const {
  QStringList types;
  types.append("text/uri-list");
  return types;
}

QMimeData *FileModel::mimeData(const QModelIndexList &indexes) const {
  const auto data {new QMimeData()};
  QList<Filesystems::AtariDirEntry> selectedEntries;
  QList<QUrl> urls;

  while (!tempDirs->isEmpty()) {
    deltree(QDir::temp().absoluteFilePath(tempDirs->last()));
    QDir::temp().rmdir(tempDirs->last());
    tempDirs->removeLast();
  }

  QTemporaryFile temp;
  temp.setFileTemplate(QDir::temp().absoluteFilePath("respeqt-dir-XXXXXX"));
  temp.open();
  QString tempPath = temp.fileName() + "v";
  temp.close();
  const QFileInfo info(tempPath);
  if (const QDir dir = QDir::temp(); !dir.mkdir(info.fileName())) {
    return data;
  }
  tempDirs->append(info.fileName());

  foreach (QModelIndex i, indexes) {
    if (i.isValid() && i.column() == 0) {
      selectedEntries.append(entries.at(i.row()));
      urls.append(QUrl::fromLocalFile(QString(tempPath) + "/" + entries.at(i.row()).niceName()));
    }
  }
  data->setUrls(urls);

  fileSystem->extractRecursive(selectedEntries, QString(tempPath));
  return data;
}

/* DiskEditDialog */

DiskEditDialog::DiskEditDialog(QWidget *parent) : QMainWindow(parent),
                                                  m_ui(new ::Ui::DiskEditDialog) {
  m_ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);

  m_fileSystemBox = new QComboBox(this);
  m_fileSystemBox->addItem(tr("No file system"));
  m_fileSystemBox->addItem(tr("Atari Dos 1.0"));
  m_fileSystemBox->addItem(tr("Atari Dos 2.0"));
  m_fileSystemBox->addItem(tr("Atari Dos 2.5"));
  m_fileSystemBox->addItem(tr("MyDos"));
  m_fileSystemBox->addItem(tr("SpartaDos"));
  m_ui->statusbar->addPermanentWidget(m_fileSystemBox);

  model = new FileModel(this);
  model->setFileSystem(nullptr);
  m_ui->fileList->setModel(model);

  void (QComboBox::*fileSystemChanged)(int) = &QComboBox::currentIndexChanged;
  connect(m_fileSystemBox, fileSystemChanged, this, &DiskEditDialog::fileSystemChanged);
  connect(m_ui->fileList->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DiskEditDialog::selectionChanged);
  connect(m_ui->onTopBox, &QCheckBox::stateChanged, this, &DiskEditDialog::stayOnTopChanged);
  connect(m_ui->actionAddFiles, &QAction::triggered, this, &DiskEditDialog::addFilesTriggered);
  connect(m_ui->actionDeleteSelectedFiles, &QAction::triggered, this, &DiskEditDialog::deleteSelectedFilesTriggered);
  connect(m_ui->actionTextConversion, &QAction::triggered, this, &DiskEditDialog::textConversionTriggered);
  connect(m_ui->actionPrint, &QAction::triggered, this, &DiskEditDialog::printTriggered);
  connect(m_ui->actionToParent, &QAction::triggered, this, &DiskEditDialog::toParentTriggered);
  connect(m_ui->actionExtractFiles, &QAction::triggered, this, &DiskEditDialog::extractFilesTriggered);
  connect(m_ui->fileList, &QTableView::doubleClicked, this, &DiskEditDialog::fileListDoubleClicked);

  m_ui->fileList->viewport()->setAcceptDrops(true);
  if (RespeqtSettings::instance()->explorerOnTop()) {
    m_ui->onTopBox->setChecked(true);
    setWindowFlags(Qt::WindowStaysOnTopHint);
  }
}

DiskEditDialog::~DiskEditDialog() {
  m_disk->setEditDialog(nullptr);
  m_disk->unlock();
  delete m_ui;
}

void DiskEditDialog::changeEvent(QEvent *e) {
  QMainWindow::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      m_ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

void DiskEditDialog::go(DiskImages::SimpleDiskImage *image, int fileSystem) {
  m_disk = image;
  m_disk->lock();
  m_disk->setEditDialog(this);
  if (fileSystem < 0) {
    fileSystem = m_disk->defaultFileSystem();
  }
  if (fileSystem != m_fileSystemBox->currentIndex()) {
    m_fileSystemBox->setCurrentIndex(fileSystem);
  } else {
    fileSystemChanged(fileSystem);
  }
}

void DiskEditDialog::fileSystemChanged(const int index) {
  Filesystems::AtariFileSystem *a = nullptr;
  switch (index) {
    default:
      break;
    case 1:
      a = new Filesystems::Dos10FileSystem(m_disk);
      break;
    case 2:
      a = new Filesystems::Dos20FileSystem(m_disk);
      break;
    case 3:
      a = new Filesystems::Dos25FileSystem(m_disk);
      break;
    case 4:
      a = new Filesystems::MyDosFileSystem(m_disk);
      break;
    case 5:
      a = new Filesystems::SpartaDosFileSystem(m_disk);
      break;
  }

  model->setFileSystem(a);
  m_ui->fileList->resizeColumnToContents(0);
  m_ui->fileList->resizeColumnToContents(1);
  m_ui->fileList->resizeColumnToContents(2);
  m_ui->fileList->resizeColumnToContents(3);
  m_ui->fileList->resizeColumnToContents(4);
  m_ui->fileList->resizeColumnToContents(5);

  if (!a) {
    m_ui->fileList->setEnabled(false);
    m_ui->actionAddFiles->setEnabled(false);
    m_ui->actionExtractFiles->setEnabled(false);
    m_ui->actionTextConversion->setEnabled(false);
  } else {
    m_ui->fileList->setEnabled(true);
    m_ui->actionAddFiles->setEnabled(true);
    m_ui->actionTextConversion->setEnabled(true);
  }

  setWindowTitle(tr("RespeQt - Exploring %1").arg(model->currentPath()));
  m_ui->actionToParent->setEnabled(false);
}

void DiskEditDialog::selectionChanged(const QItemSelection & /*selected*/, const QItemSelection & /*deselected*/) const
{
  const bool enabled = !m_ui->fileList->selectionModel()->selectedIndexes().isEmpty();
  m_ui->actionExtractFiles->setEnabled(enabled);
  m_ui->actionDeleteSelectedFiles->setEnabled(enabled);
}

void DiskEditDialog::fileListDoubleClicked(const QModelIndex& index) {
  if (model->entries.at(index.row()).attributes & Filesystems::AtariDirEntry::Directory) {
    model->setDirectory(index.row());
    m_ui->fileList->resizeColumnToContents(0);
    m_ui->fileList->resizeColumnToContents(1);
    m_ui->fileList->resizeColumnToContents(2);
    m_ui->fileList->resizeColumnToContents(3);
    m_ui->fileList->resizeColumnToContents(4);
    m_ui->fileList->resizeColumnToContents(5);
    setWindowTitle(tr("RespeQt - Exploring %1").arg(model->currentPath()));
    m_ui->actionToParent->setEnabled(true);
  }
}

void DiskEditDialog::toParentTriggered() {
  model->toParent();
  m_ui->fileList->resizeColumnToContents(0);
  m_ui->fileList->resizeColumnToContents(1);
  m_ui->fileList->resizeColumnToContents(2);
  m_ui->fileList->resizeColumnToContents(3);
  m_ui->fileList->resizeColumnToContents(4);
  m_ui->fileList->resizeColumnToContents(5);
  setWindowTitle(tr("RespeQt - Exploring %1").arg(model->currentPath()));
  m_ui->actionToParent->setEnabled(!model->isRoot());
}

void DiskEditDialog::extractFilesTriggered() {
  QModelIndexList indexes = m_ui->fileList->selectionModel()->selectedRows();
  if (indexes.isEmpty()) {
    return;
  }

  const QString target = QFileDialog::getExistingDirectory(this, tr("Extract files"), RespeqtSettings::instance()->lastExtractDir());

  if (target.isEmpty()) {
    return;
  }

  RespeqtSettings::instance()->setLastExtractDir(target);

  QList<Filesystems::AtariDirEntry> selectedEntries;
  foreach (QModelIndex i, indexes) {
    if (i.isValid() && i.column() == 0) {
      selectedEntries.append(model->entries.at(i.row()));
    }
  }
  model->fileSystem->extractRecursive(selectedEntries, target);
}

void DiskEditDialog::textConversionTriggered() const
{
  if (m_ui->actionTextConversion->isChecked()) {
    m_ui->actionTextConversion->setToolTip(tr("Text conversion is on"));
    m_ui->actionTextConversion->setStatusTip(tr("Text conversion is on"));
    model->fileSystem->setTextConversion(true);
  } else {
    m_ui->actionTextConversion->setToolTip(tr("Text conversion is off"));
    m_ui->actionTextConversion->setStatusTip(tr("Text conversion is off"));
    model->fileSystem->setTextConversion(false);
  }
}

void DiskEditDialog::deleteSelectedFilesTriggered() {
  if (QMessageBox::question(this, tr("Confirmation"), tr("Are you sure you want to delete selected files?"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes) {
    return;
  }
  const QModelIndexList indexes = m_ui->fileList->selectionModel()->selectedRows();
  model->deleteFiles(indexes);
  m_ui->fileList->selectionModel()->clearSelection();
}

void DiskEditDialog::addFilesTriggered() {
  const QStringList files = QFileDialog::getOpenFileNames(this, tr("Add files"), RespeqtSettings::instance()->lastExtractDir());
  if (files.empty()) {
    return;
  }
  model->insertFiles(files);
}

//
void DiskEditDialog::printTriggered() {

  QPrinter printer;

  if (const auto dialog {new QPrintDialog(&printer, this)}; dialog->exec() != QDialog::Accepted)
    return;

  QTextEdit dirlist;
  dirlist.clear();
  dirlist.append("Name\tExt\tSize");
  dirlist.append("========\t===\t====");

  for (const auto & entry : model->entries) {
    QString fname = entry.atariName.left(8);
    QString ext = entry.atariName.right(3);
    const int size = entry.size;
    dirlist.append(fname + "\t" + ext + "\t" + QString("%1").arg(size));
  }

  dirlist.print(&printer);
}
void DiskEditDialog::stayOnTopChanged() {
  if (m_ui->onTopBox->isChecked()) {
    setWindowFlags(Qt::WindowStaysOnTopHint);
    RespeqtSettings::instance()->setExplorerOnTop(true);
  } else {
    setWindowFlags(Qt::WindowStaysOnBottomHint);
    RespeqtSettings::instance()->setExplorerOnTop(false);
  }
  show();
}

} // namespace UI
