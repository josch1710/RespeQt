#include "filesystems/spartadosfilesystem.h"
#include "ui/diskeditdialog.h"
#include <QMessageBox>
#include <algorithm>

namespace Filesystems {
  SpartaDosFile::SpartaDosFile(SpartaDosFileSystem *fileSystem, const quint16 firstMap) {
    m_fileSystem = fileSystem;

    m_currentMapOffset = 4;
    m_fileSystem->m_image->readSector(firstMap, m_currentMap);

    m_currentSector = QByteArray();
    m_currentSectorOffset = 0;

  }

  QByteArray SpartaDosFile::read(int bytes) {
    QByteArray result;
    while (bytes) {
      if (const auto left = m_currentSector.count() - m_currentSectorOffset; bytes > left) {
        result.append(m_currentSector.right(left));
        bytes -= left;
        if (m_currentMapOffset >= m_currentMap.count()) {
          const quint16 nextMap = static_cast<quint8>(m_currentMap.at(0)) + static_cast<quint8>(m_currentMap.at(1)) * 256;
          if (nextMap == 0) {
            m_currentMap.clear();
            return result;
          }
          m_fileSystem->m_image->readSector(nextMap, m_currentMap);
          m_currentMapOffset = 4;
        }
        const quint16 sector = static_cast<quint8>(m_currentMap.at(m_currentMapOffset)) + static_cast<quint8>(m_currentMap.at(m_currentMapOffset + 1)) * 256;
        m_currentMapOffset += 2;
        if (sector == 0) {
          m_currentMap.clear();
          return result;
        }
        m_fileSystem->m_image->readSector(sector, m_currentSector);
        m_currentSectorOffset = 0;
      } else {
        result.append(m_currentSector.mid(m_currentSectorOffset, bytes));
        m_currentSectorOffset += bytes;
        bytes = 0;
      }
    }
    return result;
  }

  bool SpartaDosFile::write(const QByteArray &, int /*bytes*/) {
    return false;
  }

  SpartaDosFileSystem::SpartaDosFileSystem(DiskImages::SimpleDiskImage *image)
      : AtariFileSystem(image) {
    QByteArray boot;
    m_image->readSector(1, boot);
    m_rootDirMap = static_cast<quint8>(boot.at(9)) + static_cast<quint8>(boot.at(10)) * 256;
    m_freeSectors = static_cast<quint8>(boot.at(13)) + static_cast<quint8>(boot.at(14)) * 256;
    m_bitmapCount = static_cast<quint8>(boot.at(15));
    m_firstBitmapSector = static_cast<quint8>(boot.at(16)) + static_cast<quint8>(boot.at(17)) * 256;
    m_volumeName = boot.mid(22, 8);

    QByteArray map;
    for (quint16 i = m_firstBitmapSector; i < m_firstBitmapSector + m_bitmapCount; i++) {
      m_image->readSector(i, map);
      bitmap.append(map);
    }
  }

  QList<AtariDirEntry> SpartaDosFileSystem::getEntries(const quint16 dir) {
    QList<AtariDirEntry> list;

    SpartaDosFile sf(this, dir);
    QByteArray dosEntry = sf.read(23);

    if (dosEntry.size() < 6) {
      list.clear();
      return list;
    }
    int dirLen = static_cast<quint8>(dosEntry.at(3)) + static_cast<quint8>(dosEntry.at(4)) * 256 + static_cast<quint8>(dosEntry.at(5)) * 65536 - 23;
    int no = 0;

    while (dirLen > 0) {
      dosEntry = sf.read(23);
      const int f = static_cast<quint8>(dosEntry.at(0));
      if (f == 0) {
        break;
      }
      if ((f & 144) == 0) {
        AtariDirEntry entry;
        entry.makeFromSpartaDosEntry(dosEntry, no, dir);
        list.append(entry);
      }
      dirLen -= 23;
      no++;
    }

    return list;
  }

  uint SpartaDosFileSystem::totalCapacity() {
    return 0;
  }

  uint SpartaDosFileSystem::freeSpace() {
    return 0;
  }

  bool SpartaDosFileSystem::extract(const AtariDirEntry &entry, const QString &target) {
    QFile file(target + "/" + entry.niceName());


    QFile::OpenMode mode;

    if (m_textConversion) {
      mode = QFile::WriteOnly | QFile::Truncate | QFile::Text;
    } else {
      mode = QFile::WriteOnly | QFile::Truncate;
    }

    if (!file.open(mode)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot create file '%1'.").arg(entry.niceName()));
      return false;
    }

    SpartaDosFile sdf(this, entry.firstSector);

    int rest = entry.size;

    while (rest) {
      int bufSize = 8388608;
      bufSize = std::min(bufSize, rest);
      QByteArray buffer = sdf.read(bufSize);
      if (buffer.count() != bufSize) {
        bufSize = buffer.count();
        rest = bufSize;
      }
      if (m_textConversion) {
        for (int i = 0; i < buffer.count(); i++) {
          if (buffer.at(i) == '\n') {
            buffer[i] = '\x9b';
          } else if (buffer.at(i) == '\x9b') {
            buffer[i] = '\n';
          }
        }
      }
      if (file.write(buffer) != bufSize) {
        QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot write to '%1'.").arg(entry.niceName()));
        return false;
      }
      rest -= bufSize;
    }

    return true;
  }

  AtariDirEntry SpartaDosFileSystem::insert(quint16 /*dir*/, const QString & /*name*/) {
    return {};
  }

  AtariDirEntry SpartaDosFileSystem::makeDir(quint16 /*dir*/, const QString & /*name*/) {
    return {};
  }

  bool SpartaDosFileSystem::erase(const AtariDirEntry & /*entry*/) {
    return false;
  }

  bool SpartaDosFileSystem::rename(const AtariDirEntry & /*entry*/, const QByteArray & /*name*/) {
    return false;
  }

  int SpartaDosFileSystem::findFreeFileNo(quint16 /*dir*/) {
    return -1;
  }

  bool SpartaDosFileSystem::removeDir(const AtariDirEntry & /*entry*/) {
    return false;
  }
}// namespace Filesystems