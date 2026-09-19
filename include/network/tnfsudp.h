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
        auto start() -> void;
        auto stop() -> void;

    protected slots:
        auto readPendingDatagrams() -> void;

    private:
        QUdpSocket *udpSocket{nullptr};
    };

}  // namespace Network

#endif  //RESPEQT_TNFSUDP_H
