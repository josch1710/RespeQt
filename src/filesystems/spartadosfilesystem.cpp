#include "filesystems/spartadosfilesystem.h"
#include "diskeditdialog.h"
#include <QMessageBox>

namespace Filesystems {
  SpartaDosFile::SpartaDosFile(SpartaDosFileSystem *fileSystem, quint16 firstMap) {
    m_fileSystem = fileSystem;

    m_currentMapOffset = 4;
    m_fileSystem->m_image->readSector(firstMap, m_currentMap);

    m_currentSector = QByteArray();
    m_currentSectorOffset = 0;

  }

  QByteArray SpartaDosFile::read(uint bytes) {
    QByteArray result;
    while (bytes) {
      uint left = m_currentSector.count() - m_currentSectorOffset;
      if (bytes > left) {
        result.append(m_currentSector.right(left));
        bytes -= left;
        if (m_currentMapOffset >= m_currentMap.count()) {
          int nextMap = (quint8) m_currentMap.at(0) + (quint8) m_currentMap.at(1) * 256;
          if (nextMap == 0) {
            m_currentMap.clear();
            return result;
          }
          m_fileSystem->m_image->readSector(nextMap, m_currentMap);
          m_currentMapOffset = 4;
        }
        int sector = (quint8) m_currentMap.at(m_currentMapOffset) + (quint8) m_currentMap.at(m_currentMapOffset + 1) * 256;
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

  bool SpartaDosFile::write(const QByteArray &, uint /*bytes*/) {
    return false;
  }

  SpartaDosFileSystem::SpartaDosFileSystem(DiskImages::SimpleDiskImage *image)
      : AtariFileSystem(image) {
    QByteArray boot;
    m_image->readSector(1, boot);
    m_rootDirMap = (quint8) boot.at(9) + (quint8) boot.at(10) * 256;
    m_freeSectors = (quint8) boot.at(13) + (quint8) boot.at(14) * 256;
    m_bitmapCount = (quint8) boot.at(15);
    m_firstBitmapSector = (quint8) boot.at(16) + (quint8) boot.at(17) * 256;
    m_volumeName = boot.mid(22, 8);

    QByteArray map;
    for (int i = m_firstBitmapSector; i < m_firstBitmapSector + m_bitmapCount; i++) {
      m_image->readSector(i, map);
      bitmap.append(map);
    }
  }

  QList<AtariDirEntry> SpartaDosFileSystem::getEntries(quint16 dir) {
    QList<AtariDirEntry> list;

    SpartaDosFile sf(this, dir);
    QByteArray dosEntry = sf.read(23);

    if (dosEntry.size() < 6) {
      list.clear();
      return list;
    }
    int dirLen = (quint8) dosEntry.at(3) + (quint8) dosEntry.at(4) * 256 + (quint8) dosEntry.at(5) * 65536 - 23;
    int no = 0;

    while (dirLen > 0) {
      AtariDirEntry entry;
      dosEntry = sf.read(23);
      int f = (quint8) dosEntry.at(0);
      if (f == 0) {
        break;
      }
      if ((f & 144) == 0) {
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

    QByteArray buffer;
    while (rest) {
      int bufSize = 8388608;
      if (bufSize > rest) {
        bufSize = rest;
      }
      buffer = sdf.read(bufSize);
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
    return AtariDirEntry();
  }

  AtariDirEntry SpartaDosFileSystem::makeDir(quint16 /*dir*/, const QString & /*name*/) {
    return AtariDirEntry();
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