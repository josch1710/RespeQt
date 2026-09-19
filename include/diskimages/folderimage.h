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

#include "diskimage.h"
#include <QDir>
#include <QVector>

namespace DiskImages {
  class AtariFile {
  public:
    bool exists;
    QFileInfo original;
    QString atariName;
    QString atariExt;
    QString longName;
    [[maybe_unused]] int lastSector;
    quint64 pos;
    int sectPass;
  };

  class FolderImage : public SimpleDiskImage {
    Q_OBJECT

  protected:
    QDir dir;
    void buildDirectory();
    QVector<AtariFile> atariFiles;
    int atariFileNo{0};
    int maxEntries{64};

  public:
    explicit FolderImage(const SioWorkerPtr& worker, int maxEntries = 64);
    ~FolderImage() override;

    void close() override;
    bool open(const QString &fileName, FileTypes::FileType type) override;
    bool readSector(quint16 sector, QByteArray &data) override;
    bool writeSector(quint16 sector, const QByteArray &data) override;
    bool format(const DiskGeometry &geo) override;
    [[maybe_unused]] QString longName(const QString &lastMountedFolder, const QString &atariFileName);

    QString description() const override { return tr("Folder image"); }
  };
}
#endif// FOLDERIMAGE_H
