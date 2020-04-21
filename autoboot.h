/*
 * autoboot.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef AUTOBOOT_H
#define AUTOBOOT_H

#include <QFile>

#include "sioworker.h"

class AtariExeChunk
{
public:
    int address;
    QByteArray data;
};

class AutoBoot : public SioDevice
{
    Q_OBJECT

private:
    QByteArray bootSectors;
    QList <AtariExeChunk> chunks;
    int sectorCount;
    SioDevice *oldDevice;
    bool started, loaded;
    bool readExecutable(const QString &fileName);

public:
    AutoBoot(SioWorkerPtr worker, SioDevice *aOldDevice): SioDevice(worker) {oldDevice = aOldDevice; started = loaded = false;}
    ~AutoBoot();
    void handleCommand(const quint8 command, const quint8 aux1, const quint8 aux2) override;
    void passToOldHandler(const quint8 command, const quint8 aux1, const quint8 aux2);
    bool open(const QString &fileName, bool highSpeed);
    void close();
    bool readSector(quint16 sector, QByteArray &data);
    QString deviceName() override;
signals:
    void booterStarted();
    void booterLoaded();
    void blockRead(int current, int all);
    void loaderDone();
};

#endif // AUTOBOOT_H
