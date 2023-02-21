/*
 * folderimage.cpp
 *
 * Copyright 2017 josch1710
 * Copyright 2017 blind
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef FOLDERIMAGE_H
#define FOLDERIMAGE_H

#include <QDir>
#include <QVector>
#include "diskimage.h"

class AtariFile
{
public:
    bool exists;
    QFileInfo original;
    QString atariName;
    QString atariExt;
    QString longName;
    int lastSector;
    quint64 pos;
    int sectPass;
};

class FolderImage : public SimpleDiskImage
{
Q_OBJECT

protected:
    QDir dir;
    bool mReadOnly;
    void buildDirectory();
    QVector<AtariFile> atariFiles;
    int atariFileNo;
    int maxEntries{64};

public:
    FolderImage(SioWorkerPtr worker, int maxEntries = 64);
    ~FolderImage();

    void close();
    bool open(const QString &fileName, FileTypes::FileType /* type */);
    bool readSector(quint16 sector, QByteArray &data);
    bool writeSector(quint16 sector, const QByteArray &data);
    bool format(const DiskGeometry& geo);
    QString longName (QString &lastMountedFolder, QString &atariFileName);   //

    virtual QString description() const {return tr("Folder image");}
};

extern FolderImage *folderImage;

#endif // FOLDERIMAGE_H
