/*
 * folderdisks.cpp - implementation of DiskBrowserDialog's FolderDisks class.
 *
 * Copyright 2023 D.Caputi
 */
#include "diskbrowser/folderdisks.h"
#include "miscutils.h"
#include <QDir>

FolderDisks::FolderDisks() = default;

bool FolderDisks::load(const QString& path)
{
    if (const QString targetPath = path.isEmpty() ? QDir::currentPath() : path; dir.exists(targetPath))
    {
        dir.setPath(targetPath);

        const QStringList& filter = FileTypes::getDiskImageTypes();
        diskList = dir.entryList(filter, QDir::Files);
        dirList = dir.entryList(QDir::AllDirs | QDir::NoDot);
    }

    return !diskList.isEmpty() || !dirList.isEmpty();
}
