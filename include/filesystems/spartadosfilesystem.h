#ifndef SPARTADOSFILESYSTEM_H
#define SPARTADOSFILESYSTEM_H

#include "filesystems/ataridirentry.h"
#include "filesystems/atarifilesystem.h"
#include "include/diskimages/diskimage.h"
#include <QByteArray>
#include <QList>
#include <QString>
#include <QtGlobal>

namespace Filesystems {
  class SpartaDosFileSystem;

  class SpartaDosFile {
  protected:
    SpartaDosFileSystem *m_fileSystem;
    QByteArray m_currentSector;
    QByteArray m_currentMap;
    quint16 m_currentMapOffset;
    quint16 m_currentSectorOffset;

  public:
    SpartaDosFile(SpartaDosFileSystem *fileSystem, quint16 firstMap);
    QByteArray read(int bytes);
    static bool write(const QByteArray &data, int bytes);
  };


  class SpartaDosFileSystem : public AtariFileSystem {
    Q_OBJECT
    friend class SpartaDosFile;

  protected:
    quint16 m_rootDirMap;
    quint8 m_bitmapCount;
    quint16 m_firstBitmapSector;
    QByteArray m_volumeName;

  public:
    explicit SpartaDosFileSystem(DiskImages::SimpleDiskImage *image);
    QList<AtariDirEntry> getEntries(quint16 dir) override;

    uint totalCapacity() override;
    uint freeSpace() override;

    int findFreeFileNo(quint16 dir) override;

    bool extract(const AtariDirEntry &entry, const QString &target) override;
    AtariDirEntry insert(quint16 dir, const QString &name) override;
    AtariDirEntry makeDir(quint16 dir, const QString &name) override;
    bool erase(const AtariDirEntry &entry) override;
    bool rename(const AtariDirEntry &entry, const QByteArray &name) override;

    QString name() override { return "SpartaDos"; }
    bool removeDir(const AtariDirEntry &entry) override;
    quint16 rootDir() override { return m_rootDirMap; }
  };
}// namespace Filesystems

#endif//SPARTADOSFILESYSTEM_H
