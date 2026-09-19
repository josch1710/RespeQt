#include "diskbrowser/dbjson.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "respeqtsettings.h"

DbJson::DbJson()
{    
    if (RespeqtSettings::instance()->dbDataSource() == DbData_appFolderJson)
        DbJson::setDataDir(RespeqtSettings::instance()->appDataFolder());
}

DbJson::~DbJson()
{
    if (_dirty)
        DbJson::save();
}

void DbJson::setDataDir(const QString& dir)
{
    _dataDir.setPath(dir + "/.respeqt_db");
    // ReSharper disable once CppExpressionWithoutSideEffects
    _dataDir.mkpath(".");

    _fileName = _dataDir.absoluteFilePath("dbSettings.json");

    DbJson::load();
}

void DbJson::setPicture(const QString& pic, const QString& folder, const QString& disk)
{
    if (pic.isEmpty())
        return;

    const bool isGlobal = folder.isEmpty() && disk.isEmpty();     // program global pic?
    const bool isDirPic = !folder.isEmpty() && disk.isEmpty();
    const bool isDiskPic = !folder.isEmpty() && !disk.isEmpty();
    const QString lnxDir = QDir::fromNativeSeparators(folder);

    if (isGlobal)
        _diskPic = pic;

    if (isDirPic)
        _dirMap[lnxDir].pic = pic;

    if (isDiskPic)
        _dirMap[lnxDir].map[disk].pic = pic;

    _dirty = true;
}

void DbJson::setLabel(const DiskLabel& label, const QString& folder, const QString& disk)
{
    const QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label = label;
    _dirty = true;
}

QString DbJson::getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource)
{
    const QString dirStr  = dir.absolutePath();
    const QString lnxDir  = QDir::fromNativeSeparators(dirStr);
    auto [picturePath, artMap] = _dirMap[lnxDir];

    QString pic;
    picSource = PicSource_none;

    if (!artMap[disk].pic.isEmpty())
    {
        picSource = PicFromJson_disk;
        pic = artMap[disk].pic;
    }
    else if (!picturePath.isEmpty())
    {
        picSource = PicFromJson_dir;
        pic = picturePath;
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
    const QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label.title = title;
    _dirty = true;
}

void DbJson::setIndex(const QString& index, const QString& folder, const QString& disk)
{
    const QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label.index = index;
    _dirty = true;
}

void DbJson::setSideB(const bool sideB, const QString& folder, const QString& disk)
{
    const QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label.sideB = sideB;
    _dirty = true;
}

DiskLabel DbJson::getLabel(const QDir& dir, const QString& disk)
{
    const QString folder = dir.absolutePath();
    const QString lnxDir = QDir::fromNativeSeparators(folder);
    return _dirMap[lnxDir].map[disk].label;
}

bool DbJson::load()
{
    if (_dirty)
        DbJson::save();

    QFile file {_fileName};
    file.open(QIODevice::ReadOnly);

    const auto data {file.readAll()};
    file.close();

    _jsDoc = QJsonDocument::fromJson(data);

    auto jsRoot {_jsDoc.object()};

    for (auto itRoot {jsRoot.begin()}; itRoot != jsRoot.end(); ++itRoot)
    {
        auto key   {itRoot.key()};
        auto jsObj {itRoot.value().toObject()};
        auto pic   {jsObj["pic"].toString()};

        pic = makeFullPath(pic);    // prepend path to copied pics

        if (key == "db")
        {
            _diskPic = pic;
            continue;
        }

        DirInfo dirInfo;
        dirInfo.pic = pic;

        for (auto itChild {jsObj.begin()}; itChild != jsObj.end(); ++itChild)
        {
            auto disk {itChild.key()};
            auto obj {itChild.value().toObject()};

            if (obj.contains("pic"))
                dirInfo.map[disk].pic = makeFullPath(obj["pic"].toString());
            if (obj.contains("title"))
                dirInfo.map[disk].label.title = obj["title"].toString();
            if (obj.contains("index"))
            {
                dirInfo.map[disk].label.index = obj["index"].toString();
                dirInfo.map[disk].label.sideB  = obj["sideb"].toBool();
            }
        }

        if (key == "ArtWork")                   // subdir stored JSON using copy files
        {
            QDir upDir(_dataDir);               // disk collection dir will be the parent
            upDir.cdUp();                       // back out of .respeqt_db
            key = upDir.absolutePath();
        }
        _dirMap[key] = dirInfo;
    }
    return true;    // TBD: error check?
}

QString DbJson::checkCopyPic(const QString& name) const
{
    // remove the path name if it's not needed
    // (allows collection/folder to be moved/copied)

    if (RespeqtSettings::instance()->dbDataSource() == DbData_subDirJson)
        return DbUtils::removePrefix(_dataDir.absolutePath(), name);

    return name;
}

QString DbJson::makeFullPath(const QString& name) const
{
    if (name.isEmpty())
        return name;

    if (!name.contains('/'))
        return QFileInfo(_fileName).absolutePath() + "/" + name;

    return name;
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

    auto it {_dirMap.begin()};  // set iterator to the first disk collection folder

    const bool useSubDir {RespeqtSettings::instance()->dbDataSource() == DbData_subDirJson};
    if (useSubDir)
    {
        QDir upDir(_dataDir);               // disk collection dir will be the parent
        upDir.cdUp();                       // back out of .respeqt_db to ..
        it = _dirMap.find(upDir.path());    // reset the iterator to current data dir

        Q_ASSERT(it != _dirMap.end());
    }

    while (it != _dirMap.end())
    {
        const QString  dirName = useSubDir ? QString("ArtWork") : it.key();
        const DirInfo& dirInfo = it.value();

        if (dirInfo.isEmpty())
            continue;

        QJsonObject jsDirObj;

        if (!dirInfo.pic.isEmpty())
            jsDirObj["pic"] = checkCopyPic(dirInfo.pic);

        for (auto it2 {dirInfo.map.begin()}; it2 != dirInfo.map.end(); ++it2)
        {
            const QString&   disk {it2.key()};
            const auto& [pic, label] {it2.value()};

            QJsonObject jsNode;

            if (!pic.isEmpty())
                jsNode["pic"] = checkCopyPic(pic);

            if (!label.title.isEmpty())
                jsNode["title"] = label.title;

            if (!label.index.isEmpty())
            {
                jsNode["index"] = label.index;
                jsNode["sideb"] = label.sideB;
            }

            if (!jsNode.isEmpty())
                jsDirObj[disk] = jsNode;
        }

        if (!jsDirObj.isEmpty())
            jsRoot[dirName] = jsDirObj;

        if (useSubDir)             // using JSON file in .respeqt_db dir?
            it = _dirMap.end();     // yes: we're done (mostly)
        else
            ++it;                   // no: grab the next dir (if any)
    }

    _jsDoc = QJsonDocument(jsRoot);

    const auto jsData {_jsDoc.toJson(QJsonDocument::Indented)};
    file.write(jsData);
    file.close();

    _dirty = false;
    return true;
}
