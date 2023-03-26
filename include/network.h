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
#endif// NETWORK_H
