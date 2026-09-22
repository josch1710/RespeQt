/*
 * atarifilesystem.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "filesystems/atarifilesystem.h"

#include "ui/diskeditdialog.h"
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
        if (QList<AtariDirEntry> subs = getEntries(e.firstSector); !extractRecursive(subs, newDir)) {
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
        if (QList<AtariDirEntry> subs = getEntries(e.firstSector);!deleteRecursive(subs)) {
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

  QList<AtariDirEntry> AtariFileSystem::insertRecursive(const quint16 dir, const QStringList &files) {
    QList<AtariDirEntry> result;
    foreach (QString name, files) {
      AtariDirEntry entry;
      if (QFileInfo info(name); info.isDir()) {
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

  QByteArray AtariFileSystem::findName(const quint16 dir, const QString &name) {
    QList<AtariDirEntry> entries = getEntries(dir);
    const QFileInfo info(name);
    QString baseName = info.completeBaseName().toUpper();
    QString extension = info.suffix().toUpper();
    static QRegularExpression regexp("[^A-Z0-9]");

    baseName.remove(regexp);
    baseName = baseName.left(8);
    if (baseName.isEmpty()) {
      baseName = "BADNAME";
    }
    extension.remove(regexp);
    extension = extension.left(3);
    while (extension.count() < 3) {
      extension.append(" ");
    }
    const QString pfx {baseName};
    for (int i = 1; i < 99999999; i++) {
      QString atariName {baseName};
      while (atariName.count() < 8) {
        atariName.append(" ");
      }
      atariName.append(extension);
      QByteArray result {atariName.toLatin1()};
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
    return {};
  }

  quint16 AtariFileSystem::findFreeSector(const quint16 from) {
    const quint16 sectors {static_cast<quint16>(bitmap.count() * 8)};

    auto startFrom {from};
    if (from < 4) {
      startFrom = 4;
    }

    for (quint16 sector {startFrom}; sector < sectors; sector++)
    {
      if (constexpr quint8 masks[8] {128, 64, 32, 16, 8, 4, 2, 1}; bitmap.at(sector / 8) & masks[sector % 8]) {
        return sector;
      }
    }

    if (from == 0) {
      return 0;
    }

    return findFreeSector(0);
  }

  void AtariFileSystem::allocateSector(const quint16 sector) {
    constexpr quint8 masks[8] {128, 64, 32, 16, 8, 4, 2, 1};
    bitmap[sector / 8] = static_cast<char>(bitmap.at(sector / 8) & ~masks[sector % 8]);
    m_freeSectors--;
  }

  void AtariFileSystem::freeSector(const quint16 sector) {
    constexpr quint8 masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    bitmap[sector / 8] = static_cast<char>(bitmap.at(sector / 8) | masks[sector % 8]);
    m_freeSectors++;
  }

  bool AtariFileSystem::sectorIsFree(const quint16 sector) const
  {
    if (const quint16 sectors {static_cast<quint16>(bitmap.count() * 8)}; sector < 4 || sector > sectors) {
      return false;
    }
    constexpr quint8 masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    return (bitmap.at(sector / 8) & masks[sector % 8]) != 0;
  }

}// namespace Filesystems
