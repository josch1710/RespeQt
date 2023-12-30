
#include "diskimages/atxsectorinfo.h"
#include <QRandomGenerator>

namespace DiskImages {
  QByteArray AtxSectorInfo::sectorData() {
    if (m_sectorWeakOffset != 0xFFFF) {
      for (int i = m_sectorWeakOffset; i < m_sectorData.size(); i++) {
        m_sectorData[i] = QRandomGenerator::global()->generate() % 0xFF;
      }
    }
    return m_sectorData;
  }

  quint8 AtxSectorInfo::byteAt(int pos) {
    if (pos >= m_sectorData.size()) {
      return 0;
    }
    if ((m_sectorWeakOffset != 0xFFFF) && (pos >= m_sectorWeakOffset)) {
      return QRandomGenerator::global()->generate() & 0xFF;
    }
    return m_sectorData[pos];
  }

  __attribute__((unused)) quint8 AtxSectorInfo::rawByteAt(int pos) {
    // same as byteAt but does not interpret weak bits
    if (pos >= m_sectorData.size()) {
      return 0;
    }
    return m_sectorData[pos];
  }

  quint8 AtxSectorInfo::fillByte() {
    if (m_sectorWeakOffset != 0xFFFF) {
      return 1;
    }
    quint8 value = m_sectorData[0];
    for (int i = 0; i < m_sectorData.size(); i++) {
      if ((quint8) m_sectorData[i] != value) {
        return 1;
      }
    }
    return value;
  }

  void AtxSectorInfo::copySectorData(const QByteArray &sectorData) {
    m_sectorData.clear();
    for (int i = 0; i < sectorData.size(); i++) {
      m_sectorData[i] = sectorData[i];
    }
  }

  void AtxSectorInfo::setSectorWeakOffset(quint16 sectorWeakOffet) {
    m_sectorWeakOffset = sectorWeakOffet;
    if (sectorWeakOffet != 0xFFFF) {
      m_sectorStatus |= 0x40;
    } else {
      m_sectorStatus &= ~0x40;
    }
  }

  __attribute__((unused)) int AtxSectorInfo::dataMarkOffset(int headerOffset, int shift) {
    // skip the header size
    int index = headerOffset + 6;
    // after the sector header, we should find at least 6 $00 bytes and then a DATA address mark.
    // We only check for 5 $00 because the remaining bytes may not be byte aligned.
    int nbConsecutive00 = 0;
    while (index < size() - 1) {
      if (byteAt(index) == (quint8) (0xFF - 0x00)) {
        nbConsecutive00++;
      } else if (nbConsecutive00 >= 5) {
        int data = (((unsigned char) byteAt(index) << shift) | ((unsigned char) byteAt(index + 1) >> (8 - shift))) & 0xFF;
        if ((data == (quint8) (0xFF - DISK_DATA_ADDR_MARK1)) ||
            (data == (quint8) (0xFF - DISK_DATA_ADDR_MARK2)) ||
            (data == (quint8) (0xFF - DISK_DATA_ADDR_MARK3)) ||
            (data == (quint8) (0xFF - DISK_DATA_ADDR_MARK4))) {
          return index;
        }
      } else {
        nbConsecutive00 = 0;
      }
      index++;
    }
    return -1;
  }


}
