
#include "diskimages/diskgeometry.h"

namespace DiskImages {
  DiskGeometry::DiskGeometry()
  {
    mIsDoubleSided = false;
    mTracksPerSide = 0;
    mSectorsPerTrack = 0;
    mBytesPerSector = 0;
    mTotalSize = 0;
    mSectorCount = 0;
  }

  DiskGeometry::DiskGeometry(const DiskGeometry &other)
  {
    initialize(other);
  }

  void DiskGeometry::initialize(const DiskGeometry &other) {
    mIsDoubleSided = other.isDoubleSided();
    mTracksPerSide = other.tracksPerSide();
    mSectorsPerTrack = other.sectorsPerTrack();
    mBytesPerSector = other.bytesPerSector();
    mSectorCount = other.sectorCount();
    mTotalSize = other.totalSize();
  }

  void DiskGeometry::initialize(const bool aIsDoubleSided, const quint8 aTracksPerSide, const quint16 aSectorsPerTrack, const quint16 aBytesPerSector) {
    mIsDoubleSided = aIsDoubleSided;
    mTracksPerSide = aTracksPerSide;
    mSectorsPerTrack = aSectorsPerTrack;
    mBytesPerSector = aBytesPerSector;
    mSectorCount = (mIsDoubleSided + 1) * mTracksPerSide * mSectorsPerTrack;
    if (mBytesPerSector == 256) {
      mTotalSize = mSectorCount * 128;
      if (mTotalSize > 384) {
        mTotalSize += (mBytesPerSector - 128) * (mSectorCount - 3);
      }
    } else {
      mTotalSize = mSectorCount * mBytesPerSector;
    }
  }

  void DiskGeometry::initialize(const uint aTotalSize, const quint16 aBytesPerSector) {
    bool ds;
    quint8 tps;
    quint16 spt;

    if ((aTotalSize == 92160 && aBytesPerSector == 128)
      || ((aTotalSize == 183936 || aTotalSize == 184320) && aBytesPerSector == 256)
      || (aTotalSize == 368256 && aBytesPerSector == 256)) {
      ds = false;
      tps = 40;
      spt = 18;
    } else if (aTotalSize == 133120 && aBytesPerSector == 128) {
      ds = false;
      tps = 40;
      spt = 26;
    } else if (aTotalSize == 736896 && aBytesPerSector == 256) {
      ds = true;
      tps = 80;
      spt = 18;
    } else {
      if (aBytesPerSector == 256) {
        if (aTotalSize <= 384) {
          spt = static_cast<quint16>((aTotalSize + 127) / 128);
        } else {
          spt = static_cast<quint16>((aTotalSize + 384 + 255) / 256);
        }
      } else {
        spt = static_cast<quint16>((aTotalSize + aBytesPerSector - 1) / aBytesPerSector);
      }
      ds = false;
      tps = 1;
    }

    initialize(ds, tps, spt, aBytesPerSector);
  }

  void DiskGeometry::initialize(const uint aTotalSize) {
    bool ds;
    quint8 tps;
    quint16 spt;
    quint16 bps;

    if (aTotalSize == 92160) {
      ds = false;
      tps = 40;
      spt = 18;
      bps = 128;
    } else if (aTotalSize == 133120) {
      ds = false;
      tps = 40;
      spt = 26;
      bps = 128;
    } else if (aTotalSize == 183936 || aTotalSize == 184320) {// ATR or XFD
      ds = false;
      tps = 40;
      spt = 18;
      bps = 256;
    } else if (aTotalSize == 368256) {
      ds = true;
      tps = 40;
      spt = 18;
      bps = 256;
    } else if (aTotalSize == 736896) {
      ds = true;
      tps = 80;
      spt = 18;
      bps = 256;
    } else {
      if ((aTotalSize - 384) % 256 == 0) {
        spt = static_cast<quint16>((aTotalSize - 384) / 256);
        bps = 256;
      } else {
        spt = static_cast<quint16>((aTotalSize + 127) / 128);
        bps = 128;
      }
      ds = false;
      tps = 1;
    }

    initialize(ds, tps, spt, bps);
  }

  void DiskGeometry::initialize(const QByteArray &percom) {
    const auto aTracksPerSide {static_cast<quint8>(percom.at(0))};
    const quint16 aSectorsPerTrack = static_cast<quint8>(percom.at(2)) * 256 + static_cast<quint8>(percom.at(3));
    const bool aIsDoubleSided = static_cast<quint8>(percom.at(4));
    const quint16 aBytesPerSector = static_cast<quint8>(percom.at(6)) * 256 + static_cast<quint8>(percom.at(7));
    initialize(aIsDoubleSided, aTracksPerSide, aSectorsPerTrack, aBytesPerSector);
  }

  bool DiskGeometry::isEqual(const DiskGeometry &other) const {
    return mIsDoubleSided == other.isDoubleSided() &&
           mTracksPerSide == other.tracksPerSide() &&
           mSectorsPerTrack == other.sectorsPerTrack() &&
           mBytesPerSector == other.bytesPerSector();
  }

  bool DiskGeometry::isStandardSD() const {
    return !mIsDoubleSided && mTracksPerSide == 40 && mSectorsPerTrack == 18 && mBytesPerSector == 128;
  }

  bool DiskGeometry::isStandardED() const {
    return !mIsDoubleSided && mTracksPerSide == 40 && mSectorsPerTrack == 26 && mBytesPerSector == 128;
  }

  bool DiskGeometry::isStandardDD() const {
    return !mIsDoubleSided && mTracksPerSide == 40 && mSectorsPerTrack == 18 && mBytesPerSector == 256;
  }

  bool DiskGeometry::isStandardDSDD() const {
    return mIsDoubleSided && mTracksPerSide == 40 && mSectorsPerTrack == 18 && mBytesPerSector == 256;
  }

  bool DiskGeometry::isStandardDSQD() const {
    return mIsDoubleSided && mTracksPerSide == 80 && mSectorsPerTrack == 18 && mBytesPerSector == 256;
  }

  quint16 DiskGeometry::bytesPerSector(const quint16 sector) const {
    quint16 result = mBytesPerSector;
    if (result == 256 && sector <= 3) {
      result = 128;
    }
    return result;
  }

  QByteArray DiskGeometry::toPercomBlock() const {
    DiskGeometry temp;
    QByteArray percom(12, 0);
    percom[0] = static_cast<char>(mTracksPerSide);
    percom[1] = 1;// Step rate
    percom[2] = static_cast<char>(mSectorsPerTrack / 256);
    percom[3] = static_cast<char>(mSectorsPerTrack % 256);
    percom[4] = mIsDoubleSided;
    percom[5] = static_cast<char>((mBytesPerSector != 128) * 4 | (mTracksPerSide == 77) * 2);
    percom[6] = static_cast<char>(mBytesPerSector / 256);
    percom[7] = static_cast<char>(mBytesPerSector % 256);
    percom[8] = static_cast<char>(0xff);
    temp.initialize(percom);
    return percom;
  }

  QString DiskGeometry::humanReadable() const {
    QString result;

    if (isStandardSD()) {
      result = tr("SD Diskette");
    } else if (isStandardED()) {
      result = tr("ED Diskette");
    } else if (isStandardDD()) {
      result = tr("DD Diskette");
    } else if (isStandardDSDD()) {
      result = tr("DS/DD Diskette");
    } else if (isStandardDSQD()) {
      result = tr("DS/QD Diskette");
    } else if (mTracksPerSide == 1) {
      if (mBytesPerSector == 128) {
        result = tr("%1 sector SD hard disk").arg(mSectorCount);
      } else if (mBytesPerSector == 256) {
        result = tr("%1 sector DD hard disk").arg(mSectorCount);
      } else {
        result = tr("%1 sector, %2 bytes/sector hard disk").arg(mSectorCount).arg(mBytesPerSector);
      }
    } else {
      result = tr("%1 %2 tracks/side, %3 sectors/track, %4 bytes/sector diskette")
        .arg(mIsDoubleSided ? tr("DS") : tr("SS"))
        .arg(mTracksPerSide)
        .arg(mSectorsPerTrack)
        .arg(mBytesPerSector);
    }

    return tr("%1 (%2k)").arg(result).arg((mTotalSize + 512) / 1024);
  }

}// namespace