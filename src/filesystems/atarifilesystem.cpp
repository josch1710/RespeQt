/*
 * atarifilesystem.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "filesystems/atarifilesystem.h"

#include "diskeditdialog.h"
#include <QDir>
#include <QMessageBox>
#include <QtGlobal>

namespace Filesystems {
  /* AtariFileSystem */

  bool AtariFileSystem::extractRecursive(QList<AtariDirEntry> &entries, const QString &target) {
    foreach (AtariDirEntry e, entries) {
      if (e.attributes & AtariDirEntry::Directory) {
        QString newDir = target + "/" + e.niceName();
        if (!QDir(newDir).mkdir(newDir)) {
          QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot create directory '%1'.").arg(e.niceName()));
          return false;
        }
        QList<AtariDirEntry> subs = getEntries(e.firstSector);
        bool res = extractRecursive(subs, newDir);
        if (!res) {
          return false;
        }
        continue;
      }
      if (!extract(e, target)) {
        return false;
      }
    }
    return true;
  }

  bool AtariFileSystem::deleteRecursive(QList<AtariDirEntry> &entries) {
    foreach (AtariDirEntry e, entries) {
      if (e.attributes & AtariDirEntry::Directory) {
        QList<AtariDirEntry> subs = getEntries(e.firstSector);
        bool res = deleteRecursive(subs);
        if (!res) {
          return false;
        }
        removeDir(e);
        continue;
      }
      if (!erase(e)) {
        return false;
      }
    }
    return true;
  }

  QList<AtariDirEntry> AtariFileSystem::insertRecursive(quint16 dir, const QStringList &files) {
    QList<AtariDirEntry> result;
    foreach (QString name, files) {
      AtariDirEntry entry;
      QFileInfo info(name);
      if (info.isDir()) {
        entry = makeDir(dir, name);
        if (!entry.isValid()) {
          return result;
        }
        QDir subDir(name);
        QStringList subList;
        foreach (QFileInfo i, subDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files)) {
          subList.append(i.absoluteFilePath());
        }

        insertRecursive(entry.firstSector, subList);
      } else {
        entry = insert(dir, name);
        if (!entry.isValid()) {
          return result;
        }
      }
      result.append(entry);
    }
    return result;
  }

  QByteArray AtariFileSystem::findName(quint16 dir, const QString &name) {
    QList<AtariDirEntry> entries = getEntries(dir);
    QFileInfo info(name);
    QString baseName = info.completeBaseName().toUpper();
    QString extension = info.suffix().toUpper();
    QString atariName, pfx;
    QByteArray result;

    baseName.remove(QRegularExpression("[^A-Z0-9]"));
    baseName = baseName.left(8);
    if (baseName.isEmpty()) {
      baseName = "BADNAME";
    }
    extension.remove(QRegularExpression("[^A-Z0-9]"));
    extension = extension.left(3);
    while (extension.count() < 3) {
      extension.append(" ");
    }
    pfx = baseName;
    for (int i = 1; i < 99999999; i++) {
      atariName = baseName;
      while (atariName.count() < 8) {
        atariName.append(" ");
      }
      atariName.append(extension);
      result = atariName.toLatin1();
      bool found = false;
      foreach (AtariDirEntry e, entries) {
        if (e.atariName == result) {
          found = true;
          break;
        }
      }
      if (!found) {
        return result;
      }
      QString sfx = QString::number(i + 1);
      baseName = pfx;
      if (baseName.count() + sfx.count() > 8) {
        baseName.resize(8 - sfx.count());
      }
      baseName.append(sfx);
    }
    return QByteArray();
  }

  quint16 AtariFileSystem::findFreeSector(quint16 from) {
    quint8 masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    quint16 sector;
    uint sectors = bitmap.count() * 8;

    uint startFrom = from;
    if (from < 4) {
      startFrom = 4;
    }

    for (sector = startFrom; sector < sectors; sector++) {
      if (bitmap.at(sector / 8) & masks[sector % 8]) {
        return sector;
      }
    }

    if (from == 0) {
      return 0;
    } else {
      return findFreeSector(0);
    }
  }

  void AtariFileSystem::allocateSector(quint16 sector) {
    quint8 masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    bitmap[sector / 8] = bitmap.at(sector / 8) & ~masks[sector % 8];
    m_freeSectors--;
  }

  void AtariFileSystem::freeSector(quint16 sector) {
    quint8 masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    bitmap[sector / 8] = bitmap.at(sector / 8) | masks[sector % 8];
    m_freeSectors++;
  }

  bool AtariFileSystem::sectorIsFree(quint16 sector) {
    uint sectors = bitmap.count() * 8;
    if (sector < 4 || sector > sectors) {
      return false;
    }
    quint8 masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    return (bitmap.at(sector / 8) & masks[sector % 8]) != 0;
  }

}// namespace Filesystems
