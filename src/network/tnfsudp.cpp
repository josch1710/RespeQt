#include "network/tnfsudp.h"

namespace Network
{
    TnfsUdp::TnfsUdp() : Tnfs() {
        udpSocket = new QUdpSocket(this);
        udpSocket->bind(QHostAddress::Any, 16384);

        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    }

    TnfsUdp::~TnfsUdp() {
        disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
        delete udpSocket;
        udpSocket = NULL;
    }

    void TnfsUdp::readPendingDatagrams() {
        while (udpSocket->hasPendingDatagrams()) {
            Datagram datagram;
            datagram.resize(udpSocket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            udpSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);

            Datagram /*auto*/ answer{handleDatagram(datagram)};
            if (answer.length() > 0) {
                udpSocket->writeDatagram(answer, sender, senderPort);
            }
        }
    }

}  // namespace Network
