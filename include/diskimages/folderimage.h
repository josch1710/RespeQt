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
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
    int lastSector;
#pragma clang diagnostic pop
    quint64 pos;
    int sectPass;
  };

  class FolderImage : public DiskImage {
    Q_OBJECT

  protected:
    QDir dir;
    void buildDirectory();
    QVector<AtariFile> atariFiles;
    int atariFileNo;
    int mMaxEntries{64};

  signals:
    void maxEntriesChanged(int oldMaxEntries, int newMaxEntries);

  public:
    FolderImage(SioWorkerPtr worker, bool gzipped);
    ~FolderImage();

    void setMaxEntries(int limitEntries);
    int maxEntries() const { return mMaxEntries; }

    bool resetTrack(quint16 aux) override;
    bool writeHappyTrack(int trackNumber, bool happy1050) override;
    QByteArray readHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) override;
    bool findMappingInTrack(int nbSectors, QByteArray &mapping) override;
    bool writeHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) override;
    bool writeSectorExtended(int bitNumber, quint8 dataType, quint8 trackNumber, quint8 sideNumber, quint8 sectorNumber, quint8 sectorSize, const QByteArray &data, bool crcError, int weakOffset) override;
    quint8 writeSectorHeader(quint8 dataSize, quint16 sectorSlot, quint8 postDataCrc, quint8 preIDField, quint8 postIDCrc, quint8 track, quint8 index, quint8 nextSector) override;
    void close() override;
    bool open(const QString &fileName) override;
    bool readSector(quint16 sector, QByteArray &data) override;
    bool writeSector(quint16 sector, const QByteArray &data) override;
    bool format(const DiskImages::DiskGeometry &geo) override;
    bool create(int untitledName) override { return false; }
    bool save() override { return false; }
    bool saveImageAs() override { return false; }
    bool readHappySectorAtPosition(int trackNumber, int sectorNumber, int afterSectorNumber, int &index, QByteArray &data) override { return false; }
    bool readHappySkewAlignment(bool happy1050) override { return false; }
    void readTrack(quint16 aux, QByteArray &data, int length) override {}
    bool readSectorStatuses(QByteArray &data) override { return false; }
    bool readSectorUsingIndex(quint16 aux, QByteArray &data) override { return false; }
    bool readSkewAlignment(quint16 aux, QByteArray &data, bool timingOnly) override { return false; }
    bool writeTrack(quint16 aux, const QByteArray &data) override { return false; }
    bool writeTrackWithSkew(quint16 aux, const QByteArray &data) override { return false; }
    bool writeSectorUsingIndex(quint16 aux, const QByteArray &data, bool fuzzy) override { return false; }
    bool writeFuzzySector(quint16 aux, const QByteArray &data) override { return false; }
    int sectorsInCurrentTrack() override { return 0; }
    bool openImage(const QString &fileName) override;
    __attribute__((unused)) QString longName(QString &lastMountedFolder, QString &atariFileName);

    virtual QString description() const override { return tr("Folder image"); }
  };
}
#endif// FOLDERIMAGE_H
