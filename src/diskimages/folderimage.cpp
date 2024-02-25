/*
 * folderimage.cpp
 *
 * Copyright 2015 Joseph Zatarski
 * Copyright 2017 josch1710
 * Copyright 2017 blind
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "diskimages/folderimage.h"
#include "respeqtsettings.h"

#include <QFileInfoList>

namespace DiskImages {
  // CIRCULAR SECTORS USED FOR SERVING FILES FROM FOLDER IMAGES
  // ==========================================================
  // Circular sectors per file logic utilizes all sectors from 433 to 1023 for a total of 591 sectors.
  // Sector number will cycle back to 433 once it hits 1023, and this cycle will repeat until the entire file is read.
  // The same pool of sectors are used for every file in the Folder Image.
  // First sector number of each file however is selected from a different pool of (369-432) so that they are unique for each file.
  // This allows for dynamic calculation of the Atari file number within the code.
  // Sector numbers (5, 6, 32-134) are reserved for SpartaDos boot process.

  static QString g_respeQtAppPath;
  static bool g_disablePicoHiSpeed;

  FolderImage::FolderImage(SioWorkerPtr worker, bool)
      : DiskImage(worker, false) {}

  FolderImage::~FolderImage() {
    atariFiles.clear();
  }

  void FolderImage::close() {
    atariFiles.clear();
  }

  bool FolderImage::format(const DiskGeometry &) {
    return false;
  }

  // Return the long file name of a short Atari file name from a given (last mounted) Folder Image
  __attribute__((unused)) QString FolderImage::longName(QString &lastMountedFolder, QString &atariFileName) {
    if (FolderImage::open(lastMountedFolder)) {
      for (auto file: atariFiles) {
        if (file.atariName + "." + file.atariExt == atariFileName)
          return file.longName;
      }
    }
    return nullptr;
  }

  void FolderImage::setMaxEntries(int limitEntries) {
    auto old = mMaxEntries;
    mMaxEntries = limitEntries;
    emit maxEntriesChanged(old, mMaxEntries);
  }

  void FolderImage::buildDirectory() {
    QFileInfoList infos = dir.entryInfoList(QDir::Files, QDir::Name);
    QString name, longName;
    QString ext;
    QList<QString> knownNames, duplicateNames;

    atariFiles.clear();
    auto count = infos.count();
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"
    if (mMaxEntries > 0 && count > mMaxEntries)
      count = mMaxEntries;
#pragma clang diagnostic pop

    for (auto info: infos) {
      longName = info.completeBaseName();
      name = longName.toUpper();
      if (RespeqtSettings::instance()->filterUnderscore()) {
        name.remove(QRegularExpression("[^A-Z0-9]"));
      } else {
        name.remove(QRegularExpression("[^A-Z0-9_]"));
      }
      name = name.left(8);
      if (name.isEmpty()) {
        name = "BADNAME";
      }
      longName += "." + info.suffix();
      ext = info.suffix().toUpper();
      if (RespeqtSettings::instance()->filterUnderscore()) {
        ext.remove(QRegularExpression("[^A-Z0-9]"));
      } else {
        ext.remove(QRegularExpression("[^A-Z0-9_]"));
      }
      ext = ext.left(3);

      // Check, whether we have to shorten the filename because of duplicates, and record them.
      auto completeName = QString("%1.%2").arg(name, ext);
      if (!knownNames.contains(completeName))
        knownNames.push_back(completeName);
      else
        duplicateNames.push_back(completeName);

      AtariFile file;
      file.exists = true;
      file.original = info;
      file.atariName = name;
      file.longName = longName;
      file.atariExt = ext;
      file.lastSector = 0;
      file.pos = 0;
      file.sectPass = 0;
      atariFiles.push_back(file);
    }

    // Process duplicate file names
    for (auto duplicate: duplicateNames) {
      auto i = 1;
      for (auto j = 0; j < atariFiles.count(); j++) {
        auto file = atariFiles[j];
        auto completeName = QString("%1.%2").arg(file.atariName, file.atariExt);
        if (QString::compare(duplicate, completeName) != 0)
          continue;  // Not a duplicate
        if (i == 1) {// First filename doesn't need to be fixed.
          i++;
          continue;
        }
        auto digits = i / 10;// Integer division gives us the count of digits, we need to cut.
        if (digits > 7)      // We remove the file, because it will get shortened too much.
        {
          atariFiles[j].exists = false;
        } else {
          atariFiles[j].atariName = QString("%1%2").arg(file.atariName.left(8 - digits)).arg(i);
        }
        i++;
      }
    }

    if (atariFiles.count() < infos.count()) {
      qWarning() << "!w" << tr("Cannot mirror %1 of %2 files in '%3': Atari directory is full.").arg(infos.count() - atariFiles.count()).arg(infos.count()).arg(dir.path());
    }
  }

  bool FolderImage::open(const QString &fileName) {
    if (dir.exists(fileName)) {
      dir.setPath(fileName);

      buildDirectory();

      m_originalFileName = fileName;
      m_geometry.initialize(false, 40, 26, 128);
      m_newGeometry.initialize(m_geometry);
      m_isReadOnly = true;
      m_isModified = false;
      m_isUnmodifiable = true;
      m_isReady = true;
      return true;
    } else {
      return false;
    }
  }

  bool FolderImage::readSector(quint16 sector, QByteArray &data) {
    /* Boot */

    QFile boot(dir.path() + "/$boot.bin");
    data = QByteArray(128, 0);
    int bootFileSector;

    if (sector == 1) {
      if (!boot.open(QFile::ReadOnly)) {
        data[1] = 0x01;
        data[3] = 0x07;
        data[4] = 0x40;
        data[5] = 0x15;
        data[6] = 0x4c;
        data[7] = 0x14;
        data[8] = 0x07;   // JMP 0x0714
        data[0x14] = 0x38;// SEC
        data[0x15] = 0x60;// RTS
      } else {
        data = boot.read(128);
        buildDirectory();
        for (int i = 0; i < mMaxEntries; i++) {
          // AtariDOS, MyDos, SmartDOS  and DosXL
          if (atariFiles[i].longName.toUpper() == "DOS.SYS") {
            bootFileSector = 369 + i;
            data[15] = bootFileSector % 256;
            data[16] = bootFileSector / 256;
            break;
          }
          // MyPicoDOS
          if (atariFiles[i].longName.toUpper() == "PICODOS.SYS") {
            bootFileSector = 369 + i;
            if (g_disablePicoHiSpeed) {
              data[15] = 0;
              QByteArray speed;
              boot.open(QFile::ReadWrite);
              boot.seek(15);
              speed = boot.read(1);
              speed[0] = '\x30';
              boot.seek(15);
              boot.write(speed);
              boot.close();
            }
            data[9] = bootFileSector % 256;
            data[10] = bootFileSector / 256;
            // Create the piconame.txt file
            QFile picoName(dir.path() + "/piconame.txt");
            picoName.open(QFile::WriteOnly);
            QByteArray nameLine;
            nameLine.append(QString(dir.dirName() + '\x9B').toLatin1());
            picoName.write(nameLine);
            for (int j = 0; j < mMaxEntries; j++) {
              if (atariFiles[j].exists) {
                if (atariFiles[j].longName != "$boot.bin") {
                  nameLine.clear();
                  nameLine.append(atariFiles[j].atariName.toLatin1());
                  QByteArray space;
                  int size;
                  size = atariFiles[j].atariName.size();
                  for (int k = 0; k <= 8 - size - 1; k++) {
                    space[k] = '\x20';
                  }
                  nameLine.append(space);
                  nameLine.append(atariFiles[j].atariExt.toLatin1());
                  nameLine.append('\x20');
                  nameLine.append(atariFiles[j].longName.mid(0, atariFiles[j].longName.indexOf(".", -1) - 1).toLatin1());
                  nameLine.append('\x9B');
                  picoName.write(nameLine);
                }
              } else {
                picoName.close();
                break;
              }
            }
            break;
          }
          // SpartaDOS, force it to change to AtariDOS format after the boot
          if (atariFiles[i].longName.toUpper() == "X32.DOS") {
            QFile x32Dos(dir.path() + "/x32.dos");
            x32Dos.open(QFile::ReadOnly);
            QByteArray flag;
            flag = x32Dos.readAll();
            if (flag[0] == '\xFF') {
              flag[0] = '\x00';
              data[1] = 0x01;
              data[3] = 0x07;
              data[4] = 0x40;
              data[5] = 0x15;
              data[6] = 0x4c;
              data[7] = 0x14;
              data[8] = 0x07;
              data[0x14] = 0x38;
              data[0x15] = 0x60;
            }
            break;
          }
        }
      }
      return true;
    }
    if (sector == 2) {
      boot.open(QFile::ReadOnly);
      boot.seek(128);
      data = boot.read(128);
      return true;
    }
    if (sector == 3) {
      boot.open(QFile::ReadOnly);
      boot.seek(256);
      data = boot.read(128);
      return true;
    }
    // SpartaDOS Boot
    if ((sector >= 32 && sector <= 134) ||
        sector == 5 || sector == 6) {
      boot.open(QFile::ReadOnly);
      boot.seek((sector - 1) * 128);
      data = boot.read(128);
      if (sector == 134) {
        QFile x32Dos(dir.path() + "/x32.dos");
        x32Dos.open(QFile::ReadWrite);
        QByteArray flag;
        flag = x32Dos.readAll();
        if (flag[0] == '\x00') {
          flag[0] = '\xFF';
          x32Dos.seek(0);
          x32Dos.write(flag);
          x32Dos.close();
        }
      }
      return true;
    }

    /* VTOC */
    if (sector == 360) {
      data = QByteArray(128, 0);
      data[0] = 2;
      data[1] = uint8_t(1010 % 256);
      data[2] = 1010 / 256;
      data[10] = 0x7F;
      for (int i = 11; i < 100; i++) {
        data[i] = 0xff;
      }
      return true;
    }

    /* Directory sectors */
    if (sector >= 361 && sector <= 368) {
      if (sector == 361) {
        buildDirectory();
      }
      data.resize(0);
      for (int i = (sector - 361) * 8; i < (sector - 360) * 8; i++) {
        QByteArray entry;
        if (i >= atariFiles.count() || !atariFiles[i].exists) {
          entry = QByteArray(16, 0);
        } else {
          entry = "";
          entry[0] = 0x42;
          QFileInfo info = atariFiles[i].original;

          int size = (info.size() + 124) / 125;
          if (size > 999) {
            size = 999;
          }
          entry[1] = size % 256;
          entry[2] = size / 256;
          int first = 369 + i;
          entry[3] = first % 256;
          entry[4] = first / 256;
          entry += atariFiles[i].atariName.toLatin1();
          while (entry.count() < 13) {
            entry += 32;
          }
          entry += atariFiles[i].atariExt.toLatin1();
          while (entry.count() < 16) {
            entry += 32;
          }
        }
        data += entry;
      }
      return true;
    }

    /* Data sectors */

    /* First sector of the file */
    int size, next;
    if (sector >= 369 && sector <= 432) {
      atariFileNo = sector - 369;
      if (!atariFiles[atariFileNo].exists) {
        data = QByteArray(128, 0);
        return true;
      }
      QFile file(atariFiles[atariFileNo].original.absoluteFilePath());
      file.open(QFile::ReadOnly);
      data = file.read(125);
      size = data.size();
      data.resize(128);
      if (file.atEnd()) {
        next = 0;
      } else {
        next = 433;
      }
      data[125] = (atariFileNo * 4) | (next / 256);
      data[126] = next % 256;
      data[127] = size;
      return true;
    }

    /* Rest of the file sectors */
    if ((sector >= 433 && sector <= 1023)) {
      QFile file(atariFiles[atariFileNo].original.absoluteFilePath());
      file.open(QFile::ReadOnly);
      atariFiles[atariFileNo].pos = (125 + ((sector - 433) * 125)) + (atariFiles[atariFileNo].sectPass * 73875);
      file.seek(atariFiles[atariFileNo].pos);
      data = file.read(125);
      next = sector + 1;
      if (sector == 1023) {
        next = 433;
        atariFiles[atariFileNo].sectPass += 1;
      }
      size = data.size();
      data.resize(128);
      atariFiles[atariFileNo].lastSector = sector;
      if (file.atEnd()) {
        next = 0;
      }
      data[125] = (atariFileNo * 4) | (next / 256);
      data[126] = next % 256;
      data[127] = size;
      return true;
    }

    /* Any other sector */

    data = QByteArray(128, 0);
    return true;
  }

  bool FolderImage::writeSector(quint16, const QByteArray &) {
    return false;
  }

  bool FolderImage::openImage(const QString &fileName) {
    return true;
  }
  bool FolderImage::resetTrack(quint16 aux) {
    return false;
  }
  bool FolderImage::writeHappyTrack(int trackNumber, bool happy1050) {
    return false;
  }
  QByteArray FolderImage::readHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) {
    return QByteArray();
  }
  bool FolderImage::writeHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) {
    return false;
  }
  bool FolderImage::writeSectorExtended(int bitNumber, quint8 dataType, quint8 trackNumber, quint8 sideNumber, quint8 sectorNumber, quint8 sectorSize, const QByteArray &data, bool crcError, int weakOffset) {
    return false;
  }
  quint8 FolderImage::writeSectorHeader(quint8 dataSize, quint16 sectorSlot, quint8 postDataCrc, quint8 preIDField, quint8 postIDCrc, quint8 track, quint8 index, quint8 nextSector) {
    return 0;
  }

  bool FolderImage::findMappingInTrack(int nbSectors, QByteArray &mapping) {
    return false;
  }
}
