/*
 * serialport-unix.h
 *
 * Copyright 2016 TheMontezuma
 * Copyright 2017 blind
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef SERIALPORTUNIX_H
#define SERIALPORTUNIX_H

#include "serialport.h"
#include <atomic>

class StandardSerialPortBackend : public AbstractSerialPortBackend {
  Q_OBJECT

public:
  StandardSerialPortBackend(QObject *parent = 0);
  ~StandardSerialPortBackend();

  static QString defaultPortName();

  bool open() override;
  bool isOpen() override;
  void close() override;
  void cancel() override;
  int speedByte() override;
  QByteArray readCommandFrame() override;
  QByteArray readDataFrame(uint size, bool verbose = true) override;
  bool writeDataFrame(const QByteArray &data) override;
  bool writeCommandAck() override;
  bool writeCommandNak() override;
  bool writeDataAck() override;
  bool writeDataNak() override;
  bool writeComplete() override;
  bool writeError() override;
  bool setSpeed(int speed) override;
  bool writeRawFrame(const QByteArray &data) override;
  void setActiveSioDevices(const QByteArray &data) override;
  int speed() override;
  void forceHighSpeed(int speed) override;

private:
  std::atomic_bool mCanceled;
  bool mHighSpeed;
  int mForceHighSpeed;
  int mHandle;
  int mSpeed;
  int mMethod;
  int mWriteDelay;
  int mCompErrDelay;
  QByteArray mSioDevices;

  bool setNormalSpeed();
  bool setHighSpeed();
  quint8 sioChecksum(const QByteArray &data, uint size);
  QByteArray readRawFrame(uint size, bool verbose = true);
  QString lastErrorMessage();
};

class AtariSioBackend : public AbstractSerialPortBackend {
  Q_OBJECT

public:
  AtariSioBackend(QObject *parent = 0);
  ~AtariSioBackend();

  static QString defaultPortName();

  bool open() override;
  bool isOpen() override;
  void close() override;
  void cancel() override;
  int speedByte() override;
  QByteArray readCommandFrame() override;
  QByteArray readDataFrame(uint size, bool verbose = true) override;
  bool writeDataFrame(const QByteArray &data) override;
  bool writeCommandAck() override;
  bool writeCommandNak() override;
  bool writeDataAck() override;
  bool writeDataNak() override;
  bool writeComplete() override;
  bool writeError() override;
  bool setSpeed(int speed) override;
  bool writeRawFrame(const QByteArray &data) override;
  void setActiveSioDevices(const QByteArray &data) override;
  int speed() override;
  void forceHighSpeed(int speed) override;

private:
  int mHandle, mCancelHandles[2];
  int mSpeed;
  int mMethod;
  QString lastErrorMessage();
};

#endif// SERIALPORTUNIX_H
