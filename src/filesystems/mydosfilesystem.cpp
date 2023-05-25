#include "filesystems/mydosfilesystem.h"

namespace Filesystems {


  MyDosFileSystem::MyDosFileSystem(DiskImages::DiskImagePtr image)
      : Dos20FileSystem(image) {
    int xvtocCount;
    if (m_image->geometry().bytesPerSector() == 256) {
      xvtocCount = (quint8) vtoc.at(0) - 2;
    } else {
      xvtocCount = ((quint8) vtoc.at(0) * 2) - 4;
    }
    if (xvtocCount < 0) {
      xvtocCount = 0;
    }
    xvtoc = QByteArray();
    for (quint16 s = 359; xvtocCount > 0; xvtocCount--, s--) {
      QByteArray data;
      m_image->readSector(s, data);
      xvtoc.append(data);
    }
    bitmap.append(vtoc.right(m_image->geometry().bytesPerSector() - 100));
    bitmap.append(xvtoc);
    bitmap.resize((image->geometry().sectorCount() + 8) / 8);
  }

  bool MyDosFileSystem::writeBitmap() {
    QByteArray data;
    if (!m_image->readSector(360, data)) {
      return false;
    }

    int bps = m_image->geometry().bytesPerSector();

    int total = bitmap.count();
    if (total > bps - 10) {
      total = bps - 10;
    }

    data.replace(10, total, bitmap.left(total));

    data[3] = m_freeSectors % 256;
    data[4] = m_freeSectors / 256;
    if (!m_image->writeSector(360, data)) {
      return false;
    }

    if (bitmap.count() == total) {
      return true;
    }

    int xvtocCount = (bitmap.count() - total + bps - 1) / bps;

    for (quint16 s = 359; xvtocCount > 0; xvtocCount--, s--, total += bps) {
      int n = bitmap.count() - total;
      if (n > bps) {
        n = bps;
      }
      data = bitmap.mid(total, n);
      data.append(QByteArray(bps - n, 0));
      if (!m_image->writeSector(s, data)) {
        return false;
      }
    }
    return true;
  }

  uint MyDosFileSystem::totalCapacity() {
    return ((quint8) vtoc.at(1) + (quint8) vtoc.at(2) * 256) * (m_image->geometry().bytesPerSector() - 3);
  }


}// namespace Filesystems