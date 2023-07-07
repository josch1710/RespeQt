#ifndef DISKBROWSERDLG_H
#define DISKBROWSERDLG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QShowEvent>
#include "folderdisks.h"
#include "sioworker.h"

namespace Ui {
class DiskBrowserDlg;
}

class DiskBrowserDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DiskBrowserDlg(SioWorkerPtr pSio, QWidget *parent = nullptr);
    ~DiskBrowserDlg();

    int getHorzSplitPos();
    int getVertSplitPos();
    void setHorzSplitPos(int pos);
    void setVertSplitPos(int pos);

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void clear();
    void setItemIsFolder(QTreeWidgetItem* item, bool isFolder = true);
    bool itemIsFolder(QTreeWidgetItem* item);
    void refreshFoldersCombobox();
    bool isDiskImage(const QString& name);
    QString getMostRecentFolder();
    QString getMostRecentDisk();
    QString getRecentDisk(QString folder);
    QString getParentDir(QString fileFolder);

    Ui::DiskBrowserDlg *ui;
    FolderDisks _folderDisks;
    SioWorkerPtr sio;

private slots:
    void onBrowseFolder();
    void itemSelectionChanged();
    void onFolderChanged(QString lastDir);
    void itemDoubleClicked(QTreeWidgetItem* item, int col);
};

#endif // DISKBROWSERDLG_H
