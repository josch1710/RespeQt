/*
 * network.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef NETWORK_H
#define NETWORK_H
#include <QMainWindow>
#include <QMessageBox>
#include <QPointer>
#include <QTimer>
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
// These network classes were deprecated way back in Qt 5.15
#include <qnetworkconfigmanager.h>
#include <qnetworkinterface.h>
#include <qnetworksession.h>


class Network : public QMainWindow {
  Q_OBJECT

public:
  Network(QWidget *parent = 0);
  ~Network();

public slots:
  bool openConnection(QString &netInterface);

private:
  QPointer<QNetworkSession> m_session;
};
#else
// Network class stub for testing with Qt >= 5.15
//
class Network : public QMainWindow {
  Q_OBJECT

public:
    Network() {}
    ~Network() {}

public slots:
    bool openConnection(QString &) {return false;}
};
#endif

#endif// NETWORK_H
