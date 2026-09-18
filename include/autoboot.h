/*
 * autoboot.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef AUTOBOOT_H
#define AUTOBOOT_H

#include "sioworker.h"

class AtariExeChunk {
public:
  int address{0};
  QByteArray data;
};

class AutoBoot : public SioDevice {
  Q_OBJECT

public:
  AutoBoot(const SioWorkerPtr &worker, SioDevice *aOldDevice) : SioDevice(worker) {
    oldDevice = aOldDevice;
    started = loaded = false;
  }
  ~AutoBoot() override;
  void handleCommand(quint8 command, quint8 aux1, quint8 aux2) override;
  void passToOldHandler(quint8 command, quint8 aux1, quint8 aux2);
  bool open(const QString &fileName, bool highSpeed);
  void close();
  void readSector(quint16 sector, QByteArray &data);
  QString deviceName() override;

signals:
  void booterStarted();
  void booterLoaded();
  void blockRead(int current, int all);
  void loaderDone();

private:
  QByteArray bootSectors;
  QList<AtariExeChunk> chunks;
  qint64 sectorCount{};
  SioDevice *oldDevice;
  bool started, loaded;
  bool readExecutable(const QString &fileName);


};

#endif// AUTOBOOT_H
