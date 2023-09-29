/*
 * diskbrowserdlg.h - declaration of RespeQt's DiskBrowserDlg class.
 *
 * Copyright 2023 D.Caputi
 */
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
    void parsePicInfo();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void clear();
    void setItemIsFolder(QTreeWidgetItem* item, bool isFolder = true);
    bool itemIsFolder(QTreeWidgetItem* item);
    void refreshFoldersCombobox();
    bool isDiskImage(const QString& name);
    QString findImage();
    QString getMostRecentFolder();
    QString getMostRecentDisk();
    QString getRecentDisk(QString folder);
    QString getParentDir(QString fileFolder);

    const QString FLOPPY_DEFAULT_PNG  {":/icons/other-icons/floppy.png"};
    const QString FLOPPY_INDEXED_PNG  {":/icons/other-icons/floppy_front.png"};
    const QString FLOPPY_BACKSIDE_PNG {":/icons/other-icons/floppy_back.png"};

    Ui::DiskBrowserDlg *ui;
    FolderDisks _folderDisks;
    FloppyArt _picInfo;
    SioWorkerPtr sio;
    QString _diskName;

    void actionSetPic();
    void actionSetDefault();
    void actionSetTitle();
    void actionSetIndex();
    void actionClear();

private slots:
    void popupMenuReq(const QPoint& pos);
    void onBrowseFolder();
    void itemSelectionChanged();
    void onFolderChanged(QString lastDir);
    void itemDoubleClicked(QTreeWidgetItem* item, int col);
};

#endif // DISKBROWSERDLG_H
