/*
 * dbsettings.cpp - implementation of DiskBrowserDialog's DbSettings class.
 *
 * Copyright 2023 D.Caputi
 */
#include "diskbrowser/dbini.h"
#include "respeqtsettings.h"
#include <QDir>

DbIni::DbIni()
{
    _appData  = RespeqtSettings::instance()->appDataFolder();
    _settings = RespeqtSettings::instance()->mSettings;         // did I make mSettings public for this? (probably bad)
                                                                // _settings was also once optionally used for seperate ini file.
                                                                // This is now only a ref to the app QSetting instance (TBD: fix).
    DbIni::load();
}

DbIni::~DbIni()
{
    if (_dirty)
        DbIni::save();
}

void DbIni::setDataDir(const QString&)
{
    qDebug() << "!e" << "Disk Collection Browser: ini file format not used with .respeqt_db subdirs";
    Q_ASSERT(0);
}

void DbIni::setPicture(const QString& pic, const QString& folder, const QString& disk)
{
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

QString DbIni::getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource)
{
    const QString dirStr  {dir.absolutePath()};
    const QString lnxDir  {QDir::fromNativeSeparators(dirStr)};
    auto    [picturePath, artMap] {_dirMap[lnxDir]};

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

void DbIni::setTitle(const QString& title, const QString& folder, const QString& disk)
{
    const QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label.title = title;
    _dirty = true;
}

void DbIni::setIndex(const QString& index, const QString& folder, const QString& disk)
{
    const QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label.index = index;
    _dirty = true;
}

void DbIni::setSideB(const bool sideB, const QString& folder, const QString& disk)
{
    const QString lnxDir = QDir::fromNativeSeparators(folder);
    _dirMap[lnxDir].map[disk].label.sideB = sideB;
    _dirty = true;
}

DiskLabel DbIni::getLabel(const QDir& dir, const QString& disk)
{
    const QString folder = dir.absolutePath();
    const QString lnxDir = QDir::fromNativeSeparators(folder);
    return _dirMap[lnxDir].map[disk].label;
}

bool DbIni::load()
{
    _settings->beginGroup("/DiskBrowserDlg");
    _settings->beginGroup("Artwork");

    // global Disk Browser settings (do not step on current/default values)
    if (_settings->contains("pic"))
        _diskPic = _settings->value("pic").toString();
    if (_settings->contains("b_side_pic"))
        _bSidePic = _settings->value("b_side_pic").toString();
    if (_settings->contains("title_font"))
        _titleFont = _settings->value("title_font").toString();
    if (_settings->contains("index_font"))
        _indexFont = _settings->value("index_font").toString();

    foreach (QString group, _settings->childGroups())
    {
        DirInfo dirInfo;

        _settings->beginGroup(group);

        if (_settings->contains("pic"))
            dirInfo.pic = _settings->value("pic").toString().replace('@','/');

        foreach (const QString& childGroup, _settings->childGroups())
        {
            _settings->beginGroup(childGroup);

            if (_settings->contains("pic"))
                dirInfo.map[childGroup].pic = _settings->value("pic").toString().replace('@','/');
            if (_settings->contains("title"))
                dirInfo.map[childGroup].label.title = _settings->value("title").toString();
            if (_settings->contains("index"))
                dirInfo.map[childGroup].label.index = _settings->value("index").toString();
            if (_settings->contains("sideb"))
                dirInfo.map[childGroup].label.sideB  = _settings->value("sideb").toBool();

            _settings->endGroup();
        }
        _dirMap[group.replace('@','/')] = dirInfo;

        _settings->endGroup();
    }

    _settings->endGroup();  // Artwork
    _settings->endGroup();  // DiskBrowserDlg

    _dirty = false;

    return true;
}

bool DbIni::save()
{
    _settings->beginGroup("/DiskBrowserDlg");
    _settings->beginGroup("Artwork");
    _settings->remove("");

    if (!_diskPic.isEmpty())
        _settings->setValue("pic", _diskPic.replace('/','@'));

    for (auto it {_dirMap.begin()}; it != _dirMap.end(); ++it)
    {
        auto& [picturePath, artMap] = it.value();
        QString  escDir  = it.key();
        escDir.replace('/','@');

        _settings->beginGroup(escDir);

        if (!picturePath.isEmpty())
            _settings->setValue("pic", picturePath.replace('/','@'));

        for (auto it2 {artMap.begin()}; it2 != artMap.end(); ++it2)
        {
            FloppyArt art {it2.value()};
            const QString& group {it2.key()};

            if (art.isEmpty())
                continue;

            _settings->beginGroup(group);
            if (!art.pic.isEmpty())
                _settings->setValue("pic", art.pic.replace('/','@'));
            if (!art.label.title.isEmpty())
                _settings->setValue("title", art.label.title);
            if (!art.label.index.isEmpty())
                _settings->setValue("index", art.label.index);

            _settings->setValue("sideb", art.label.sideB);
            _settings->endGroup();
        }
        _settings->endGroup();
    }

    _settings->endGroup();  // Artwork
    _settings->endGroup();  // DiskBrowserDlg

    _dirty = false;

    return true;
}

/*
void DbIni::clear()
{
    _settings->clear();
    _diskPic.clear();
    _bSidePic.clear();
    _labelPos = { QRect(), QRect() };
    _bSidePos = { QRect(), QRect() };
    _dirty = false;
    _dirMap.clear();
}*/
