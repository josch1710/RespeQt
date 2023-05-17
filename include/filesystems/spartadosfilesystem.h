#ifndef SPARTADOSFILESYSTEM_H
#define SPARTADOSFILESYSTEM_H

#include "diskimage.h"
#include "filesystems/ataridirentry.h"
#include "filesystems/atarifilesystem.h"
#include <QByteArray>
#include <QDateTime>
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
    QByteArray read(uint bytes);
    bool write(const QByteArray &data, uint bytes);
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
    SpartaDosFileSystem(SimpleDiskImage *image);
    QList<AtariDirEntry> getEntries(quint16 dir);

    uint totalCapacity();
    uint freeSpace();

    int findFreeFileNo(quint16 dir);

    bool extract(const AtariDirEntry &entry, const QString &target);
    AtariDirEntry insert(quint16 dir, const QString &name);
    AtariDirEntry makeDir(quint16 dir, const QString &name);
    bool erase(const AtariDirEntry &entry);
    bool rename(const AtariDirEntry &entry, const QByteArray &name);

    QString name() { return "SpartaDos"; }
    bool removeDir(const AtariDirEntry &entry);
    quint16 rootDir() { return m_rootDirMap; }
  };
}// namespace Filesystems

#endif//SPARTADOSFILESYSTEM_H
