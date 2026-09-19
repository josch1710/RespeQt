#ifndef MYDOSFILESYSTEM_H
#define MYDOSFILESYSTEM_H

#include "filesystems/dos20filesystem.h"

namespace Filesystems {

  class MyDosFileSystem : public Dos20FileSystem {
    Q_OBJECT

  protected:
    QByteArray xvtoc;
    bool writeBitmap() override;

  public:
    explicit MyDosFileSystem(DiskImages::SimpleDiskImage *image);

    uint totalCapacity() override;

    QString name() override { return "MyDos"; }
  };
}// namespace Filesystems
#endif//MYDOSFILESYSTEM_H
