#include "diskbrowser/dbjson.h"
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>

DbJson::DbJson()
{
    auto    locType   = QStandardPaths::AppDataLocation;
    QString appFolder = QStandardPaths::writableLocation(locType);
    QDir appDataDir(appFolder);

    if (!appDataDir.exists())
        appDataDir.mkpath(".");

    _fileName = appDataDir.absoluteFilePath("dbSettings.json");

    load();
}

DbJson::~DbJson()
{
    save();
}

void DbJson::setPicture(const QString& pic, const QString& dir, const QString& disk)
{
    bool isGlobal = (dir.isEmpty() && disk.isEmpty());     // program global pic?
    bool isDirPic = (!dir.isEmpty() && disk.isEmpty());
    bool isDiskPic = (!dir.isEmpty() && !disk.isEmpty());
    QString lnxDir = QDir::fromNativeSeparators(dir);

    if (isGlobal)
        _diskPic = pic;

    if (isDirPic)
        _dirMap[lnxDir].pic = pic;

    if (isDiskPic)
        _dirMap[lnxDir].map[disk].pic = pic;

    _dirty = true;
}

QString DbJson::getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource)
{
    QString dirStr  = dir.absolutePath();
    QString lnxDir  = QDir::fromNativeSeparators(dirStr);
    DirInfo dirInfo = _dirMap[lnxDir];

    QString pic;
    picSource = PicSource_none;

    if (!dirInfo.map[disk].pic.isEmpty())
    {
        picSource = PicFromJson_disk;
        pic = dirInfo.map[disk].pic;
    }
    else if (!dirInfo.pic.isEmpty())
    {
        picSource = PicFromJson_dir;
        pic = dirInfo.pic;
    }
    else if (!_diskPic.isEmpty())
    {
        picSource = PicFromJson_global;
        pic = _diskPic;
    }

    return pic;
}

void DbJson::setTitle(const QString& title, const QString& folder, const QString& disk)
{
    QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label.title = title;
    _dirty = true;
}

void DbJson::setIndex(const QString& index, const QString& folder, const QString& disk)
{
    QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label.index = index;
    _dirty = true;
}

void DbJson::setSideB(bool sideB, const QString& folder, const QString& disk)
{
    QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label.sideB = sideB;
    _dirty = true;
}

DiskLabel DbJson::getLabel(const QDir& dir, const QString& disk)
{
    QString folder = dir.absolutePath();
    QString lnxDir = QDir::fromNativeSeparators(folder);
    return _dirMap[lnxDir].map[disk].label;
}

bool DbJson::load()
{
    QFile file {_fileName};
    file.open(QIODevice::ReadOnly);

    auto data = file.readAll();
    file.close();

    _jsDoc = QJsonDocument::fromJson(data);

    auto jsRoot = _jsDoc.object();

    for (auto itRoot = jsRoot.begin(); itRoot != jsRoot.end(); ++itRoot)
    {
        auto key   = itRoot.key();
        auto jsObj = itRoot.value().toObject();
        auto pic   = jsObj["pic"].toString();

        if (key == "db")
        {
            _diskPic = pic;
            continue;
        }

        DirInfo dirInfo;
        dirInfo.pic = pic;

        for (auto itChild = jsObj.begin(); itChild != jsObj.end(); ++itChild)
        {
            auto dir = itChild.key();
            auto obj = itChild.value().toObject();

            if (obj.contains("pic"))
                dirInfo.map[dir].pic = obj["pic"].toString();
            if (obj.contains("title"))
                dirInfo.map[dir].label.title = obj["title"].toString();
            if (obj.contains("index"))
            {
                dirInfo.map[dir].label.index = obj["index"].toString();
                dirInfo.map[dir].label.sideB  = obj["sideb"].toBool();
            }
        }

        _dirMap[key] = dirInfo;
    }
    return true;    // TBD: error check?
}

bool DbJson::save()
{
    QFile file {_fileName};
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QJsonObject jsRoot;

    if (!_diskPic.isEmpty())
    {
        QJsonObject jsObj;
        jsObj["pic"] = _diskPic;
        jsRoot["db"] = jsObj;
    }

    for (auto it = _dirMap.begin(); it != _dirMap.end(); ++it)
    {
        const QString  dirName = it.key();
        const DirInfo& dirInfo = it.value();

        if (dirInfo.isEmpty())
            continue;

        QJsonObject jsDirObj;

        if (!dirInfo.pic.isEmpty())
            jsDirObj["pic"] = dirInfo.pic;

        for (auto it = dirInfo.map.begin(); it != dirInfo.map.end(); ++it)
        {
            const QString   disk = it.key();
            const FloppyArt& art = it.value();

            QJsonObject jsNode;

            if (!art.pic.isEmpty())
                jsNode["pic"] = art.pic;

            if (!art.label.title.isEmpty())
                jsNode["title"] = art.label.title;

            if (!art.label.index.isEmpty())
            {
                jsNode["index"] = art.label.index;
                jsNode["sideb"] = art.label.sideB;
            }

            if (!jsNode.isEmpty())
                jsDirObj[disk] = jsNode;
        }

        if (!jsDirObj.isEmpty())
            jsRoot[dirName] = jsDirObj;
    }

    _jsDoc = QJsonDocument(jsRoot);

    auto jsData = _jsDoc.toJson(QJsonDocument::Indented);
    file.write(jsData);
    file.close();

    _dirty = false;
    return true;
}

/*static*/ void DbJson::Export()
{
    // TBD: omit?
}
