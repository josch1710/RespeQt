#include "network/tnfstcp.h"

namespace Network
{
    TnfsTcp::TnfsTcp() {
         tcpServer = new QTcpServer(this);
         if (!tcpServer->listen(QHostAddress::Any, 16384)) {
             delete tcpServer;
             tcpServer = nullptr;
             qCritical() << "!e" << "Tnfs via TCP could not be started.";
             return;
         }
         connect(tcpServer, &QTcpServer::newConnection, this, &TnfsTcp::newConnection);
    }

    TnfsTcp::~TnfsTcp() {
        disconnect(tcpServer, &QTcpServer::newConnection, this, &TnfsTcp::newConnection);
        delete tcpServer;
        tcpServer = nullptr;
    }

    void TnfsTcp::newConnection() {
        auto tcpServerConnection = tcpServer->nextPendingConnection();
        connect(tcpServerConnection, &QTcpSocket::readyRead, this, [this, tcpServerConnection] { this->readPendingData(tcpServerConnection); });
    }

    void TnfsTcp::readPendingData(QTcpSocket *const socket) {
        Datagram datagram{};
        datagram.resize(socket->bytesAvailable());
        socket->read(datagram.data(), socket->bytesAvailable());
        Datagram /*auto*/ answer{handleDatagram(datagram)};
        if (answer.length() > 0) {
            socket->write(answer);
        }
    }

}  // namespace Network