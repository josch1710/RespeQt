#ifndef DOS20FILESYSTEM_H
#define DOS20FILESYSTEM_H

#include "filesystems/dos10filesystem.h"

namespace Filesystems {

  class Dos20FileSystem : public Dos10FileSystem {
    Q_OBJECT

  public:
    Dos20FileSystem(DiskImages::DiskImagePtr image);

    uint totalCapacity();

    QString name() { return "Atari Dos 2.0"; }
  };
}// namespace Filesystems
#endif//DOS20FILESYSTEM_H
