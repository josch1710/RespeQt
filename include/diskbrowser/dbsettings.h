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
// size=1
// 0\title=<text>
// 0\index=<text>   example: "01"
// 0\side=<a|b>
//

typedef QMap<QString,FloppyArt> ArtMap;

struct DirInfo
{
    QString pic;    // dir/folder default pic
    ArtMap  map;    // disk-name -> FloppyArt
};

// FolderMap - map/collection structured for above scheme
// key: <folderpath>
// -> DirInfo:
//    - default pic for whole dir
//    - ArtMap[disk name] -> FloppyArt { pic, label text }
//
typedef QMap<QString,DirInfo> DirMap;

struct LabelFont
{
    QString name;
    int     size = 0;
    QColor  color;
    bool    bold = false;
    bool    italic = false;
};

struct LabelPos
{
    QRect title;
    QRect index;
};


class DbSettings
{
public:
    explicit DbSettings();
    virtual ~DbSettings();

    void setPicture(const QString& pic, const QString& folder = QString(), const QString& disk = QString());
    QString getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource);

    void load();
    void save();
    void clear();

private:
    QSettings* _settings = nullptr;
    bool _useAppSettings = false;
    bool _dirty = false;

    QString  _diskPic;
    QString  _bSidePic;
    LabelPos _labelPos;
    LabelPos _bSidePos;
    DirMap   _dirMap;
};

#endif // DBSETTINGS_H
