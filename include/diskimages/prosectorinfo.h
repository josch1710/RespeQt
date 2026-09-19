
#ifndef RESPEQT_PROSECTORINFO_H
#define RESPEQT_PROSECTORINFO_H

#include <QtGlobal>
#include <QByteArray>

namespace DiskImages {
  struct ProSectorInfo {
    bool used;
    bool paired;
    bool inserted;
    bool firstPass;
    quint8 sectorNumber;
    quint8 totalDuplicate;
    quint8 duplicateOffset[5];
    quint8 driveStatus;
    quint8 wd1771Status;
    [[maybe_unused]]quint8 reservedByte;
    quint16 absoluteSector;
    bool notEmpty;
    quint8 fillByte;
    quint16 weakBits;
    quint8 sectorTiming;
    quint8 shortSectorSize;
    quint8 lastSectorRead;
    quint16 beforeSlot;
    QByteArray sectorData;
  };
}

#endif//RESPEQT_PROSECTORINFO_H
