#ifndef DBINI_H
#define DBINI_H

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
    ~DbIni() override;

    void setDataDir(const QString& dir) override;    // not used (used only with JSON impl)

    void setPicture(const QString& pic, const QString& folder = QString(), const QString& disk = QString()) override;
    QString getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource) override;

    void setTitle(const QString& title, const QString& folder, const QString& disk) override;
    void setIndex(const QString& index, const QString& folder, const QString& disk) override;
    void setSideB(bool sideB, const QString& folder, const QString& disk) override;
    DiskLabel getLabel(const QDir& dir, const QString& disk) override;

    bool load() override;
    bool save() override;
//  void clear();

    bool isJson() override { return false; }

private:
    QSettings* _settings = nullptr;
};
#endif // DBINI_H
