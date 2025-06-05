#ifndef RESPEQT_ATXTRACKINFO_H
#define RESPEQT_ATXTRACKINFO_H

#include <QtGlobal>
#include <QObject>
#include "diskimages/atxsectorinfo.h"

#include "diskimages/atxsectorinfo.h"

namespace DiskImages {
  class AtxTrackInfo : public QObject {
    Q_OBJECT

  private:
    QList<AtxSectorInfo *> m_sectors;

  public:
    AtxTrackInfo() {}
    virtual ~AtxTrackInfo() { clear(); }
    AtxSectorInfo *add(quint8 sectorNumber, quint8 sectorStatus, quint16 sectorPosition);
    int count(quint8 sectorNumber);
    int numberOfExtendedSectors();
    int numberOfSectorsWithData();
    void clear();
    int size() { return m_sectors.size(); }
    AtxSectorInfo *at(int pos) { return m_sectors.at(pos); }
    AtxSectorInfo *find(quint8 sectorNumber, quint16 distance);
    int duplicateIndex(AtxSectorInfo *sectorInfo, int sectorNumber);
    __attribute__((unused)) int shortSectorSize(int track, int sectorIndex, int *bitShift);
  };
}
#endif//RESPEQT_ATXTRACKINFO_H
