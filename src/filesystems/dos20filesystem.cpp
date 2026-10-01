#include "filesystems/dos20filesystem.h"

namespace Filesystems {

  Dos20FileSystem::Dos20FileSystem(DiskImages::SimpleDiskImage *image)
      : Dos10FileSystem(image) {
  }

  uint Dos20FileSystem::totalCapacity() {
    return static_cast<quint16>(vtoc.at(1) + vtoc.at(2) * 256  * (m_image->geometry().bytesPerSector() - 3));
  }

}// namespace Filesystems