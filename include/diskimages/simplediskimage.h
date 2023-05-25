#ifndef RESPEQT_SIMPLEDISKIMAGE_H
#define RESPEQT_SIMPLEDISKIMAGE_H

#include "diskimages/diskimage.h"

namespace DiskImages {

  class SimpleDiskImage : public DiskImage {
  public:
    explicit SimpleDiskImage(SioWorkerPtr worker, bool gzipped);
    bool openImage(const QString &fileName) override;
    bool create(int untitledName) override;
    bool save() override;
    bool format(const DiskGeometry &geo) override;
    bool readHappySectorAtPosition(int trackNumber, int sectorNumber, int afterSectorNumber, int &index, QByteArray &data) override;
    bool readHappySkewAlignment(bool happy1050) override;
    void readTrack(quint16 aux, QByteArray &data, int length) override;
    bool readSectorStatuses(QByteArray &data) override;
    bool readSectorUsingIndex(quint16 aux, QByteArray &data) override;
    bool readSector(quint16 aux, QByteArray &data) override;
    bool readSkewAlignment(quint16 aux, QByteArray &data, bool timingOnly) override;
    bool writeTrack(quint16 aux, const QByteArray &data) override;
    bool writeTrackWithSkew(quint16 aux, const QByteArray &data) override;
    bool writeSectorUsingIndex(quint16 aux, const QByteArray &data, bool fuzzy) override;
    bool writeFuzzySector(quint16 aux, const QByteArray &data) override;
    bool writeSector(quint16 aux, const QByteArray &data) override;
    int sectorsInCurrentTrack() override;
    bool writeHappyTrack(int trackNumber, bool happy1050) override;
    QByteArray readHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) override;
    bool writeHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) override;
    bool resetTrack(quint16 aux) override;
    bool writeSectorExtended(int bitNumber, quint8 dataType, quint8 trackNumber, quint8 sideNumber, quint8 sectorNumber, quint8 sectorSize, const QByteArray &data, bool crcError, int weakOffset) override;
    quint8 writeSectorHeader(quint8 dataSize, quint16 sectorSlot, quint8 postDataCrc, quint8 preIDField, quint8 postIDCrc, quint8 track, quint8 index, quint8 nextSector) override;
  };
}

#endif//RESPEQT_SIMPLEDISKIMAGE_H
