#ifndef DOS25FILESYSTEM_H
#define DOS25FILESYSTEM_H

#include "filesystems/dos20filesystem.h"

namespace Filesystems {

  class Dos25FileSystem : public Dos20FileSystem {
    Q_OBJECT

  protected:
    QByteArray vtoc2;
    bool writeBitmap();

  public:
    Dos25FileSystem(DiskImages::DiskImagePtr image);

    uint totalCapacity();

    QString name() { return "Atari Dos 2.5"; }
  };
}// namespace Filesystems
#endif//DOS25FILESYSTEM_H
