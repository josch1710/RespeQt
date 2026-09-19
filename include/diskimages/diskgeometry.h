#ifndef RESPEQT_DISKGEOMETRY_H
#define RESPEQT_DISKGEOMETRY_H

#include <QObject>

namespace DiskImages {
  class DiskGeometry : public QObject {
    Q_OBJECT

  public:
    DiskGeometry(const DiskGeometry &other);
    DiskGeometry();
    void initialize(const DiskGeometry &other);
    void initialize(bool aIsDoubleSided, quint8 aTracksPerSide, quint16 aSectorsPerTrack, quint16 aBytesPerSector);
    void initialize(uint aTotalSize, quint16 aBytesPerSector);
    void initialize(uint aTotalSize);
    void initialize(const QByteArray &percom);
    [[nodiscard]] bool isEqual(const DiskGeometry &other) const;
    [[nodiscard]] bool isStandardSD() const;
    [[nodiscard]] bool isStandardED() const;
    [[nodiscard]] bool isStandardDD() const;
    [[nodiscard]] bool isStandardDSDD() const;
    [[nodiscard]] bool isStandardDSQD() const;
    [[nodiscard]] bool isDoubleSided() const { return mIsDoubleSided; }
    [[nodiscard]] quint8 tracksPerSide() const { return mTracksPerSide; }
    [[nodiscard]] quint16 sectorsPerTrack() const { return mSectorsPerTrack; }
    [[nodiscard]] quint16 bytesPerSector() const { return mBytesPerSector; }
    [[nodiscard]] quint16 bytesPerSector(quint16 sector) const;
    [[nodiscard]] uint sectorCount() const { return mSectorCount; }
    [[nodiscard]] uint totalSize() const { return mTotalSize; }
    [[nodiscard]] QByteArray toPercomBlock() const;
    [[nodiscard]] QString humanReadable() const;

  private:
    bool mIsDoubleSided{};
    quint8 mTracksPerSide{};
    quint16 mSectorsPerTrack{};
    quint16 mBytesPerSector{};
    uint mSectorCount{};
    uint mTotalSize{};
  };

}// namespace DiskImages
#endif//RESPEQT_DISKGEOMETRY_H
