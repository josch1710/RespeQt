#include "diskimages/diskimagefactory.h"
#include "diskimages/diskimage.h"
#include "diskimages/simplediskimage.h"
#include "diskimages/diskimageatx.h"
#include "diskimages/diskimagepro.h"
#include "diskimages/folderimage.h"

namespace DiskImages {
  std::shared_ptr<DiskImageFactory> DiskImageFactory::sInstance = std::shared_ptr<DiskImageFactory>(new DiskImageFactory);

  DiskImageFactory::DiskImageFactory() {
    registerDiskImage<SimpleDiskImage>(FileType::Atr);
    registerDiskImage<SimpleDiskImage>(FileType::AtrGz);
    registerDiskImage<SimpleDiskImage>(FileType::Xfd);
    registerDiskImage<SimpleDiskImage>(FileType::XfdGz);

    registerDiskImage<DiskImageAtx>(FileType::Atx);
    registerDiskImage<DiskImageAtx>(FileType::AtxGz);

    registerDiskImage<DiskImagePro>(FileType::Pro);
    registerDiskImage<DiskImagePro>(FileType::ProGz);

    registerDiskImage<FolderImage>(FileType::Dir);
  }
}
