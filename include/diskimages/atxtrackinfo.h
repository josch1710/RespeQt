#ifndef RESPEQT_ATXTRACKINFO_H
#define RESPEQT_ATXTRACKINFO_H

#include <QtGlobal>
#include <QObject>
#include "diskimages/atxsectorinfo.h"

namespace DiskImages {
  class AtxTrackInfo : public QObject {
    Q_OBJECT

  public:
    AtxTrackInfo() = default;
    ~AtxTrackInfo() override { clear(); }
    AtxSectorInfo *add(quint8 sectorNumber, quint8 sectorStatus, quint16 sectorPosition);
    int count(quint8 sectorNumber);
    int numberOfExtendedSectors();
    int numberOfSectorsWithData();
    void clear();
    [[nodiscard]] int size() const { return m_sectors.size(); }
    [[nodiscard]] AtxSectorInfo *at(const int pos) const { return m_sectors.at(pos); }
    AtxSectorInfo *find(quint8 sectorNumber, quint16 distance);
    int duplicateIndex(const AtxSectorInfo *sectorInfo, int sectorNumber);
    [[maybe_unused]] int shortSectorSize(int track, int sectorIndex, int *bitShift) const;

  private:
    QList<AtxSectorInfo *> m_sectors;


  };
}
#endif//RESPEQT_ATXTRACKINFO_H
