
#include "diskimages/atxsectorinfo.h"
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
#include <QRandomGenerator>
#endif

namespace DiskImages {
  QByteArray AtxSectorInfo::sectorData() {
    if (m_sectorWeakOffset != 0xFFFF) {
      for (int i = m_sectorWeakOffset; i < m_sectorData.size(); i++) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        m_sectorData[i] = static_cast<char>(QRandomGenerator::global()->generate() % 0xFF);
#else
        m_sectorData[i] = qrand() % 0xFF;
#endif
      }
    }
    return m_sectorData;
  }

  quint8 AtxSectorInfo::byteAt(const int pos) {
    if (pos >= m_sectorData.size()) {
      return 0;
    }
    if (m_sectorWeakOffset != 0xFFFF && pos >= m_sectorWeakOffset) {
      return QRandomGenerator::global()->generate() & 0xFF;
    }
    // ReSharper disable once CppRedundantCastExpression
    return static_cast<quint8>(m_sectorData[pos]);
  }

  quint8 AtxSectorInfo::rawByteAt(const int pos) {
    // same as byteAt but does not interpret weak bits
    if (pos >= m_sectorData.size()) {
      return 0;
    }
    // ReSharper disable once CppRedundantCastExpression
    return static_cast<quint8>(m_sectorData[pos]);
  }

  quint8 AtxSectorInfo::fillByte() {
    if (m_sectorWeakOffset != 0xFFFF) {
      return 1;
    }
    const auto value = static_cast<quint8>(m_sectorData[0]);
    for (int i = 0; i < m_sectorData.size(); i++) {
      if (static_cast<quint8>(m_sectorData[i]) != value) {
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

  void AtxSectorInfo::setSectorWeakOffset(const quint16 sectorWeakOffet) {
    m_sectorWeakOffset = sectorWeakOffet;
    if (sectorWeakOffet != 0xFFFF) {
      m_sectorStatus |= 0x40;
    } else {
      m_sectorStatus &= ~0x40;
    }
  }

  int AtxSectorInfo::dataMarkOffset(const int headerOffset, const int shift) {
    // skip the header size
    int index = headerOffset + 6;
    // after the sector header, we should find at least 6 $00 bytes and then a DATA address mark.
    // We only check for 5 $00 because the remaining bytes may not be byte aligned.
    int nbConsecutive00 = 0;
    while (index < size() - 1) {
      if (byteAt(index) == static_cast<quint8>(0xFF - 0x00)) {
        nbConsecutive00++;
      } else if (nbConsecutive00 >= 5) {
        if (const int data = ((byteAt(index) << shift) | (byteAt(index + 1) >> (8 - shift))) & 0xFF;
            data == static_cast<quint8>(0xFF - DISK_DATA_ADDR_MARK1) ||
            data == static_cast<quint8>(0xFF - DISK_DATA_ADDR_MARK2) ||
            data == static_cast<quint8>(0xFF - DISK_DATA_ADDR_MARK3) ||
            data == static_cast<quint8>(0xFF - DISK_DATA_ADDR_MARK4)
        ) {
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
