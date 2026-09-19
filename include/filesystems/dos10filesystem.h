#ifndef DOS10FILESYSTEM_H
#define DOS10FILESYSTEM_H

#include "filesystems/atarifilesystem.h"

namespace Filesystems {

  class Dos10FileSystem : public AtariFileSystem {
    Q_OBJECT

  protected:
    QByteArray vtoc;
    virtual bool writeBitmap();

  public:
    explicit Dos10FileSystem(DiskImages::SimpleDiskImage *image);
    QList<AtariDirEntry> getEntries(quint16 dir) override;
    uint totalCapacity() override;
    int findFreeFileNo(quint16 dir) override;
    uint freeSpace() override { return m_freeSectors * (m_image->geometry().bytesPerSector() - 3); }
    bool extract(const AtariDirEntry &entry, const QString &target) override;
    AtariDirEntry insert(quint16 dir, const QString &name) override;
    AtariDirEntry makeDir(quint16 dir, const QString &name) override;
    bool erase(const AtariDirEntry &entry) override;
    bool rename(const AtariDirEntry &entry, const QByteArray &name) override;
    QString name() override { return "Atari Dos 1.0"; }
    bool removeDir(const AtariDirEntry &entry) override;
    quint16 rootDir() override { return 361; }
  };
}// namespace Filesystems

#endif//DOS10FILESYSTEM_H
