#ifndef DBINI_H
#define DBINI_H

#include <QSettings>
#include <QDir>
#include <QRect>
#include <QColor>
#include "folderdisks.h"
#include "picsourcetype.h"
#include "diskbrowser/dbsettings.h"

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

class DbIni : public DbSettings
{
public:
    explicit DbIni();
    virtual ~DbIni();

    void setDataDir(const QString& dir);    // not used (used only with JSON impl)

    void setPicture(const QString& pic, const QString& folder = QString(), const QString& disk = QString());
    QString getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource);

    void setTitle(const QString& title, const QString& folder, const QString& disk);
    void setIndex(const QString& index, const QString& folder, const QString& disk);
    void setSideB(bool sideB, const QString& folder, const QString& disk);
    DiskLabel getLabel(const QDir& dir, const QString& disk);

    bool load();
    bool save();
//  void clear();

    bool isJson() { return false; }

private:
    QSettings* _settings = nullptr;
};
#endif // DBSETTINGS_H
