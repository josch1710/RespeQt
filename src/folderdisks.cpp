/*
 * folderdisks.cpp - implementation of DiskBrowserDialog's FolderDisks class.
 *
 * Copyright 2023 D.Caputi
 */
#include "folderdisks.h"
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
    }

    return !diskList.isEmpty() || !dirList.isEmpty();
}
