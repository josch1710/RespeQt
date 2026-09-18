/*
 * sioworker.h
 *
 * Copyright 2016 TheMontezuma
 * Copyright 2017 Joseph Zatarski
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef SIOWORKER_H
#define SIOWORKER_H

#include <QMutex>
#include <QSharedPointer>
#include <QThread>
#ifndef QT_NO_DEBUG
#include <QFile>
#endif

#include "serialport.h"
#include <atomic>

enum SIO_CDEVIC : quint8 {
  DISK_BASE_CDEVIC = 0x31,
  PRINTER_BASE_CDEVIC = 0x40,
  SMART_CDEVIC = 0x45,
  RESPEQT_CLIENT_CDEVIC = 0x46,
  RS232_BASE_CDEVIC = 0x50,
  PCLINK_CDEVIC = 0x6F
};

constexpr quint8 DISK_COUNT = 15;
constexpr quint8 PRINTER_COUNT = 4;

class SioWorker;
using SioWorkerPtr = QSharedPointer<SioWorker>;

class SioDevice : public QObject {
  Q_OBJECT

protected:
  int m_deviceNo;
  QMutex mLock;
  SioWorkerPtr sio;

public:
  explicit SioDevice(const SioWorkerPtr& worker);
  ~SioDevice() override;
  virtual void handleCommand(quint8 command, quint8 aux1, quint8 aux2) = 0;
  virtual QString deviceName();
  void lock() { mLock.lock(); }
  bool tryLock() { return mLock.tryLock(); }
  void unlock() { mLock.unlock(); }
  void setDeviceNo(const int no) {
    emit statusChanged(m_deviceNo);
    m_deviceNo = no;
    emit statusChanged(no);
  }
  int deviceNo() const { return m_deviceNo; }
signals:
  void statusChanged(int deviceNo);
};

class SioWorker : public QThread {
  Q_OBJECT

public:
  AbstractSerialPortBackendPtr port() { return mPort; }

  SioWorker();
#ifdef RESPEQT_TEST
  // This is totally ugly, we must get rid of this special ctor
  explicit SioWorker(AbstractSerialPortBackend *port) : QThread(), mPort(port) {
    deviceMutex = nullptr;
    for (int i = 0; i <= 255; i++)
      devices[i] = nullptr;
  }
#endif
  ~SioWorker() override;

  bool waitOnPort(unsigned long time = ULONG_MAX);

  void setAutoReconnect(bool autoReconnect);
  void installDevice(quint8 no, SioDevice *device);
  virtual void uninstallDevice(quint8 no);
  void swapDevices(quint8 d1, quint8 d2);
  SioDevice *getDevice(quint8 no) const;

  QString guessDiskCommand(quint8 command, quint8 aux1, quint8 aux2);
  QString deviceName(int device);
  void setDisplayCommandName(const bool display) { displayCommandName = display; }
  static void usleep(const unsigned long time) { QThread::usleep(time); }

  // QThread creation doesn't work in other way correctly
  void startThread(Priority p);

signals:
  void statusChanged(QString status);

protected:
  void run() override;

private:
  QRecursiveMutex *deviceMutex;
  SioDevice *devices[256] {};
  AbstractSerialPortBackendPtr mPort;
  std::atomic_bool mustTerminate{};
  bool displayCommandName;
  bool mAutoReconnect;

};

class CassetteRecord {
public:
  int baudRate;
  int gapDuration;
  int totalDuration;
  QByteArray data;
};

class CassetteWorker : public QThread {
  Q_OBJECT

public:
  AbstractSerialPortBackendPtr port() { return mPort; }

  CassetteWorker();
  ~CassetteWorker() override;

  bool loadCasImage(const QString &fileName);

  // TODO Look why is does this and how implement in another way
  bool waitForThread(unsigned long time = ULONG_MAX);
  // QThread creation doesn't work in other way correctly
  void startThread(Priority p);

  int mTotalDuration;
signals:
  void statusChanged(int remainingTime);

protected:
  void run() override;

private:
  QMutex mustTerminate;
  AbstractSerialPortBackendPtr mPort;
  QList<CassetteRecord> mRecords;

};

#endif// SIOWORKER_H
