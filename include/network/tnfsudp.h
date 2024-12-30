#ifndef RESPEQT_TNFSUDP_H
#define RESPEQT_TNFSUDP_H

#include "network/tnfs.h"

#include <QUdpSocket>

namespace Network
{

    class TnfsUdp: public Tnfs
    {
        Q_OBJECT
    public:
        TnfsUdp();
        virtual ~TnfsUdp();

    protected slots:
        void readPendingDatagrams(); // No auto -> type notation, because moc.

    private:
        QUdpSocket *udpSocket;
    };

}  // namespace Network

#endif  //RESPEQT_TNFSUDP_H
