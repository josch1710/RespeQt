#include "printers/baseprinter.h"

#include <utility>

#include "logdisplaydialog.h"
#include "respeqtsettings.h"

namespace Printers {
  BasePrinter::BasePrinter(SioWorkerPtr worker)
      : SioDevice(std::move(worker))//,
                                    //mOutput()
  {}

  BasePrinter::~BasePrinter() = default;

  const QChar BasePrinter::translateAtascii(const unsigned char b) const {
    return mAtascii(b);
  }

  void BasePrinter::handleCommand(const quint8 command, const quint8 aux1, const quint8 aux2) {
    if (RespeqtSettings::instance()->printerEmulation() && mOutputWindow) {// Ignore printer commands  if Emulation turned OFF)    //
      qDebug() << "!n"
               << "[" << deviceName() << "] "
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
               << Qt::hex
#else
               << hex
#endif
               << "command: " << command << " aux1: " << aux1 << " aux2: " << aux2;
      switch (command) {
        case 0x53:// dec 83
        {
          // Get status
          if (!sio->port()->writeCommandAck()) {
            return;
          }

          QByteArray status(4, 0);
          status[0] = 0;
          status[1] = m_lastOperation;
          status[2] = 3;
          status[3] = 0;
          if (!sio->port()->writeComplete()) {
            return;
          }
          qDebug() << "!n" << tr("[%1] Get status: $%2").arg(deviceName()).arg((unsigned char) status[1], 2, 16, QChar('0'));
          writeDataFrame(status);
          break;
        }
        case 0x57:// dec 87
        {

          unsigned int len;
          switch (aux1) {
            case 0x4e:// dec 78
              // Normal
              len = 40;
              break;
            case 0x53:// dec 83
              // Sideways
              len = 29;
              break;
            case 0x44:// dec 68
              // Double width
              len = 21;
              break;
            default:
              sio->port()->writeCommandNak();
              qWarning() << "!w" << tr("[%1] Command: $%2, aux1: $%3, aux2: $%4 NAKed because aux1 is not supported").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
              return;
          }
          if (!sio->port()->writeCommandAck()) {
            return;
          }

          QByteArray data = readDataFrame(len, false);
          if (data.isEmpty()) {
            qCritical() << "!e" << tr("[%1] Print: data frame failed").arg(deviceName());
            sio->port()->writeDataNak();
            return;
          }

          if (!sio->port()->writeDataAck()) {
            return;
          }
          handleBuffer(data, len);
          qDebug() << "!n" << tr("[%1] Print (%2 chars)").arg(deviceName()).arg(len);
          sio->port()->writeComplete();
          break;
        }
        default:
          sio->port()->writeCommandNak();
          qWarning() << "!w" << tr("[%1] command: $%2, aux1: $%3, aux2: $%4 NAKed.").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
      }
    } else {
      qDebug() << "!u" << tr("[%1] ignored").arg(deviceName());
    }
  }

  void BasePrinter::setOutputWindow(OutputWindowPtr outputWindow) {
    if (mOutputWindow) {
      mOutputWindow->close();
    }
    mOutputWindow = outputWindow;
    mOutputWindow->setSceneRect(getSceneRect());
  }

  const QRectF BasePrinter::getSceneRect() const {
    return QRectF(0, 0, 0, 0);
  }

  void BasePrinter::resetOutputWindow() {
    mOutputWindow.reset();
  }


  QByteArray BasePrinter::readDataFrame(uint size, bool isCommandFrame, bool verbose) {
    QByteArray data = sio->port()->readDataFrame(size, isCommandFrame, verbose);
    if (RespeqtSettings::instance()->isPrinterSpyMode()) {
      qDebug() << "!u" << tr("[%1] Receiving %2 bytes from Atari").arg(deviceName()).arg(data.length());
      dumpBuffer((unsigned char *) data.data(), data.length());
    }
    return data;
  }

  bool BasePrinter::writeDataFrame(QByteArray data) {
    if (RespeqtSettings::instance()->isPrinterSpyMode()) {
      qDebug() << "!u" << tr("[%1] Sending %2 bytes to Atari").arg(deviceName()).arg(data.length());
      dumpBuffer((unsigned char *) data.data(), data.length());
    }
    return sio->port()->writeDataFrame(data);
  }

  void BasePrinter::dumpBuffer(unsigned char *buf, int len) {
    for (int i = 0; i < ((len + 15) >> 4); i++) {
      char line[80];
      int ofs = i << 4;
      fillBuffer(line, buf, len, ofs, true);
      qDebug() << "!u" << tr("[%1] §%2").arg(deviceName(), line);
    }
  }

  void BasePrinter::fillBuffer(char *line, unsigned char *buf, int len, int ofs, bool dumpAscii) {
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
      snprintf(line, 8, "$%04X:", ofs);
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

  void BasePrinter::executeGraphicsPrimitive(GraphicsPrimitive *primitive) {
    if (mOutputWindow) {
      if (mClearPane) {
        mClearPane = false;
        mOutputWindow->executeGraphicsPrimitive(new GraphicsClearPane());
      }
      mOutputWindow->executeGraphicsPrimitive(primitive);
    }
  }

}// namespace Printers
