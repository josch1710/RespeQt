
#ifndef RESPEQT_DISKIMAGEPRO_H
#define RESPEQT_DISKIMAGEPRO_H

#include "diskimages/diskimage.h"
#include "diskimages/prosectorinfo.h"

namespace DiskImages {

  class DiskImagePro : public DiskImage {
  public:
    explicit DiskImagePro(SioWorkerPtr worker, bool gzipped);
    bool openImage(const QString &fileName) override;
    bool create(int untitledName) override;
    bool save() override;
    bool saveImageAs() override;
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
    bool writeHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) override;
    bool resetTrack(quint16 aux) override;
    quint8 writeSectorHeader(quint8 dataSize, quint16 sectorSlot, quint8 postDataCrc, quint8 preIDField, quint8 postIDCrc, quint8 track, quint8 index, quint8 nextSector) override;
    bool writeSectorExtended(int, quint8 dataType, quint8 trackNumber, quint8, quint8 sectorNumber, quint8, const QByteArray &data, bool crcError, int weakOffset) override;
    QByteArray readHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) override;
    bool findMappingInTrack(int nbSectors, QByteArray &mapping) override;

  protected:
    bool fillSectorInfo(const QString &fileName, QFile *sourceFile, quint16 slot, quint16 absoluteSector);
    quint16 findPositionInTrack(int track, int indexInProSector, bool withoutData);
    void guessWeakSector(int slot);

    // Pro specific data
    ProSectorInfo m_proSectorInfo[1040 + 256];// to support an enhanced density PRO file + 256 phantom sectors

  };

}

#endif//RESPEQT_DISKIMAGEPRO_H
