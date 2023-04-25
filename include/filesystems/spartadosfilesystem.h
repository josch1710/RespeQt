#ifndef SPARTADOSFILESYSTEM_H
#define SPARTADOSFILESYSTEM_H

#include "filesystems/atarifilesystem.h"
#include "filesystems/ataridirentry.h"
#include "diskimage.h"
#include <QList>
#include <QByteArray>
#include <QString>
#include <QDateTime>
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
    quint16 m_firstMap;
    uint m_size;
    uint m_position;
    bool m_isDir;

    bool toNextSector();

  public:
    SpartaDosFile(SpartaDosFileSystem *fileSystem, quint16 firstMap, int size, bool isDir = false);
    bool seek(uint position);
    QByteArray read(uint bytes);
    bool write(const QByteArray &data, uint bytes);
  };


  class SpartaDosFileSystem : public AtariFileSystem {
    Q_OBJECT
    friend class SpartaDosFile;

  protected:
    quint16 m_rootDirMap;
    quint16 m_sectorCount;
    quint8 m_bitmapCount;
    quint16 m_firstBitmapSector;
    quint16 m_firstDataSector;
    quint16 m_firstDirSector;
    QByteArray m_volumeName;
    quint8 m_fileSystemVersion;
    quint8 m_sequenceNumber;

  public:
    SpartaDosFileSystem(SimpleDiskImage *image);
    QList<AtariDirEntry> getEntries(quint16 dir);

    uint totalCapacity();
    uint freeSpace();
    QString volumeLabel();
    bool setVolumeLabel(const QString &label);

    int findFreeFileNo(quint16 dir);

    bool extract(const AtariDirEntry &entry, const QString &target);
    AtariDirEntry insert(quint16 dir, const QString &name);
    AtariDirEntry makeDir(quint16 dir, const QString &name);
    bool erase(const AtariDirEntry &entry);
    bool rename(const AtariDirEntry &entry, const QByteArray &name);

    bool setTime(const AtariDirEntry &entry, const QDateTime &time);

    bool setReadOnly(const AtariDirEntry &entry);

    bool setHidden(const AtariDirEntry &entry);

    bool setArchived(const AtariDirEntry &entry);
    QString name() { return "SpartaDos"; }
    int fileSystemCode() { return 5; }
    bool removeDir(const AtariDirEntry &entry);
    quint16 rootDir() { return m_rootDirMap; }
  };
}// namespace Filesystems

#endif//SPARTADOSFILESYSTEM_H
