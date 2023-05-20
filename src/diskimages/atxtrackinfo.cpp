
#include "diskimages/atxsectorinfo.h"
#include "diskimages/atxtrackinfo.h"
#include "crc16.h"

namespace DiskImages {

    AtxSectorInfo *AtxTrackInfo::add(quint8 sectorNumber, quint8 sectorStatus, quint16 sectorPosition) {
      auto sector = new AtxSectorInfo(sectorNumber, sectorStatus, sectorPosition);
      m_sectors.append(sector);
      return sector;
    }

    int AtxTrackInfo::count(quint8 sectorNumber) {
      int nb = 0;
      if (!m_sectors.isEmpty()) {
        for (QList<AtxSectorInfo *>::iterator it = m_sectors.begin(); it != m_sectors.end(); ++it) {
          AtxSectorInfo *sector = *it;
          if (sector->sectorNumber() == sectorNumber) {
            nb++;
          }
        }
      }
      return nb;
    }

    int AtxTrackInfo::numberOfExtendedSectors() {
      int nb = 0;
      if (!m_sectors.isEmpty()) {
        for (QList<AtxSectorInfo *>::iterator it = m_sectors.begin(); it != m_sectors.end(); ++it) {
          AtxSectorInfo *sector = *it;
          if (sector->sectorWeakOffset() != 0xFFFF) {
            nb++;
          }
        }
      }
      return nb;
    }

    int AtxTrackInfo::numberOfSectorsWithData() {
      int nb = 0;
      if (!m_sectors.isEmpty()) {
        for (QList<AtxSectorInfo *>::iterator it = m_sectors.begin(); it != m_sectors.end(); ++it) {
          AtxSectorInfo *sector = *it;
          if ((sector->sectorStatus() & 0x10) == 0) {
            nb++;
          }
        }
      }
      return nb;
    }

    void AtxTrackInfo::clear() {
      if (!m_sectors.isEmpty()) {
        qDeleteAll(m_sectors.begin(), m_sectors.end());
      }
      m_sectors.clear();
    }

    AtxSectorInfo *AtxTrackInfo::find(quint8 sectorNumber, quint16 distance) {
      if (!m_sectors.isEmpty()) {
        for (QList<AtxSectorInfo *>::iterator it = m_sectors.begin(); it != m_sectors.end(); ++it) {
          AtxSectorInfo *sector = *it;
          if ((sector->sectorNumber() == sectorNumber) && (sector->sectorPosition() >= distance) /*&& ((sector->sectorStatus() & 0x30) == 0)*/) {
            return sector;
          }
        }
        for (QList<AtxSectorInfo *>::iterator it = m_sectors.begin(); it != m_sectors.end(); ++it) {
          AtxSectorInfo *sector = *it;
          if (sector->sectorNumber() == sectorNumber) {
            return sector;
          }
        }
      }
      return nullptr;
    }

    int AtxTrackInfo::duplicateIndex(AtxSectorInfo *sectorInfo, int sectorNumber) {
      if (!m_sectors.isEmpty()) {
        int index = 0;
        for (QList<AtxSectorInfo *>::iterator it = m_sectors.begin(); it != m_sectors.end(); ++it) {
          AtxSectorInfo *sector = *it;
          if (sector->sectorNumber() == sectorNumber) {
            index++;
          }
          if (sector == sectorInfo) {
            return index;
          }
        }
      }
      return 1;
    }

    __attribute__((unused)) int AtxTrackInfo::shortSectorSize(int track, int sectorIndex, int *bitShift) {
      if ((sectorIndex < 0) || (sectorIndex >= size())) {
        return 0;
      }
      AtxSectorInfo *sector = at(sectorIndex++);
      // Try to find the offset of the ID address mark in the sector data.
      // The main problem is that the next sector header may or may not be byte aligned with the current one.
      // It means we have to find a header considering all shift positions
      AtxSectorInfo *nextSector = at(sectorIndex < size() ? sectorIndex : 0);
      // if the next sector is too far, no need to check for short sector.
      int sectorPos = (int) (quint32) sector->sectorPosition();
      int nextSectorPos = (int) (quint32) nextSector->sectorPosition();
      int gap = nextSectorPos - sectorPos;
      if (gap > (145 << 3)) {
        return 0;
      }
      unsigned char invertedTrack = (0xFF - (unsigned char) track) & 0xFF;
      for (int index = 0; index < sector->size() - 9; index++) {
        for (int shift = 0; shift < 8; shift++) {
          // shift the sector data to get the real header values
          unsigned char sectorHeader[8];
          for (unsigned int headerByte = 0; headerByte < sizeof(sectorHeader); headerByte++) {
            sectorHeader[headerByte] = (((unsigned char) sector->byteAt(index + headerByte) << shift) | ((unsigned char) sector->byteAt(index + headerByte + 1) >> (8 - shift))) & 0xFF;
          }
          // the header must start with $00 $FE <track> but data is inverted so we check for $FF $01 <invertedTrack>
          if ((sectorHeader[0] == 0xFF) && (sectorHeader[1] == 0x01) && (sectorHeader[2] == invertedTrack)) {
            quint8 sectorNumber = (0xFF - sectorHeader[4]) & 0xFF;
            if (sectorNumber == nextSector->sectorNumber()) {
              Crc16 crc16;
              crc16.Reset();
              for (int m = 0; m < 5; m++) {
                crc16.Add((unsigned char) ((0xFF - sectorHeader[m + 1]) & 0xFF));
              }
              unsigned short readCrc = 0xFFFF - ((((unsigned short) sectorHeader[6]) << 8) | (((unsigned short) sectorHeader[7]) & 0xFF));
              if (readCrc == crc16.GetCrc()) {
                *bitShift = shift;
                return index + 1;
              }
            }
          }
        }
      }
      return 0;
    }

};