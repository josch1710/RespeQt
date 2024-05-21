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

        auto mountPoints() const -> const QDirVector& { return _mountPoints; }
        auto addMountPoint(QDir mountPoint) -> void;
        auto removeMountPoint(QDir mountPoint) -> void;

    protected slots:
        void readPendingDatagrams(); // No auto -> type notation, because moc.

    signals:
        // No auto -> type notation, because moc.
        void sessionConnected();
        void allSessionsDisconnected();

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
        auto fsFree(const Datagram &datagram) -> Datagram;
        auto fsSize(const Datagram &datagram) -> Datagram;

    private:
        QUdpSocket *socket;
        quint16 _sessionID{1};
        // Session infos
        QVector<SessionInfoPtr> sessions{10};
        constexpr static quint8 TNFS_EOF = 0x21;
        const quint16 MAX_PACKET_SIZE = 512;
        QDirVector _mountPoints{};

        // Version number 1.2
        constexpr static quint16 versionSupported = 0x0102;
        // 1000 (0x03E8) ms timeout
        constexpr static quint16 timeout = 0x03E8;
    };
}
#endif
