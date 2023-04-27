#include "filesystems/dos10filesystem.h"
#include "filesystems/dos20filesystem.h"
#include "filesystems/mydosfilesystem.h"
#include "diskeditdialog.h"
#include <QMessageBox>

namespace Filesystems {

  Dos10FileSystem::Dos10FileSystem(SimpleDiskImage *image)
      : AtariFileSystem(image) {
    m_image->readSector(360, vtoc);
    bitmap = vtoc.mid(10, 90);
    m_freeSectors = (quint8) vtoc.at(3) + (quint8) vtoc.at(4) * 256;
  }

  QList<AtariDirEntry> Dos10FileSystem::getEntries(quint16 dir) {
    QList<AtariDirEntry> list;

    bool dd = m_image->geometry().bytesPerSector() == 256;

    int no = 0;
    for (quint16 s = dir; s < dir + 8; s++) {
      QByteArray data;
      m_image->readSector(s, data);
      for (uint e = 0; e < 8; e++, no++) {
        QByteArray dosEntry = data.mid(e * 16, 16);
        auto f = (quint8) dosEntry.at(0);
        if (f == 0) {
          goto bailout;
        }
        if (((f & 65) == 65) || (f & 128)) {
          continue;
        }
        AtariDirEntry a;
        a.makeFromAtariDosEntry(dosEntry, no, dir, dd);
        list.append(a);
      }
    }
  bailout:

    return list;
  }

  uint Dos10FileSystem::totalCapacity() {
    return 707 * 125;
  }

  bool Dos10FileSystem::extract(const AtariDirEntry &entry, const QString &target) {
    QFile file(target + "/" + entry.niceName());
    QFile::OpenMode mode;

    mode = QFile::WriteOnly | QFile::Truncate;

    if (!file.open(mode)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot create file '%1'.").arg(entry.niceName()));
      return false;
    }

    quint16 sector = entry.firstSector;
    for (uint n = entry.size / (m_image->geometry().bytesPerSector() - 3); n > 0 && sector != 0; n--) {
      QByteArray data;
      if (!m_image->readSector(sector, data)) {
        QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot read '%1': %2").arg(entry.niceName(), tr("Sector read failed.")));
        return false;
      }
      if (!(entry.attributes & AtariDirEntry::MyDos)) {
        int fileNo = (quint8) data.at(data.count() - 3) >> 2;
        if (fileNo != entry.no) {
          QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot read '%1': %2").arg(entry.niceName(), tr("File number mismatch.")));
          return false;
        }
        sector = ((quint8) data.at(data.count() - 3) & 0x03) * 256 + (quint8) data.at(data.count() - 2);
      } else {
        sector = (quint8) data.at(data.count() - 3) * 256 + (quint8) data.at(data.count() - 2);
      }
      uint size = (quint8) data.at(data.count() - 1);
      if (!(entry.attributes & AtariDirEntry::Dos10)) {
        data.resize(size);
      } else {
        if (size & 128) {
          data.resize(size - 128);
        } else {
          data.resize(125);
        }
      }
      QByteArray nData;
      nData.clear();
      if (m_textConversion) {
        int j = 0;
        for (int i = 0; i < data.count(); i++) {
#ifdef Q_OS_WIN
          if (data.at(i) == '\r') {
            // ignore carriage return
          } else if (data.at(i) == '\n') {
            nData[j] = '\x9b';
            j += 1;
          } else if (data.at(i) == '\x9b') {
            nData[j] = '\r';
            j += 1;
            nData[j] = '\n';
            j += 1;
          } else {
            nData[j] = data.at(i);
            j += 1;
          }
#else
          if (data.at(i) == '\n') {
            nData[j] = '\x9b';
            j += 1;
          } else if (data.at(i) == '\x9b') {
            nData[j] = '\n';
            j += 1;
          } else {
            nData[j] = data.at(i);
            j += 1;
          }
#endif
        }
      } else {
        nData = data;
      }
      if (file.write(nData) != nData.count()) {
        QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot write to '%1': %2").arg(file.fileName(), file.errorString()));
        return false;
      }
    }

    return true;
  }

  AtariDirEntry Dos10FileSystem::insert(quint16 dir, const QString &name) {
    AtariDirEntry result;
    QByteArray dosEntry = findName(dir, name);
    if (dosEntry.isEmpty()) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Cannot find a suitable file name.")));
      return result;
    }
    int no = findFreeFileNo(dir);
    if (no < 0) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Directory is full.")));
      return result;
    }

    dosEntry.prepend(QByteArray(5, 0));

    QFile file(name);

    QFile::OpenMode mode;

    if (m_textConversion) {
      mode = QFile::ReadOnly | QFile::Text;
    } else {
      mode = QFile::ReadOnly;
    }

    if (!file.open(mode)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot open '%1': %2").arg(name, file.errorString()));
      return result;
    }

    if (file.size() > freeSpace()) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Disk is full.")));
      return result;
    }

    int dataSize = m_image->geometry().bytesPerSector() - 3;

    int sector, newSector;
    bool hiUsed = false;
    bool myDos = typeid(*this) == typeid(MyDosFileSystem)
       && (vtoc.at(0) > 2);

    int firstSector = findFreeSector(0);
    sector = firstSector;
    if (sector == 0) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Disk is full.")));
      return result;
    }
    allocateSector(sector);

    int sectorCount = 0;

    do {
      sectorCount++;
      if (sector > 719) {
        hiUsed = true;
      }
      int size = dataSize;
      if (file.bytesAvailable() < size) {
        size = file.bytesAvailable();
      }
      QByteArray data = file.read(size);

      // Only in binary mode data.count must be = size, so if the file opened in text mode don't display an error
      // The message below should normally never display unless there is a system error.
      if (data.count() < size) {
        if (mode == (QFile::ReadOnly | QFile::Text)) {
          size = data.count();
        } else {
          QMessageBox::critical(m_image->editDialog(), tr("File system error"), tr("Number of bytes (%1) read from '%2' is not equal to expected data size of (%3)").arg(data.count()).arg(name).arg(size));
          return result;
        }
      }
      if (!file.atEnd()) {
        newSector = findFreeSector(sector + 1);
        allocateSector(newSector);
      } else {
        newSector = 0;
      }
      if (m_textConversion) {
        for (int i = 0; i < data.count(); i++) {
          if (data.at(i) == '\n') {
            data[i] = '\x9b';
          } else if (data.at(i) == '\x9b') {
            data[i] = '\n';
          }
        }
      }
      data.resize(dataSize + 3);
      if (myDos) {
        data[dataSize] = newSector / 256;
        data[dataSize + 1] = newSector % 256;
        data[dataSize + 2] = size;
      } else {
        data[dataSize] = (newSector / 256) | (no * 4);
        data[dataSize + 1] = newSector % 256;
        // TODO WHY?
        // Always true if (fileSystemCode() != 0) {
          data[dataSize + 2] = size;
        /*} else {
          if (file.atEnd()) {
            data[dataSize + 2] = size | 128;
          } else {
            data[dataSize + 2] = 0;
          }
        }*/
      }
      if (!m_image->writeSector(sector, data)) {
        QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Sector write failed.")));
        return result;
      }
      sector = newSector;
    } while (!file.atEnd());

    quint16 dirsec = dir + no / 8;
    int start = (no % 8) * 16;
    QByteArray data;
    if (!image()->readSector(dirsec, data)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Sector read failed.")));
      return result;
    }

    quint8 flag;

    if (myDos) {
      flag = 0x47;
    } else if (typeid(*this) == typeid(Dos20FileSystem) && hiUsed) {
      flag = 0x03;
    } else {
      flag = 0x42;
    }

    dosEntry[0] = flag;
    dosEntry[1] = sectorCount % 256;
    dosEntry[2] = sectorCount / 256;
    dosEntry[3] = firstSector % 256;
    dosEntry[4] = firstSector / 256;

    data.replace(start, 16, dosEntry);

    if (!image()->writeSector(dirsec, data)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Sector write failed.")));
      return result;
    }

    writeBitmap();
    result.makeFromAtariDosEntry(dosEntry, no, dir, m_image->geometry().bytesPerSector() == 256);

    return result;
  }

  AtariDirEntry Dos10FileSystem::makeDir(quint16 dir, const QString &name) {
    AtariDirEntry result;
    QByteArray dosEntry = findName(dir, name);
    if (dosEntry.isEmpty()) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Cannot find a suitable file name.")));
      return result;
    }
    int no = findFreeFileNo(dir);
    if (no < 0) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Directory is full.")));
      return result;
    }

    dosEntry.prepend(QByteArray(5, 0));
    dosEntry[0] = 0x10;
    dosEntry[1] = 8;

    int first = findFreeSector(369);
    int sector = first;
    if (sector == 0) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Disk is full.")));
      return result;
    }
    bool found = false;
    do {
      if (sectorIsFree(sector + 1) &&
          sectorIsFree(sector + 2) &&
          sectorIsFree(sector + 3) &&
          sectorIsFree(sector + 4) &&
          sectorIsFree(sector + 5) &&
          sectorIsFree(sector + 6) &&
          sectorIsFree(sector + 7)) {
        found = true;
        break;
      }
      sector = findFreeSector(sector + 1);
    } while (sector != first);

    if (!found) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Disk is full.")));
      return result;
    }

    QByteArray empty(m_image->geometry().bytesPerSector(), 0);

    for (int i = sector; i < sector + 8; i++) {
      allocateSector(i);
      if (!m_image->writeSector(i, empty)) {
        QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Sector write failed.")));
        return result;
      }
    }

    dosEntry[3] = sector % 256;
    dosEntry[4] = sector / 256;

    quint16 dirsec = dir + no / 8;
    int start = (no % 8) * 16;
    QByteArray data;
    if (!image()->readSector(dirsec, data)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Sector read failed.")));
      return result;
    }

    data.replace(start, 16, dosEntry);

    if (!image()->writeSector(dirsec, data)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot insert '%1': %2").arg(name, tr("Sector write failed.")));
      return result;
    }

    writeBitmap();
    result.makeFromAtariDosEntry(dosEntry, no, dir, m_image->geometry().bytesPerSector() == 256);

    return result;
  }

  bool Dos10FileSystem::erase(const AtariDirEntry &entry) {
    quint16 dirsec = entry.dir + entry.no / 8;
    int start = (entry.no % 8) * 16;
    QByteArray data;
    if (!image()->readSector(dirsec, data)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot delete '%1': %2").arg(entry.niceName(), tr("Sector read failed.")));
      return false;
    }
    data[start] = 0x80;
    if (!image()->writeSector(dirsec, data)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot delete '%1': %2").arg(entry.niceName(), tr("Sector write failed.")));
      return false;
    }

    quint16 sector = entry.firstSector;
    for (uint n = entry.size / (m_image->geometry().bytesPerSector() - 3); n > 0 && sector != 0; n--) {
      freeSector(sector);
      //QByteArray data;
      data.clear();
      if (!m_image->readSector(sector, data)) {
        QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot delete '%1': %2").arg(entry.niceName(), tr("Sector read failed.")));
        return false;
      }
      if (!(entry.attributes & AtariDirEntry::MyDos)) {
        int fileNo = (quint8) data.at(data.count() - 3) >> 2;
        if (fileNo != entry.no) {
          QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot delete '%1': %2").arg(entry.niceName(), tr("File number mismatch.")));
          return false;
        }
        sector = ((quint8) data.at(data.count() - 3) & 0x03) * 256 + (quint8) data.at(data.count() - 2);
      } else {
        sector = (quint8) data.at(data.count() - 3) * 256 + (quint8) data.at(data.count() - 2);
      }
    }

    if (!writeBitmap()) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot delete '%1': %2").arg(entry.niceName(), tr("Bitmap write failed.")));
      return false;
    }
    return true;
  }

  bool Dos10FileSystem::rename(const AtariDirEntry &entry, const QByteArray &name) {
    quint16 dirsec = entry.dir + entry.no / 8;
    int start = (entry.no % 8) * 16;
    QByteArray data;
    if (!image()->readSector(dirsec, data)) {
      return false;
    }
    data.replace(start + 5, 11, name);
    if (!image()->writeSector(dirsec, data)) {
      return false;
    }
    return true;
  }

  int Dos10FileSystem::findFreeFileNo(quint16 dir) {
    int no = 0;
    for (int sector = dir; sector < dir + 8; sector++) {
      QByteArray data;
      if (!m_image->readSector(sector, data)) {
        return -1;
      }
      for (int i = 0; i < 128; i += 16) {
        int f = (quint8) data.at(i);
        if (f == 0x80 || f == 0x00) {
          return no;
        }
        no++;
      }
    }
    return -1;
  }

  bool Dos10FileSystem::writeBitmap() {
    QByteArray data;
    if (!m_image->readSector(360, data)) {
      return false;
    }
    data.replace(10, 90, bitmap);
    data[3] = m_freeSectors % 256;
    data[4] = m_freeSectors / 256;
    return m_image->writeSector(360, data);
  }

  bool Dos10FileSystem::removeDir(const AtariDirEntry &entry) {
    quint16 dirsec = entry.dir + entry.no / 8;
    int start = (entry.no % 8) * 16;
    QByteArray data;
    if (!image()->readSector(dirsec, data)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot delete '%1': %2").arg(entry.niceName(), tr("Sector read failed.")));
      return false;
    }
    data[start] = 0x80;
    if (!image()->writeSector(dirsec, data)) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot delete '%1': %2").arg(entry.niceName(), tr("Sector write failed.")));
      return false;
    }

    for (int s = entry.firstSector; s < entry.firstSector + 8; s++) {
      freeSector(s);
    }

    if (!writeBitmap()) {
      QMessageBox::critical(m_image->editDialog(), tr("Atari file system error"), tr("Cannot delete '%1': %2").arg(entry.niceName(), tr("Bitmap write failed.")));
      return false;
    }

    return true;
  }

}// namespace Filesystems