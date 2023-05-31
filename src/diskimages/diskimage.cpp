/*
 * diskimage.cpp
 *
 * Copyright 2015 Joseph Zatarski
 * Copyright 2016 TheMontezuma
 * Copyright 2018 Eric BACHER
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "diskimages/diskimage.h"
#include "diskimages/diskimageatx.h"
#include "diskimages/diskimagepro.h"
#include "diskimages/diskimagefactory.h"
#include "zlib.h"

#include "diskeditdialog.h"
#include "filesystems/atarifilesystem.h"
#include "respeqtsettings.h"
#include <QDir>
#include <QFileInfo>
#include <algorithm>

/* DiskImage */
namespace DiskImages {
  DiskImage::DiskImage(SioWorkerPtr worker, bool gzipped)
      : SioDevice(worker),
        m_gzipped(gzipped) {
    m_editDialog = 0;
    m_displayTransmission = false;
    m_dumpDataFrame = false;
    m_displayTrackLayout = false;
    m_wd1771Status = 0xFF;
    m_isLeverOpen = false;
    m_isReady = false;
    m_timer.start();
    m_conversionInProgress = false;
    m_translatorAutomaticDetection = false;
    m_OSBMode = false;
    m_toolDiskMode = false;
    m_translatorDisk.reset();
    m_toolDisk.reset();
  }

  DiskImage::~DiskImage() {
    closeTranslator();
    closeToolDisk();
    if (isOpen()) {
      DiskImage::close();
    }
  }

  void DiskImage::closeTranslator() {
    if (m_translatorDisk != nullptr) {
      m_translatorDisk->close();
      m_translatorDisk.reset();
    }
  }

  void DiskImage::closeToolDisk() {
    if (m_toolDisk != nullptr) {
      m_toolDisk->close();
      m_toolDisk.reset();
    }
  }

  QString DiskImage::originalFileName() const {
    if (m_board.isToolDiskActive()) {
      return m_toolDiskImagePath;
    }
    return m_originalFileName;
  }

  QString DiskImage::description() const {
    if (m_board.isToolDiskActive()) {
      if (m_activateChipModeWithTool || m_board.isChipOpen()) {
        return QString(tr("Favorite tool disk")).append(" - ").append(tr("CHIP mode"));
      } else if (m_activateHappyModeWithTool || m_board.isHappyEnabled()) {
        return QString(tr("Favorite tool disk")).append(" - ").append(tr("HAPPY mode"));
      } else {
        return QString(tr("Favorite tool disk"));
      }
    }
    QString sides;
    if (m_numberOfSides > 1) {
      sides.append(" - ").append(tr("Image %1/%2").arg(m_currentSide).arg(m_numberOfSides));
    }
    if (m_board.isChipOpen()) {
      return QString(m_geometry.humanReadable()).append(" - ").append(tr("CHIP mode")).append(sides);
    } else if (m_board.isHappyEnabled()) {
      return QString(m_geometry.humanReadable()).append(" - ").append(tr("HAPPY mode")).append(sides);
    } else {
      return m_geometry.humanReadable().append(sides);
    }
  }

  QString DiskImage::getNextSideLabel() {
    int nextSide = m_currentSide + 1;
    if (nextSide > m_numberOfSides) {
      nextSide = 1;
    }
    return QString(tr("Load image %1 of %2:\n%3").arg(nextSide).arg(m_numberOfSides).arg(m_nextSideFilename));
  }

  void DiskImage::refreshNewGeometry() {
    m_newGeometry.initialize(m_geometry);
  }

  void DiskImage::setChipMode(bool enable) {
    m_board.setChipOpen(enable);
    m_board.setLastArchiverUploadCrc16(0);
    if (m_board.isChipOpen()) {
      m_board.setHappyEnabled(false);
    }
    emit statusChanged(m_deviceNo);
  }

  void DiskImage::setHappyMode(bool enable) {
    m_board.setHappyEnabled(enable);
    m_board.setLastHappyUploadCrc16(0);
    if (m_board.isHappyEnabled()) {
      m_board.setChipOpen(false);
      if (m_board.m_happyRam.size() == 0) {
        m_board.m_happyRam.reserve(6144);
        for (unsigned int i = 0; i < 6144; i++) {
          m_board.m_happyRam[i] = 0;
        }
        m_board.m_happyRam[0] = 0x80;
        for (unsigned int i = 0; i < sizeof(HAPPY_INITIAL_SECTOR_LIST); i++) {
          m_board.m_happyRam[0x280 + i] = HAPPY_INITIAL_SECTOR_LIST[i];
        }
      }
    } else {
      m_board.m_happyRam[0] = 0x80;
      m_board.setHappy1050(false);
    }
    emit statusChanged(m_deviceNo);
  }

  void DiskImage::setOSBMode(bool enable) {
    m_OSBMode = enable;
    setTranslatorActive(true);
  }

  void DiskImage::setTranslatorActive(bool resetTranslatorState) {
    bool oldState = m_board.isTranslatorActive();
    bool enable = m_OSBMode && ((m_deviceNo == -1) || (m_deviceNo == 0x31));
    if (enable) {
      enable = translatorDiskImageAvailable();
    }
    if (enable && resetTranslatorState) {
      m_board.setTranslatorState(BOOT_STATE::NOT_BOOTED);
    }
    if (m_board.isTranslatorActive() != enable) {
      m_board.setTranslatorActive(enable);
    }
    if ((m_OSBMode != enable) || (m_board.isTranslatorActive() != oldState)) {
      m_OSBMode = enable;
      emit statusChanged(m_deviceNo);
    }
    if ((!enable) || (!m_board.isTranslatorActive())) {
      closeTranslator();
    }
  }

  bool DiskImage::translatorDiskImageAvailable() {
    if (m_translatorDiskImagePath.isEmpty()) {
      qWarning() << "!w" << tr("[%1] No Translator disk image defined. Please, check settings in menu Disk images>OS-B emulation.").arg(deviceName());
      return false;
    }
    auto translatorFile = new QFile(m_translatorDiskImagePath);
    if (!translatorFile->open(QFile::ReadOnly)) {
      delete translatorFile;
      qWarning() << "!w" << tr("[%1] Translator '%2' not found. Please, check settings in menu Disk images>OS-B emulation.").arg(deviceName(), m_translatorDiskImagePath);
      return false;
    }
    translatorFile->close();
    delete translatorFile;
    return true;
  }

  void DiskImage::setToolDiskMode(bool enable) {
    m_toolDiskMode = enable;
    setToolDiskActive();
  }

  void DiskImage::setToolDiskActive() {
    bool oldState = m_board.isToolDiskActive();
    bool enable = m_toolDiskMode && (m_deviceNo == 0x31);
    if (enable) {
      enable = toolDiskImageAvailable();
    }
    if (m_board.isToolDiskActive() != enable) {
      m_board.setToolDiskActive(enable);
    }
    if ((m_toolDiskMode != enable) || (oldState != m_board.isToolDiskActive())) {
      m_toolDiskMode = enable;
      emit statusChanged(m_deviceNo);
    }
    if ((!enable) || (!m_board.isToolDiskActive())) {
      closeToolDisk();
    }
  }

  bool DiskImage::toolDiskImageAvailable() {
    if (m_toolDiskImagePath.isEmpty()) {
      qWarning() << "!w" << tr("[%1] No tool disk image defined. Please, check settings in menu Disk images>Favorite tool disk.").arg(deviceName());
      return false;
    }
    auto toolDiskFile = new QFile(m_toolDiskImagePath);
    if (!toolDiskFile->open(QFile::ReadOnly)) {
      delete toolDiskFile;
      qWarning() << "!w" << tr("[%1] Tool disk '%2' not found. Please, check settings in menu Disk images>Favorite tool disk.").arg(deviceName(), m_toolDiskImagePath);
      return false;
    }
    toolDiskFile->close();
    delete toolDiskFile;
    return true;
  }

  void DiskImage::setDisplayTransmission(bool active) {
    m_displayTransmission = active;
  }

  void DiskImage::setSpyMode(bool enable) {
    m_dumpDataFrame = enable;
  }

  void DiskImage::setTrackLayout(bool enable) {
    m_displayTrackLayout = enable;
  }

  void DiskImage::setDisassembleUploadedCode(bool enable) {
    m_disassembleUploadedCode = enable;
  }

  void DiskImage::setTranslatorAutomaticDetection(bool enable) {
    m_translatorAutomaticDetection = enable;
  }

  void DiskImage::setTranslatorDiskImagePath(const QString &filename) {
    m_translatorDiskImagePath = filename;
    setTranslatorActive(false);
  }

  void DiskImage::setToolDiskImagePath(const QString &filename) {
    m_toolDiskImagePath = filename;
    setToolDiskActive();
  }

  void DiskImage::setActivateChipModeWithTool(bool activate) {
    m_activateChipModeWithTool = activate;
  }

  void DiskImage::setActivateHappyModeWithTool(bool activate) {
    m_activateHappyModeWithTool = activate;
  }

  __attribute__((unused)) void DiskImage::setLeverOpen(bool open) {
    if (m_isLeverOpen != open) {
      m_isLeverOpen = open;
      if (open) {
        qDebug() << "!n" << tr("[%1] Drive door lever open. Drive is no longer ready").arg(deviceName());
      } else {
        qDebug() << "!n" << tr("[%1] Drive door lever closed. Drive is now ready").arg(deviceName());
      }
    }
  }

  bool DiskImage::open(const QString &fileName) {
    m_currentSide = 1;
    m_numberOfSides = 1;
    m_nextSideFilename.clear();
    auto bResult = openImage(fileName);
    if (bResult) {
      QFileInfo fileInfo(fileName);
      QString baseName = fileInfo.completeBaseName();
      if ((m_translatorAutomaticDetection) && (baseName.contains("OS-B", Qt::CaseInsensitive))) {
        qDebug() << "!u" << tr("Translator '%1' activated").arg(m_translatorDiskImagePath);
        setOSBMode(true);
      }
      if (baseName.contains("Side", Qt::CaseInsensitive) || baseName.contains("Disk", Qt::CaseInsensitive)) {
        QStringList imageList;
        imageList << fileInfo.absoluteDir().absoluteFilePath(fileInfo.fileName());
        QStringList images = fileInfo.absoluteDir().entryList(
          QStringList()  << "*.xfd" << "*.XFD" << "*.atr" << "*.ATR" << "*.pro" << "*.PRO" << "*.atx" << "*.ATX",
          QDir::Files);
        foreach (QString otherFileName, images) {
          QFileInfo otherFileInfo(otherFileName);
          QString otherBaseName = otherFileInfo.completeBaseName();
          if (otherBaseName.contains("Side", Qt::CaseInsensitive) || otherBaseName.contains("Disk", Qt::CaseInsensitive)) {
            if (sameSoftware(baseName, otherBaseName)) {
              imageList << fileInfo.absoluteDir().absoluteFilePath(otherFileInfo.fileName());
            }
          }
        }
        if (imageList.size() > 1) {
          m_numberOfSides = imageList.size();
          std::sort(imageList.begin(), imageList.end(), std::less<QString>());
          int currentIndex = 0;
          foreach (QString otherFileName, imageList) {
            if (otherFileName == fileName) {
              break;
            } else {
              currentIndex++;
            }
          }
          m_currentSide = currentIndex + 1;
          int nextIndex = m_currentSide >= m_numberOfSides ? 0 : m_currentSide;
          m_nextSideFilename.append(imageList.at(nextIndex));
          QFileInfo nextFileInfo(m_nextSideFilename);
          qDebug() << "!u" << tr("Image is %1 of %2. Next image will be %3").arg(currentIndex + 1).arg(m_numberOfSides).arg(nextFileInfo.fileName());
        }
      }
    }
    return bResult;
  }

  bool DiskImage::sameSoftware(const QString &fileName, const QString &otherFileName) {
    int same = 0;
    int minLength = fileName.length() < otherFileName.length() ? fileName.length() : otherFileName.length();
    QChar c1, c2;
    for (int i = 0; i < minLength; i++) {
      if (fileName.data()[i] == otherFileName.data()[i]) {
        same++;
      } else {
        c1 = fileName.data()[i];
        c2 = otherFileName.data()[i];
        break;
      }
    }
    if (same == minLength) {
      return false;
    }
    if (same < 7) {
      return false;
    }
    if (c1 >= 'A' && c1 <= 'H') {
      if (c2 < 'A' || c2 > 'H') {
        return false;
      }
    }
    if (c1 >= 'a' && c1 <= 'h') {
      if (c2 < 'a' || c2 > 'h') {
        return false;
      }
    }
    if (c1 >= '1' && c1 <= '8') {
      if (c2 < '1' || c2 > '8') {
        return false;
      }
    }
    QString samePart = fileName.left(same);
    if ((!samePart.contains("Side", Qt::CaseInsensitive)) && (!samePart.contains("Disk", Qt::CaseInsensitive))) {
      return false;
    }
    return true;
  }

bool DiskImage::saveAs(const QString &fileName) {
    m_currentSide = 1;
    m_numberOfSides = 1;
    m_nextSideFilename.clear();

    auto newImage = DiskImageFactory::instance()->createDiskImage(fileName, sio);
    newImage->saveImageAs();
    return false;
  }

  void DiskImage::reopen() {
    close();
    open(m_originalFileName);
  }

  void DiskImage::close() {
    closeTranslator();
    closeToolDisk();
    m_currentSide = 1;
    m_numberOfSides = 1;
    m_nextSideFilename.clear();
    if (m_editDialog) {
      delete m_editDialog;
    }
    if (false /* simplediskimage*/) {
      file.close();
    }
    setReady(false);
  }

  void DiskImage::setReady(bool bReady) {
    m_isReady = bReady;
  }

  bool DiskImage::executeArchiverCode(quint16 aux, QByteArray &data) {
    bool ok = true;
    if (((quint8) data[0] == 0x18) && ((quint8) data[1] == 0x60)) {// Super Archiver 3.12 command to check if chip is open
      qDebug() << "!u" << tr("[%1] Uploaded code is: Check if drive is a Super Archiver").arg(deviceName());
    } else {
      // compute CRC16 and save it to know what data should be returned by Read Memory command
      Crc16 crc16;
      crc16.Reset();
      unsigned short firstDataPartCrc16 = 0;
      for (int j = 0; j < data.size(); j++) {
        crc16.Add((unsigned char) (data.at(j)));
        if (j == 12) {
          // when SK+ option is on in Super Archiver, an Execute Code command is sent with code and data.
          // The code is in the first 13 bytes and the data is at offset 128 for Super Archiver 3.03
          // and 13 (just after the code) for Super Archiver 3.02 and 3.12.
          // The track data looks like the data sent in the Write Track command (sector list, fill bytes, gap sizes,...).
          // So the crc in this specific case should be computed only on the first 13 bytes.
          firstDataPartCrc16 = crc16.GetCrc();
        }
      }
      m_board.setLastArchiverUploadCrc16(crc16.GetCrc());
      if (m_board.getLastArchiverUploadCrc16() == 0xFD2E) {
        qDebug() << "!u" << tr("[%1] Uploaded code is: Speed check").arg(deviceName());
      } else if ((m_board.getLastArchiverUploadCrc16() == 0x61F6) || (m_board.getLastArchiverUploadCrc16() == 0xBFE7)) {
        qDebug() << "!u" << tr("[%1] Uploaded code is: Diagnostic").arg(deviceName());
      } else if ((m_board.getLastArchiverUploadCrc16() == 0x16BB) || (m_board.getLastArchiverUploadCrc16() == 0x7430)) {
        qDebug() << "!u" << tr("[%1] Uploaded code is: End of diagnostic").arg(deviceName());
      } else if (m_board.getLastArchiverUploadCrc16() == 0xE3B5) {
        qDebug() << "!u" << tr("[%1] Uploaded code is: Read address marks for track $%2").arg(deviceName()).arg((aux & 0x3F), 2, 16, QChar('0'));
      } else if (m_board.getLastArchiverUploadCrc16() == 0x912F) {
        qDebug() << "!u" << tr("[%1] Uploaded code is: BitWriter clear memory").arg(deviceName());
      } else if (m_board.getLastArchiverUploadCrc16() == 0x01DA) {
        qDebug() << "!u" << tr("[%1] Uploaded code is: BitWriter read track $%2").arg(deviceName()).arg((aux & 0x3F), 2, 16, QChar('0'));
      } else if ((firstDataPartCrc16 == 0x9537) || (firstDataPartCrc16 == 0xBEAF) || (firstDataPartCrc16 == 0xD2A0)) {// Super Archiver 3.02, 3.03, 3.12 respectively
        m_board.setLastArchiverUploadCrc16(firstDataPartCrc16);
        int startOffset = ((int) data[3]) & 0xFF;// this byte contains the offset of the first track data byte
        m_board.m_trackData.clear();
        m_board.m_trackData.append(data.mid(startOffset, 128));
        qDebug() << "!u" << tr("[%1] Uploaded code is: Prepare track data at offset $%2").arg(deviceName()).arg(startOffset, 2, 16, QChar('0'));
      } else if ((firstDataPartCrc16 == 0x335F) || (firstDataPartCrc16 == 0x936E) || (firstDataPartCrc16 == 0x4B4B)) {
        qDebug() << "!u" << tr("[%1] Uploaded code is: Super Archiver open Chip").arg(deviceName());
      } else if (firstDataPartCrc16 == 0xB180) {
        qDebug() << "!u" << tr("[%1] Uploaded code is: BitWriter open Chip").arg(deviceName());
      } else if (firstDataPartCrc16 == 0x0E8E) {
        qDebug() << "!u" << tr("[%1] Uploaded code is: Super Archiver clear memory").arg(deviceName());
      } else {
        // when SK+ option is on in Archiver 3.03, an Execute Code command is sent with code and data.
        // The code begins at offset 48. The first part is the sector list of the track given in AUX1.
        // We have to build a sector list with the byte offsets for track AUX2 respecting skew alignment with AUX1.
        crc16.Reset();
        for (int j = 0x30; j < 0xDA; j++) {
          crc16.Add((unsigned char) (data.at(j)));
        }
        if ((crc16.GetCrc() == 0x603D) || (crc16.GetCrc() == 0xBAC2) || (crc16.GetCrc() == 0xDFFF)) {// Super Archiver 3.02, 3.03, 3.12 respectively
          m_board.setLastArchiverUploadCrc16(crc16.GetCrc());
          // 2 implementations: one expects the sector list (Super Archiver 3.02 and 3.03) and the second expects a timing (3.12)
          bool timingOnly = crc16.GetCrc() == 0xDFFF;
          if (timingOnly) {
            qDebug() << "!u" << tr("[%1] Uploaded code is: Skew alignment of track $%2 sector $%3 with track $%4 sector $%5").arg(deviceName()).arg(((aux >> 8) & 0xFF), 2, 16, QChar('0')).arg(data[3], 2, 16, QChar('0')).arg((aux & 0xFF), 2, 16, QChar('0')).arg(data[4], 2, 16, QChar('0'));
          } else {
            qDebug() << "!u" << tr("[%1] Uploaded code is: Skew alignment of track $%2 (%3 sectors) with track $%4 (%5 sectors)").arg(deviceName()).arg(((aux >> 8) & 0xFF), 2, 16, QChar('0')).arg(data[3], 2, 16, QChar('0')).arg((aux & 0xFF), 2, 16, QChar('0')).arg(data[4], 2, 16, QChar('0'));
          }
          ok = readSkewAlignment(aux, data, timingOnly);
        } else if ((crc16.GetCrc() == 0xAD1D) || (crc16.GetCrc() == 0x8A65) || (crc16.GetCrc() == 0x64B1)) {// Super Archiver 3.02, 3.03, 3.12 respectively
          m_board.setLastArchiverUploadCrc16(crc16.GetCrc());
          if (crc16.GetCrc() == 0x64B1) {
            quint16 timing = (((quint16) data[4]) & 0xFF) + ((((quint16) data[5]) << 8) & 0xFF00);
            qDebug() << "!u" << tr("[%1] Uploaded code is: Format track $%2 with skew alignment $%3 with track $%4").arg(deviceName()).arg(aux & 0xFF, 2, 16, QChar('0')).arg(timing, 4, 16, QChar('0')).arg(data[3] & 0x3F, 2, 16, QChar('0'));
          } else {
            qDebug() << "!u" << tr("[%1] Uploaded code is: Format track $%2 with skew alignment with track $%3").arg(deviceName()).arg(aux & 0xFF, 2, 16, QChar('0')).arg(data[3] & 0x3F, 2, 16, QChar('0'));
          }
          ok = writeTrackWithSkew(aux, data);
        } else {
          qWarning() << "!w" << tr("[%1] Data Crc16 is $%2. Command ignored").arg(deviceName()).arg(crc16.GetCrc(), 4, 16, QChar('0'));
        }
      }
    }
    return ok;
  }

  void DiskImage::readHappyTrack(int trackNumber, bool happy1050) {
    Crc16 crc;
    QByteArray data;
    readTrack(0xEA00 | (quint8) trackNumber, data, 256);
    quint8 nbSectors = data[0];
    int nbSectorsInTrack = sectorsInCurrentTrack();
    if (nbSectors > (nbSectorsInTrack + 1)) {
      nbSectors = nbSectorsInTrack + 1;
    }
    int srcOffset = 1;
    int startOffset;
    int dstOffset;
    if (happy1050) {
      startOffset = 0xD00;
      dstOffset = startOffset + 1;
      m_board.m_happyRam[dstOffset++] = 0x8D;
      m_board.m_happyRam[dstOffset++] = 0x7F;
    } else {
      startOffset = 0x300;
      dstOffset = startOffset + 1;
      m_board.m_happyRam[dstOffset++] = 0x0B;
      m_board.m_happyRam[dstOffset++] = 0xFE;
    }
    quint8 totalTiming = 0;
    for (quint8 sector = 0; sector < nbSectors; sector++) {
      // ignore short sectors
      if ((unsigned char) data[srcOffset + 4] >= 2) {
        crc.Reset();
        crc.Add(DISK_ID_ADDR_MARK);
        for (int i = 0; i < 4; i++) {
          m_board.m_happyRam[dstOffset++] = 0xFF - crc.Add((unsigned char) data[srcOffset++]);
        }
        m_board.m_happyRam[dstOffset++] = 0xFF - (unsigned char) ((crc.GetCrc() >> 8) & 0xFF);
        m_board.m_happyRam[dstOffset++] = 0xFF - (unsigned char) (crc.GetCrc() & 0xFF);
        m_board.m_happyRam[dstOffset++] = 0xFF;
        totalTiming += (quint8) data[srcOffset++];
        srcOffset++;
        if (happy1050) {
          m_board.m_happyRam[dstOffset++] = 0x7F - totalTiming;
        } else {
          m_board.m_happyRam[dstOffset++] = 0xFF - totalTiming;
        }
      } else {
        totalTiming += (quint8) data[srcOffset + 4];
        srcOffset += 6;
      }
    }
    m_board.m_happyRam[startOffset] = (quint8) ((dstOffset - 1) & 0xFF);
    for (int i = dstOffset; i < (startOffset + 0x100); i++) {
      m_board.m_happyRam[i] = 0;
    }
    if (!happy1050) {
      m_board.m_happyRam[0x28D] = (quint8) 0x43;
    }
  }

  QByteArray DiskImage::readHappySectors(int trackNumber, int afterSectorNumber, bool happy1050) {
    int baseOffset = happy1050 ? 0xC80 : 0x280;
    bool rev50 = happy1050 ? false : (quint8) m_board.m_happyRam[0x14D] == 0x18;
    quint8 fdcMask = rev50 ? 0x18 : m_board.m_happyRam[baseOffset + 0x01];
    quint8 nbSectors = m_board.m_happyRam[baseOffset + 0x0F];
    int start = (int) m_board.m_happyRam[baseOffset + 0x12];
    if (start < 0) {
      start = 0;
    }
    if (start > 35) {
      start = 35;
    }
    int nbRetry = 4;
    bool firstSector = true;
    do {

      // read all sectors requested by Happy
      int index = 0;
      for (int i = start; i >= 0; i--) {
        if (((0xFF - m_board.m_happyRam[baseOffset + 0x14 + i]) & fdcMask) != 0) {
          int sector = (int) ((0xFF - m_board.m_happyRam[baseOffset + 0x38 + i]) & 0xFF);
          int offset = baseOffset + 0x80 + ((i % 18) * 128);
          QByteArray data;
          int after = firstSector ? afterSectorNumber : 0;
          firstSector = false;
          readHappySectorAtPosition(trackNumber, sector, after, index, data);
          for (int j = 0; j < data.size(); j++) {
            m_board.m_happyRam[offset + j] = data[j];
          }
          m_board.m_happyRam[baseOffset + 0x14 + i] = m_wd1771Status;
          nbSectors--;
          if (nbSectors <= 0) {
            break;
          }
        }
      }

      // check that all sectors have been read
      nbSectors = 0;
      int nbSlots = 18;
      for (int i = start; i >= 0; i--) {
        if (((0xFF - m_board.m_happyRam[baseOffset + 0x14 + i]) & fdcMask) != 0) {
          nbSectors++;
          if ((m_board.m_happyRam[baseOffset + 0x5C + i] & 0x80) != 0) {
            int nbOtherSlots = 18;
            for (int j = start; j >= 0; j--) {
              if ((m_board.m_happyRam[baseOffset + 0x38 + i] == m_board.m_happyRam[baseOffset + 0x38 + j]) && (((0xFF - m_board.m_happyRam[baseOffset + 0x14 + j]) & fdcMask) == 0)) {
                m_board.m_happyRam[baseOffset + 0x14 + j] = 0xEF;
                /*
                            if (j >= *$0091) {
                                nbSectors++;
                            }
                            */
              }
              nbOtherSlots--;
              if (nbOtherSlots <= 0) {
                break;
              }
            }
          }
        }
        nbSlots--;
        if (nbSlots <= 0) {
          break;
        }
      }
      nbRetry--;
    } while ((nbSectors > 0) && (nbRetry > 0));
    if (!happy1050) {
      m_board.m_happyRam[0x293] = 0x03;
      m_board.m_happyRam[0x290] = 0x7F;
    }
    m_board.m_happyRam[baseOffset + 0x0F] = 0x00;

    QByteArray status = m_board.m_happyRam.mid(baseOffset, 128);
    return status;
  }

  int DiskImage::findNearestSpeed(int speed) {
    if (speed < 20000) {
      return 19200;
    }
    int lastSpeed = 300000;
    int bestSpeed = 19200;
    for (int divisor = 0; divisor <= 40; divisor++) {
      int newSpeed = sio->port()->divisorToBaud(divisor);
      if (speed == newSpeed) {
        bestSpeed = newSpeed;
        break;
      }
      if ((speed >= newSpeed) && (speed <= lastSpeed)) {
        bestSpeed = ((speed - newSpeed) > (lastSpeed - speed)) ? lastSpeed : newSpeed;
        break;
      }
      lastSpeed = newSpeed;
    }
    return bestSpeed;
  }

  void DiskImage::handleCommand(const quint8 command, const quint8 aux1, const quint8 aux2) {
    if ((!isReady()) || isLeverOpen()) {
      if ((command != 0x4E) && (command != 0x4F) && (command != 0x53)) {
        qWarning() << "!w" << tr("[%1] command: $%2, aux: $%3 ignored because the drive is not ready.").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
        writeCommandNak();
        return;
      }
    }
    if ((m_deviceNo == 0x31) && (command == 0x52)) {
      quint16 sector = aux1 + aux2 * 256;
      if (m_board.isChipOpen()) {
        sector &= 0x3FF;
      }
      if (m_board.isTranslatorActive()) {
        if (sector == 1) {
          if (m_board.getTranslatorState() == BOOT_STATE::NOT_BOOTED) {
            closeTranslator();

            m_translatorDisk = DiskImageFactory::instance()->createDiskImage(m_translatorDiskImagePath, sio);
            m_translatorDisk->setReadOnly(true);
            m_translatorDisk->setDeviceNo(0x31);
            m_translatorDisk->setDisplayTransmission(false);
            m_translatorDisk->setSpyMode(false);
            m_translatorDisk->setTrackLayout(false);
            m_translatorDisk->setDisassembleUploadedCode(false);
            m_translatorDisk->setTranslatorAutomaticDetection(false);
            m_translatorDisk->setReady(true);
            if (!m_translatorDisk->open(m_translatorDiskImagePath)) {
              m_board.setTranslatorActive(false);
            } else {
              m_board.setTranslatorState(BOOT_STATE::FIRST_SECTOR_1);
              qWarning() << "!i" << tr("[%1] Booting Translator '%2' first").arg(deviceName(), m_translatorDiskImagePath);
            }
          } else if (m_board.getTranslatorState() == BOOT_STATE::READ_OTHER_SECTOR) {
            m_board.setTranslatorState(BOOT_STATE::SECOND_SECTOR_1);
            m_board.setTranslatorActive(false);
            setTranslatorActive(false);
            qWarning() << "!i" << tr("[%1] Removing Translator to boot on '%2'").arg(deviceName(), m_originalFileName);
          }
        } else if ((sector != 1) && (m_board.getTranslatorState() == BOOT_STATE::FIRST_SECTOR_1)) {
          m_board.setTranslatorState(BOOT_STATE::READ_OTHER_SECTOR);
        }
        if (m_board.isTranslatorActive() && (m_translatorDisk != nullptr)) {
          m_translatorDisk->handleCommand(command, aux1, aux2);
          return;
        }
      } else if (m_board.isToolDiskActive()) {
/* TODO        if (m_toolDisk == nullptr) {
          m_toolDisk = new DiskImage(sio);
          m_toolDisk->setReadOnly(true);
          m_toolDisk->setDeviceNo(0x31);
          m_toolDisk->setDisplayTransmission(false);
          m_toolDisk->setSpyMode(false);
          m_toolDisk->setTrackLayout(false);
          m_toolDisk->setDisassembleUploadedCode(false);
          m_toolDisk->setTranslatorAutomaticDetection(false);
          m_toolDisk->setReady(true);
          if (m_activateChipModeWithTool) {
            setChipMode(true);
            m_toolDisk->setChipMode(true);
          } else if (m_activateHappyModeWithTool) {
            setHappyMode(true);
            m_toolDisk->setHappyMode(true);
          }
          FileTypes::FileType type = FileTypes::getFileType(m_toolDiskImagePath);
          if (!m_toolDisk->open(m_toolDiskImagePath, type)) {
            m_board.setToolDiskActive(false);
          } else {
            qWarning() << "!i" << tr("[%1] Booting tool disk '%2' first").arg(deviceName(), m_toolDiskImagePath);
          }
        } */
        if (m_board.isToolDiskActive() && (m_toolDisk != nullptr)) {
          m_toolDisk->handleCommand(command, aux1, aux2);
          return;
        }
      }
    }
    switch (command) {
      case 0x21:// Format
      {
        if (!writeCommandAck()) {
          break;
        }
        QByteArray data(m_newGeometry.bytesPerSector(), 0);
        data[0] = 0xff;
        data[1] = 0xff;
        if (!m_isReadOnly) {
          if (format(m_newGeometry)) {
            qWarning() << "!w" << tr("[%1] Format.").arg(deviceName());
            writeComplete();
          } else {
            writeError();
            qCritical() << "!e" << tr("[%1] Format failed.").arg(deviceName());
          }
        } else {
          qWarning() << "!w" << tr("[%1] Format denied.").arg(deviceName());
          writeError();
        }
        writeDataFrame(data);
        break;
      }
      case 0x22:// Format ED
      {
        if (!writeCommandAck()) {
          break;
        }
        QByteArray data(128, 0);
        data[0] = data[1] = 0xFF;
        if (m_isReadOnly) {
          writeError();
          qWarning() << "!w" << tr("[%1] Format ED denied.").arg(deviceName());
          break;
        } else {
          m_newGeometry.initialize(false, 40, 26, 128);
          if (format(m_newGeometry)) {
            qDebug() << "!n" << tr("[%1] Format ED.").arg(deviceName());
            writeComplete();
          } else {
            writeError();
            qCritical() << "!e" << tr("[%1] Format ED failed.").arg(deviceName());
          }
        }
        writeDataFrame(data);
        break;
      }
      case 0x23:// Run Speed Diagnostic (1050)
      {
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Run Speed Diagnostic with AUX1=$%2 and AUX2=$%3").arg(deviceName()).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
        QByteArray data = readDataFrame(m_geometry.bytesPerSector());
        if (!writeDataAck()) {
          break;
        }
        m_diagData.clear();
        m_diagData.append(data);
        writeComplete();
        break;
      }
      case 0x24:// Run Diagnostic (1050)
      {
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Run Diagnostic with AUX1=$%2 and AUX2=$%3").arg(deviceName()).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
        if (m_diagData.size() < 128) {
          m_diagData.resize(128);
          m_diagData[0] = m_diagData[1] = 0x00;
        }
        if (((quint8) m_diagData[0] == (quint8) 0x01) && ((quint8) m_diagData[1] == (quint8) 0x9B)) {
          m_diagData.resize(122);
          m_diagData[0] = 0x02;
          m_diagData[1] = 0x00;
          m_diagData[122] = 0x14;
          m_diagData[123] = 0x00;
          m_diagData[124] = 0x02;
          m_diagData[125] = 0x00;
          m_diagData[126] = 0x56;
          m_diagData[127] = 0xC6;
        } else if (((quint8) m_diagData[0] == (quint8) 0x00) && ((quint8) m_diagData[1] == (quint8) 0x00)) {
          m_diagData.resize(122);
          m_diagData[0] = 0x22;
          m_diagData[1] = 0x08;
          m_diagData[122] = 0x14;
          m_diagData[123] = 0x00;
          m_diagData[124] = 0x02;
          m_diagData[125] = 0x4C;
          m_diagData[126] = 0x04;
          m_diagData[127] = 0x1A;
        }
        writeComplete();
        writeDataFrame(m_diagData);
        break;
      }
      case 0x3F:// Speed poll
      {
        if (!writeCommandAck()) {
          break;
        }
        QByteArray speed(1, 0);
        speed[0] = sio->port()->speedByte();
        qDebug() << "!n" << tr("[%1] Speed poll: %2").arg(deviceName()).arg((int) speed[0]);
        writeComplete();
        writeDataFrame(speed);
        break;
      }
      case 0x41:// Read track (HAPPY Rev.7)
      {
        if (m_board.isHappyEnabled() && m_board.isHappy1050()) {
          if (m_board.getLastHappyUploadCrc16() == 0x4312) {
            if (!writeCommandAck()) {
              break;
            }
            quint16 aux = aux1 + aux2 * 256;
            int trackNumber = (int) (0xFF - (aux & 0xFF));
            qDebug() << "!n" << tr("[%1] Happy Read Track %2 ($%3)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0'));
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            writeComplete();
            readHappyTrack(trackNumber, true);
            writeDataFrame(m_board.m_happyRam.mid(0xD00, 128));
          } else {
            qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX1 $%3, AUX2 $%4 and CRC16 $%5. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            writeCommandNak();
          }
        } else {
          qWarning() << "!w" << tr("[%1] Command $41 NAKed (HAPPY is not enabled).").arg(deviceName());
          writeCommandNak();
        }
        break;
      }
      case 0x42:// Write Sector using Index (CHIP/ARCHIVER) or Read All Sectors (HAPPY Rev.7)
      {
        quint16 aux = aux1 + aux2 * 256;
        if (m_board.isHappyEnabled() && m_board.isHappy1050()) {
          if (m_board.getLastHappyUploadCrc16() == 0x4312) {
            if (!writeCommandAck()) {
              break;
            }
            int trackNumber = static_cast<int>(0xFF - aux1);
            int afterSectorNumber = (aux2 == 0) ? 0 : 0xFF - aux2;
            if (afterSectorNumber == 0) {
              qDebug() << "!n" << tr("[%1] Happy Read Sectors of track %2 ($%3)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0'));
            } else {
              qDebug() << "!n" << tr("[%1] Happy Read Sectors of track %2 ($%3) starting after sector %4 ($%5)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0')).arg(afterSectorNumber).arg(afterSectorNumber, 2, 16, QChar('0'));
            }
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            QByteArray data = readHappySectors(trackNumber, afterSectorNumber, true);
            if (!writeComplete()) {
              break;
            }
            writeDataFrame(data);
          } else {
            qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX $%3 and CRC16 $%4. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux, 4, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            writeCommandNak();
          }
          break;
        }
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Write Sector using Index denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Super Archiver Write Sector using Index with AUX1=$%2 and AUX2=$%3").arg(deviceName()).arg((aux & 0xFF), 2, 16, QChar('0')).arg(((aux >> 8) & 0xFF), 2, 16, QChar('0'));
        QByteArray data = readDataFrame(m_geometry.bytesPerSector());
        if (!data.isEmpty()) {
          if (!writeDataAck()) {
            break;
          }
          if (m_isReadOnly) {
            writeError();
            qWarning() << "!w" << tr("[%1] Super Archiver Write Sector using Index denied.").arg(deviceName());
            break;
          }
          if (writeSectorUsingIndex(aux, data, false)) {
            writeComplete();
          } else {
            writeError();
            qCritical() << "!e" << tr("[%1] Super Archiver Write Sector using Index failed.").arg(deviceName());
          }
        } else {
          qCritical() << "!e" << tr("[%1] Super Archiver Write Sector using Index data frame failed.").arg(deviceName());
          writeDataNak();
        }
        break;
      }
      case 0x43:// Read all Sector Statuses (CHIP/ARCHIVER) or Set Skew Alignment (HAPPY Rev.7)
      {
        if (m_board.isHappyEnabled() && m_board.isHappy1050()) {
          quint16 aux = aux1 + aux2 * 256;
          if (m_board.getLastHappyUploadCrc16() == 0x4312) {
            if (!writeCommandAck()) {
              break;
            }
            for (int i = 0; i < 128; i++) {
              m_board.m_happyRam[0x380 + i] = 0;
            }
            m_board.m_happyRam[0x3CB] = aux & 0xFF;
            m_board.m_happyRam[0x3CC] = (aux >> 8) & 0xFF;
            qDebug() << "!n" << tr("[%1] Happy Set Skew Alignment track %2 ($%3) and sector %4 ($%5)").arg(deviceName()).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CB]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CB], 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CC]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CC], 2, 16, QChar('0'));
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            writeComplete();
          } else {
            qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX $%3 and CRC16 $%4. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux, 4, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            writeCommandNak();
          }
          break;
        }
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Read all Sector Statuses denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Super Archiver Read All Sector Statuses").arg(deviceName());
        QByteArray data;
        if (readSectorStatuses(data)) {
          if (!writeComplete()) {
            break;
          }
          writeDataFrame(data);
        } else {
          qCritical() << "!e" << tr("[%1] Super Archiver Read All Sector Statuses failed.").arg(deviceName());
          QByteArray result(128, 0);
          if (!writeError()) {
            break;
          }
          writeDataFrame(result);
        }
        break;
      }
      case 0x44:// Read Sector using Index (CHIP/ARCHIVER) or Read Skew Alignment (HAPPY Rev.7)
      {
        quint16 aux = aux1 + aux2 * 256;
        if (m_board.isHappyEnabled() && m_board.isHappy1050()) {
          if (m_board.getLastHappyUploadCrc16() == 0x4312) {
            if (!writeCommandAck()) {
              break;
            }
            m_board.m_happyRam[0x3C9] = aux & 0xFF;
            m_board.m_happyRam[0x3CA] = (aux >> 8) & 0xFF;
            qDebug() << "!n" << tr("[%1] Happy Read Skew alignment of track %2 ($%3) sector %4 ($%5) with track %6 ($%7) sector %8 ($%9)").arg(deviceName()).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CB]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CB], 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CC]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CC], 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3C9]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3C9], 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CA]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CA], 2, 16, QChar('0'));
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            if (readHappySkewAlignment(true)) {
              if (!writeComplete()) {
                break;
              }
            } else {
              qCritical() << "!e" << tr("[%1] Happy Read Skew alignment failed.").arg(deviceName()).arg(aux, 3, 16, QChar('0'));
              if (!writeError()) {
                break;
              }
            }
            writeDataFrame(m_board.m_happyRam.mid(0x380, 128));
          } else {
            qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX $%3 and CRC16 $%4. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux, 4, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            writeCommandNak();
          }
          break;
        }
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Read Sector using Index denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Super Archiver Read Sector using Index with AUX1=$%2 and AUX2=$%3").arg(deviceName()).arg((aux & 0xFF), 2, 16, QChar('0')).arg(((aux >> 8) & 0xFF), 2, 16, QChar('0'));
        QByteArray data;
        if (readSectorUsingIndex(aux, data)) {
          if (!writeComplete()) {
            break;
          }
          writeDataFrame(data);
        } else {
          qCritical() << "!e" << tr("[%1] Super Archiver Read sector using Index with AUX1=$%2 and AUX2=$%3 failed.").arg(deviceName()).arg((aux & 0xFF), 2, 16, QChar('0')).arg(((aux >> 8) & 0xFF), 2, 16, QChar('0'));
          QByteArray result(128, 0);
          if (!writeError()) {
            break;
          }
          writeDataFrame(result);
        }
        break;
      }
      case 0x46:// Write Track (CHIP/ARCHIVER) or Write Track (HAPPY Rev.7)
      {
        if (m_board.isHappyEnabled() && m_board.isHappy1050()) {
          if (m_board.getLastHappyUploadCrc16() == 0x4312) {
            if (!writeCommandAck()) {
              break;
            }
            int trackNumber = (int) (0xFF - aux1);
            qDebug() << "!n" << tr("[%1] Happy Write track %2 ($%3)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0'));
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            if (writeHappyTrack(trackNumber, true)) {
              writeComplete();
            } else {
              writeError();
              qCritical() << "!e" << tr("[%1] Happy Write track failed.").arg(deviceName());
            }
          } else {
            qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX1 $%3, AUX2 $%4 and CRC16 $%5. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            writeCommandNak();
          }
          break;
        }
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Write Track denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Super Archiver Write Track with AUX1=$%2 and AUX2=$%3").arg(deviceName()).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
        quint16 aux = aux1 + aux2 * 256;
        QByteArray data = readDataFrame(m_geometry.bytesPerSector(aux));
        if (!data.isEmpty()) {
          if (!writeDataAck()) {
            break;
          }
          if (m_isReadOnly) {
            writeError();
            qWarning() << "!w" << tr("[%1] Super Archiver Write Track denied.").arg(deviceName());
            break;
          }
          if (writeTrack(aux, data)) {
            writeComplete();
          } else {
            writeError();
            qCritical() << "!e" << tr("[%1] Super Archiver Write track failed.").arg(deviceName());
          }
        } else {
          qCritical() << "!e" << tr("[%1] Super Archiver Write track data frame failed.").arg(deviceName());
          writeDataNak();
        }
        break;
      }
      case 0x47:// Read Track (128 bytes) (CHIP/ARCHIVER) or Write all Sectors (HAPPY Rev.7)
      {
        quint16 aux = aux1 + aux2 * 256;
        if (m_board.isHappyEnabled() && m_board.isHappy1050()) {
          if (m_board.getLastHappyUploadCrc16() == 0x4312) {
            if (!writeCommandAck()) {
              break;
            }
            int trackNumber = static_cast<int>(0xFF - aux1);
            int afterSectorNumber = (aux2 == 0) ? 0 : 0xFF - aux2;
            if (afterSectorNumber == 0) {
              qDebug() << "!n" << tr("[%1] Happy Write Sectors of track %2 ($%3)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0'));
            } else {
              qDebug() << "!n" << tr("[%1] Happy Write Sectors of track %2 ($%3) starting after sector %4 ($%5)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0')).arg(afterSectorNumber).arg(afterSectorNumber, 2, 16, QChar('0'));
            }
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            if (writeHappySectors(trackNumber, afterSectorNumber, true)) {
              if (!writeComplete()) {
                break;
              }
            } else {
              qCritical() << "!e" << tr("[%1] Happy Write Sectors failed.").arg(deviceName()).arg(aux, 3, 16, QChar('0'));
              if (!writeError()) {
                break;
              }
            }
            writeDataFrame(m_board.m_happyRam.mid(m_board.isHappy1050() ? 0xC80 : 0x280, 128));
          } else {
            qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX $%3 and CRC16 $%4. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux, 4, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            writeCommandNak();
          }
          break;
        }
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Read Track (128 bytes) denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Super Archiver Read Track (128 bytes) with AUX1=$%2 and AUX2=$%3").arg(deviceName()).arg((aux & 0xFF), 2, 16, QChar('0')).arg(((aux >> 8) & 0xFF), 2, 16, QChar('0'));
        QByteArray data;
        readTrack(aux, data, 128);
        if (!writeComplete()) {
          break;
        }
        writeDataFrame(data);
        break;
      }
      case 0x48:// Configure drive (HAPPY Rev.7)
      {
        if (m_board.isHappyEnabled()) {
          if (!writeCommandAck()) {
            break;
          }

          qDebug() << "!n" << tr("[%1] Happy Configure drive with AUX1 $%2, AUX2 $%3").arg(deviceName()).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
          if (aux1 == 0x03) {
            for (int i = 0; i < m_board.m_happyRam.length(); i++) {
              m_board.m_happyRam[i] = (unsigned char) 0;
            }
          }
          writeComplete();
          m_board.setHappy1050(true);
        } else {
          writeCommandNak();
          qWarning() << "!w" << tr("[%1] Happy Configure drive NAKed (HAPPY is not enabled).").arg(deviceName());
        }
        break;
      }
      case 0x49:// Write track with skew alignment (HAPPY Rev.7)
      {
        if (m_board.isHappyEnabled() && m_board.isHappy1050()) {
          if (m_board.getLastHappyUploadCrc16() == 0x4312) {
            if (!writeCommandAck()) {
              break;
            }
            int trackNumber = (int) (0xFF - aux1);
            qDebug() << "!n" << tr("[%1] Happy Write track using skew alignment of track %2 ($%3) with track %4 ($%5) sector %6 ($%7)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CB]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CB], 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CC]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CC], 2, 16, QChar('0'));
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            if (writeHappyTrack(trackNumber, true)) {
              writeComplete();
            } else {
              writeError();
              qCritical() << "!e" << tr("[%1] Happy Write track failed.").arg(deviceName());
            }
            writeComplete();
          } else {
            qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX1 $%3, $%4 and CRC16 $%5. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            writeCommandNak();
          }
        } else {
          qWarning() << "!w" << tr("[%1] Command $49 NAKed (HAPPY is not enabled).").arg(deviceName());
          writeCommandNak();
        }
        break;
      }
      case 0x4A:// Init Skew Alignment (HAPPY Rev.7)
      {
        if (m_board.isHappyEnabled() && m_board.isHappy1050()) {
          if (m_board.getLastHappyUploadCrc16() == 0x4312) {
            if (!writeCommandAck()) {
              break;
            }
            qDebug() << "!n" << tr("[%1] Happy Init Skew alignment").arg(deviceName());
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            writeComplete();
          } else {
            qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX1 $%3, AUX2 $%4 and CRC16 $%5. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            writeCommandNak();
          }
        } else {
          qWarning() << "!w" << tr("[%1] Command $4A NAKed (HAPPY is not enabled).").arg(deviceName());
          writeCommandNak();
        }
        break;
      }
      case 0x4B:// Prepare backup (HAPPY Rev.7)
      {
        if (m_board.isHappyEnabled()) {
          if (!writeCommandAck()) {
            break;
          }
          qDebug() << "!n" << tr("[%1] Happy Prepare backup with AUX1 $%2, AUX2 $%3").arg(deviceName()).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
          QThread::usleep(150);
          sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
          writeComplete();
          m_board.setHappy1050(true);
        } else {
          writeCommandNak();
          qWarning() << "!w" << tr("[%1] Happy Prepare backup NAKed (HAPPY is not enabled).").arg(deviceName());
        }
        break;
      }
      case 0x4C:// Set RAM Buffer (CHIP/ARCHIVER)
      {
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Set RAM Buffer denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Super Archiver Set RAM Buffer").arg(deviceName());
        QByteArray ram = readDataFrame(128);
        if (ram.size() == 128) {
          if (!writeDataAck()) {
            break;
          }
          writeComplete();
          for (int i = 0; i < 32; i++) {
            m_board.m_chipRam[i] = (unsigned char) ram[i];
          }
        } else {
          qCritical() << "!e" << tr("[%1] Super Archiver Set RAM Buffer data frame failed.").arg(deviceName());
          m_board.m_chipRam[0] = 0;
          writeDataNak();
        }
        break;
      }
      case 0x4D:// Execute code (CHIP)
      {
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Chip Execute code denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Chip Execute code (ignored)").arg(deviceName());
        QByteArray ram = readDataFrame(128);
        if (ram.size() == 128) {
          if (!writeDataAck()) {
            break;
          }
          if (m_disassembleUploadedCode) {
            int address = getUploadCodeStartAddress(command, aux1 + aux2 * 256, ram);
            if (address != -1) {
              m_remainingBytes.clear();
              disassembleCode(ram, (unsigned short) address, false, false);
            }
          }
          writeComplete();
        } else {
          qCritical() << "!e" << tr("[%1] Chip Execute code data frame failed.").arg(deviceName());
          writeDataNak();
        }
        break;
      }
      case 0x4E:// Get PERCOM block
      {
        if (!writeCommandAck()) {
          break;
        }
        QByteArray percom = m_newGeometry.toPercomBlock();
        qDebug() << "!n" << tr("[%1] Get PERCOM block (%2).").arg(deviceName(), m_newGeometry.humanReadable());
        writeComplete();
        writeDataFrame(percom);
        break;
      }
      case 0x4F:// Set PERCOM block or Open CHIP if code is 2267 or ABCD
      {
        if (!writeCommandAck()) {
          break;
        }
        quint16 aux = aux1 + aux2 * 256;
        if ((aux == 0x2267) || (aux == 0xABCD)) {
          if (m_geometry.sectorCount() <= 1040 && m_geometry.bytesPerSector() == 128) {
            m_board.setChipOpen(true);
            m_board.setLastArchiverUploadCrc16(0);
            emit statusChanged(m_deviceNo);
            qDebug() << "!n" << tr("[%1] Open CHIP with code aux1 %2, aux2 %2").arg(deviceName()).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
          } else {
            qWarning() << "!w" << tr("[%1] Open CHIP denied on disk %2").arg(deviceName(), m_newGeometry.humanReadable());
          }
          writeComplete();
          break;
        }
        qDebug() << "!n" << tr("[%1] Set PERCOM block (%2).").arg(deviceName(), m_newGeometry.humanReadable());
        QByteArray percom = readDataFrame(12);
        if (percom.isEmpty()) {
          writeDataNak();
          break;
        }
        if (!writeDataAck()) {
          break;
        }
        m_newGeometry.initialize(percom);

        writeComplete();
        break;
      }
      case 0x50:// Write sector with verify (ALL) or Write memory (HAPPY 810)
      case 0x57:// Write sector without verify (ALL) or Write memory (HAPPY 810)
      case 0x70:// High Speed Write sector with verify or Write memory (HAPPY 1050) or BitWriter read memory
      case 0x77:// High Speed Write sector without verify or Write memory (HAPPY 1050)
      {
        quint16 aux = aux1 + aux2 * 256;
        quint16 sector = aux;
        if (m_board.isChipOpen()) {
          sector = aux & 0x3FF;
          if (command == 0x70) {
            if (!writeCommandAck()) {
              break;
            }
            qDebug() << "!n" << tr("[%1] BitWriter read memory with AUX=$%2").arg(deviceName()).arg(aux, 4, 16, QChar('0'));
            if (!writeComplete()) {
              break;
            }
            writeDataFrame(QByteArray((const char *) ARCHIVER_SPEED_CHECK, sizeof(ARCHIVER_SPEED_CHECK)));
            break;
          }
        } else if (m_board.isHappyEnabled()) {
          if ((!m_board.isHappy1050()) && (aux >= 0x0800) && (aux <= 0x1380)) {// Write memory (HAPPY 810)
            if (!writeCommandAck()) {
              break;
            }
            qDebug() << "!n" << tr("[%1] Happy Write memory at $%2").arg(deviceName()).arg(aux, 4, 16, QChar('0'));
            QByteArray ram = readDataFrame(m_geometry.bytesPerSector(sector));
            if (!ram.isEmpty()) {
              if (!writeDataAck()) {
                break;
              }
              for (int i = 0; i < ram.size(); i++) {
                m_board.m_happyRam[aux - 0x0800 + i] = ram[i];
              }
              if (aux == 0x0800) {
                // compute CRC16 and save it to know what data should be returned by other commands
                Crc16 crc16;
                crc16.Reset();
                for (int j = 0; j < ram.size(); j++) {
                  crc16.Add((unsigned char) (ram.at(j)));
                }
                m_board.setLastHappyUploadCrc16(crc16.GetCrc());
                m_remainingBytes.clear();
              }
              if (m_disassembleUploadedCode) {
                int address = getUploadCodeStartAddress(command, aux, ram);
                if (address != -1) {
                  disassembleCode(ram, (unsigned short) address, false, m_board.hasHappySignature());
                }
              }
              writeComplete();
            } else {
              writeError();
            }
            break;
          } else if ((aux & 0x8000) && (aux < 0x9800)) {// Write memory (HAPPY Rev.7)
            if (!writeCommandAck()) {
              break;
            }
            m_board.setHappy1050(true);
            qDebug() << "!n" << tr("[%1] Happy %2Write memory at $%3").arg(deviceName(), (command == 0x70) || (command == 0x77) ? tr("High Speed ") : "").arg(aux, 4, 16, QChar('0'));
            // We should set high speed for data with Happy 1050 Rev.7 but it does not work.
            // A patched version of Happy Warp Speed Software V7.1.atr has been prepared to send data at normal speed to RespeQt
            // The original version is not compatible with RespeQt.
            QByteArray ram = readDataFrame(m_geometry.bytesPerSector(sector));
            if (!ram.isEmpty()) {
              if (!writeDataAck()) {
                break;
              }
              for (int i = 0; i < ram.size(); i++) {
                m_board.m_happyRam[aux - 0x8000 + i] = ram[i];
              }
              if (aux == 0x8000) {
                // compute CRC16 and save it to know what data should be returned by other commands
                Crc16 crc16;
                crc16.Reset();
                for (int j = 0; j < ram.size(); j++) {
                  crc16.Add((unsigned char) (ram.at(j)));
                }
                m_board.setLastHappyUploadCrc16(crc16.GetCrc());
                m_remainingBytes.clear();
              }
              if (m_disassembleUploadedCode) {
                int address = getUploadCodeStartAddress(command, aux, ram);
                if (address != -1) {
                  disassembleCode(ram, (unsigned short) address, true, true);
                }
              }
              writeComplete();
            } else {
              writeError();
            }
            break;
          } else {
            writeCommandNak();
            qWarning() << "!w" << tr("[%1] Write memory at $%2 NAKed (Address out of range).").arg(deviceName()).arg(aux, 4, 16, QChar('0'));
          }
        }
        if ((sector >= 1) && (sector <= m_geometry.sectorCount())) {
          if (!writeCommandAck()) {
            break;
          }
          int track = (sector - 1) / m_geometry.sectorsPerTrack();
          int relativeSector = ((sector - 1) % m_geometry.sectorsPerTrack()) + 1;
          if (aux != sector) {
            qDebug() << "!n" << tr("[%1] %2Write Sector %3 ($%4) #%5 in track %6 ($%7) with AUX=$%8").arg(deviceName()).arg((command == 0x70) || (command == 0x77) ? tr("Happy High Speed ") : "").arg(sector).arg(sector, 3, 16, QChar('0')).arg(relativeSector).arg(track).arg(track, 2, 16, QChar('0')).arg(aux, 4, 16, QChar('0'));// clazy:exclude=qstring-arg
          } else {
            qDebug() << "!n" << tr("[%1] %2Write Sector %3 ($%4) #%5 in track %6 ($%7)").arg(deviceName()).arg((command == 0x70) || (command == 0x77) ? tr("Happy High Speed ") : "").arg(sector).arg(sector, 3, 16, QChar('0')).arg(relativeSector).arg(track).arg(track, 2, 16, QChar('0'));// clazy:exclude=qstring-arg
          }
          // We should set high speed for data with Happy 1050 Rev.7 but it does not work.
          // A patched version of Happy Warp Speed Software V7.1.atr has been prepared to send data at normal speed to RespeQt
          // The original version is not compatible with RespeQt.
          QByteArray data = readDataFrame(m_geometry.bytesPerSector(sector));
          if (!data.isEmpty()) {
            if (!writeDataAck()) {
              break;
            }
            if (m_isReadOnly) {
              writeError();
              qWarning() << "!w" << tr("[%1] Write sector denied.").arg(deviceName());
              break;
            }
            if (writeSector(aux, data)) {
              writeComplete();
            } else {
              writeError();
              qCritical() << "!e" << tr("[%1] Write sector failed.").arg(deviceName());
            }
          } else {
            qCritical() << "!e" << tr("[%1] Write sector data frame failed.").arg(deviceName());
            writeDataNak();
          }
        } else {
          writeCommandNak();
          qWarning() << "!w" << tr("[%1] Write sector %2 ($%3) NAKed.").arg(deviceName()).arg(aux).arg(aux, 4, 16, QChar('0'));
        }
        break;
      }
      case 0x51:// Execute custom code (HAPPY)
      {
        if (m_board.isHappyEnabled() && m_board.hasHappySignature()) {
          if (!writeCommandAck()) {
            break;
          }
          writeComplete();
          if ((m_board.getLastHappyUploadCrc16() == 0xEF1A) || (m_board.getLastHappyUploadCrc16() == 0xC8C8)) {// Happy Rev.5 and Rev.7
            qDebug() << "!n" << tr("[%1] Happy Ram check").arg(deviceName());
            writeDataFrame(QByteArray((const char *) HAPPY_RAM_CHECK, sizeof(HAPPY_RAM_CHECK)));
          } else if (m_board.getLastHappyUploadCrc16() == 0xC545) {
            qDebug() << "!n" << tr("[%1] Happy Rom 810 test").arg(deviceName());
            writeDataFrame(QByteArray((const char *) HAPPY_ROM810_CHECK, sizeof(HAPPY_ROM810_CHECK)));
          } else if (m_board.getLastHappyUploadCrc16() == 0x72CD) {
            qDebug() << "!n" << tr("[%1] Happy Extended ram check").arg(deviceName());
            writeDataFrame(QByteArray((const char *) HAPPY_RAM_EXTENDED_CHECK, sizeof(HAPPY_RAM_EXTENDED_CHECK)));
          } else if (m_board.getLastHappyUploadCrc16() == 0xEABE) {
            qDebug() << "!n" << tr("[%1] Happy Run diagnostic").arg(deviceName());
            writeDataFrame(QByteArray((const char *) HAPPY_DIAGNOSTIC, sizeof(HAPPY_DIAGNOSTIC)));
          } else if (m_board.getLastHappyUploadCrc16() == 0x8735) {
            qDebug() << "!n" << tr("[%1] Happy Speed check").arg(deviceName());
            writeDataFrame(QByteArray((const char *) HAPPY_SPEED_CHECK, sizeof(HAPPY_SPEED_CHECK)));
          } else if (m_board.getLastHappyUploadCrc16() == 0x8ABA) {
            // Step-in, Step-out commands. No data to return
            qDebug() << "!n" << tr("[%1] Happy Step-in / Step-out").arg(deviceName());
          } else if (m_board.getLastHappyUploadCrc16() == 0xE610) {
            qDebug() << "!n" << tr("[%1] Happy Read/Write check").arg(deviceName());
            writeDataFrame(QByteArray((const char *) HAPPY_RW_CHECK, sizeof(HAPPY_RW_CHECK)));
          } else if (m_board.getLastHappyUploadCrc16() == 0x9831) {
            qDebug() << "!n" << tr("[%1] Happy Set unhappy mode").arg(deviceName());
            m_board.setHappyEnabled(false);
            emit statusChanged(m_deviceNo);
            writeDataFrame(m_board.m_happyRam.left(128));
          } else if (m_board.getLastHappyUploadCrc16() == 0x5D3D) {
            int trackNumber = (int) (0xFF - aux1);
            qDebug() << "!n" << tr("[%1] Happy Read Track %2 ($%3)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0'));
            readHappyTrack(trackNumber, false);
            writeDataFrame(m_board.m_happyRam.mid(0x300, 128));
          } else if (m_board.getLastHappyUploadCrc16() == 0xEF08) {
            qDebug() << "!n" << tr("[%1] Happy init backup 1").arg(deviceName());
            writeDataFrame(QByteArray((const char *) HAPPY_BACKUP_BUF1, sizeof(HAPPY_BACKUP_BUF1)));
          } else if (m_board.getLastHappyUploadCrc16() == 0xCC68) {
            qDebug() << "!n" << tr("[%1] Happy init backup 2").arg(deviceName());
            writeDataFrame(QByteArray((const char *) HAPPY_BACKUP_BUF2, sizeof(HAPPY_BACKUP_BUF2)));
          } else {
            // Clear buffer command contains 64 bytes of code but the remaining bytes are garbage.
            // Just check the last instruction ($083D: JMP $1EFC)
            if (((quint8) m_board.m_happyRam[0x3D] == (quint8) 0x4C) && ((quint8) m_board.m_happyRam[0x3E] == (quint8) 0xFC) && ((quint8) m_board.m_happyRam[0x3F] == (quint8) 0x1E)) {
              qDebug() << "!n" << tr("[%1] Happy Clear buffer").arg(deviceName());
              m_board.m_happyRam[0] = 0x80;
            } else {
              qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX1 $%3, AUX2 $%4 and CRC16 $%5. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            }
          }
        } else if (m_board.isHappyEnabled() && m_board.isHappy1050()) {
          if (m_board.getLastHappyUploadCrc16() == 0x4312) {
            if (!writeCommandAck()) {
              break;
            }
            qDebug() << "!n" << tr("[%1] Happy init backup").arg(deviceName());
            writeComplete();
          } else {
            qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX1 $%3, AUX2 $%4 and CRC16 $%5. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
            writeCommandNak();
          }
        } else {
          qWarning() << "!w" << tr("[%1] Command $51 NAKed (HAPPY is not enabled).").arg(deviceName());
          writeCommandNak();
        }
        break;
      }
      case 0x52:// Read sector (ALL) or Read memory (HAPPY 810)
      case 0x72:// High Speed Read sector or Read memory (HAPPY 1050)
      {
        quint16 aux = aux1 + aux2 * 256;
        quint16 sector = aux;
        if (m_board.isChipOpen()) {
          sector = aux & 0x3FF;
        } else if (m_board.isHappyEnabled()) {
          if ((!m_board.isHappy1050()) && (aux >= 0x800) && (aux <= 0x1380)) {// Read memory (HAPPY 810 Rev.5)
            if (!writeCommandAck()) {
              break;
            }
            qDebug() << "!n" << tr("[%1] Happy Read memory at $%2").arg(deviceName()).arg(aux, 4, 16, QChar('0'));
            if (!writeComplete()) {
              break;
            }
            writeDataFrame(m_board.m_happyRam.mid(aux - 0x800, 128));
            break;
          } else if ((aux & 0x8000) && (aux < 0x9800)) {// Read memory (HAPPY Rev.7)
            if (!writeCommandAck()) {
              break;
            }
            qDebug() << "!n" << tr("[%1] Happy %2Read memory at $%3").arg(deviceName()).arg((command == 0x72) ? tr("High Speed ") : "").arg(aux, 4, 16, QChar('0'));// clazy:exclude=qstring-arg
            if (command == 0x72) {
              QThread::usleep(150);
              sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            }
            if (!writeComplete()) {
              break;
            }
            writeDataFrame(m_board.m_happyRam.mid(aux & 0x7FFF, 128));
            break;
          }
        }
        if ((sector >= 1) && (sector <= m_geometry.sectorCount())) {
          if (!writeCommandAck()) {
            break;
          }
          int track = (sector - 1) / m_geometry.sectorsPerTrack();
          int relativeSector = ((sector - 1) % m_geometry.sectorsPerTrack()) + 1;
          if (aux != sector) {
            qDebug() << "!n" << tr("[%1]%2 Read Sector %3 ($%4) #%5 in track %6 ($%7) with AUX=$%8").arg(deviceName()).arg((command == 0x72) ? tr("Happy High Speed ") : "").arg(sector).arg(sector, 3, 16, QChar('0')).arg(relativeSector).arg(track).arg(track, 2, 16, QChar('0')).arg(aux, 4, 16, QChar('0'));// clazy:exclude=qstring-arg
          } else {
            qDebug() << "!n" << tr("[%1] %2Read Sector %3 ($%4) #%5 in track %6 ($%7)").arg(deviceName()).arg((command == 0x72) ? tr("Happy High Speed ") : "").arg(sector).arg(sector, 3, 16, QChar('0')).arg(relativeSector).arg(track).arg(track, 2, 16, QChar('0'));// clazy:exclude=qstring-arg
          }
          if (command == 0x72) {
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
          }
          QByteArray data;
          if (readSector(aux, data)) {
            quint16 chipFlags = m_board.isChipOpen() ? aux & 0xFC00 : 0;
            if ((m_wd1771Status != 0xFF) && (chipFlags == 0)) {
              if (!writeError()) {
                break;
              }
            } else {
              if (!writeComplete()) {
                break;
              }
              // Are we booting Happy software in Atari memory ?
              // Then patch Happy software on the fly to remove Write commands at double speed
              if (m_board.isHappyEnabled() && (m_deviceNo == 0x31)) {
                if ((command == 0x72) && ((sector == 397) || (sector == 421))) {
                  // if we are loading Happy Warp Speed Software V7.1 disk A.atr from RespeQt, patch it to remove Write commands at double speed !
                  Crc16 crc16;
                  crc16.Reset();
                  for (int j = 0; j < data.size(); j++) {
                    crc16.Add((unsigned char) (data.at(j)));
                  }
                  if ((sector == 397) && (crc16.GetCrc() == 0x7102)) {
                    writeDataFrame(QByteArray((const char *) HAPPY_DOUBLESPEEDREV7_PATCH1, sizeof(HAPPY_DOUBLESPEEDREV7_PATCH1)));
                    m_board.setHappyPatchInProgress(true);
                    qDebug() << "!u" << tr("[%1] Happy Warp Speed Software V7.1 patched on the fly to be compatible with RespeQt").arg(deviceName());
                    break;
                  } else if ((sector == 421) && (crc16.GetCrc() == 0xF00A) && (m_board.isHappyPatchInProgress())) {
                    writeDataFrame(QByteArray((const char *) HAPPY_DOUBLESPEEDREV7_PATCH2, sizeof(HAPPY_DOUBLESPEEDREV7_PATCH2)));
                    m_board.setHappyPatchInProgress(false);
                    break;
                  }
                }
#if HAPPY_PATCH_V53
                // not mandatory as the main feature (Backup protected disk) does not use double speed (only Sector copy uses double speed)
                else if ((command == 0x52) && ((sector == 59) || (sector == 71) || (sector == 85))) {
                  // if we are loading Happy Warp Speed Software V5.3.atr from RespeQt, patch it to remove Write commands at double speed !
                  Crc16 crc16;
                  crc16.Reset();
                  for (int j = 0; j < data.size(); j++) {
                    crc16.Add((unsigned char) (data.at(j)));
                  }
                  if ((sector == 59) && (crc16.GetCrc() == 0x8CB3)) {
                    writeDataFrame(QByteArray((const char *) HAPPY_DOUBLESPEEDREV5_PATCH1, sizeof(HAPPY_DOUBLESPEEDREV5_PATCH1)));
                    m_board.setHappyPatchInProgress(true);
                    qDebug() << "!u" << tr("[%1] Happy Warp Speed Software V5.3 patched on the fly to be compatible with RespeQt").arg(deviceName());
                    break;
                  } else if ((sector == 71) && (crc16.GetCrc() == 0x4552) && (m_board.isHappyPatchInProgress())) {
                    writeDataFrame(QByteArray((const char *) HAPPY_DOUBLESPEEDREV5_PATCH2, sizeof(HAPPY_DOUBLESPEEDREV5_PATCH2)));
                    break;
                  } else if ((sector == 85) && (crc16.GetCrc() == 0xF00A) && (m_board.isHappyPatchInProgress())) {
                    writeDataFrame(QByteArray((const char *) HAPPY_DOUBLESPEEDREV5_PATCH3, sizeof(HAPPY_DOUBLESPEEDREV5_PATCH3)));
                    m_board.setHappyPatchInProgress(false);
                    break;
                  }
                }
#endif
              }
            }
            writeDataFrame(data);
          } else {
            qCritical() << "!e" << tr("[%1] Read sector failed.").arg(deviceName());
            if (!writeError()) {
              break;
            }
            writeDataFrame(data);
          }
        } else {
          writeCommandNak();
          qWarning() << "!w" << tr("[%1] Read sector $%2 NAKed.").arg(deviceName()).arg(aux, 4, 16, QChar('0'));
        }
        break;
      }
      case 0x53:// Get status
      {
        if (!writeCommandAck()) {
          break;
        }
        QByteArray status(4, 0);
        getStatus(status);
        quint16 aux = aux1 + aux2 * 256;
        if (aux == 0xABCD) {
          status[2] = 0xEB;// This is a RespeQt drive
          status[3] = computeVersionByte();
          qDebug() << "!n" << tr("[%1] RespeQt version inquiry: $%2").arg(deviceName()).arg((unsigned char) status[3], 2, 16, QChar('0'));
        } else {
          qDebug() << "!n" << tr("[%1] Get status: $%2").arg(deviceName()).arg((unsigned char) status[1], 2, 16, QChar('0'));
        }
        writeComplete();
        writeDataFrame(status);
        break;
      }
      case 0x54:// Get RAM Buffer (CHIP/ARCHIVER) or Write sector and Write track (Happy 810 Rev.5)
      {
        if (m_board.isHappyEnabled()) {
          if (m_board.hasHappySignature()) {// Happy 810 Rev. 5
            if (m_board.getLastHappyUploadCrc16() == 0x4416) {
              if (!writeCommandAck()) {
                break;
              }
              quint16 track = 0xFF - aux1;
              quint16 relativeSector = 0xFF - aux2;
              quint16 sector = (track * m_geometry.sectorsPerTrack()) + relativeSector;
              qDebug() << "!n" << tr("[%1] Happy High Speed Write Sector %2 ($%3) #%4 in track %5 ($%6)").arg(deviceName()).arg(sector).arg(sector, 3, 16, QChar('0')).arg(relativeSector).arg(track).arg(track, 2, 16, QChar('0'));
              // We should set high speed for data with Happy 810 Rev.5 but it does not work.
              // A patched version of Happy Warp Speed Software V5.3.atr has been prepared to send data at normal speed to RespeQt
              // The original version is not compatible with RespeQt.
              QByteArray data = readDataFrame(m_geometry.bytesPerSector(sector));
              if (!data.isEmpty()) {
                if (!writeDataAck()) {
                  break;
                }
                if (m_isReadOnly) {
                  writeError();
                  qWarning() << "!w" << tr("[%1] Happy Write sector denied.").arg(deviceName());
                  break;
                }
                if (writeSector(sector, data)) {
                  writeComplete();
                } else {
                  writeError();
                  qCritical() << "!e" << tr("[%1] Happy Write sector failed.").arg(deviceName());
                }
              } else {
                qCritical() << "!e" << tr("[%1] Happy Write sector data frame failed.").arg(deviceName());
                writeDataNak();
              }
              break;
            } else if (m_board.getLastHappyUploadCrc16() == 0xC96C) {
              if (!writeCommandAck()) {
                break;
              }
              int trackNumber = (int) (0xFF - aux1);
              qDebug() << "!n" << tr("[%1] Happy Write track %2 ($%3)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0'));
              if (writeHappyTrack(trackNumber, false)) {
                writeComplete();
              } else {
                writeError();
                qCritical() << "!e" << tr("[%1] Happy Write track failed.").arg(deviceName());
              }
              break;
            }
          } else if (m_board.isHappy1050()) {
            // try computing CRC16 for 0x9500 which is where diagnostic code is uploaded by Rev.7
            Crc16 crc16;
            crc16.Reset();
            QByteArray ram = m_board.m_happyRam.mid(0x1500, 128);
            for (int j = 0; j < ram.size(); j++) {
              crc16.Add((unsigned char) (ram.at(j)));
            }
            if (crc16.GetCrc() == 0x544B) {
              if (!writeCommandAck()) {
                break;
              }
              if (!writeComplete()) {
                break;
              }
              qDebug() << "!n" << tr("[%1] Happy Rom 1050 test").arg(deviceName());
              writeDataFrame(QByteArray((const char *) HAPPY_ROM1050_CHECK, sizeof(HAPPY_ROM1050_CHECK)));
              break;
            }
          }
        }
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Get RAM Buffer denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Get RAM Buffer").arg(deviceName());
        if (!writeComplete()) {
          break;
        }
        QByteArray result(128, 0);
        for (int i = 0; i < 32; i++) {
          result[i] = m_board.m_chipRam[i];
        }
        writeDataFrame(result);
        break;
      }
      case 0x55:// Execute custom code (HAPPY)
      case 0x56:// Execute custom code (HAPPY)
      {
        if (m_board.isHappyEnabled() && m_board.hasHappySignature()) {
          if (!writeCommandAck()) {
            break;
          }
          if ((command == 0x55) && (m_board.getLastHappyUploadCrc16() == 0x4416)) {
            // Read sector at high speed
            quint16 aux = aux1 + 256 * aux2;
            int track = (aux - 1) / m_geometry.sectorsPerTrack();
            qDebug() << "!n" << tr("[%1] Happy High Speed Read Sector %2 ($%3) in track %4 ($%5)").arg(deviceName()).arg(aux).arg(aux, 3, 16, QChar('0')).arg(track).arg(track, 2, 16, QChar('0'));
            QThread::usleep(150);
            sio->port()->setSpeed(findNearestSpeed(38400) + 1);// +1 (odd number) is a trick to set 2 stop bits (needed by Happy 810)
            QByteArray data;
            if (readSector(aux, data)) {
              if (m_wd1771Status != 0xFF) {
                if (!writeError()) {
                  break;
                }
              } else if (!writeComplete()) {
                break;
              }
              writeDataFrame(data);
            } else {
              qCritical() << "!e" << tr("[%1] Happy Read Sector failed.").arg(deviceName()).arg(aux, 3, 16, QChar('0'));
              writeError();
              writeDataFrame(data);
            }
          } else if ((command == 0x55) && (m_board.getLastHappyUploadCrc16() == 0x5D3D)) {
            qDebug() << "!n" << tr("[%1] Happy Read Skew alignment of track %2 ($%3) sector %4 ($%5) with track %6 ($%7) sector %8 ($%9)").arg(deviceName()).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CB]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CB], 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CC]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CC], 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3C9]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3C9], 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CA]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x3CA], 2, 16, QChar('0'));
            if (readHappySkewAlignment(false)) {
              if (!writeComplete()) {
                break;
              }
            } else {
              qCritical() << "!e" << tr("[%1] Happy Read Skew alignment failed.").arg(deviceName());
              if (!writeError()) {
                break;
              }
            }
            writeDataFrame(m_board.m_happyRam.mid(0x380, 128));
          } else if ((command == 0x56) && (m_board.getLastHappyUploadCrc16() == 0x5D3D)) {
            int trackNumber = (int) (0xFF - aux1);
            int afterSectorNumber = (aux2 == 0) ? 0 : 0xFF - aux2;
            if (afterSectorNumber == 0) {
              qDebug() << "!n" << tr("[%1] Happy Read Sectors of track %2 ($%3)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0'));
            } else {
              qDebug() << "!n" << tr("[%1] Happy Read Sectors of track %2 ($%3) starting after sector %4 ($%5)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0')).arg(afterSectorNumber).arg(afterSectorNumber, 2, 16, QChar('0'));
            }
            QByteArray data = readHappySectors(trackNumber, afterSectorNumber, false);
            if (!writeComplete()) {
              break;
            }
            writeDataFrame(data);
          } else if ((command == 0x55) && (m_board.getLastHappyUploadCrc16() == 0xC96C)) {
            int trackNumber = (int) (0xFF - aux1);
            int afterSectorNumber = (aux2 == 0) ? 0 : 0xFF - aux2;
            if (afterSectorNumber == 0) {
              qDebug() << "!n" << tr("[%1] Happy Write Sectors of track %2 ($%3)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0'));
            } else {
              qDebug() << "!n" << tr("[%1] Happy Write Sectors of track %2 ($%3) starting after sector %4 ($%5)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0')).arg(afterSectorNumber).arg(afterSectorNumber, 2, 16, QChar('0'));
            }
            if (writeHappySectors(trackNumber, afterSectorNumber, false)) {
              if (!writeComplete()) {
                break;
              }
            } else {
              qCritical() << "!e" << tr("[%1] Happy Write Sectors failed.").arg(deviceName());
              if (!writeError()) {
                break;
              }
            }
            writeDataFrame(m_board.m_happyRam.mid(0x280, 128));
          } else if ((command == 0x56) && (m_board.getLastHappyUploadCrc16() == 0xC96C)) {
            int trackNumber = (int) ((quint8) 0xFF - (quint8) m_board.m_happyRam[0x84F]);
            int sectorNumber = (int) ((quint8) 0xFF - (quint8) m_board.m_happyRam[0x81A]);
            qDebug() << "!n" << tr("[%1] Happy Write track %2 ($%3) skew aligned with track %4 ($%5) sector %6 ($%7)").arg(deviceName()).arg(trackNumber).arg(trackNumber, 2, 16, QChar('0')).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x816]).arg((quint8) 0xFF - (quint8) m_board.m_happyRam[0x816], 2, 16, QChar('0')).arg(sectorNumber).arg(sectorNumber, 2, 16, QChar('0'));
            if (writeHappyTrack(trackNumber, false)) {
              writeComplete();
            } else {
              writeError();
              qCritical() << "!e" << tr("[%1] Happy Write track failed.").arg(deviceName());
            }
          } else if ((command == 0x56) && (m_board.getLastHappyUploadCrc16() == 0x4416)) {
            writeComplete();
            qDebug() << "!n" << tr("[%1] Happy init sector copy").arg(deviceName());
          } else {
            if (m_board.getLastHappyUploadCrc16() == 0x8ABA) {
              // Step-in, Step-out commands. No data to return
              qDebug() << "!n" << tr("[%1] Happy Step-in / Step-out").arg(deviceName());
              writeComplete();
            } else {
              qWarning() << "!w" << tr("[%1] Happy Execute custom code $%2 with AUX1 $%3, AUX2 $%4 and CRC16 $%5. Ignored").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0')).arg(m_board.getLastHappyUploadCrc16(), 4, 16, QChar('0'));
              writeError();
            }
          }
        } else {
          writeCommandNak();
          qWarning() << "!w" << tr("[%1] Command $%2 NAKed (HAPPY is not enabled).").arg(deviceName()).arg(command, 2, 16, QChar('0'));
        }
        break;
      }
      case 0x62:// Write Fuzzy Sector using Index (ARCHIVER)
      {
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Write Fuzzy Sector using Index denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Super Archiver Write Fuzzy Sector using Index with AUX1=$%2 and AUX2=$%3").arg(deviceName()).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
        QByteArray data = readDataFrame(m_geometry.bytesPerSector());
        if (!data.isEmpty()) {
          if (!writeDataAck()) {
            break;
          }
          if (m_isReadOnly) {
            writeError();
            qWarning() << "!w" << tr("[%1] Super Archiver Write Fuzzy Sector using Index denied.").arg(deviceName());
            break;
          }
          quint16 aux = aux1 + aux2 * 256;
          if (writeSectorUsingIndex(aux, data, true)) {
            writeComplete();
          } else {
            writeError();
            qCritical() << "!e" << tr("[%1] Super Archiver Write Fuzzy Sector using Index failed.").arg(deviceName());
          }
        } else {
          qCritical() << "!e" << tr("[%1] Super Archiver Write Fuzzy Sector using Index data frame failed.").arg(deviceName());
          writeDataNak();
        }
        break;
      }
      case 0x66:// Format with custom sector skewing
      {
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!u" << tr("[%1] Format with custom sector skewing.").arg(deviceName());
        QByteArray percom = readDataFrame(128);
        if (percom.isEmpty()) {
          writeDataNak();
          break;
        }
        writeDataAck();
        m_newGeometry.initialize(percom);
        if (!m_isReadOnly) {
          if (!format(m_newGeometry)) {
            qCritical() << "!e" << tr("[%1] Format with custom sector skewing failed.").arg(deviceName());
            break;
          }
          writeComplete();
        } else {
          writeError();
          qWarning() << "!w" << tr("[%1] Format with custom sector skewing denied.").arg(deviceName());
        }
        break;
      }
      case 0x67:// Read Track (256 bytes) (ARCHIVER)
      {
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Read Track (256 bytes) denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Super Archiver Read Track (256 bytes) with AUX1=$%2 and AUX2=$%3").arg(deviceName()).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
        QByteArray data;
        quint16 aux = aux1 + aux2 * 256;
        readTrack(aux, data, 256);
        if (!writeComplete()) {
          break;
        }
        writeDataFrame(data);
        break;
      }
      case 0x71:// Write fuzzy sector (ARCHIVER)
      {
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Write Fuzzy sector denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        quint16 aux = aux1 + aux2 * 256;
        quint16 sector = aux & 0x3FF;
        if (sector >= 1 && sector <= m_geometry.sectorCount()) {
          if (!writeCommandAck()) {
            break;
          }
          int track = (aux - 1) / m_geometry.sectorsPerTrack();
          qDebug() << "!n" << tr("[%1] Super Archiver Write Fuzzy Sector %2 ($%3) in track %4 ($%5) with AUX=$%6").arg(deviceName()).arg(sector).arg(sector, 3, 16, QChar('0')).arg(track).arg(track, 2, 16, QChar('0')).arg(aux, 4, 16, QChar('0'));
          QByteArray data = readDataFrame(m_geometry.bytesPerSector(sector));
          if (!data.isEmpty()) {
            if (!writeDataAck()) {
              break;
            }
            if (m_isReadOnly) {
              writeError();
              qWarning() << "!w" << tr("[%1] Super Archiver Write Fuzzy sector denied.").arg(deviceName());
              break;
            }
            if (writeFuzzySector(aux, data)) {
              writeComplete();
            } else {
              writeError();
              qCritical() << "!e" << tr("[%1] Super Archiver Write Fuzzy sector failed.").arg(deviceName());
            }
          } else {
            qCritical() << "!e" << tr("[%1] Super Archiver Write Fuzzy sector data frame failed.").arg(deviceName());
            writeDataNak();
          }
        } else {
          writeCommandNak();
          qWarning() << "!w" << tr("[%1] Super Archiver Write Fuzzy sector $%2 NAKed.").arg(deviceName()).arg(aux, 4, 16, QChar('0'));
        }
        break;
      }
      case 0x73:// Set Speed (ARCHIVER)
      {
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Set Speed denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        quint16 aux = aux1 + aux2 * 256;
        qDebug() << "!n" << tr("[%1] Super Archiver Set Speed %2").arg(deviceName()).arg(aux);
        m_board.setLastArchiverSpeed(aux & 0xFF);
        if (!writeComplete()) {
          break;
        }
        break;
      }
      case 0x74:// Read Memory (ARCHIVER)
      {
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Read Memory denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        if (!writeCommandAck()) {
          break;
        }
        if (!writeComplete()) {
          break;
        }
        // this is the code for the speed check (item 5 of Super Archiver menu then Check drive RPM)
        if (m_board.getLastArchiverUploadCrc16() == 0xFD2E) {
          qDebug() << "!n" << tr("[%1] Super Archiver Read memory (Speed check)").arg(deviceName());
          auto frame = QByteArray((const char *) ARCHIVER_SPEED_CHECK, sizeof(ARCHIVER_SPEED_CHECK));
          frame[2] = m_board.getLastArchiverSpeed();
          frame[248] = m_board.getLastArchiverSpeed() == 2 ? 0xCC : 0xD8;
          writeDataFrame(frame);
        }
        // this is the code for the diagnostic test (Archiver memory, 6532, 6810, Archiver version and rom checksum)
        else if ((m_board.getLastArchiverUploadCrc16() == 0x61F6) || (m_board.getLastArchiverUploadCrc16() == 0xBFE7)) {
          qDebug() << "!n" << tr("[%1] Super Archiver Read memory (Diagnostic)").arg(deviceName());
          writeDataFrame(QByteArray((const char *) ARCHIVER_DIAGNOSTIC, sizeof(ARCHIVER_DIAGNOSTIC)));
        }
        // this is the code for reading the address marks
        else if (m_board.getLastArchiverUploadCrc16() == 0xE3B5) {
          qDebug() << "!n" << tr("[%1] Super Archiver Read memory (Address marks)").arg(deviceName());
          QByteArray data;
          quint16 aux = aux1 + aux2 * 256;
          readTrack(aux, data, 256);
          QByteArray result(256, 0);
          for (quint16 i = 0; i < sizeof(ARCHIVER_ADDRESS_CHECK); i++) {
            result[i] = ARCHIVER_ADDRESS_CHECK[i];
          }
          result[2] = (char) (aux & 0x3F);
          for (quint16 i = 0; i < (256 - sizeof(ARCHIVER_ADDRESS_CHECK)); i++) {
            result[i + (quint16) sizeof(ARCHIVER_ADDRESS_CHECK)] = data[i + 4];
          }
          result[8] = data[3];
          writeDataFrame(result);
        }
        // this is the code to get the sector timing for skew alignment
        else if ((m_board.getLastArchiverUploadCrc16() == 0x603D) || (m_board.getLastArchiverUploadCrc16() == 0xBAC2) || (m_board.getLastArchiverUploadCrc16() == 0xDFFF)) {// Super Archiver 3.02, 3.03, 3.12 respectively
          bool timingOnly = m_board.getLastArchiverUploadCrc16() == 0xDFFF;
          if (timingOnly) {
            quint16 timing = (((quint16) m_board.m_trackData[0xF4]) & 0xFF) + ((((quint16) m_board.m_trackData[0xF5]) << 8) & 0xFF00);
            qDebug() << "!n" << tr("[%1] Super Archiver Read Memory (Skew alignment of $%2)").arg(deviceName()).arg(timing, 4, 16, QChar('0'));
          } else {
            qDebug() << "!n" << tr("[%1] Super Archiver Read Memory (Skew alignment)").arg(deviceName());
          }
          writeDataFrame(m_board.m_trackData);
        } else {
          qDebug() << "!n" << tr("[%1] Super Archiver Read Memory").arg(deviceName());
          writeDataFrame(QByteArray(256, 0));
        }
        break;
      }
      case 0x75:// Upload and execute code (ARCHIVER)
      {
        m_board.setLastArchiverUploadCrc16(0);
        if (!m_board.isChipOpen()) {
          qWarning() << "!w" << tr("[%1] Super Archiver Upload and execute code denied (CHIP is not open)").arg(deviceName());
          writeCommandNak();
          break;
        }
        // accept command
        if (!writeCommandAck()) {
          break;
        }
        qDebug() << "!n" << tr("[%1] Super Archiver Upload and Execute Code").arg(deviceName());
        QByteArray data = readDataFrame(256);
        if (!data.isEmpty()) {
          if (!writeDataAck()) {
            break;
          }
          quint16 aux = aux1 + aux2 * 256;
          if (m_disassembleUploadedCode) {
            int address = getUploadCodeStartAddress(command, aux, data);
            if (address != -1) {
              disassembleCode(data, (unsigned short) address, true, false);
            }
          }
          bool ok = executeArchiverCode(aux, data);
          if (ok) {
            writeComplete();
          } else {
            writeError();
          }
        } else {
          qCritical() << "!e" << tr("[%1] Super Archiver Upload and Execute Code data frame failed.").arg(deviceName());
          writeDataNak();
        }
        break;
      }
      default:// Unknown command
      {
        writeCommandNak();
        qWarning() << "!w" << tr("[%1] command: $%2, aux1: $%3, aux2: $%4 NAKed.").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
        break;
      }
    }
  }

  void DiskImage::getStatus(QByteArray &status) {
    quint8 motorSpinning = 0x10;
    quint8 readOnlyStatus = isReadOnly() || isLeverOpen() ? 0x08 : 0;// if lever not open, the disk is reported as read-only in the status byte
    if (typeid(this) == typeid(DiskImageAtx) || typeid(this) == typeid(DiskImagePro)) {
      status[0] = motorSpinning | readOnlyStatus | m_driveStatus;
    } else {
      status[0] = motorSpinning | readOnlyStatus |
                  ((m_newGeometry.bytesPerSector() == 256) * 32) |
                  ((m_newGeometry.bytesPerSector() == 128 && m_newGeometry.sectorsPerTrack() == 26) * 128);
    }
    status[1] = m_wd1771Status;
    if ((!isReady()) || (isLeverOpen())) {
      status[1] = status[1] & (quint8) ~0x80;
    }
    if (isReadOnly()) {
      status[1] = status[1] & (quint8) ~0x40;
    }
    status[2] = 0xE0;// Timeout for format ($E0) - Time the drive will need to format a disk (1050 returns 0xe0, XF551 0xfe)
    status[3] = 0;
  }

  quint8 DiskImage::computeVersionByte() {
    const char *str = VERSION;
    int index = 0;
    if (str[index] == 'r') {
      index++;
    }
    int major = str[index++] - '0';
    int minor = 0;
    if (str[index] == '.') {
      index++;
      minor = str[index] - '0';
    }
    return (quint8) ((major << 4) + minor);
  }

  int DiskImage::defaultFileSystem() {
    if (m_geometry.sectorCount() < 1) {
      return 0;
    }

    QByteArray data;
    if (!readSector(1, data)) {
      return 0;
    }

    if (m_geometry.bytesPerSector() == 512) {
      if ((quint8) data.at(32) == 0x21) {
        return 5;
      } else {
        return 0;
      }
    } else {
      if ((quint8) data.at(7) == 0x80 /*&& ((quint8)data.at(32) == 0x21 || (quint8)data.at(32) == 0x20)*/) {
        return 5;
      } else {
        if (m_geometry.sectorCount() < 368) {
          return 0;
        }
        if (!readSector(360, data)) {
          return 0;
        }
        uint v = (quint8) data.at(0);
        uint s = (quint8) data.at(1) + (quint8) data.at(2) * 256;
        uint f = (quint8) data.at(3) + (quint8) data.at(4) * 256;
        if (m_geometry.isStandardSD() && v == 1 && s == 709 && f <= s) {
          return 1;
        } else if ((m_geometry.isStandardSD() || m_geometry.isStandardDD()) && v == 2 && s == 707 && f <= s) {
          return 2;
        } else if (m_geometry.isStandardED() && v == 2 && s == 1010 && f <= s) {
          return 3;
        } else if (v == 2 && s + 12 == m_geometry.sectorCount() && f <= s) {
          return 4;
        } else if (m_geometry.bytesPerSector() == 128 && s + (v * 2 - 3) + 10 == m_geometry.sectorCount() && f <= s) {
          return 4;
        } else if (m_geometry.bytesPerSector() == 256 && s + v + 9 == m_geometry.sectorCount() && f <= s) {
          return 4;
        } else {
          return 0;
        }
      }
    }
  }

  bool DiskImage::writeCommandAck() {
    if (m_displayTransmission) {
      qDebug() << "!u" << tr("[%1] Sending [COMMAND ACK] to Atari").arg(deviceName());
    }
    return sio->port()->writeCommandAck();
  }

  bool DiskImage::writeDataAck() {
    if (m_displayTransmission) {
      qDebug() << "!u" << tr("[%1] Sending [DATA ACK] to Atari").arg(deviceName());
    }
    return sio->port()->writeDataAck();
  }

  bool DiskImage::writeCommandNak() {
    qWarning() << "!w" << tr("[%1] Sending [COMMAND NAK] to Atari").arg(deviceName());
    return sio->port()->writeCommandNak();
  }

  bool DiskImage::writeDataNak() {
    qWarning() << "!w" << tr("[%1] Sending [DATA NAK] to Atari").arg(deviceName());
    return sio->port()->writeDataNak();
  }

  bool DiskImage::writeComplete() {
    if (m_displayTransmission) {
      qDebug() << "!u" << tr("[%1] Sending [COMPLETE] to Atari").arg(deviceName());
    }
    return sio->port()->writeComplete();
  }

  bool DiskImage::writeError() {
    qWarning() << "!w" << tr("[%1] Sending [ERROR] to Atari").arg(deviceName());
    return sio->port()->writeError();
  }

  QByteArray DiskImage::readDataFrame(uint size) {
    QByteArray data = sio->port()->readDataFrame(size, false);
    if (m_dumpDataFrame) {
      qDebug() << "!u" << tr("[%1] Receiving %2 bytes from Atari").arg(deviceName()).arg(data.length());
      dumpBuffer((unsigned char *) data.data(), data.length());
    }
    return data;
  }

  bool DiskImage::writeDataFrame(QByteArray data) {
    if (m_dumpDataFrame) {
      qDebug() << "!u" << tr("[%1] Sending %2 bytes to Atari").arg(deviceName()).arg(data.length());
      dumpBuffer((unsigned char *) data.data(), data.length());
    }
    return sio->port()->writeDataFrame(data);
  }

  quint16 DiskImage::getBigEndianWord(QByteArray &array, int offset) {
    return (((quint16) array[offset + 1]) & 0xFF) + ((((quint16) array[offset]) << 8) & 0xFF00);
  }

  quint16 DiskImage::getLittleEndianWord(QByteArray &array, int offset) {
    return (((quint16) array[offset]) & 0xFF) + ((((quint16) array[offset + 1]) << 8) & 0xFF00);
  }

  quint32 DiskImage::getLittleEndianLong(QByteArray &array, int offset) {
    return (((quint32) array[offset]) & 0xFF) + ((((quint32) array[offset + 1]) << 8) & 0xFF00) + ((((quint32) array[offset + 2]) << 16) & 0xFF0000) + ((((quint32) array[offset + 3]) << 24) & 0xFF000000);
  }

  void DiskImage::setLittleEndianWord(QByteArray &array, int offset, quint16 value) {
    array[offset] = (quint8) (value & 0xFF);
    array[offset + 1] = (quint8) ((value >> 8) & 0xFF);
  }

  void DiskImage::setLittleEndianLong(QByteArray &array, int offset, quint32 value) {
    array[offset] = (quint8) (value & 0xFF);
    array[offset + 1] = (quint8) ((value >> 8) & 0xFF);
    array[offset + 2] = (quint8) ((value >> 16) & 0xFF);
    array[offset + 3] = (quint8) ((value >> 24) & 0xFF);
  }

  void DiskImage::fillBuffer(char *line, unsigned char *buf, int len, int ofs, bool dumpAscii) {
    *line = 0;
    if ((len - ofs) >= 16) {
      if (dumpAscii) {
        unsigned char car[16];
        for (int j = 0; j < 16; j++) {
          if ((buf[ofs + j] > 32) && (buf[ofs + j] < 127)) {
            car[j] = buf[ofs + j];
          } else if ((buf[ofs + j] > 160) && (buf[ofs + j] < 255)) {
            car[j] = buf[ofs + j] & 0x7F;
          } else {
            car[j] = ' ';
          }
        }
        snprintf(line, 75, "$%04X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X | %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
                 ofs & 0xFFFF, ((unsigned int) buf[ofs + 0]) & 0xFF, ((unsigned int) buf[ofs + 1]) & 0xFF,
                 ((unsigned int) buf[ofs + 2]) & 0xFF, ((unsigned int) buf[ofs + 3]) & 0xFF, ((unsigned int) buf[ofs + 4]) & 0xFF,
                 ((unsigned int) buf[ofs + 5]) & 0xFF, ((unsigned int) buf[ofs + 6]) & 0xFF, ((unsigned int) buf[ofs + 7]) & 0xFF,
                 ((unsigned int) buf[ofs + 8]) & 0xFF, ((unsigned int) buf[ofs + 9]) & 0xFF, ((unsigned int) buf[ofs + 10]) & 0xFF,
                 ((unsigned int) buf[ofs + 11]) & 0xFF, ((unsigned int) buf[ofs + 12]) & 0xFF, ((unsigned int) buf[ofs + 13]) & 0xFF,
                 ((unsigned int) buf[ofs + 14]) & 0xFF, ((unsigned int) buf[ofs + 15]) & 0xFF, car[0], car[1], car[2], car[3],
                 car[4], car[5], car[6], car[7], car[8], car[9], car[10], car[11], car[12], car[13], car[14], car[15]);
      } else {
        snprintf(line, 55, "$%04X: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
                 ofs & 0xFFFF, ((unsigned int) buf[ofs + 0]) & 0xFF, ((unsigned int) buf[ofs + 1]) & 0xFF,
                 ((unsigned int) buf[ofs + 2]) & 0xFF, ((unsigned int) buf[ofs + 3]) & 0xFF, ((unsigned int) buf[ofs + 4]) & 0xFF,
                 ((unsigned int) buf[ofs + 5]) & 0xFF, ((unsigned int) buf[ofs + 6]) & 0xFF, ((unsigned int) buf[ofs + 7]) & 0xFF,
                 ((unsigned int) buf[ofs + 8]) & 0xFF, ((unsigned int) buf[ofs + 9]) & 0xFF, ((unsigned int) buf[ofs + 10]) & 0xFF,
                 ((unsigned int) buf[ofs + 11]) & 0xFF, ((unsigned int) buf[ofs + 12]) & 0xFF, ((unsigned int) buf[ofs + 13]) & 0xFF,
                 ((unsigned int) buf[ofs + 14]) & 0xFF, ((unsigned int) buf[ofs + 15]) & 0xFF);
      }
    } else if (ofs < len) {
      int nbRemaining = len - ofs;
      memset(line, ' ', 73);
      line[73] = 0;
      snprintf(line, 7, "$%04X:", ofs);
      for (int i = 0; i < nbRemaining; i++) {
        snprintf(&line[strlen(line)], 4, " %02X", ((unsigned int) buf[ofs + i]) & 0xFF);
      }
      if (dumpAscii) {
        for (int i = strlen(line); i < 54; i++) {
          line[i] = ' ';
        }
        strcpy(&line[54], " | ");
        for (int i = 0; i < nbRemaining; i++) {
          if ((buf[ofs + i] > 32) && (buf[ofs + i] < 127)) {
            line[57 + i] = buf[ofs + i];
          } else if ((buf[ofs + i] > 160) && (buf[ofs + i] < 255)) {
            line[57 + i] = buf[ofs + i] & 0x7F;
          }
        }
        line[57 + nbRemaining] = 0;
      }
    }
  }

  void DiskImage::dumpBuffer(unsigned char *buf, int len) {
    for (int i = 0; i < ((len + 15) >> 4); i++) {
      char line[80];
      int ofs = i << 4;
      fillBuffer(line, buf, len, ofs, false);
      qDebug() << "!u" << tr("[%1] §%2").arg(deviceName(), line);
    }
  }

  int DiskImage::getUploadCodeStartAddress(quint8 command, quint16 aux, QByteArray &) {
    if (m_board.isChipOpen()) {
      if (command == 0x4D) {
        return 0x0080;
      } else if (command == 0x75) {
        return 0x1000;
      }
    }
    if (m_board.isHappyEnabled()) {
      if (((command == 0x57) && (aux >= 0x800) && (aux < 0x1380)) || ((command == 0x77) && (aux >= 0x800) && (aux < 0x1380))) {
        if (m_board.hasHappySignature()) {
          return (int) aux;
        }
      }
      if (((command == 0x50) || (command == 0x57) || (command == 0x70) || (command == 0x77)) && (aux >= 0x8000) && (aux <= 0x97FF)) {
        return (int) aux;
      }
    }
    return -1;
  }

  void DiskImage::disassembleCode(QByteArray &data, unsigned short address, bool drive1050, bool happy) {
    // CRC16 is not a good hash but this is only for very few "upload and execute code" commands so this is OK
    // This CRC is used to implement these commands at SIO level in diskimage.cpp.
    // This is useful to make Fuzzy sectors with Super Archiver when hardware emulation level.
    Crc16 crc;
    crc.Reset();
    int len = data.size();
    for (int i = 0; i < len; i++) {
      crc.Add(data[i]);
    }
    if (crc.GetCrc() == 0xF00A) {
      // empty data
      return;
    }
    qDebug() << "!n" << tr("[%1] Disassembly of %2 bytes at $%3 with CRC $%4").arg(deviceName()).arg(len).arg(address, 4, 16, QChar('0')).arg(crc.GetCrc(), 4, 16, QChar('0'));
    char buf[256];
    int offset = 0;
    QByteArray code;
    if (happy) {
      if (address == 0x0800) {
        while (offset < 9) {
          qDebug() << "!u" << tr("[%1] §$%2: %3 %4 %5 ; Happy signature").arg(deviceName()).arg((address + (unsigned short) offset), 4, 16, QChar('0')).arg((unsigned char) data[offset], 2, 16, QChar('0')).arg((unsigned char) data[offset + 1], 2, 16, QChar('0')).arg((unsigned char) data[offset + 2], 2, 16, QChar('0'));
          offset += 3;
        }
        int command = 0x50;
        while (offset < 32) {
          qDebug() << "!u" << tr("[%1] §$%2: %3 %4 %5 JMP $%6 ; Command $%7").arg(deviceName()).arg((address + (unsigned short) offset), 4, 16, QChar('0')).arg((unsigned char) data[offset], 2, 16, QChar('0')).arg((unsigned char) data[offset + 1], 2, 16, QChar('0')).arg((unsigned char) data[offset + 2], 2, 16, QChar('0')).arg(((unsigned char) data[offset + 1] + ((unsigned char) data[offset + 2] << 8)), 4, 16, QChar('0')).arg(command, 2, 16, QChar('0'));
          offset += 3;
          command++;
        }
      } else if ((m_remainingAddress == address) && (m_remainingBytes.size() > 0)) {
        address -= m_remainingBytes.size();
        code.append(m_remainingBytes);
      }
    }
    m_remainingAddress = 0;
    m_remainingBytes.clear();
    code.append(data);
    len = code.size();
    auto codePtr = (unsigned char *) code.data();
    while (offset < len) {
      int lenOpCode = -1;
      if (drive1050) {
        lenOpCode = m_disassembly1050.BuildInstruction(buf, &codePtr[offset], len - offset, address + (unsigned short) offset);
      } else {
        lenOpCode = m_disassembly810.BuildInstruction(buf, &codePtr[offset], len - offset, address + (unsigned short) offset);
      }
      if (lenOpCode == -1) {
        break;
      } else {
        qDebug() << "!u" << tr("[%1] §%2").arg(deviceName(), buf);
      }
      offset += lenOpCode;
    }
    if (offset < len) {
      m_remainingAddress = address + len;
      m_remainingBytes.append(code.right(len - offset));
    }
  }

 bool DiskImage::writeHappyTrack(int trackNumber, bool happy1050) {
   return false;
 }

 bool DiskImage::seekToSector(quint16 sector) {
   if (sector < 1 || sector > m_geometry.sectorCount()) {
      qCritical() << "!e" << tr("[%1] Cannot seek to sector %2: %3").arg(deviceName()).arg(sector).arg(tr("Sector number is out of bounds."));
   }
   qint64 pos = (sector - 1) * m_geometry.bytesPerSector();
   // all sectors in a XFD file have the same size (this is the difference with the ATR format).
   // For example, XFD files are used to store CP/M disk images with 256 bytes boot sectors for Indus GT with RamCharger (not possible with ATR files).
   if ((m_geometry.bytesPerSector() == 256)) {
      if (sector <= 3) {
        pos = (sector - 1) * 128;
      } else {
        pos -= 384;
      }
   }
   if (!file.seek(pos)) {
      qCritical() << "!e" << tr("[%1] Cannot seek to sector %2: %3").arg(deviceName()).arg(sector).arg(file.errorString());
      return false;
   }
   return true;
 }
}