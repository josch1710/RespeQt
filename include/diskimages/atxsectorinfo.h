#ifndef RESPEQT_ATXSECTORINFO_H
#define RESPEQT_ATXSECTORINFO_H

#include <QtGlobal>
#include <QObject>

namespace DiskImages {

  __attribute__((unused)) static constexpr quint8 DISK_CRC_MARK{0xF7};
  static const quint8 DISK_DATA_ADDR_MARK1{0xF8};
  static const quint8 DISK_DATA_ADDR_MARK2{0xF9};
  static const quint8 DISK_DATA_ADDR_MARK3{0xFA};
  static const quint8 DISK_DATA_ADDR_MARK4{0xFB};
  __attribute__((unused)) static const quint8 DISK_INDEX_ADDR_MARK{0xFC};
  __attribute__((unused)) static const quint8 DISK_SPARE_MARK{0xFD};
  static const quint8 DISK_ID_ADDR_MARK{0xFE};

  class AtxSectorInfo : public QObject {
    Q_OBJECT

  public:
    AtxSectorInfo(quint8 sectorNumber, quint8 sectorStatus, quint16 sectorPosition) {
      m_sectorNumber = sectorNumber;
      m_sectorStatus = sectorStatus;
      m_sectorPosition = sectorPosition;
      m_sectorWeakOffset = (quint16) 0xFFFF;
    }
    virtual ~AtxSectorInfo() {}
    quint8 sectorNumber() { return m_sectorNumber; }
    quint8 sectorStatus() { return m_sectorStatus; }
    quint16 sectorPosition() { return m_sectorPosition; }
    QByteArray sectorData();
    __attribute__((unused)) QByteArray rawSectorData() { return m_sectorData; }
    quint8 byteAt(int pos);
    __attribute__((unused)) __attribute__((unused)) quint8 rawByteAt(int pos);
    quint8 fillByte();
    int size() { return (((wd1771Status() & 0x06) == 0) && ((wd1771Status() & 0x08) != 0)) ? m_sectorData.size() * 2 : m_sectorData.size(); }
    void setSectorData(QByteArray &sectorData) { m_sectorData = sectorData; }
    void copySectorData(const QByteArray &sectorData);
    quint16 sectorWeakOffset() { return m_sectorWeakOffset; }
    void setSectorWeakOffset(quint16 sectorWeakOffet);
    quint8 driveStatus() { return 0x10; }
    quint8 wd1771Status() { return (~m_sectorStatus & 0x3E) | 0xC1; }
    void setWd1771Status(quint8 status) { m_sectorStatus |= ~status & 0x3E; }
    __attribute__((unused)) int dataMarkOffset(int headerOffset, int shift);

  private:
    quint8 m_sectorNumber;
    quint8 m_sectorStatus;
    quint16 m_sectorPosition;
    QByteArray m_sectorData;
    quint16 m_sectorWeakOffset;
  };
}

#endif//RESPEQT_ATXSECTORINFO_H
