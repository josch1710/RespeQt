#ifndef DISKBROWSERDLG_H
#define DISKBROWSERDLG_H

#include <QDialog>
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

private:
    void refreshFoldersCombobox();
    QString getMostRecentFolder();
    QString getMostRecentDisk();
    QString getRecentDisk(QString folder);

    Ui::DiskBrowserDlg *ui;
    FolderDisks folderDisks;
    SioWorkerPtr sio;

private slots:
    void onBrowseFolder();
    void onDiskChanged();
    void onFolderChanged(QString lastDir);
};

#endif // DISKBROWSERDLG_H
