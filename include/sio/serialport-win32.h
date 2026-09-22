/*
 * serialport-win32.h
 *
 * Copyright 2016 TheMontezuma
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef SERIALPORTWIN32_H
#define SERIALPORTWIN32_H

#include "sio/serialport.h"
#include <atomic>

namespace SIO {

class StandardSerialPortBackend : public AbstractSerialPortBackend {
  Q_OBJECT

public:
  StandardSerialPortBackend(QObject *parent = nullptr);
  ~StandardSerialPortBackend() override;

  static QString defaultPortName();

  bool open() override;
  bool isOpen() override;
  void close() override;
  void cancel() override;
  int speedByte() override;
  QByteArray readCommandFrame() override;
  QByteArray readDataFrame(uint size, bool isCommandFrame, bool verbose = true) override;
  bool writeDataFrame(const QByteArray &data) override;
  bool writeCommandAck() override;
  bool writeCommandNak() override;
  bool writeDataAck() override;
  bool writeDataNak() override;
  bool writeComplete() override;
  bool writeError() override;
  bool setSpeed(unsigned long speed) override;
  unsigned long speed() override;
  bool writeRawFrame(const QByteArray &data) override;
  void setActiveSioDevices(const QByteArray &data) override;
  void forceHighSpeed(unsigned int speed) override;

private:
  std::atomic_bool mCanceled;
  bool mHighSpeed;
  int mForceHighSpeed;
  void *mHandle, *mCancelHandle;
  int mSpeed;
  int mMethod;
  int mWriteDelay;
  int mCompErrDelay;
  QByteArray mSioDevices;

  bool setNormalSpeed();
  bool setHighSpeed();
  quint8 sioChecksum(const QByteArray &data, uint size);
  QByteArray readRawFrame(size_t size, bool verbose = true);
  QString lastErrorMessage();
};

/* Dummy AtariSIO backend */
// TODO Define only for Linux
class AtariSioBackend : public AbstractSerialPortBackend {
  Q_OBJECT

public:
  static QString defaultPortName();

  AtariSioBackend(QObject *parent = nullptr);
  ~AtariSioBackend() override;
  bool open() override;
  bool isOpen() override;
  void close() override;
  void cancel() override;
  int speedByte() override;
  QByteArray readCommandFrame() override;
  QByteArray readDataFrame(uint size, bool isCommandFrame, bool verbose = true) override;
  bool writeDataFrame(const QByteArray &data) override;
  bool writeCommandAck() override;
  bool writeCommandNak() override;
  bool writeDataAck() override;
  bool writeDataNak() override;
  bool writeComplete() override;
  bool writeError() override;
  bool setSpeed(unsigned long speed) override;
  bool writeRawFrame(const QByteArray &data) override;
  void setActiveSioDevices(const QByteArray &data) override;
  unsigned long speed() override;
  void forceHighSpeed(unsigned int speed) override;
};

} // namespace SIO

#endif// SERIALPORTWIN32_H
