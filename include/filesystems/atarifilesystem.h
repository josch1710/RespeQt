/*
 * atarifilesystem.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef ATARIFILESYSTEM_H
#define ATARIFILESYSTEM_H

#include "filesystems/ataridirentry.h"
#include "include/diskimages/diskimage.h"
#include <QDateTime>
#include <QList>
#include <QString>

namespace Filesystems {
  class AtariFileSystem : public QObject {
    Q_OBJECT

  protected:
    DiskImages::DiskImagePtr m_image;
    bool m_textConversion;
    QByteArray bitmap;
    quint16 m_freeSectors;
    quint16 findFreeSector(quint16 from = 0);
    void allocateSector(quint16 sector);
    void freeSector(quint16 sector);
    bool sectorIsFree(quint16 sector);

  public:
    AtariFileSystem(DiskImages::DiskImagePtr image) {
      m_image = image;
      m_textConversion = false;
    }
    virtual QList<AtariDirEntry> getEntries(quint16 dir) = 0;
    virtual uint totalCapacity() = 0;
    virtual uint freeSpace() = 0;
    bool extractRecursive(QList<AtariDirEntry> &entries, const QString &target);
    bool deleteRecursive(QList<AtariDirEntry> &entries);
    QList<AtariDirEntry> insertRecursive(quint16 dir, const QStringList &files);
    QByteArray findName(quint16 dir, const QString &name);
    virtual int findFreeFileNo(quint16 dir) = 0;
    virtual bool extract(const AtariDirEntry &entry, const QString &target) = 0;
    virtual AtariDirEntry insert(quint16 dir, const QString &name) = 0;
    virtual bool erase(const AtariDirEntry &entry) = 0;
    virtual bool rename(const AtariDirEntry &entry, const QByteArray &name) = 0;
    inline DiskImages::DiskImagePtr image() { return m_image; }
    inline void setTextConversion(bool conv) { m_textConversion = conv; }
    virtual QString name() = 0;
    virtual bool removeDir(const AtariDirEntry &entry) = 0;
    virtual AtariDirEntry makeDir(quint16 dir, const QString &name) = 0;
    virtual quint16 rootDir() = 0;
  };

}// namespace Filesystems
#endif// ATARIFILESYSTEM_H
