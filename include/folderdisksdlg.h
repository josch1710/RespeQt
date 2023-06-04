#ifndef FOLDERDISKSDLG_H
#define FOLDERDISKSDLG_H

#include <QDialog>
#include "folderdisks.h"
#include "sioworker.h"

namespace Ui {
class FolderDisksDlg;
}

class FolderDisksDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FolderDisksDlg(SioWorkerPtr pSio, QWidget *parent = nullptr);
    ~FolderDisksDlg();

    int getSplitPos();
    void setSplitPos(int pos);

private:
    void refreshFoldersCombobox();
    QString getMostRecentFolder();
    QString getMostRecentDisk();
    QString getRecentDisk(QString folder);

    Ui::FolderDisksDlg *ui;
    FolderDisks folderDisks;
    SioWorkerPtr sio;

private slots:
    void onBrowseFolder();
    void onDiskChanged();
    void onFolderChanged(QString lastDir);
};

#endif // FOLDERDISKSDLG_H
