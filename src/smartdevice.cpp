/*
 * smartdevice.cpp
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

#include "respeqtsettings.h"
#include "smartdevice.h"


#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>

//SmartDevice (ApeTime + URL submit)

void SmartDevice::handleCommand(const quint8 command, const quint8 aux1, const quint8 aux2) {
  switch (command) {
    // Get APE Time
    case 0x93: {
      if (!sio->port()->writeCommandAck()) {
        return;
      }

      QByteArray data(6, 0);
      QDateTime dateTime = QDateTime::currentDateTime();

      data[0] = static_cast<char>(dateTime.date().day());
      data[1] = static_cast<char>(dateTime.date().month());
      data[2] = static_cast<char>(dateTime.date().year() % 100);
      data[3] = static_cast<char>(dateTime.time().hour());
      data[4] = static_cast<char>(dateTime.time().minute());
      data[5] = static_cast<char>(dateTime.time().second());

      sio->port()->writeComplete();
      sio->port()->writeDataFrame(data);

      qDebug() << "!n" << tr("[%1] Read date/time (%2).").arg(deviceName(), dateTime.toString(QLocale::system().dateFormat(QLocale::ShortFormat)));
      break;
    }

    // Submit URL
    case 0x55: {
      quint16 aux = aux1 + aux2 * 256;
      if (RespeqtSettings::instance()->isURLSubmitEnabled() && aux != 0 && aux <= 2000) {
        if (!sio->port()->writeCommandAck()) {
          return;
        }

        QByteArray data(aux, 0);
        data = sio->port()->readDataFrame(aux, false);
        if (data.isEmpty()) {
          qCritical() << "!e" << tr("[%1] Read data frame failed").arg(deviceName());
          sio->port()->writeDataNak();
          sio->port()->writeError();
          return;
        }
        sio->port()->writeDataAck();
        sio->port()->writeComplete();

        QString urlstr(data);
        QDesktopServices::openUrl(QUrl(urlstr));

        qDebug() << "!n" << tr("URL [%1] submitted").arg(urlstr);
      } else {
        sio->port()->writeCommandNak();
        qWarning() << "!w" << tr("[%1] command: $%2, aux: $%3 NAKed.").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux, 4, 16, QChar('0'));
        return;
      }
      break;
    }

    default: {
      sio->port()->writeCommandNak();
      qWarning() << "!w" << tr("[%1] command: $%2, aux1: $%3, aux2: $%4 NAKed.").arg(deviceName()).arg(command, 2, 16, QChar('0')).arg(aux1, 2, 16, QChar('0')).arg(aux2, 2, 16, QChar('0'));
      break;
    }
  }
}
