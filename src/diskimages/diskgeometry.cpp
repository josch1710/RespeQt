
#include "diskimages/diskgeometry.h"

namespace DiskImages {
  DiskGeometry::DiskGeometry()
      : QObject() {
    mIsDoubleSided = false;
    mTracksPerSide = 0;
    mSectorsPerTrack = 0;
    mBytesPerSector = 0;
    mTotalSize = 0;
    mSectorCount = 0;
  }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
  DiskGeometry::DiskGeometry(const DiskGeometry &other)
      : QObject() {
    initialize(other);
  }
#pragma clang diagnostic pop

  void DiskGeometry::initialize(const DiskGeometry &other) {
    mIsDoubleSided = other.isDoubleSided();
    mTracksPerSide = other.tracksPerSide();
    mSectorsPerTrack = other.sectorsPerTrack();
    mBytesPerSector = other.bytesPerSector();
    mSectorCount = other.sectorCount();
    mTotalSize = other.totalSize();
  }

  void DiskGeometry::initialize(bool aIsDoubleSided, quint8 aTracksPerSide, quint16 aSectorsPerTrack, quint16 aBytesPerSector) {
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

  void DiskGeometry::initialize(uint aTotalSize, quint16 aBytesPerSector) {
    bool ds;
    quint8 tps;
    quint16 spt;

    if (aTotalSize == 92160 && aBytesPerSector == 128) {
      ds = false;
      tps = 40;
      spt = 18;
    } else if (aTotalSize == 133120 && aBytesPerSector == 128) {
      ds = false;
      tps = 40;
      spt = 26;
    } else if (((aTotalSize == 183936) || (aTotalSize == 184320)) && aBytesPerSector == 256) {// ATR or XFD
      ds = false;
      tps = 40;
      spt = 18;
    } else if (aTotalSize == 368256 && aBytesPerSector == 256) {
      ds = true;
      tps = 40;
      spt = 18;
    } else if (aTotalSize == 736896 && aBytesPerSector == 256) {
      ds = true;
      tps = 80;
      spt = 18;
    } else {
      if (aBytesPerSector == 256) {
        if (aTotalSize <= 384) {
          spt = (aTotalSize + 127) / 128;
        } else {
          spt = (aTotalSize + 384 + 255) / 256;
        }
      } else {
        spt = (aTotalSize + aBytesPerSector - 1) / aBytesPerSector;
      }
      ds = false;
      tps = 1;
    }

    initialize(ds, tps, spt, aBytesPerSector);
  }

  void DiskGeometry::initialize(uint aTotalSize) {
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
    } else if ((aTotalSize == 183936) || (aTotalSize == 184320)) {// ATR or XFD
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
        spt = (aTotalSize - 384) / 256;
        bps = 256;
      } else {
        spt = (aTotalSize + 127) / 128;
        bps = 128;
      }
      ds = false;
      tps = 1;
    }

    initialize(ds, tps, spt, bps);
  }

  void DiskGeometry::initialize(const QByteArray &percom) {
    auto aTracksPerSide = (quint8) percom.at(0);
    quint16 aSectorsPerTrack = (quint8) percom.at(2) * 256 + (quint8) percom.at(3);
    bool aIsDoubleSided = (quint8) percom.at(4);
    quint16 aBytesPerSector = (quint8) percom.at(6) * 256 + (quint8) percom.at(7);
    initialize(aIsDoubleSided, aTracksPerSide, aSectorsPerTrack, aBytesPerSector);
  }

  bool DiskGeometry::isEqual(const DiskGeometry &other) {
    return mIsDoubleSided == other.isDoubleSided() &&
           mTracksPerSide == other.tracksPerSide() &&
           mSectorsPerTrack == other.sectorsPerTrack() &&
           mBytesPerSector == other.bytesPerSector();
  }

  bool DiskGeometry::isStandardSD() const {
    return (!mIsDoubleSided) && mTracksPerSide == 40 && mSectorsPerTrack == 18 && mBytesPerSector == 128;
  }

  bool DiskGeometry::isStandardED() const {
    return (!mIsDoubleSided) && mTracksPerSide == 40 && mSectorsPerTrack == 26 && mBytesPerSector == 128;
  }

  bool DiskGeometry::isStandardDD() const {
    return (!mIsDoubleSided) && mTracksPerSide == 40 && mSectorsPerTrack == 18 && mBytesPerSector == 256;
  }

  bool DiskGeometry::isStandardDSDD() const {
    return mIsDoubleSided && mTracksPerSide == 40 && mSectorsPerTrack == 18 && mBytesPerSector == 256;
  }

  bool DiskGeometry::isStandardDSQD() const {
    return mIsDoubleSided && mTracksPerSide == 80 && mSectorsPerTrack == 18 && mBytesPerSector == 256;
  }

  quint16 DiskGeometry::bytesPerSector(quint16 sector) {
    quint16 result = mBytesPerSector;
    if (result == 256 && sector <= 3) {
      result = 128;
    }
    return result;
  }

  QByteArray DiskGeometry::toPercomBlock() {
    DiskGeometry temp;
    QByteArray percom(12, 0);
    percom[0] = mTracksPerSide;
    percom[1] = 1;// Step rate
    percom[2] = mSectorsPerTrack / 256;
    percom[3] = mSectorsPerTrack % 256;
    percom[4] = mIsDoubleSided;
    percom[5] = (mBytesPerSector != 128) * 4 | (mTracksPerSide == 77) * 2;
    percom[6] = mBytesPerSector / 256;
    percom[7] = mBytesPerSector % 256;
    percom[8] = 0xff;
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
      result = tr("DS/DD Diskette");
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