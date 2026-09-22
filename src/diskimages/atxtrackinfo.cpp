
#include "diskimages/atxsectorinfo.h"
#include "diskimages/atxtrackinfo.h"
#include "diskimages/crc16.h"

namespace DiskImages {

    AtxSectorInfo *AtxTrackInfo::add(const quint8 sectorNumber, const quint8 sectorStatus, const quint16 sectorPosition) {
      const auto sector {new AtxSectorInfo(sectorNumber, sectorStatus, sectorPosition)};
      m_sectors.append(sector);
      return sector;
    }

    int AtxTrackInfo::count(const quint8 sectorNumber) {
      int nb = 0;
      if (!m_sectors.isEmpty()) {
        for (const auto sector : m_sectors) {
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
        for (const auto sector : m_sectors) {
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
        for (const auto sector : m_sectors) {
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

    AtxSectorInfo *AtxTrackInfo::find(const quint8 sectorNumber, const quint16 distance) {
      if (!m_sectors.isEmpty()) {
        for (const auto sector : m_sectors) {
          if (sector->sectorNumber() == sectorNumber && sector->sectorPosition() >= distance /*&& ((sector->sectorStatus() & 0x30) == 0)*/) {
            return sector;
          }
        }
        for (const auto sector : m_sectors) {
          if (sector->sectorNumber() == sectorNumber) {
            return sector;
          }
        }
      }
      return nullptr;
    }

    int AtxTrackInfo::duplicateIndex(const AtxSectorInfo *sectorInfo, const int sectorNumber) {
      if (!m_sectors.isEmpty()) {
        int index = 0;
        for (const auto sector : m_sectors) {
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

    [[maybe_unused]] int AtxTrackInfo::shortSectorSize(const int track, int sectorIndex, int *bitShift) const {
      if (sectorIndex < 0 || sectorIndex >= size()) {
        return 0;
      }
      AtxSectorInfo *sector = at(sectorIndex++);
      // Try to find the offset of the ID address mark in the sector data.
      // The main problem is that the next sector header may or may not be byte aligned with the current one.
      // It means we have to find a header considering all shift positions
      const AtxSectorInfo *nextSector = at(sectorIndex < size() ? sectorIndex : 0);
      // if the next sector is too far, no need to check for short sector.
      const int sectorPos = sector->sectorPosition();
      const int nextSectorPos = nextSector->sectorPosition();
      if (const int gap = nextSectorPos - sectorPos; gap > 145 << 3) {
        return 0;
      }
      const unsigned char invertedTrack = 0xFF - static_cast<unsigned char>(track) & 0xFF;
      for (int index = 0; index < sector->size() - 9; index++) {
        for (int shift = 0; shift < 8; shift++) {
          // shift the sector data to get the real header values
          unsigned char sectorHeader[8];
          for (unsigned int headerByte = 0; headerByte < sizeof(sectorHeader); headerByte++)
          {
            sectorHeader[headerByte] = 0xFF & (
              sector->byteAt((static_cast<int>(headerByte) + index) << shift)
              | (sector->byteAt(static_cast<int>(headerByte) + 1 + index) >> (8 - shift))
            );
          }
          // the header must start with $00 $FE <track> but data is inverted so we check for $FF $01 <invertedTrack>
          if (sectorHeader[0] == 0xFF && sectorHeader[1] == 0x01 && sectorHeader[2] == invertedTrack) {
            if (const quint8 sectorNumber = 0xFF - sectorHeader[4] & 0xFF; sectorNumber == nextSector->sectorNumber()) {
              Crc16 crc16;
              crc16.Reset();
              for (int m = 0; m < 5; m++) {
                crc16.Add(static_cast<unsigned char>(0xFF - sectorHeader[m + 1] & 0xFF));
              }
              if (const unsigned short readCrc = static_cast<unsigned short>(0xFFFF - (sectorHeader[6] << 8) | (sectorHeader[7] & 0xFF)); readCrc == crc16.GetCrc()) {
                *bitShift = shift;
                return index + 1;
              }
            }
          }
        }
      }
      return 0;
    }

}