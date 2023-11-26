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
#include "dbsettings.h"
#include "picsourcetype.h"
#include "dbjson.h"

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
    DiskLabel parsePicLabel();

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void clear();
    void setItemIsFolder(QTreeWidgetItem* item, bool isFolder = true);
    bool itemIsFolder(QTreeWidgetItem* item);
    void refreshFoldersCombobox();
    bool isDiskImage(const QString& name);
    void update();
    QString findPicFile();
    QString getFloppyPic();
    QString getMostRecentFolder();
    QString getMostRecentDisk();
    QString getRecentDisk(QString folder);
    QString getParentDir(QString fileFolder);
    QString browseForPic(const QString& start);

    const QString FLOPPY_INDEXED_PNG  {":/icons/other-icons/floppy_front.png"};
    const QString FLOPPY_BACKSIDE_PNG {":/icons/other-icons/floppy_back.png"};

    Ui::DiskBrowserDlg *ui;
    FolderDisks _folderDisks;
    FloppyArt _picInfo;
    SioWorkerPtr sio;
    QString _currentDir;
    QString _diskFileName;
    QString _diskFullName;
    PicSourceType _picSource = PicSource_none;
    DbSettings _dbSettings;
    DbJson _dbJson;

    void actionSetPic();
    void actionSetDirPic();
    void actionSetDefault();
    void actionSetTitle();
    void actionSetIndex();
    void actionClearPic();

private slots:
    void popupMenuReq(const QPoint& pos);
    void titleChanged(QString title);
    void onBrowseFolder();
    void itemSelectionChanged();
    void onFolderChanged(QString lastDir);
    void itemDoubleClicked(QTreeWidgetItem* item, int col);
};

#endif // DISKBROWSERDLG_H
