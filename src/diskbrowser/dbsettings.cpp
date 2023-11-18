/*
 * dbsettings.cpp - implementation of DiskBrowserDialog's DbSettings class.
 *
 * Copyright 2023 D.Caputi
 */
#include "diskbrowser/dbsettings.h"
#include "respeqtsettings.h"
#include <QStandardPaths>
#include <QDir>

DbSettings::DbSettings()
{
    auto    locType = QStandardPaths::AppDataLocation;
    QString appFolder = QStandardPaths::writableLocation(locType);
    QDir appDataDir(appFolder);

    if (!appDataDir.exists())
        appDataDir.mkpath(".");

    QString file = appDataDir.absoluteFilePath("dbSettings.ini");
    _settings = new QSettings(file, QSettings::IniFormat);

    bool ok = (_settings && _settings->isWritable() && _settings->status() == QSettings::NoError);
    if (!ok)
    {
        // TBD: overkill? maybe just error popup (but how?)
        _settings = RespeqtSettings::instance()->mSettings;
        _useAppSettings = true;
    }
    load();
}

DbSettings::~DbSettings()
{
    if (_dirty)
        save();

    delete _settings;
}

void DbSettings::setPicture(const QString& pic, const QString& dir, const QString& disk)
{
    bool isGlobal = (dir.isEmpty() && disk.isEmpty());     // program global pic?
    bool isDirPic = (!dir.isEmpty() && disk.isEmpty());
    bool isDiskPic = (!dir.isEmpty() && !disk.isEmpty());

    QString escDir  = QDir::fromNativeSeparators(dir).replace('/','@');
    QString escDisk = QDir::fromNativeSeparators(disk).replace('/','@');

    if (isGlobal)
        _diskPic = pic;

    if (isDirPic)
        _dirMap[escDir].pic = pic;

    if (isDiskPic)
        _dirMap[escDir].map[escDisk].pic = pic;

    _dirty = true;
}

QString DbSettings::getPicture(const QDir& dir, const QString& disk, PicSourceType& picSource)
{
    QString dirStr  = dir.absolutePath();
    QString escDir  = QDir::fromNativeSeparators(dirStr).replace('/','@');
    QString escDisk = QDir::fromNativeSeparators(disk).replace('/','@');
    auto dirInfo = _dirMap[escDir];

    QString pic;
    picSource = PicSource_none;

    if (!dirInfo.map[escDisk].pic.isEmpty())
    {
        picSource = PicFromIni_disk;
        pic = dirInfo.map[escDisk].pic;
    }
    else if (!dirInfo.pic.isEmpty())
    {
        picSource = PicFromIni_dir;
        pic = dirInfo.pic;
    }
    else if (!_diskPic.isEmpty())
    {
        picSource = PicFromIni_global;
        pic = _diskPic;
    }

    return pic;
}

void DbSettings::load()
{
    _diskPic = _settings->value("db/pic").toString();

    foreach (QString group, _settings->childGroups())
    {
        if (group == "db")
            continue;

        DirInfo dirInfo;

        _settings->beginGroup(group);

        if (_settings->contains("pic"))
            dirInfo.pic = _settings->value("pic").toString();

        foreach (const QString& childGroup, _settings->childGroups())
        {
            _settings->beginGroup(childGroup);

            if (_settings->contains("pic"))
                dirInfo.map[childGroup].pic = _settings->value("pic").toString();

            _settings->endGroup();
        }
        _dirMap[group] = dirInfo;

        _settings->endGroup();
    }

    _dirty = false;
}

void DbSettings::save()
{
    _settings->clear();

    if (!_diskPic.isEmpty())
        _settings->setValue("db/pic", _diskPic);

    for (auto it = _dirMap.begin(); it != _dirMap.end(); ++it)
    {
        DirInfo& dirInfo = it.value();
        QString escDir = it.key();

        _settings->beginGroup(escDir);

        if (!dirInfo.pic.isEmpty())
            _settings->setValue("pic",dirInfo.pic);

        for (auto it = dirInfo.map.begin(); it != dirInfo.map.end(); ++it)
        {
            const FloppyArt& art = it.value();

            if (!art.pic.isEmpty())
            {
                QString group  = it.key();
                _settings->beginGroup(group);
                _settings->setValue("pic", art.pic);
                _settings->endGroup();
            }
        }
        _settings->endGroup();
    }

    _dirty = false;
}

void DbSettings::clear()
{
    _settings->clear();
    _diskPic.clear();
    _bSidePic.clear();
    _labelPos = { QRect(), QRect() };
    _bSidePos = { QRect(), QRect() };
    _dirty = false;
    _dirMap.clear();
}
