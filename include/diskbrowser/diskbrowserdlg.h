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


namespace Ui {
class DiskBrowserDlg;
}


class DbItem : public QTreeWidgetItem
{
public:
    DbItem(QTreeWidget* parent) : QTreeWidgetItem(parent) {}
    bool isFolder() const { return data(0, Qt::UserRole).toBool(); }
    void setFolder(bool folder = true) { setData(0, Qt::UserRole, folder); }

private:
    bool operator<(const QTreeWidgetItem& other) const override;
    bool compNumberVal(const QString& index, const QString& other, bool& comp) const;
};


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
    void update();
    QString checkCopyPic(const QString& fname);
    QString findPicFile();
    QString getFloppyPic();
    QString getMostRecentFolder();
    QString getMostRecentDisk();
    QString getRecentDisk(QString folder);
    QString browseForPic(const QString& start, const QString& action);
    QString diskIndex(const QString& folder, const QString& disk);
    DiskLabel parsePicLabel(const QString& diskName = QString());

    const QString FLOPPY_INDEXED_PNG  {":/icons/other-icons/floppy_front.png"};
    const QString FLOPPY_BACKSIDE_PNG {":/icons/other-icons/floppy_back.png"};

    Ui::DiskBrowserDlg *ui;
    FolderDisks _folderDisks;
    FloppyArt _picInfo;
    SioWorkerPtr sio;
    QString _currentDir;
    QString _diskTitle;
    QString _diskFileName;
    QString _diskFullName;
    PicSourceType _picSource  = PicSource_none;

    void actionSetPic();
    void actionSetDirPic();
    void actionSetDefault();
    void actionSetTitle();
    void actionSetIndex();
    void actionClearPic();
    void actionBackSide();

private slots:
    void popupMenuReq(const QPoint& pos);
    void titleChanged(QString title);
    void indexChanged(QString index);
    void onBrowseFolder();
    void itemSelectionChanged();
    void onFolderChanged(QString lastDir);
    void itemDoubleClicked(QTreeWidgetItem* item, int col);
};

#endif // DISKBROWSERDLG_H
