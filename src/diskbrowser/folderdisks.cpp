/*
 * folderdisks.cpp - implementation of DiskBrowserDialog's FolderDisks class.
 *
 * Copyright 2023 D.Caputi
 */
#include "diskbrowser/folderdisks.h"
#include "miscutils.h"
#include <QDir>

FolderDisks::FolderDisks() : QObject()
{
}

bool FolderDisks::load(const QString& path)
{
    QString targetPath = path.isEmpty() ? QDir::currentPath() : path;

    if (dir.exists(targetPath))
    {
        dir.setPath(targetPath);

        QStringList filter = FileTypes::getDiskImageTypes();
        diskList = dir.entryList(filter, QDir::Files);
        dirList = dir.entryList(QDir::AllDirs | QDir::NoDot);

        if (_settings)
            delete _settings;

        _settings = new FolderSettings(targetPath);
    }

    return !diskList.isEmpty() || !dirList.isEmpty();
}

// FolderSettings class -
// Derived from QSettings, this class encapsulates DiskBrowser's pics, titles and indexing required for the PicPreview widget.

FolderSettings::FolderSettings(const QString& path) : QSettings(path + ".respeqt_db.ini", QSettings::Format::IniFormat),
    _path(path)
{
    load();
}

FolderSettings::~FolderSettings()
{
    save();
}

void FolderSettings::load()
{

}

void FolderSettings::save()
{

}
