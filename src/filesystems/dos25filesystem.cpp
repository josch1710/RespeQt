#include "filesystems/dos25filesystem.h"

namespace Filesystems {

  Dos25FileSystem::Dos25FileSystem(DiskImages::SimpleDiskImage *image)
      : Dos20FileSystem(image) {
    m_freeSectors = static_cast<quint8>(vtoc.at(3)) + static_cast<quint8>(vtoc.at(4)) * 256;
    if (m_image->readSector(1024, vtoc2)) {
      bitmap.append(vtoc2.mid(84, 38));
      m_freeSectors += static_cast<quint8>(vtoc2.at(122)) + static_cast<quint8>(vtoc2.at(123)) * 256;
    }
  }

  bool Dos25FileSystem::writeBitmap() {
    QByteArray data;
    if (!m_image->readSector(360, data)) {
      return false;
    }
    data.replace(10, 90, bitmap.left(90));

    quint16 sectors = 0;

    for (quint16 sector = 0; sector < 720; sector++)
    {
      if (constexpr quint8 masks[8] = {128, 64, 32, 16, 8, 4, 2, 1}; bitmap.at(sector / 8) & masks[sector % 8]) {
        sectors++;
      }
    }
    data[3] = static_cast<char>(sectors % 256);
    data[4] = static_cast<char>(sectors / 256);
    if (!m_image->writeSector(360, data)) {
      return false;
    }

    if (!m_image->readSector(1024, data)) {
      return false;
    }
    data.replace(0, 122, bitmap.right(122));
    data[122] = static_cast<char>((m_freeSectors - sectors) % 256);
    data[123] = static_cast<char>((m_freeSectors - sectors) / 256);
    return m_image->writeSector(1024, data);
  }

  uint Dos25FileSystem::totalCapacity() {
    return (static_cast<quint8>(vtoc.at(1)) + static_cast<quint8>(vtoc.at(2)) * 256) * (m_image->geometry().bytesPerSector() - 3);
  }

}// namespace Filesystems