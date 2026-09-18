/*
 * serialport.h
 *
 * Copyright 2016, 2017 TheMontezuma
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QByteArray>
#include <QObject>

enum eHandshake {
  HANDSHAKE_RI = 0,
  HANDSHAKE_DSR = 1,
  HANDSHAKE_CTS = 2,
  HANDSHAKE_NO_HANDSHAKE = 3,
  HANDSHAKE_SOFTWARE = 4
};

enum class SerialBackend : int {
  NONE = -1,
  STANDARD = 0,
  SIO_DRIVER = 1,
  NETSIO_DRIVER = 2,
  TEST = 99
};

enum eSIOConstants {
  SIO_ACK = 65,
  SIO_NACK = 78,
  SIO_COMPLETE = 67,
  SIO_ERROR = 69
};

#define SLEEP_FACTOR 10000

class AbstractSerialPortBackend : public QObject {
  Q_OBJECT
  //    Q_ENUMS(MessageType::UiMessageType)
  //    Q_ENUMS(SerialLine)
public:
  explicit AbstractSerialPortBackend(QObject *parent = nullptr);
  ~AbstractSerialPortBackend() override;

  static int baudToDivisor(const unsigned int baud) { return static_cast<int>(1781610.0 / baud / 2 - 7); }
  constexpr static unsigned int divisorToBaud(const int divisor) {
    switch (divisor) {
      case 0:
        return 125000;

      case 1:
        return 110598;

      case 2:
        return 98797;

      default:
        return static_cast<unsigned int>(1781610.0 / (2 * (divisor + 7)));
    }
  }

  virtual bool open() = 0;
  virtual bool isOpen() = 0;
  virtual void close() = 0;
  virtual void cancel() = 0;
  virtual int speedByte() = 0;
  virtual QByteArray readCommandFrame() = 0;
  virtual QByteArray readDataFrame(uint size, bool isCommandFrame, bool verbose = true) = 0;
  virtual bool writeDataFrame(const QByteArray &data) = 0;
  virtual bool writeCommandAck() = 0;
  virtual bool writeCommandNak() = 0;
  virtual bool writeDataAck() = 0;
  virtual bool writeDataNak() = 0;
  virtual bool writeComplete() = 0;
  virtual bool writeError() = 0;
  virtual unsigned long speed() = 0;
  virtual bool setSpeed(unsigned long speed) = 0;
  virtual bool writeRawFrame(const QByteArray &data) = 0;
  virtual void setActiveSioDevices(const QByteArray &data) = 0;
  virtual void forceHighSpeed(unsigned int speed) = 0;

signals:
  void statusChanged(QString status);
};

using AbstractSerialPortBackendPtr = QSharedPointer<AbstractSerialPortBackend>;


#ifdef Q_OS_WIN
#define SERIAL_PORT_LOCATION "\\\\.\\"
// ReSharper disable once CppUnusedIncludeDirective
#include "serialport-win32.h"
#endif
#ifdef Q_OS_UNIX
#define SERIAL_PORT_LOCATION "/dev/"
// ReSharper disable once CppUnusedIncludeDirective
#include "serialport-unix.h"
#endif

#endif// SERIALPORT_H
