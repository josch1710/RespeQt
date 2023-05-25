#ifndef MYDOSFILESYSTEM_H
#define MYDOSFILESYSTEM_H

#include "filesystems/dos20filesystem.h"

namespace Filesystems {

  class MyDosFileSystem : public Dos20FileSystem {
    Q_OBJECT

  protected:
    QByteArray xvtoc;
    bool writeBitmap();

  public:
    MyDosFileSystem(DiskImages::DiskImagePtr image);

    uint totalCapacity();

    QString name() { return "MyDos"; }
  };
}// namespace Filesystems
#endif//MYDOSFILESYSTEM_H
