#ifndef DBSETTINGS_H
#define DBSETTINGS_H

#include <QSettings>
#include <QDir>
#include <QRect>
#include <QColor>
#include "folderdisks.h"
#include "picsourcetype.h"

// [db]
// pic=<filepath>
// title_pos=@Rect(x,y,w,h) ?
// title_pos_b=
// index_pos=
// index_pos_b=
//
// [<path>]
// pic=<file>   (path-wide preview)
// <disk>\title=<text>
// <disk>\index=<text>   example: "01"
// <disk>\sideb=[true|false]
//

typedef QMap<QString,FloppyArt> ArtMap;

struct DirInfo
{
    QString pic;    // dir/folder default pic
    ArtMap  map;    // disk-name -> FloppyArt

    bool isEmpty() const {return pic.isEmpty() && map.isEmpty();}
};

// FolderMap - map/collection structured for above scheme
// key: <folderpath>
// -> DirInfo:
//    - default pic for whole dir
//    - ArtMap[disk name] -> FloppyArt { pic, label text }
//
typedef QMap<QString,DirInfo> DirMap;


struct LabelPos
{
    QRect title;
    QRect index;

    bool isEmpty() { return title.isEmpty() && index.isEmpty(); }
};


class DbSettings
{
public:
    explicit DbSettings();
    virtual ~DbSettings();

    virtual void setDataDir(const QString& dir) = 0;

    virtual void setPicture(const QString& pic, const QString& folder = QString(), const QString& disk = QString()) = 0;
    virtual QString getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource) = 0;

    virtual void setTitle(const QString& title, const QString& folder, const QString& disk) = 0;
    virtual void setIndex(const QString& index, const QString& folder, const QString& disk) = 0;
    virtual void setSideB(bool sideB, const QString& folder, const QString& disk) = 0;
    virtual DiskLabel getLabel(const QDir& dir, const QString& disk) = 0;

    virtual bool load()  = 0;
    virtual bool save()  = 0;
//  virtual void clear() = 0;

    bool isEmpty();
    void clone(DbSettings& other);
    void merge(DbSettings& other);
    virtual bool isJson() = 0;          // TBD omit (not used)

    DirMap& getDirMap() { return _dirMap; }
    QStringList getDirs() { return _dirMap.keys(); }
    QString getDirPic(const QString& dir) { return _dirMap[dir].pic; }
    QStringList getDisks(const QString& dir) { return _dirMap[dir].map.keys(); }

protected:
    bool _dirty = false;

    QString  _appData;
    QString  _diskPic;
    QString  _bSidePic; // used?
//  LabelPos _labelPos; not used
//  LabelPos _bSidePos; not used
    QString  _titleFont;
    QString  _indexFont;
    DirMap   _dirMap;
};

#endif
