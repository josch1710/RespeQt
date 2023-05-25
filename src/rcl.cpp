/*
 * rcl.cpp
 *
 * Copyright 2015, 2017 Joseph Zatarski
 * Copyright 2016, 2017 TheMontezuma
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifdef Q_WS_WIN
#include "windows.h"
#endif

#include "mainwindow.h"
#include "rcl.h"
#include "respeqtsettings.h"
#include "diskimages/diskimage.h"

#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>
#include <QtDebug>

char RCl::rclSlotNo;

// RespeQt Client ()
void RCl::handleCommand(const quint8 command, const quint8 aux1, const quint8 aux2) {
  QByteArray data(6, 0);
  QByteArray fdata(21, 0);
  QDateTime dateTime = QDateTime::currentDateTime();

  switch (command) {

    case 0x91:// list rcl folder(up to 250 files)
    {
      if (!sio->port()->writeCommandAck()) {
        return;
      }

      quint8 list = aux2;
      quint8 offset = aux1;

      if (!list) {
        QByteArray ddata = sio->port()->readDataFrame(32, false);
        if (ddata.isEmpty()) {
          qCritical() << "!e" << tr("[%1] Read data frame failed").arg(deviceName());
          sio->port()->writeDataNak();
          sio->port()->writeError();
          g_fileFilter = "*";
          return;
        }
        sio->port()->writeDataAck();
        sio->port()->writeComplete();
        g_fileFilter = ddata;
        qCritical() << "!i" << tr("[%1] List filter set: [%2]").arg(deviceName(), g_fileFilter);
      } else {
        QByteArray ddata(255, 0);
        quint8 index = 0;
        QString pth = RespeqtSettings::instance()->lastRclDir();
        QDir dir(pth);
        QStringList filters;
        QString fileFilter = g_fileFilter.trimmed();
        (fileFilter == "*" || fileFilter == "") ? filters << "*.atr"
                                                          << "*.xfd"
                                                          << "*.atx"
                                                          << "*.pro"
                                                          << "*.xex"
                                                          << "*.exe"
                                                          << "*.com"
                                                : filters << (fileFilter + ".atr") << (fileFilter + ".xfd") << (fileFilter + ".atx") << (fileFilter + "+.pro") << (fileFilter + ".xex") << (fileFilter + "+.exe") << (fileFilter + "+.com");

        dir.setNameFilters(filters);
        QFileInfoList filelist = dir.entryInfoList();

        QByteArray fn = ("Path: " + pth).toUtf8();
        for (int n = 0; n < fn.length() && n < 37; n++)
          ddata[index++] = fn[n] & 0xff;

        ddata[index++] = 155;

        for (quint8 i = offset; i < filelist.size() && i < 250; ++i) {
          QFileInfo fileInfo = filelist.at(i);
          QString dosfilname = fileInfo.fileName();
          QString atarifilname = toAtariFileName(dosfilname);
          QString atariFilenum = QString(QChar::fromLatin1(i - offset + 0x41));
          QByteArray fnumber = (" " + atariFilenum + " " + atarifilname).toUtf8();
          if (index + fnumber.length() < 252 && i - offset < 16) {
            for (int n = 0; n < fnumber.length(); n++)
              ddata[index++] = fnumber[n] & 0xff;
            ddata[index++] = 155;
            ddata[254] = 0x00;
          } else {
            ddata[254] = i;
            break;
          }
        }

        for (int n = index; n < 253; n++) ddata[index++] = 0x00;
        sio->port()->writeComplete();
        sio->port()->writeDataFrame(ddata);
      }
      return;
    }

    case 0x92:// get slots filename
    {
      if (!sio->port()->writeCommandAck()) {
        return;
      }

      qint8 deviceNo;
      deviceNo = aux1;
      deviceNo = (deviceNo > 9) ? (deviceNo - 16) : deviceNo;

      if (deviceNo >= 0x0 && deviceNo <= 0xF) {
        auto img = qobject_cast<DiskImages::DiskImage *>(sio->getDevice(deviceNo - 1 + DISK_BASE_CDEVIC));
        QString filename = "";
        if (img) {
          auto i = img->originalFileName().lastIndexOf("/");
          if ((i != -1) || (img->originalFileName().mid(0, 14) == "Untitled image"))
            filename = " " + img->originalFileName().right(img->originalFileName().size() - ++i);
        }


        QByteArray fn = filename.toUtf8();
        for (int i = 0; i < 22; i++)
          fdata[i] = (fn.length() > i) ? (fn[i] & 0xff) : 0x00;

        sio->port()->writeComplete();
        sio->port()->writeDataFrame(fdata);
        return;
      }
      sio->port()->writeDataNak();
    }

    break;
    case 0x93:// Send Date/Time
    {
      if (!sio->port()->writeCommandAck()) {
        return;
      }

      data[0] = static_cast<char>(dateTime.date().day());
      data[1] = static_cast<char>(dateTime.date().month());
      data[2] = static_cast<char>(dateTime.date().year() % 100);
      data[3] = static_cast<char>(dateTime.time().hour());
      data[4] = static_cast<char>(dateTime.time().minute());
      data[5] = static_cast<char>(dateTime.time().second());
      qDebug() << "!n" << tr("[%1] Date/time sent to client (%2).").arg(deviceName(), dateTime.toString(Qt::SystemLocaleShortDate));

      sio->port()->writeComplete();
      sio->port()->writeDataFrame(data);
    } break;

    case 0x94:// Swap Disks
    {
      auto swapDisk1 = static_cast<qint8>(aux2);
      auto swapDisk2 = static_cast<qint8>(aux1);
      if (swapDisk1 > 25)
        swapDisk1 -= 16;
      if (swapDisk2 > 25)
        swapDisk2 -= 16;
      if (swapDisk1 > 0 && swapDisk1 <= 15 && swapDisk2 > 0 && swapDisk2 <= 15 && swapDisk1 != swapDisk2) {
        sio->swapDevices(swapDisk1 + DISK_BASE_CDEVIC - 1, swapDisk2 + DISK_BASE_CDEVIC - 1);
        RespeqtSettings::instance()->swapImages(swapDisk1 - 1, swapDisk2 - 1);
        qDebug() << "!n" << tr("[%1] Swapped disk %2 with disk %3.").arg(deviceName()).arg(swapDisk2).arg(swapDisk1);
        sio->port()->writeCommandAck();
      } else {
        sio->port()->writeCommandNak();
        qDebug() << "!e" << tr("[%1] Invalid swap request for drives: (%2)-(%3).").arg(deviceName()).arg(swapDisk2).arg(swapDisk1);
      }
      sio->port()->writeComplete();
    } break;

    case 0x95:// Unmount Disk(s)
    {
      auto unmountDisk = static_cast<qint8>(aux1);
      if (unmountDisk == -6)
        unmountDisk = 0;// All drives
      if (unmountDisk > 25)
        unmountDisk -= 16;// Drive 10-15
      if (unmountDisk >= 0 && unmountDisk <= 15) {
        if (unmountDisk == 0) {
          // Eject All disks
          int toBeSaved = 0;
          for (int i = 0; i <= 14; i++) {//
            auto img = qobject_cast<DiskImages::DiskImage *>(sio->getDevice(static_cast<quint8>(i + DISK_BASE_CDEVIC)));
            if (img && img->isModified()) {
              toBeSaved++;
            }
          }
          if (!toBeSaved) {
            for (int i = 14; i >= 0; i--) {
              auto img = qSharedPointerCast<DiskImages::DiskImage>(sio->getDevice(static_cast<quint8>(i + DISK_BASE_CDEVIC)));
              sio->uninstallDevice(static_cast<quint8>(i + DISK_BASE_CDEVIC));
              img.reset();
              RespeqtSettings::instance()->unmountImage(i);
              qDebug() << "!n" << tr("[%1] Unmounted disk %2").arg(deviceName()).arg(i + 1);
            }
            qDebug() << "!n" << tr("[%1] ALL images were remotely unmounted").arg(deviceName());
            sio->port()->writeCommandAck();
          } else {
            sio->port()->writeCommandNak();
            qDebug() << "!e" << tr("[%1] Can not remotely unmount ALL images due to pending changes.").arg(deviceName());
          }
        } else {
          // Single Disk Eject
          auto img = qobject_cast<DiskImages::DiskImage *>(sio->getDevice(static_cast<quint8>(unmountDisk - 1 + DISK_BASE_CDEVIC)));

          if (img && img->isModified()) {
            sio->port()->writeCommandNak();
            qDebug() << "!e" << tr("[%1] Can not remotely unmount disk %2 due to pending changes.").arg(deviceName()).arg(unmountDisk);
          } else {
            sio->uninstallDevice(static_cast<quint8>(unmountDisk - 1 + DISK_BASE_CDEVIC));
            img.reset();
            RespeqtSettings::instance()->unmountImage(unmountDisk - 1);
            qDebug() << "!n" << tr("[%1] Remotely unmounted disk %2").arg(deviceName()).arg(unmountDisk);
          }
          sio->port()->writeCommandAck();
        }
      } else {
        sio->port()->writeCommandNak();
        qDebug() << "!e" << tr("[%1] Invalid drive number: %2 for remote unmount").arg(deviceName()).arg(unmountDisk);
      }
      sio->port()->writeComplete();
    } break;

    case 0x96:// Mount Disk Image
    case 0x97:// Create and Mount a new Disk Image
    {
      if (!sio->port()->writeCommandAck()) {// TODO MOVE
        return;
      }
      // If no Folder Image has ever been mounted abort the command as we won't
      // know which folder to use to remotely create/mount an image file.
      if (RespeqtSettings::instance()->lastFolderImageDir() == "") {
        qCritical() << "!e" << tr("[%1] RespeQt can't determine the folder where the image file must be created/mounted!").arg(deviceName());
        qCritical() << "!e" << tr("[%1] Mount a Folder Image at least once before issuing a remote mount command.").arg(deviceName());
        sio->port()->writeDataNak();
        sio->port()->writeError();
        return;
      }
      // Get the name of the image file
      int len;
      if (command == 0x96) {
        len = 12;
      } else {
        len = 14;
      }
      if (aux1 == 0 && aux2 == 0) {
        QByteArray _data(len, 0);
        _data = sio->port()->readDataFrame(static_cast<uint>(len), false);

        if (_data.isEmpty()) {
          qCritical() << "!e" << tr("[%1] Read data frame failed").arg(deviceName());
          sio->port()->writeDataNak();
          sio->port()->writeError();
          return;
        }
        imageFileName = _data;
        if (command == 0x97) {// Create new image file first
          int i, type;
          bool ok;
          i = imageFileName.lastIndexOf(".");
          type = imageFileName.midRef(i + 1).toInt(&ok, 10);
          if (ok && (type < 1 || type > 6)) ok = false;
          if (!ok) {
            qCritical() << "!e" << tr("[%1] Invalid image file attribute: %2").arg(deviceName()).arg(type);
            sio->port()->writeDataNak();
            sio->port()->writeError();
            return;
          }
          imageFileName = imageFileName.left(i);
          QFile file(RespeqtSettings::instance()->lastRclDir() + "/" + imageFileName);
          if (!file.open(QIODevice::WriteOnly)) {
            qCritical() << "!e" << tr("[%1] Can not create PC File: %2").arg(deviceName(), imageFileName);
            sio->port()->writeDataNak();
            sio->port()->writeError();
            return;
          }
          sio->port()->writeDataAck();

          int fileSize;
          QByteArray fileData;
          switch (type) {
            case 1:// Single Density
            {
              fileSize = 92160;
              fileData.resize(fileSize + 16);
              fileData.fill(0);
              fileData[2] = static_cast<char>(0x80);
              fileData[3] = static_cast<char>(0x16);
              fileData[4] = static_cast<char>(0x80);
            } break;
            case 2:// Enhanced Density
            {
              fileSize = 133120;
              fileData.resize(fileSize + 16);
              fileData.fill(0);
              fileData[2] = static_cast<char>(0x80);
              fileData[3] = static_cast<char>(0x20);
              fileData[4] = static_cast<char>(0x80);
            } break;
            case 3:// Double Density
            {
              fileSize = 183936;
              fileData.resize(fileSize + 16);
              fileData.fill(0);
              fileData[2] = static_cast<char>(0xE8);
              fileData[3] = static_cast<char>(0x2C);
              fileData[4] = static_cast<char>(0x00);
              fileData[5] = static_cast<char>(0x01);
            } break;
            case 4:// Double Sided, Double Density
            {
              fileSize = 368256;
              fileData.resize(fileSize + 16);
              fileData.fill(0);
              fileData[2] = static_cast<char>(0xE8);
              fileData[3] = static_cast<char>(0x59);
              fileData[4] = static_cast<char>(0x00);
              fileData[5] = static_cast<char>(0x01);
            } break;
            case 5:// Double Density Hard Disk
            {
              fileSize = 16776576;
              fileData.resize(fileSize + 16);
              fileData.fill(0);
              fileData[2] = static_cast<char>(0xD8);
              fileData[3] = static_cast<char>(0xFF);
              fileData[4] = static_cast<char>(0x00);
              fileData[5] = static_cast<char>(0x01);
              fileData[6] = static_cast<char>(0x0F);
            } break;
            case 6:// Quad Density Hard Disk
            {
              fileSize = 33553576;
              fileData.resize(fileSize + 16);
              fileData.fill(0);
              fileData[2] = static_cast<char>(0xE0);
              fileData[3] = static_cast<char>(0xFF);
              fileData[4] = static_cast<char>(0x00);
              fileData[5] = static_cast<char>(0x02);
              fileData[6] = static_cast<char>(0x1F);
            } break;
          }
          fileData[0] = static_cast<char>(0x96);
          fileData[1] = static_cast<char>(0x02);
          file.write(fileData);
          fileData.clear();
          file.close();

        }// Cmd 0x97 -- Create new image file first

        sio->port()->writeDataAck();

        imageFileName = "*" + toDosFileName(imageFileName);


        // Ask the MainWindow for the next available slot number
        mutex.lock();
        emit findNewSlot(0, true);

      } else {

        if (mutex.tryLock()) {
          // Return the last mounted drive number
          QByteArray _data(1, 0);
          _data[0] = rclSlotNo;
          mutex.unlock();
          sio->port()->writeComplete();
          sio->port()->writeDataFrame(_data);
        } else {
          sio->port()->writeCommandNak();
        }
      }
    } break;

    case 0x98:// Auto-Commit toggle
    {
      if (!sio->port()->writeCommandAck()) {
        return;
      }
      qint8 commitDisk;
      bool commitOnOff;
      commitDisk = aux1 - 1;
      commitOnOff = aux2 ? false : true;

      if (commitDisk > 9) commitDisk -= 16;
      if (commitDisk != -7 && (commitDisk < 0 || commitDisk > 14)) {
        sio->port()->writeCommandNak();
        return;
      }

      // All disks or a given disk
      if (commitDisk == -7) {
        for (int i = 0; i < 15; i++) {
          emit toggleAutoCommit(i, commitOnOff);
        }
      } else {
        emit toggleAutoCommit(commitDisk, commitOnOff);
      }

      sio->port()->writeComplete();
    } break;

    case 0x99:// save disks
    {
      if (!sio->port()->writeCommandAck()) {
        return;
      }

      int diskSaved = 0;
      auto deviceNo = static_cast<qint8>(aux2);

      if (deviceNo == -6) deviceNo = 0;// All drives
      if (deviceNo > 9) deviceNo -= 16;// Drive 10-15
      if (deviceNo >= 0 and deviceNo <= 15) {
        if (deviceNo == 0) {
          // Eject All disks

          for (int i = 0; i <= 14; i++) {//
            auto img = qobject_cast<DiskImages::DiskImage *>(sio->getDevice(i + DISK_BASE_CDEVIC));
            if (img && img->isModified() && !img->isUnnamed()) {
              img->save();
              qDebug() << "!n" << tr("[%1] Saved disk %2").arg(deviceName()).arg(i + 1);
              diskSaved++;
            }
          }

        } else {
          // Single Disk save
          auto img = qobject_cast<DiskImages::DiskImage *>(sio->getDevice(deviceNo - 1 + DISK_BASE_CDEVIC));

          if (img && img->isModified() && !img->isUnnamed()) {
            img->save();
            qDebug() << "!n" << tr("[%1] Saved disk %2").arg(deviceName()).arg(deviceNo);
            diskSaved++;
          }
        }
        if (!diskSaved)
          sio->port()->writeCommandNak();

      } else {
        sio->port()->writeCommandNak();
        qDebug() << "!e" << tr("[%1] Invalid drive number: %2 for remote unmount").arg(deviceName()).arg(deviceNo);
      }
      sio->port()->writeComplete();
    } break;


    case 0x9A:// Mount slot and boot
    {
      if (!sio->port()->writeCommandAck()) {
        return;
      }

      // If no Folder Image has ever been mounted abort the command as we won't
      // know which folder to use to remotely create/mount an image file.
      if (RespeqtSettings::instance()->lastRclDir() == "") {
        qCritical() << "!e" << tr("[%1] RespeQt can't determine the folder where the image file must be created/mounted!").arg(deviceName());
        qCritical() << "!e" << tr("[%1] Mount a Folder Image at least once before issuing a remote mount command.").arg(deviceName());
        sio->port()->writeDataNak();
        sio->port()->writeError();
        return;
      }
      // Get the name of the image file


      QByteArray _data(12, 0);
      _data = sio->port()->readDataFrame(12, false);
      if (_data.isEmpty()) {
        qCritical() << "!e" << tr("[%1] Read data frame failed").arg(deviceName());
        sio->port()->writeDataNak();
        sio->port()->writeError();
        return;
      }

      sio->port()->writeDataAck();
      sio->port()->writeComplete();

      imageFileName = _data;
      imageFileName = imageFileName.trimmed();
      auto isDiskImage = !(imageFileName.endsWith("XEX") || imageFileName.endsWith("EXE") || imageFileName.endsWith("COM"));

      if (isDiskImage) {
        imageFileName = "*" + toDosFileName(imageFileName);
        emit mountFile(0, imageFileName);
      } else {
        imageFileName = toDosFileName(imageFileName);
        emit bootExe(imageFileName);
      }
    } break;


    case 0x9B:// happy toggle
    {
      if (!sio->port()->writeCommandAck()) {
        return;
      }
      auto happyDisk = static_cast<qint8>(aux1) - 1;
      auto happyOnOff = static_cast<qint8>(aux2) ? false : true;

      if (happyDisk > 9) happyDisk -= 16;
      if (happyDisk != -7 && (happyDisk < 0 || happyDisk > 14)) {
        sio->port()->writeCommandNak();
        return;
      }

      // All disks or a given disk
      if (happyDisk == -7) {
        for (int i = 0; i < 15; i++) {
          emit toggleHappy(i, happyOnOff);
        }
      } else {
        emit toggleHappy(happyDisk, happyOnOff);
      }

      sio->port()->writeComplete();
    } break;


    case 0x9C://  chip toggle
    {
      if (!sio->port()->writeCommandAck()) {
        return;
      }
      auto chipDisk = static_cast<qint8>(aux1) - 1;
      auto chipOnOff = aux2 ? false : true;

      if (chipDisk > 9) chipDisk -= 16;
      if (chipDisk != -7 && (chipDisk < 0 || chipDisk > 14)) {
        sio->port()->writeCommandNak();
        return;
      }

      // All disks or a given disk
      if (chipDisk == -7) {
        for (int i = 0; i < 15; i++) {
          emit toggleChip(i, chipOnOff);
        }
      } else {
        emit toggleChip(chipDisk, chipOnOff);
      }

      sio->port()->writeComplete();
    } break;


    default:
      // Invalid Command
      sio->port()->writeCommandNak();
      qWarning() << "!e" << tr("[%1] command: $%2, aux1: $%3, aux2: $%4 NAKed.").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
  }
}

// Get the next slot number available for mounting a disk image
void RCl::gotNewSlot(int slot) {
  mutex.lock();
  rclSlotNo = static_cast<char>(slot);
  mutex.unlock();

  // Ask the MainWindow to mount the file
  emit mountFile(slot, imageFileName);
}

void RCl::fileMounted(bool mounted) {
  if (mounted) {
    sio->port()->writeComplete();
    qDebug() << "!n" << tr("[%1] Image %2 mounted").arg(deviceName(), imageFileName.mid(1, imageFileName.size() - 1));
  } else {
    sio->port()->writeDataNak();
  }
}


QString RCl::toAtariFileName(QString dosFileName) {
  QString name = "";
  QString ext = "";
  QString filename = dosFileName.toUpper();
  int t = filename.lastIndexOf(".");
  if (t > 0) {
    name = filename.left(t);
    ext = filename.right(filename.length() - t - 1);
  }
  name.remove(QRegExp("[^A-Z0-9_]"));
  name = (name.length() > 8) ? name.left(5) + "$" + name.right(2) : name;
  ext.remove(QRegExp("[^A-Z0-9_]"));
  return name + "." + ext;
}


QString RCl::toDosFileName(QString atariFileName) {
  QString pth = RespeqtSettings::instance()->lastRclDir();
  QDir dir(pth);
  QStringList filters;
  filters << "*.atr"
          << "*.xfd"
          << "*.atx"
          << "*.pro"
          << "*.xex"
          << "*.exe"
          << "*.com";
  dir.setNameFilters(filters);
  QFileInfoList list = dir.entryInfoList();
  for (int i = 0; i < list.size(); ++i) {
    QFileInfo fileInfo = list.at(i);
    QString dosFilename = fileInfo.fileName();
    if (toAtariFileName(dosFilename) == atariFileName)
      return dosFilename;
  }
  return "";
}
