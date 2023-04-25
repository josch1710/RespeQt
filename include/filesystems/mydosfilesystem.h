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
    MyDosFileSystem(SimpleDiskImage *image);

    uint totalCapacity();

    QString name() { return "MyDos"; }
    int fileSystemCode() { return 4; }
  };
}
#endif//MYDOSFILESYSTEM_H
