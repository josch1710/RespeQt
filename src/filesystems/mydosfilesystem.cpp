#include "filesystems/mydosfilesystem.h"

#include <algorithm>

namespace Filesystems {


  MyDosFileSystem::MyDosFileSystem(DiskImages::SimpleDiskImage *image)
      : Dos20FileSystem(image) {
    int xvtocCount;
    if (m_image->geometry().bytesPerSector() == 256) {
      xvtocCount = static_cast<quint8>(vtoc.at(0)) - 2;
    } else {
      xvtocCount = static_cast<quint8>(vtoc.at(0)) * 2 - 4;
    }
    xvtocCount = std::max(xvtocCount, 0);
    xvtoc = QByteArray();
    for (quint16 s = 359; xvtocCount > 0; xvtocCount--, s--) {
      QByteArray data;
      m_image->readSector(s, data);
      xvtoc.append(data);
    }
    bitmap.append(vtoc.right(m_image->geometry().bytesPerSector() - 100));
    bitmap.append(xvtoc);
    bitmap.resize((static_cast<int>(image->geometry().sectorCount()) + 8) / 8);
  }

  bool MyDosFileSystem::writeBitmap() {
    QByteArray data;
    if (!m_image->readSector(360, data)) {
      return false;
    }

    const int bps = m_image->geometry().bytesPerSector();

    int total = bitmap.count();
    total = std::min(total, bps - 10);

    data.replace(10, total, bitmap.left(total));

    data[3] = static_cast<char>(m_freeSectors % 256);
    data[4] = static_cast<char>(m_freeSectors / 256);
    if (!m_image->writeSector(360, data)) {
      return false;
    }

    if (bitmap.count() == total) {
      return true;
    }

    int xvtocCount = (bitmap.count() - total + bps - 1) / bps;

    for (quint16 s = 359; xvtocCount > 0; xvtocCount--, s--, total += bps) {
      int n = bitmap.count() - total;
      n = std::min(n, bps);
      data = bitmap.mid(total, n);
      data.append(QByteArray(bps - n, 0));
      if (!m_image->writeSector(s, data)) {
        return false;
      }
    }
    return true;
  }

  uint MyDosFileSystem::totalCapacity() {
    return (static_cast<quint8>(vtoc.at(1)) + static_cast<quint8>(vtoc.at(2)) * 256) * (m_image->geometry().bytesPerSector() - 3);
  }


}// namespace Filesystems