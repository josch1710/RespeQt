#ifndef DBJSON_H
#define DBJSON_H

#include <qjsondocument.h>
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
    ~DbJson() override;

    void setDataDir(const QString& dir) override;

    void setPicture(const QString& pic, const QString& folder = QString(), const QString& disk = QString()) override;
    QString getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource) override;

    void setTitle(const QString& title, const QString& folder, const QString& disk) override;
    void setIndex(const QString& index, const QString& folder, const QString& disk) override;
    void setSideB(bool sideB, const QString& folder, const QString& disk) override;
    DiskLabel getLabel(const QDir& dir, const QString& disk) override;
    void setLabel(const DiskLabel& label, const QString& folder, const QString& disk);

    bool load() override;
    bool save() override;
//  void clear();

    bool isJson() override { return true; }

private:
    QJsonDocument _jsDoc;
    QString       _fileName;
    QDir          _dataDir;

    [[nodiscard]] QString checkCopyPic(const QString& name) const;
    [[nodiscard]] QString makeFullPath(const QString& name) const;
};

#endif
