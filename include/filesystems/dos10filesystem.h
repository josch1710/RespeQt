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
    Dos10FileSystem(SimpleDiskImage *image);
    QList<AtariDirEntry> getEntries(quint16 dir);
    uint totalCapacity();
    int findFreeFileNo(quint16 dir);
    uint freeSpace() { return m_freeSectors * (m_image->geometry().bytesPerSector() - 3); }
    QString volumeLabel();
    bool setVolumeLabel(const QString &label);
    bool extract(const AtariDirEntry &entry, const QString &target);
    AtariDirEntry insert(quint16 dir, const QString &name);
    AtariDirEntry makeDir(quint16 dir, const QString &name);
    bool erase(const AtariDirEntry &entry);
    bool rename(const AtariDirEntry &entry, const QByteArray &name);
    bool setTime(const AtariDirEntry &entry, const QDateTime &time);
    bool setReadOnly(const AtariDirEntry &entry);
    bool setHidden(const AtariDirEntry &entry);
    bool setArchived(const AtariDirEntry &entry);
    QString name() { return "Atari Dos 1.0"; }
    int fileSystemCode() { return 1; }
    bool removeDir(const AtariDirEntry &entry);
    quint16 rootDir() { return 361; }
  };
}// namespace Filesystems

#endif//DOS10FILESYSTEM_H
