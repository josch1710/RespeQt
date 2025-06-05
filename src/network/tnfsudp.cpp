#include "network/tnfsudp.h"

namespace Network
{
    TnfsUdp::TnfsUdp()
    {}

    TnfsUdp::~TnfsUdp()
    {
        stop();
    }

    void TnfsUdp::start()
    {
        if (udpSocket)
            return; // Already started, don't do anything

        udpSocket = new QUdpSocket(this);
        udpSocket->bind(QHostAddress::Any, 16384);

        connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
        qDebug() << "!n" << tr("Tnfs via UDP started");
    }

    void TnfsUdp::stop()
    {
        disconnect(udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
        delete udpSocket;
        udpSocket = nullptr;
        reset();
        qDebug() << "!n" << tr("Tnfs via UDP stopped");
    }

    void TnfsUdp::readPendingDatagrams() {
        if (!udpSocket)
            return;

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
