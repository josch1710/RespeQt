#ifndef RESPEQT_DISKIMAGEFACTORY_H
#define RESPEQT_DISKIMAGEFACTORY_H

#include <QSharedPointer>
#include "sioworker.h"
#include "diskimages/diskimage.h"
#include "miscutils.h"

namespace DiskImages {
  class DiskImageFactory {
  private:
    template<class TDerived>
    static DiskImagePtr creator(SioWorkerPtr worker, bool gzipped) {
      return QSharedPointer<TDerived>::create(worker, gzipped);
    }

    // Instantiation maps
    using Creator = DiskImagePtr(SioWorkerPtr worker, bool gzipped);
    using CreatorPair = std::pair<FileType, Creator *>;
    using CreatorVector = std::vector<CreatorPair>;
    CreatorVector creatorFunctions;

    static std::shared_ptr<DiskImageFactory> sInstance;
    DiskImageFactory();

  public:
    static std::shared_ptr<DiskImageFactory> instance() {
      return sInstance;
    }

    template<class TDerived>
    void registerDiskImage(FileType type) {
      static_assert(std::is_base_of<DiskImage, TDerived>::value, "DiskImageFactory::registerDiskImage doesn't accept this type because it doesn't derive from base class");
      creatorFunctions.push_back(CreatorPair(type, &creator<TDerived>));
    }

    DiskImagePtr createDiskImage(const QString &fileName, const SioWorkerPtr &worker) const {
      auto type = getFileType(fileName);

      for (const auto &it: creatorFunctions) {
        if (it.first == type) {
          // This is a call to the function pointed to by it.second.
          return it.second(worker, isArchive(type));
        }
      }
      return DiskImagePtr{};
    }

    const QVector<QString> getDiskImageNames() const {
      QVector<QString> names;
      for (const auto &it: creatorFunctions) {
        auto name = getFileTypeName(it.first);
        names.append(name);
      }
      return names;
    }
  };
}

#endif//RESPEQT_DISKIMAGEFACTORY_H
