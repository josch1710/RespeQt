#include "filesystems/dos20filesystem.h"

namespace Filesystems {

  Dos20FileSystem::Dos20FileSystem(SimpleDiskImage *image)
      : Dos10FileSystem(image) {
  }

  uint Dos20FileSystem::totalCapacity() {
    return ((quint8) vtoc.at(1) + (quint8) vtoc.at(2) * 256) * (m_image->geometry().bytesPerSector() - 3);
  }

}// namespace Filesystems