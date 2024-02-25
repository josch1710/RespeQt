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

#include "serialport.h"

#include <QMutex>
#include <QSharedPointer>
#include <QThread>
#ifndef QT_NO_DEBUG
#include <QFile>
#endif
#include <QVector>
#include <QSharedPointer>

#include <atomic>
#include <memory>

enum SIO_CDEVIC : quint8 {
  DISK_BASE_CDEVIC = 0x31,
  PRINTER_BASE_CDEVIC = 0x40,
  SMART_CDEVIC = 0x45,
  RESPEQT_CLIENT_CDEVIC = 0x46,
  RS232_BASE_CDEVIC = 0x50,
  PCLINK_CDEVIC = 0x6F
};

const quint8 DISK_COUNT = 15;
const quint8 PRINTER_COUNT = 4;

class SioWorker;
using SioWorkerPtr = QSharedPointer<SioWorker>;

class SioDevice : public QObject {
  Q_OBJECT

protected:
  int m_deviceNo;
  QMutex mLock;
  SioWorkerPtr sio;

public:
  explicit SioDevice(SioWorkerPtr worker);
  ~SioDevice() override;
  virtual void handleCommand(const quint8 command, const quint8 aux1, const quint8 aux2) = 0;
  virtual QString deviceName();
  inline void lock() { mLock.lock(); }
  inline bool tryLock() { return mLock.tryLock(); }
  inline void unlock() { mLock.unlock(); }
  inline void setDeviceNo(int no) {
    emit statusChanged(m_deviceNo);
    m_deviceNo = no;
    emit statusChanged(no);
  }
  inline int deviceNo() const { return m_deviceNo; }
signals:
  void statusChanged(int deviceNo);
};

using SioDevicePtr = QSharedPointer<SioDevice>;

class SioWorker : public QThread {
  Q_OBJECT

private:
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
  QMutex *deviceMutex;
#else
QRecursiveMutex *deviceMutex;
#endif
  QVector<SioDevicePtr> devices{256};
  AbstractSerialPortBackendPtr mPort;
  std::atomic_bool mustTerminate;
  bool displayCommandName;
  bool mAutoReconnect;

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
  virtual ~SioWorker();

  bool waitOnPort(unsigned long time = ULONG_MAX);

  void run();

  void setAutoReconnect(bool autoReconnect);
  void installDevice(quint8 no, SioDevicePtr device);
  virtual void uninstallDevice(quint8 no);
  void swapDevices(quint8 d1, quint8 d2);
  SioDevicePtr getDevice(quint8 no);

  QString guessDiskCommand(const quint8 command, const quint8 aux1, const quint8 aux2);
  QString deviceName(int device);
  void setDisplayCommandName(bool display) { displayCommandName = display; }
  static void usleep(unsigned long time) { QThread::usleep(time); }

signals:
  void statusChanged(QString status);

public slots:
  void start(Priority p);
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

private:
  QMutex mustTerminate;
  AbstractSerialPortBackendPtr mPort;
  QList<CassetteRecord> mRecords;

public:
  AbstractSerialPortBackendPtr port() { return mPort; }

  CassetteWorker();
  ~CassetteWorker();

  bool loadCasImage(const QString &fileName);

  bool wait(unsigned long time = ULONG_MAX);

  void run();

  int mTotalDuration;
signals:
  void statusChanged(int remainingTime);

public slots:
  void start(Priority p);
};

#endif// SIOWORKER_H
