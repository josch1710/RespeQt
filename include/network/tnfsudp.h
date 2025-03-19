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
        ~TnfsUdp() override;

    public slots:
        // No auto -> type notation, because moc.
        void start();
        void stop();

    protected slots:
        void readPendingDatagrams(); // No auto -> type notation, because moc.

    private:
        QUdpSocket *udpSocket{nullptr};
    };

}  // namespace Network

#endif  //RESPEQT_TNFSUDP_H
