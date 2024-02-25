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
    _settings = RespeqtSettings::instance()->mSettings;

    DbIni::load();
}

DbIni::~DbIni()
{
    DbIni::save();
}

void DbIni::setDataDir(const QString &dir)
{
    qDebug() << "!e" << "Disk Collection Browser: ini file format not used with .respeqt_db subdirs";
    Q_ASSERT(0);
}

void DbIni::setPicture(const QString& pic, const QString& dir, const QString& disk)
{
    bool isGlobal = (dir.isEmpty() && disk.isEmpty());     // program global pic?
    bool isDirPic = (!dir.isEmpty() && disk.isEmpty());
    bool isDiskPic = (!dir.isEmpty() && !disk.isEmpty());
    QString escDir = QDir::fromNativeSeparators(dir).replace('/','@');

    if (isGlobal)
        _diskPic = pic;

    if (isDirPic)
        _dirMap[escDir].pic = pic;

    if (isDiskPic)
        _dirMap[escDir].map[disk].pic = pic;

    _dirty = true;
}

QString DbIni::getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource)
{
    QString dirStr  = dir.absolutePath();
    QString escDir  = QDir::fromNativeSeparators(dirStr).replace('/','@');
    auto    dirInfo = _dirMap[escDir];

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

void DbIni::setTitle(const QString& title, const QString& folder, const QString& disk)
{
    QString escDir = QDir::fromNativeSeparators(folder).replace('/','@');
    _dirMap[escDir].map[disk].label.title = title;
    _dirty = true;
}

void DbIni::setIndex(const QString& index, const QString& folder, const QString& disk)
{
    QString escDir = QDir::fromNativeSeparators(folder).replace('/','@');
    _dirMap[escDir].map[disk].label.index = index;
    _dirty = true;
}

void DbIni::setSideB(bool sideB, const QString& folder, const QString& disk)
{
    QString escDir = QDir::fromNativeSeparators(folder).replace('/','@');
    _dirMap[escDir].map[disk].label.sideB = sideB;
    _dirty = true;
}

DiskLabel DbIni::getLabel(const QDir& dir, const QString& disk)
{
    QString folder = dir.absolutePath();
    QString escDir = QDir::fromNativeSeparators(folder).replace('/','@');
    return _dirMap[escDir].map[disk].label;
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
            dirInfo.pic = _settings->value("pic").toString();

        foreach (const QString& childGroup, _settings->childGroups())
        {
            _settings->beginGroup(childGroup);

            if (_settings->contains("pic"))
                dirInfo.map[childGroup].pic = _settings->value("pic").toString();
            if (_settings->contains("title"))
                dirInfo.map[childGroup].label.title = _settings->value("title").toString();
            if (_settings->contains("index"))
                dirInfo.map[childGroup].label.index = _settings->value("index").toString();
            if (_settings->contains("sideb"))
                dirInfo.map[childGroup].label.sideB  = _settings->value("sideb").toBool();

            _settings->endGroup();
        }
        _dirMap[group] = dirInfo;

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
        _settings->setValue("pic", _diskPic);

    for (auto it = _dirMap.begin(); it != _dirMap.end(); ++it)
    {
        DirInfo& dirInfo = it.value();
        QString  escDir  = it.key();

        _settings->beginGroup(escDir);

        if (!dirInfo.pic.isEmpty())
            _settings->setValue("pic",dirInfo.pic);

        for (auto it = dirInfo.map.begin(); it != dirInfo.map.end(); ++it)
        {
            const FloppyArt& art = it.value();
            const QString group  = it.key();

            if (art.isEmpty())
                continue;

            _settings->beginGroup(group);
            if (!art.pic.isEmpty())
                _settings->setValue("pic", art.pic);
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
