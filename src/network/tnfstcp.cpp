#include "network/tnfstcp.h"

namespace Network
{
    TnfsTcp::TnfsTcp()
    {}

    TnfsTcp::~TnfsTcp()
    {
        stop();
    }

    void TnfsTcp::start()
    {
        if (tcpServer)
            return; // Already started, don't do anything

        tcpServer = new QTcpServer(this);
        if (!tcpServer->listen(QHostAddress::Any, 16384)) {
            delete tcpServer;
            tcpServer = nullptr;
            qCritical() << "!e" << "Tnfs via TCP could not be started.";
            return;
        }
        connect(tcpServer, &QTcpServer::newConnection, this, &TnfsTcp::newConnection);
        qDebug() << "!n" << tr("Tnfs via TCP started");
    }

    void TnfsTcp::stop()
    {
        if (tcpServer != nullptr) {
            disconnect(tcpServer, &QTcpServer::newConnection, this, &TnfsTcp::newConnection);
        }
        delete tcpServer;
        tcpServer = nullptr;
        reset();
        qDebug() << "!n" << tr("Tnfs via TCP stopped");
    }

    void TnfsTcp::newConnection() {
        auto tcpServerConnection = tcpServer->nextPendingConnection();
        connect(tcpServerConnection, &QTcpSocket::readyRead, this, [this, tcpServerConnection] { this->readPendingData(tcpServerConnection); });
    }

    void TnfsTcp::readPendingData(QTcpSocket *const socket) {
        Datagram datagram{};
        datagram.resize(socket->bytesAvailable());
        socket->read(datagram.data(), socket->bytesAvailable());
        auto answer{handleDatagram(datagram)};
        if (answer.length() > 0) {
            socket->write(answer);
        }
    }

}  // namespace Network