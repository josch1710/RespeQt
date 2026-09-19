#ifndef DOS25FILESYSTEM_H
#define DOS25FILESYSTEM_H

#include "filesystems/dos20filesystem.h"

namespace Filesystems {

  class Dos25FileSystem : public Dos20FileSystem {
    Q_OBJECT

  protected:
    QByteArray vtoc2;
    bool writeBitmap() override;

  public:
    explicit Dos25FileSystem(DiskImages::SimpleDiskImage *image);

    uint totalCapacity() override;

    QString name() override { return "Atari Dos 2.5"; }
  };
}// namespace Filesystems
#endif//DOS25FILESYSTEM_H
