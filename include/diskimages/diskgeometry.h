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
    bool isEqual(const DiskGeometry &other);
    bool isStandardSD() const;
    bool isStandardED() const;
    bool isStandardDD() const;
    bool isStandardDSDD() const;
    bool isStandardDSQD() const;
    inline bool isDoubleSided() const { return mIsDoubleSided; }
    inline quint8 tracksPerSide() const { return mTracksPerSide; }
    inline quint16 sectorsPerTrack() const { return mSectorsPerTrack; }
    inline quint16 bytesPerSector() const { return mBytesPerSector; }
    quint16 bytesPerSector(quint16 sector);
    inline uint sectorCount() const { return mSectorCount; }
    inline uint totalSize() const { return mTotalSize; }
    QByteArray toPercomBlock();
    QString humanReadable() const;

  private:
    bool mIsDoubleSided;
    quint8 mTracksPerSide;
    quint16 mSectorsPerTrack;
    quint16 mBytesPerSector;
    uint mSectorCount;
    uint mTotalSize;
  };

}// namespace DiskImages
#endif//RESPEQT_DISKGEOMETRY_H
