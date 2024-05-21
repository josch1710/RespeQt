#ifndef TNFS_H
#define TNFS_H

#include <QUdpSocket>
#include <QVector>

#include "network/sessioninfo.h"
#include "network/datagram.h"

namespace Network {

    class Tnfs: public QObject {
        Q_OBJECT
    public:
        Tnfs();
        virtual ~Tnfs();
    protected slots:
        void readPendingDatagrams(); // Not auto -> type notation, because moc.

    protected:
        auto mount(const Datagram &datagram) -> Datagram;
        auto unmount(const Datagram &datagram) -> Datagram;
        auto opendir(const Datagram &datagram) -> Datagram;
        auto readdir(const Datagram &datagram) -> Datagram;
        auto telldir(const Datagram &datagram) -> Datagram;
        auto seekdir(const Datagram &datagram) -> Datagram;
        auto closedir(const Datagram &datagram) -> Datagram;
        auto mkdir(const Datagram &datagram) -> Datagram;
        auto rmdir(const Datagram &datagram) -> Datagram;
        auto open(const Datagram &datagram) -> Datagram;
        auto close(const Datagram &datagram) -> Datagram;
        auto read(const Datagram &datagram) -> Datagram;
        auto stat(const Datagram &datagram) -> Datagram;

    private:
        QUdpSocket *socket;
        quint16 sessionID{1};
        // Session infos
        QVector<SessionInfoPtr> sessions{10};
        constexpr static quint8 TNFS_EOF = 0x21;
        const quint16 MAX_PACKET_SIZE = 512;
    };
}
#endif
