/*
 * serialport-unix.cpp
 *
 * Copyright 2015 DrVenkman
 * Copyright 2015 Joseph Zatarski
 * Copyright 2016, 2017 TheMontezuma
 * Copyright 2017 josch1710
 * Copyright 2017 blind
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "atarisio.h"
#include "respeqtsettings.h"
#include "sio/serialport.h"
#include "sio/sioworker.h"
#include "sio/siorecorder.h"
#include <QTime>
#include <QtDebug>

#include <algorithm>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#ifdef Q_OS_UNIX
#ifdef Q_OS_LINUX
//#include <stropts.h>
#include <linux/serial.h>
#include <termio.h>
#endif

#ifdef Q_OS_MAC
#include <sys/ioctl.h>
#include <termios.h>
#define IOSSIOSPEED _IOW('T', 2, speed_t)
#endif
#endif

namespace SIO {

AbstractSerialPortBackend::AbstractSerialPortBackend(QObject *parent)
    : QObject(parent) {}

AbstractSerialPortBackend::~AbstractSerialPortBackend() = default;

StandardSerialPortBackend::StandardSerialPortBackend(QObject *parent)
  : AbstractSerialPortBackend(parent), mHighSpeed(false), mSpeed(0), mMethod(0), mWriteDelay(0), mCompErrDelay(0)
{
  mHandle = -1;
  mForceHighSpeed = 0;
}

StandardSerialPortBackend::~StandardSerialPortBackend() {
  if (StandardSerialPortBackend::isOpen()) {
    StandardSerialPortBackend::close();
  }
}

#ifdef Q_OS_LINUX
QString StandardSerialPortBackend::defaultPortName() {
  return QString("ttyS0");
}
#endif

#ifdef Q_OS_MAC
QString StandardSerialPortBackend::defaultPortName() {
  return {"tty.usbserial"};
}
#endif

bool StandardSerialPortBackend::open() {
  if (isOpen()) {
    close();
  }

  QString name(SERIAL_PORT_LOCATION);
  name.append(RespeqtSettings::instance()->serialPortName());
  mMethod = RespeqtSettings::instance()->serialPortHandshakingMethod();
  mWriteDelay = static_cast<unsigned long>(SLEEP_FACTOR * RespeqtSettings::instance()->serialPortWriteDelay());
  mCompErrDelay = RespeqtSettings::instance()->serialPortCompErrDelay();

  mHandle = ::open(name.toLocal8Bit().constData(), O_RDWR | O_NOCTTY | O_NDELAY);

  if (mHandle < 0) {
    qCritical() << "!e" << tr("Cannot open serial port '%1': %2").arg(name, lastErrorMessage());
    return false;
  }

  if (mMethod != HANDSHAKE_SOFTWARE) {
    int status;
    if (ioctl(mHandle, TIOCMGET, &status) < 0) {
      qCritical() << "!e" << tr("Cannot get serial port status");
      return false;
    }
    status |= TIOCM_DTR | TIOCM_RTS;
    if (ioctl(mHandle, TIOCMSET, &status) < 0) {
      qCritical() << "!e" << tr("Cannot set RTS and CTS lines in serial port '%1': %2").arg(name, lastErrorMessage());
      return false;
    }
  }

  mCanceled = false;

  if (!setNormalSpeed()) {
    close();
    return false;
  }

  QString m;
  switch (mMethod) {
    case HANDSHAKE_RI:
      m = "RI";
      break;
    case HANDSHAKE_DSR:
      m = "DSR";
      break;
    case HANDSHAKE_CTS:
      m = "CTS";
      break;
    case HANDSHAKE_NO_HANDSHAKE:
      m = "NO";
      break;
    case HANDSHAKE_SOFTWARE:
    default:
      m = "SOFTWARE (SIO2BT)";
      break;
  }
  /* Notify the user that emulation is started */
  qWarning() << "!i" << tr("Emulation started through standard serial port backend on '%1' with %2 handshaking.").arg(RespeqtSettings::instance()->serialPortName(), m);
  return true;
}

bool StandardSerialPortBackend::isOpen() {
  return mHandle >= 0;
}

void StandardSerialPortBackend::close() {
  cancel();
  if (::close(mHandle)) {
    qCritical() << "!e" << tr("Cannot close serial port: %1").arg(lastErrorMessage());
  }
  mHandle = -1;
}

void StandardSerialPortBackend::cancel() {
  mCanceled = true;
}

int StandardSerialPortBackend::speedByte() {
  if (RespeqtSettings::instance()->serialPortHandshakingMethod() == HANDSHAKE_SOFTWARE) {
    return 0x28;// standard speed (19200)
  }
  if (mForceHighSpeed != 0) {
    return baudToDivisor(mForceHighSpeed);// speed when SuperArchiver/BitWriter is active
  }
  if (RespeqtSettings::instance()->serialPortUsePokeyDivisors()) {
    return RespeqtSettings::instance()->serialPortPokeyDivisor();
  }
  int speed = 0x08;
  switch (RespeqtSettings::instance()->serialPortMaximumSpeed()) {
  default:
  case 0:
    speed = 0x28;
    break;
  case 1:
    speed = 0x10;
    break;
  case 2:
    speed = 0x08;
    break;
  }
  return speed;
}

void StandardSerialPortBackend::forceHighSpeed(const unsigned int speed) {
  mForceHighSpeed = speed;
}

bool StandardSerialPortBackend::setNormalSpeed() {
  mHighSpeed = false;
  return setSpeed(19200);
}

bool StandardSerialPortBackend::setHighSpeed() {
  mHighSpeed = true;
  if (mForceHighSpeed != 0) {
    return setSpeed(mForceHighSpeed);// used to force 52400 when SuperArchiver/BitWriter is active
  }
  if (RespeqtSettings::instance()->serialPortUsePokeyDivisors()) {
    return setSpeed(divisorToBaud(RespeqtSettings::instance()->serialPortPokeyDivisor()));
  }
  auto speed {57600U};
  switch (RespeqtSettings::instance()->serialPortMaximumSpeed()) {
  default:
  case 0:
    speed = 19200;
    break;
  case 1:
    speed = 38400;
    break;
  case 2:
    speed = 57600;
    break;
  }
  return setSpeed(speed);
}

#ifdef Q_OS_LINUX
bool StandardSerialPortBackend::setSpeed(int speed) {
  termios tios;
  struct serial_struct ss;

  tcgetattr(mHandle, &tios);
  tios.c_cflag &= ~CSTOPB;
  cfmakeraw(&tios);
  switch (speed) {
    case 600:
      cfsetispeed(&tios, B600);
      cfsetospeed(&tios, B600);
      break;
    case 19200:
      cfsetispeed(&tios, B19200);
      cfsetospeed(&tios, B19200);
      break;
    case 38400:
      // reset special handling of 38400bps back to 38400
      ioctl(mHandle, TIOCGSERIAL, &ss);
      ss.flags &= ~ASYNC_SPD_MASK;
      ioctl(mHandle, TIOCSSERIAL, &ss);

      cfsetispeed(&tios, B38400);
      cfsetospeed(&tios, B38400);
      break;
    case 57600:
      cfsetispeed(&tios, B57600);
      cfsetospeed(&tios, B57600);
      break;
    default:
      // configure port to use custom speed instead of 38400
      ioctl(mHandle, TIOCGSERIAL, &ss);
      ss.flags = (ss.flags & ~ASYNC_SPD_MASK) | ASYNC_SPD_CUST;
      ss.custom_divisor = (ss.baud_base + (speed / 2)) / speed;
      int customSpeed = ss.baud_base / ss.custom_divisor;

      if (customSpeed < speed * 98 / 100 || customSpeed > speed * 102 / 100) {
        qCritical() << "!e" << tr("Cannot set serial port speed to %1: %2").arg(speed).arg(tr("Closest possible speed is %2.").arg(customSpeed));
        return false;
      }

      ioctl(mHandle, TIOCSSERIAL, &ss);

      cfsetispeed(&tios, B38400);
      cfsetospeed(&tios, B38400);
      break;
  }

  /* Set serial port state */
  if (tcsetattr(mHandle, TCSANOW, &tios) != 0) {
    qCritical() << "!e" << tr("Cannot set serial port speed to %1: %2").arg(speed).arg(lastErrorMessage());
    return false;
  }

  emit statusChanged(tr("%1 bits/sec").arg(speed));
  qWarning() << "!i" << tr("Serial port speed set to %1.").arg(speed);
  mSpeed = speed;
  return true;
}
#endif

#ifdef Q_OS_MAC
bool StandardSerialPortBackend::setSpeed(const unsigned long speed) {
  termios tios{};

  tcgetattr(mHandle, &tios);
  tios.c_cflag &= static_cast<tcflag_t>(~CSTOPB);
  cfmakeraw(&tios);
  tios.c_cflag |= CREAD | CLOCAL;// turn on READ
  tios.c_cflag |= CS8;

  tios.c_cc[VMIN] = 0;
  tios.c_cc[VTIME] = 10;// 1 sec timeout

  /* Default speed */
  cfsetispeed(&tios, B19200);
  cfsetospeed(&tios, B19200);

  /* Set serial port state */
  if (tcsetattr(mHandle, TCSANOW, &tios) != 0) {
    qCritical() << "!e" << tr("Cannot set serial port speed to %1: %2").arg(19200).arg(lastErrorMessage());
    return false;
  }

  /* Set requested speed */
  speed_t spd = speed;
  if (ioctl(mHandle, IOSSIOSPEED, &spd) < 0) {
    qCritical() << "!e" << tr("Failed to set serial port speed to %1").arg(speed);
    return false;
  }

  emit statusChanged(tr("%1 bits/sec").arg(speed));
  qWarning() << "!i" << tr("Serial port speed set to %1.").arg(speed);
  mSpeed = speed;
  return true;
}
#endif

unsigned long StandardSerialPortBackend::speed() {
  return mSpeed;
}

QByteArray StandardSerialPortBackend::readCommandFrame() {
  QByteArray data;

  if (mMethod == HANDSHAKE_SOFTWARE) {
    if (tcflush(mHandle, TCIFLUSH) != 0) {
      qCritical() << "!e" << tr("Cannot clear serial port read buffer: %1").arg(lastErrorMessage());
      return data;
    }

    constexpr int size = 4;
    quint8 expected = 0;
    quint8 got = 1;
    do {
      char c;
      if (1 == ::read(mHandle, &c, 1)) {
        data.append(c);
        if (data.size() == size + 2) {
          data.remove(0, 1);
        }
        if (data.size() == size + 1) {
          for (int i = 0; i < mSioDevices.size(); i++) {
            if (data.at(0) == mSioDevices[i]) {
              expected = static_cast<quint8>(data.at(size));
              got = sioChecksum(data, size);
              break;
            }
          }
        }
      } else {
// avoid high CPU load in idle state
#if defined Q_OS_UNIX || defined Q_OS_MAC
        QThread::usleep(300);
#endif
      }
    } while (got != expected && !mCanceled);

    if (got == expected) {
      data.resize(size);

      if (const auto recorder = SioRecorder::instance(); recorder->isSnapshotRunning())
        // ReSharper disable once CppRedundantCastExpression
        recorder->writeSnapshotCommandFrame(static_cast<quint8>(data[0]), static_cast<quint8>(data[1]), static_cast<quint8>(data[2]), static_cast<quint8>(data[3]));

      // After sending the last byte of the command frame
      // ATARI does not drop the command line immediately.
      // Within this small time window ATARI is not able to process the ACK byte.
      // For the "software handshake" approach, we need to wait here a little bit.
      QThread::usleep(500);
    } else {
      data.clear();
    }
  } else {
    // RI/DSR/CTS/- handshake

    int mask;
    switch (mMethod) {
      case HANDSHAKE_RI:
        mask = TIOCM_RI;
        break;
      case HANDSHAKE_DSR:
        mask = TIOCM_DSR;
        break;
      case HANDSHAKE_CTS:
      default:
        mask = TIOCM_CTS;
        break;
    }

    int status;
    int retries = 0, totalRetries = 0;

    do {
      data.clear();

      if (mMethod == HANDSHAKE_NO_HANDSHAKE) {
        int bytes;
        do {
          ioctl(mHandle, FIONREAD, &bytes);
#ifdef Q_OS_UNIX
          QThread::yieldCurrentThread();
#endif

#ifdef Q_OS_MAC
          QThread::usleep(300);
#endif
        } while (bytes == 0 && !mCanceled);
      } else {
        // RI/DSR/CTS handshake
        /* First, wait until command line goes off */
        do {
          if (ioctl(mHandle, TIOCMGET, &status) < 0) {
            qCritical() << "!e" << tr("Cannot retrieve serial port status: %1").arg(lastErrorMessage());
            return data;
          }
          if (status & mask) {
#ifdef Q_OS_UNIX
            QThread::yieldCurrentThread();// Venkman 07132015 OS definition blocks added
#endif

#ifdef Q_OS_MAC
            QThread::usleep(500);
#endif
          }
        } while (status & mask && !mCanceled);
        /* Now wait for it to go on again */
        do {
          if (ioctl(mHandle, TIOCMGET, &status) < 0) {
            qCritical() << "!e" << tr("Cannot retrieve serial port status: %1").arg(lastErrorMessage());
            return data;
          }
          if (!(status & mask)) {
#ifdef Q_OS_UNIX
            QThread::yieldCurrentThread();// Venkman 07132015 OS definition blocks added
#endif

#ifdef Q_OS_MAC
            QThread::usleep(500);
#endif
          }
        } while (!(status & mask) && !mCanceled);

        if (tcflush(mHandle, TCIFLUSH) != 0) {
          qCritical() << "!e" << tr("Cannot clear serial port read buffer: %1").arg(lastErrorMessage());
          return data;
        }
      }

      if (mCanceled) {
        return data;
      }

      data = readDataFrame(4, true);

      if (!data.isEmpty()) {
        if (mMethod != HANDSHAKE_NO_HANDSHAKE) {
          // wait for command line to go off
          do {
            if (ioctl(mHandle, TIOCMGET, &status) < 0) {
              qCritical() << "!e" << tr("Cannot retrieve serial port status: %1").arg(lastErrorMessage());
              return data;
            }
          } while (status & mask && !mCanceled);
        } else {
          // After sending the last byte of the command frame
          // ATARI does not drop the command line immediately.
          // Within this small time window ATARI is not able to process the ACK byte.
          // For the "no handshake" approach, we need to wait here a little bit.
          QThread::usleep(500);
        }
        break;
      }
      retries++;
      totalRetries++;
      if (retries == 2) {
        retries = 0;
        if (mHighSpeed) {
          setNormalSpeed();
        } else {
          setHighSpeed();
        }
      }
    } while (totalRetries < 100);
    //} while (true);
  }
  return data;
}

QByteArray StandardSerialPortBackend::readDataFrame(const uint size, const bool isCommandFrame, const bool verbose) {
  QByteArray data = readRawFrame(size + 1, verbose);
  if (data.isEmpty()) {
    return data;
  }

  const auto expected = static_cast<quint8>(data.at(static_cast<int>(size)));
  const auto got = sioChecksum(data, size);
  if (expected == got) {
    data.resize(static_cast<int>(size));

    if (const auto recorder = SioRecorder::instance(); recorder->isSnapshotRunning()) {
      if (isCommandFrame) {
        // ReSharper disable CppRedundantCastExpression
        recorder->writeSnapshotCommandFrame(static_cast<quint8>(data[0]), static_cast<quint8>(data[1]), static_cast<quint8>(data[2]), static_cast<quint8>(data[3]));
        // ReSharper enable CppRedundantCastExpression
      } else {
        recorder->writeSnapshotDataFrame(data);
      }
    }

    return data;
  }
  if (verbose) {
    qWarning() << "!w" << tr("Data frame checksum error, expected: %1, got: %2. (%3)").arg(expected).arg(got).arg(QString(data.toHex()));
  }
  data.clear();
  return data;
}

bool StandardSerialPortBackend::writeDataFrame(const QByteArray &data) {
  QByteArray copy(data);
  copy.resize(copy.size() + 1);
  copy[copy.size() - 1] = static_cast<char>(sioChecksum(copy, static_cast<uint>(copy.size() - 1)));
  if (mMethod == HANDSHAKE_SOFTWARE) SioWorker::usleep(mWriteDelay);
  SioWorker::usleep(50);
  return writeRawFrame(copy);
}

bool StandardSerialPortBackend::writeCommandAck() {
  return writeRawFrame(QByteArray(1, SIO_ACK));
}

bool StandardSerialPortBackend::writeCommandNak() {
  return writeRawFrame(QByteArray(1, SIO_NACK));
}

bool StandardSerialPortBackend::writeDataAck() {
  return writeRawFrame(QByteArray(1, SIO_ACK));
}

bool StandardSerialPortBackend::writeDataNak() {
  return writeRawFrame(QByteArray(1, SIO_NACK));
}

bool StandardSerialPortBackend::writeComplete() {
  if (mMethod == HANDSHAKE_SOFTWARE) SioWorker::usleep(mWriteDelay);
  else
    SioWorker::usleep(mCompErrDelay);
  return writeRawFrame(QByteArray(1, SIO_COMPLETE));
}

bool StandardSerialPortBackend::writeError() {
  if (mMethod == HANDSHAKE_SOFTWARE) SioWorker::usleep(mWriteDelay);
  else
    SioWorker::usleep(mCompErrDelay);
  return writeRawFrame(QByteArray(1, SIO_ERROR));
}

// ReSharper disable once CppMemberFunctionMayBeStatic
quint8 StandardSerialPortBackend::sioChecksum(const QByteArray &data, const uint size) { // NOLINT(*-convert-member-functions-to-static)
  uint sum = 0;

  for (int i = 0; i < static_cast<int>(size); i++) {
    sum += static_cast<quint8>(data.at(i));
    if (sum > 255) {
      sum -= 255;
    }
  }

  return static_cast<quint8>(sum);
}

QByteArray StandardSerialPortBackend::readRawFrame(const size_t size, bool /*verbose*/) {
  QByteArray data;
  data.resize(static_cast<int>(size));

  uint total = 0;
  ssize_t rest = static_cast<ssize_t>(size);
  const QTime startTime = QTime::currentTime();

  auto timeOut = static_cast<unsigned long>(data.count()) * 12000 / mSpeed + 100;
  if (mMethod == HANDSHAKE_SOFTWARE) {
    timeOut += 100;
  }

  unsigned long elapsed;
  do {
    auto result = ::read(mHandle, data.data() + total, static_cast<size_t>(rest));
    if (result < 0 && errno != EAGAIN) {
      qCritical() << "!e" << tr("Cannot read from serial port: %1").arg(lastErrorMessage());
      data.clear();
      return data;
    }
    result = std::max<ssize_t>(result, 0);
    total += result;
    rest -= result;
    elapsed = static_cast<unsigned long>(startTime.msecsTo(QTime::currentTime()));
  } while (total < size && elapsed < timeOut);

  if (total != size) {
    qCritical() << "!e" << tr("Serial port read timeout. %1 of %2 read in %3 ms").arg(total).arg(data.count()).arg(elapsed);
    data.clear();
    return data;
  }
  return data;
}

bool StandardSerialPortBackend::writeRawFrame(const QByteArray &data) {
  uint total = 0;
  ssize_t rest = data.count();
  const QTime startTime = QTime::currentTime();

  auto timeOut = static_cast<unsigned long>(data.count()) * 12000 / mSpeed + 100;
  if (mMethod == HANDSHAKE_SOFTWARE) {
    timeOut += 100;
  }

  int elapsed;
  do {
    auto result = ::write(mHandle, data.constData() + total, static_cast<size_t>(rest));
    if (result < 0 && errno != EAGAIN) {
      qCritical() << "!e" << tr("Cannot read from serial port: %1").arg(lastErrorMessage());
      return false;
    }
    result = std::max<ssize_t>(result, 0);
    total += result;
    rest -= result;
    elapsed = startTime.msecsTo(QTime::currentTime());
  } while (total < static_cast<uint>(data.count()) && elapsed < static_cast<int>(timeOut));

  if (total != static_cast<uint>(data.count())) {
    qCritical() << "!e" << tr("Serial port write timeout. %1 of %2 written in %3 ms").arg(total).arg(data.count()).arg(elapsed);
    return false;
  }

  if (tcdrain(mHandle) != 0) {
    qCritical() << "!e" << tr("Cannot flush serial port write buffer: %1").arg(lastErrorMessage());
    return false;
  }

  return true;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
QString StandardSerialPortBackend::lastErrorMessage() { // NOLINT(*-convert-member-functions-to-static)
  return QString::fromUtf8(strerror(errno)) + ".";
}

void StandardSerialPortBackend::setActiveSioDevices(const QByteArray &data) {
  mSioDevices = data;
}

AtariSioBackend::AtariSioBackend(QObject *parent)
  : AbstractSerialPortBackend(parent), mCancelHandles{}, mSpeed(0), mMethod(0)
{
  mHandle = -1;
}

AtariSioBackend::~AtariSioBackend() {
  if (AtariSioBackend::isOpen()) {
    AtariSioBackend::close();
  }
}

QString AtariSioBackend::defaultPortName() {
  return {"atarisio0"};
}

bool AtariSioBackend::open() {
  if (isOpen()) {
    close();
  }

  QString name(SERIAL_PORT_LOCATION);
  name.append(RespeqtSettings::instance()->atariSioDriverName());

  mHandle = ::open(name.toLocal8Bit().constData(), O_RDWR);

  if (mHandle < 0) {
    qCritical() << "!e" << tr("Cannot open serial port '%1': %2").arg(name, lastErrorMessage());
    return false;
  }

  const int version = ioctl(mHandle, ATARISIO_IOC_GET_VERSION);

  if (version < 0) {
    qCritical() << "!e" << tr("Cannot open AtariSio driver '%1': %2").arg(name, "Cannot determine AtariSio version.");
    close();
    return false;
  }

  if (version >> 8 != ATARISIO_VERSION >> 8 ||
      (version & 0xff) < (ATARISIO_VERSION & 0xff)) {
    qCritical() << "!e" << tr("Cannot open AtariSio driver '%1': %2").arg(name, "Incompatible AtariSio version.");
    close();
    return false;
  }

  int mode;

  mMethod = RespeqtSettings::instance()->atariSioHandshakingMethod();

  switch (mMethod) {
    case HANDSHAKE_RI:
      mode = ATARISIO_MODE_SIOSERVER;
      break;
    case HANDSHAKE_DSR:
      mode = ATARISIO_MODE_SIOSERVER_DSR;
      break;
    default:
    case HANDSHAKE_CTS:
      mode = ATARISIO_MODE_SIOSERVER_CTS;
      break;
  }

  if (ioctl(mHandle, ATARISIO_IOC_SET_MODE, mode) < 0) {
    qCritical() << "!e" << tr("Cannot set AtariSio driver mode: %1").arg(lastErrorMessage());
    close();
    return false;
  }

  if (ioctl(mHandle, ATARISIO_IOC_SET_AUTOBAUD, true) < 0) {
    qCritical() << "!e" << tr("Cannot set AtariSio to autobaud mode: %1").arg(lastErrorMessage());
    close();
    return false;
  }

  if (pipe(mCancelHandles) != 0) {
    qCritical() << "!e" << tr("Cannot create the cancel pipe");
  }

  QString m;
  switch (mMethod) {
    case HANDSHAKE_RI:
      m = "RI";
      break;
    case HANDSHAKE_DSR:
      m = "DSR";
      break;
    default:
    case HANDSHAKE_CTS:
      m = "CTS";
      break;
  }

  /* Notify the user that emulation is started */
  qWarning() << "!i" << tr("Emulation started through AtariSIO backend on '%1' with %2 handshaking.").arg(RespeqtSettings::instance()->atariSioDriverName(), m);

  return true;
}

bool AtariSioBackend::isOpen() {
  return mHandle >= 0;
}

void AtariSioBackend::close() {
  cancel();
  if (::close(mHandle)) {
    qCritical() << "!e" << tr("Cannot close serial port: %1").arg(lastErrorMessage());
  }
  ::close(mCancelHandles[0]);
  ::close(mCancelHandles[1]);
  mHandle = -1;
}

void AtariSioBackend::cancel() {
  if (write(mCancelHandles[1], "C", 1) < 1) {
    qCritical() << "!e" << tr("Cannot stop AtariSio backend.");
  }
}

void AtariSioBackend::forceHighSpeed(const unsigned int) {
}

bool AtariSioBackend::setSpeed(const unsigned long speed) {
  if (ioctl(mHandle, ATARISIO_IOC_SET_BAUDRATE, speed) < 0) {
    qCritical() << "!e" << tr("Cannot set AtariSio speed to %1: %2").arg(speed).arg(lastErrorMessage());
    return false;
  }
  emit statusChanged(tr("%1 bits/sec").arg(speed));
  qWarning() << "!i" << tr("Serial port speed set to %1.").arg(speed);
  return true;
}

QByteArray AtariSioBackend::readCommandFrame() {
  QByteArray data;

  fd_set read_set;
  fd_set except_set;

  int maxfd = mHandle;

  FD_ZERO(&except_set);
  FD_SET(mHandle, &except_set);

  if (mCancelHandles[0] > maxfd) {
    maxfd = mCancelHandles[0];
  }
  FD_ZERO(&read_set);
  FD_SET(mCancelHandles[0], &read_set);

  if (const int ret = select(maxfd + 1, &read_set, nullptr, &except_set, nullptr); ret == -1 || ret == 0) {
    return data;
  }
  if (FD_ISSET(mCancelHandles[0], &read_set)) {
    return data;
  }
  if (FD_ISSET(mHandle, &except_set)) {
    SIO_command_frame frame;
    if (ioctl(mHandle, ATARISIO_IOC_GET_COMMAND_FRAME, &frame) < 0) {
      return data;
    }
    data.resize(4);
    data[0] = static_cast<char>(frame.device_id);
    data[1] = static_cast<char>(frame.command);
    data[2] = static_cast<char>(frame.aux1);
    data[3] = static_cast<char>(frame.aux2);

    if (const auto recorder = SioRecorder::instance(); recorder->isSnapshotRunning())
      recorder->writeSnapshotCommandFrame(static_cast<quint8>(data[0]), static_cast<quint8>(data[1]), static_cast<quint8>(data[2]), static_cast<quint8>(data[3]));


    if (const auto sp = ioctl(mHandle, ATARISIO_IOC_GET_BAUDRATE); sp >= 0 && mSpeed != static_cast<unsigned long>(sp)) {
      emit statusChanged(tr("%1 bits/sec").arg(sp));
      qWarning() << "!i" << tr("Serial port speed set to %1.").arg(sp);
      mSpeed = static_cast<unsigned long>(sp);
    }

    return data;
  }
  qCritical() << "!e" << tr("Illegal condition using select!");
  return data;
}

int AtariSioBackend::speedByte() {
  return 0x08;
}

unsigned long AtariSioBackend::speed() {
  return mSpeed;
}

QByteArray AtariSioBackend::readDataFrame(const uint size, const bool isCommandFrame, const bool verbose) {
  QByteArray data;
  SIO_data_frame frame;

  data.resize(static_cast<int>(size));

  if (const auto recorder = SioRecorder::instance(); recorder->isSnapshotRunning()) {
    if (isCommandFrame) {
      recorder->writeSnapshotCommandFrame(static_cast<quint8>(data[0]), static_cast<quint8>(data[1]), static_cast<quint8>(data[2]), static_cast<quint8>(data[3]));
    } else {
      recorder->writeSnapshotDataFrame(data);
    }
  }

  frame.data_buffer = reinterpret_cast<unsigned char*>(data.data());
  frame.data_length = size;

  if (ioctl(mHandle, ATARISIO_IOC_RECEIVE_DATA_FRAME, &frame) < 0) {
    if (verbose) {
      qCritical() << "!e" << tr("Cannot read data frame: %1").arg(lastErrorMessage());
    }
    data.clear();
  }

  return data;
}

bool AtariSioBackend::writeDataFrame(const QByteArray &data) {
  SIO_data_frame frame;

  frame.data_buffer = reinterpret_cast<unsigned char*>(const_cast<char*>(data.constData()));
  frame.data_length = static_cast<unsigned int>(data.size());

  if (ioctl(mHandle, ATARISIO_IOC_SEND_DATA_FRAME, &frame) < 0) {
    qCritical() << "!e" << tr("Cannot write data frame: %1").arg(lastErrorMessage());
    return false;
  }

  return true;
}

bool AtariSioBackend::writeCommandAck() {
  if (ioctl(mHandle, ATARISIO_IOC_SEND_COMMAND_ACK) < 0) {
    if (errno != EATARISIO_COMMAND_TIMEOUT) {
      qCritical() << "!e" << tr("Cannot write command ACK: %1").arg(lastErrorMessage());
    }
    return false;
  }
  return true;
}

bool AtariSioBackend::writeCommandNak() {
  if (ioctl(mHandle, ATARISIO_IOC_SEND_COMMAND_NAK) < 0) {
    qCritical() << "!e" << tr("Cannot write command NAK: %1").arg(lastErrorMessage());
    return false;
  }
  return true;
}

bool AtariSioBackend::writeDataAck() {
  if (ioctl(mHandle, ATARISIO_IOC_SEND_DATA_ACK) < 0) {
    qCritical() << "!e" << tr("Cannot write data ACK: %1").arg(lastErrorMessage());
    return false;
  }
  return true;
}

bool AtariSioBackend::writeDataNak() {
  if (ioctl(mHandle, ATARISIO_IOC_SEND_DATA_NAK) < 0) {
    qCritical() << "!e" << tr("Cannot write data NAK: %1").arg(lastErrorMessage());
    return false;
  }
  return true;
}

bool AtariSioBackend::writeComplete() {
  if (ioctl(mHandle, ATARISIO_IOC_SEND_COMPLETE) < 0) {
    qCritical() << "!e" << tr("Cannot write COMPLETE byte: %1").arg(lastErrorMessage());
    return false;
  }
  return true;
}

bool AtariSioBackend::writeError() {
  if (ioctl(mHandle, ATARISIO_IOC_SEND_ERROR) < 0) {
    qCritical() << "!e" << tr("Cannot write ERROR byte: %1").arg(lastErrorMessage());
    return false;
  }
  return true;
}

bool AtariSioBackend::writeRawFrame(const QByteArray &data) {
  SIO_data_frame frame;

  frame.data_buffer = reinterpret_cast<unsigned char*>(const_cast<char*>(data.constData()));
  frame.data_length = static_cast<unsigned int>(data.size());

  if (ioctl(mHandle, ATARISIO_IOC_SEND_RAW_FRAME, &frame) < 0) {
    qCritical() << "!e" << tr("Cannot write raw frame: %1").arg(lastErrorMessage());
    return false;
  }

  return true;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
QString AtariSioBackend::lastErrorMessage() { // NOLINT(*-convert-member-functions-to-static)
  switch (errno) {
    case EATARISIO_ERROR_BLOCK_TOO_LONG:
      return tr("Block too long.");
      break;
    case EATARISIO_COMMAND_NAK:
      return tr("Command not acknowledged.");
      break;
    case EATARISIO_COMMAND_TIMEOUT:
      return tr("Command timeout.");
      break;
    case EATARISIO_CHECKSUM_ERROR:
      return tr("Checksum error.");
      break;
    case EATARISIO_COMMAND_COMPLETE_ERROR:
      return tr("Device error.");
      break;
    case EATARISIO_DATA_NAK:
      return tr("Data frame not acknowledged.");
      break;
    case EATARISIO_UNKNOWN_ERROR:
      return tr("Unknown AtariSio driver error.");
      break;
    default:
      return QString::fromUtf8(strerror(errno)) + ".";
  }
}

void AtariSioBackend::setActiveSioDevices(const QByteArray &) {}

} // namespace SIO
