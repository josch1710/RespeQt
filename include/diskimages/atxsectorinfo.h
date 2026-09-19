#ifndef RESPEQT_ATXSECTORINFO_H
#define RESPEQT_ATXSECTORINFO_H

#include <QObject>

namespace DiskImages {

  [[maybe_unused]] static constexpr quint8 DISK_CRC_MARK{0xF7};
  static constexpr quint8 DISK_DATA_ADDR_MARK1{0xF8};
  static constexpr quint8 DISK_DATA_ADDR_MARK2{0xF9};
  static constexpr quint8 DISK_DATA_ADDR_MARK3{0xFA};
  static constexpr quint8 DISK_DATA_ADDR_MARK4{0xFB};
  [[maybe_unused]] static constexpr quint8 DISK_INDEX_ADDR_MARK{0xFC};
  [[maybe_unused]] static constexpr quint8 DISK_SPARE_MARK{0xFD};
  static constexpr quint8 DISK_ID_ADDR_MARK{0xFE};

  class AtxSectorInfo : public QObject {
    Q_OBJECT

  public:
    AtxSectorInfo(const quint8 sectorNumber, const quint8 sectorStatus, const quint16 sectorPosition) {
      m_sectorNumber = sectorNumber;
      m_sectorStatus = sectorStatus;
      m_sectorPosition = sectorPosition;
      m_sectorWeakOffset = static_cast<quint16>(0xFFFF);
    }
    ~AtxSectorInfo() override = default;
    [[nodiscard]] quint8 sectorNumber() const { return m_sectorNumber; }
    [[nodiscard]] quint8 sectorStatus() const { return m_sectorStatus; }
    [[nodiscard]] quint16 sectorPosition() const { return m_sectorPosition; }
    QByteArray sectorData();
    [[maybe_unused]] QByteArray rawSectorData() { return m_sectorData; }
    quint8 byteAt(int pos);
    [[maybe_unused]] quint8 rawByteAt(int pos);
    quint8 fillByte();
    [[nodiscard]] int size() const { return (wd1771Status() & 0x06) == 0 && (wd1771Status() & 0x08) != 0 ? m_sectorData.size() * 2 : m_sectorData.size(); }
    void setSectorData(const QByteArray &sectorData) { m_sectorData = sectorData; }
    void copySectorData(const QByteArray &sectorData);
    [[nodiscard]] quint16 sectorWeakOffset() const { return m_sectorWeakOffset; }
    void setSectorWeakOffset(quint16 sectorWeakOffet);
    static quint8 driveStatus() { return 0x10; }
    [[nodiscard]] quint8 wd1771Status() const { return (~m_sectorStatus & 0x3E) | 0xC1; }
    void setWd1771Status(const quint8 status) { m_sectorStatus |= ~status & 0x3E; }
    [[maybe_unused]] int dataMarkOffset(int headerOffset, int shift);

  private:
    quint8 m_sectorNumber;
    quint8 m_sectorStatus;
    quint16 m_sectorPosition;
    QByteArray m_sectorData;
    quint16 m_sectorWeakOffset;
  };
}

#endif//RESPEQT_ATXSECTORINFO_H
