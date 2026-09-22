/*
 * diskeditdialog.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef DISKEDITDIALOG_H
#define DISKEDITDIALOG_H

#include <QMainWindow>

namespace Ui {
  class DiskEditDialog;
}

#include <QComboBox>
#include <QItemSelection>
#include <QMimeData>

#include "filesystems/atarifilesystem.h"

namespace UI {

class FileModel : public QAbstractTableModel {
  Q_OBJECT

public:
  explicit FileModel(QObject *parent);
  ~FileModel() override;
  QList<Filesystems::AtariDirEntry> entries;
  Filesystems::AtariFileSystem *fileSystem;
  [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
  [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role) override;
  void deleteFiles(const QModelIndexList& indexes);
  [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
  void setDirectory(int row);
  void toParent();
  void setRoot();
  void setFileSystem(Filesystems::AtariFileSystem *aFileSystem);
  [[nodiscard]] QMimeData *mimeData(const QModelIndexList &indexes) const override;
  [[nodiscard]] QStringList mimeTypes() const override;
  void insertFiles(const QStringList& names);
  bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
  [[nodiscard]] bool isRoot() const { return dirs.count() == 1; }
  [[nodiscard]] QString currentPath() const { return m_currentPath; }

private:
  QString m_currentPath;
  QStringList paths;
  QStringList *tempDirs;

  QList<quint16> dirs;
};

class DiskEditDialog : public QMainWindow {
  Q_OBJECT

public:
  explicit DiskEditDialog(QWidget *parent = nullptr);
  ~DiskEditDialog() override;
  void go(DiskImages::SimpleDiskImage *image, int fileSystem = -1);

protected:
  FileModel *model;
  void changeEvent(QEvent *e) override;

private:
  ::Ui::DiskEditDialog *m_ui;
  DiskImages::SimpleDiskImage *m_disk{};
  QComboBox *m_fileSystemBox;

private slots:
  void addFilesTriggered();
  void deleteSelectedFilesTriggered();
  void textConversionTriggered() const;
  void extractFilesTriggered();
  void toParentTriggered();
  void printTriggered();
  void fileListDoubleClicked(const QModelIndex& index);
  void stayOnTopChanged();

  void fileSystemChanged(int index);
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) const;
};

} // namespace UI

#endif// DISKEDITDIALOG_H
