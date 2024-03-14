#include "diskimages/diskimagefactory.h"
#include "diskimages/diskimage.h"
#include "diskimages/simplediskimage.h"
#include "diskimages/diskimageatx.h"
#include "diskimages/diskimagepro.h"
#include "diskimages/folderimage.h"

namespace DiskImages {
  std::shared_ptr<DiskImageFactory> DiskImageFactory::sInstance = std::shared_ptr<DiskImageFactory>(new DiskImageFactory);

  DiskImageFactory::DiskImageFactory() {
    registerDiskImage<SimpleDiskImage>(FileTypes::FileType::Atr);
    registerDiskImage<SimpleDiskImage>(FileTypes::FileType::AtrGz);
    registerDiskImage<SimpleDiskImage>(FileTypes::FileType::Xfd);
    registerDiskImage<SimpleDiskImage>(FileTypes::FileType::XfdGz);

    registerDiskImage<DiskImageAtx>(FileTypes::FileType::Atx);
    registerDiskImage<DiskImageAtx>(FileTypes::FileType::AtxGz);

    registerDiskImage<DiskImagePro>(FileTypes::FileType::Pro);
    registerDiskImage<DiskImagePro>(FileTypes::FileType::ProGz);

    registerDiskImage<FolderImage>(FileTypes::FileType::Dir);
  }
}
