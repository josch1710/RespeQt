#ifndef DBJSON_H
#define DBJSON_H

#include <qjsondocument.h>
#include <QDir>
#include <QRect>
#include <QColor>
#include "folderdisks.h"
#include "picsourcetype.h"
#include "dbsettings.h"

/*  JSON file format:
{
  "db": {
      "pic": "<filepath>",
      "title_pos": @Rect(x,y,w,h),
      "title_pos_b":
      "index_pos":
      "index_pos_b":
  },
  "<path>": {
      "pic": "<filepath>",    (path-wide preview)
      "diskname": {
          "title": "<title text>",
          "index": "<index text>",   (example: "01")
          "sideb":[true|false]
      }
  }
}
*/

class DbJson : public DbSettings
{
public:
    explicit DbJson();
    virtual ~DbJson();

    void setDataDir(const QString& dir);

    void setPicture(const QString& pic, const QString& folder = QString(), const QString& disk = QString());
    QString getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource);

    void setTitle(const QString& title, const QString& folder, const QString& disk);
    void setIndex(const QString& index, const QString& folder, const QString& disk);
    void setSideB(bool sideB, const QString& folder, const QString& disk);
    DiskLabel getLabel(const QDir& dir, const QString& disk);
    void setLabel(const DiskLabel& label, const QString& folder, const QString& disk);

    bool load();
    bool save();
//  void clear();

    bool isJson() { return true; }

private:
    QJsonDocument _jsDoc;
    QString       _fileName;
    QDir          _dataDir;

    QString checkCopyPic(const QString& name);
    QString makeFullPath(const QString& name);
};

#endif
