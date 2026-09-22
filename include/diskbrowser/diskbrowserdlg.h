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
#include "sio/sioworker.h"
#include "picsourcetype.h"


namespace Ui {
class DiskBrowserDlg;
}


class DbItem : public QTreeWidgetItem
{
public:
    explicit DbItem(QTreeWidget* parent) : QTreeWidgetItem(parent) {}
    [[nodiscard]] bool isFolder() const { return data(0, Qt::UserRole).toBool(); }
    void setFolder(const bool folder = true) { setData(0, Qt::UserRole, folder); }
    bool operator<(const QTreeWidgetItem& other) const override;

private:
    bool compNumberVal(const QString& index, const QString& other, bool& comp) const;
};


class DiskBrowserDlg : public QDialog
{
    Q_OBJECT

public:
    explicit DiskBrowserDlg(SIO::SioWorkerPtr pSio, QWidget *parent = nullptr);
    ~DiskBrowserDlg() override;

    [[nodiscard]] int getHorzSplitPos() const;
    [[nodiscard]] int getVertSplitPos() const;
    void setHorzSplitPos(int pos) const;
    void setVertSplitPos(int pos) const;

protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void clear() const;
    //void setItemIsFolder(QTreeWidgetItem* item, bool isFolder = true);
    bool itemIsFolder(QTreeWidgetItem* item);
    void refreshFoldersCombobox() const;
    void updateDiskBrowser();
    QString checkCopyPic(const QString& fname);
    QString findPicFile();
    [[nodiscard]] QString getFloppyPic() const;
    QString getMostRecentFolder();
    QString getMostRecentDisk();
    QString getRecentDisk(const QString& folder);
    QString browseForPic(const QString& start, const QString& action);
    [[nodiscard]] QString diskIndex(const QString& folder, const QString& disk) const;
    [[nodiscard]] DiskLabel parsePicLabel(const QString& diskName = QString()) const;

    const QString FLOPPY_INDEXED_PNG  {":/icons/other-icons/floppy_front.png"};
    const QString FLOPPY_BACKSIDE_PNG {":/icons/other-icons/floppy_back.png"};

    Ui::DiskBrowserDlg *ui;
    FolderDisks _folderDisks;
    FloppyArt _picInfo;
    SIO::SioWorkerPtr sio;
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
    void titleChanged(const QString& title);
    void indexChanged(const QString& index);
    void onBrowseFolder();
    void itemSelectionChanged();
    void onFolderChanged(QString lastFolder);
    void itemDoubleClicked(QTreeWidgetItem* item, int col);
};

#endif // DISKBROWSERDLG_H
