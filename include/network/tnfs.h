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
        auto opendirx(const Datagram &datagram) -> Datagram;
        auto readdirx(const Datagram &datagram) -> Datagram;
        auto telldir(const Datagram &datagram) -> Datagram;
        auto seekdir(const Datagram &datagram) -> Datagram;
        auto closedir(const Datagram &datagram) -> Datagram;
        auto mkdir(const Datagram &datagram) -> Datagram;
        auto rmdir(const Datagram &datagram) -> Datagram;

        auto openfile(const Datagram &datagram) -> Datagram;
        auto closefile(const Datagram &datagram) -> Datagram;
        auto readfile(const Datagram &datagram) -> Datagram;
        auto writefile(const Datagram &datagram) -> Datagram;
        auto statfile(const Datagram &datagram) -> Datagram;
        auto seekfile(const Datagram &datagram) -> Datagram;
        auto unlinkfile(const Datagram &datagram) -> Datagram;
        auto chmodfile(const Datagram &datagram) -> Datagram;
        auto renamefile(const Datagram &datagram) -> Datagram;

        auto fsFree(const Datagram &datagram) -> Datagram;
        auto fsSize(const Datagram &datagram) -> Datagram;

        template<typename T>
        auto findFreeSlot(const QVector<QSharedPointer<T>> &list) -> qint16 {
            qint16 handle{0};
            while (handle < list.length() && !list[handle].isNull()) {
                handle++;
            }
            if (handle > 255) {
                return -1;
            }
            return handle;
        }


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

        constexpr static quint8 TNFS_MOUNT = 0x00;
        constexpr static quint8 TNFS_UMOUNT = 0x01;

        constexpr static quint8 TNFS_OPENDIR = 0x10;
        constexpr static quint8 TNFS_READDIR = 0x11;
        constexpr static quint8 TNFS_CLOSEDIR = 0x12;
        constexpr static quint8 TNFS_MKDIR = 0x13;
        constexpr static quint8 TNFS_RMDIR = 0x14;
        constexpr static quint8 TNFS_TELLDIR = 0x15;
        constexpr static quint8 TNFS_SEEKDIR = 0x16;
        constexpr static quint8 TNFS_OPENDIRX = 0x17;
        constexpr static quint8 TNFS_READDIRX = 0x18;

        constexpr static quint8 TNFS_READBLOCK = 0x21;
        constexpr static quint8 TNFS_WRITEBLOCK = 0x22;
        constexpr static quint8 TNFS_CLOSEFILE = 0x23;
        constexpr static quint8 TNFS_STATFILE = 0x24;
        constexpr static quint8 TNFS_SEEKFILE = 0x25;
        constexpr static quint8 TNFS_UNLINKFILE = 0x26;
        constexpr static quint8 TNFS_CHMODFILE = 0x27;
        constexpr static quint8 TNFS_RENAMEFILE = 0x28;
        constexpr static quint8 TNFS_OPENFILE = 0x29;

        constexpr static quint8 TNFS_FSSIZE = 0x30;
        constexpr static quint8 TNFS_FSFREE = 0x31;

        constexpr static quint8 TNFS_DIROPT_NO_FOLDERSFIRST = 0x01;
        constexpr static quint8 TNFS_DIROPT_NO_SKIPHIDDEN = 0x02;
        constexpr static quint8 TNFS_DIROPT_NO_SKIPSPECIAL = 0x04;
        constexpr static quint8 TNFS_DIROPT_DIR_PATTERN = 0x08;

        constexpr static quint8 TNFS_DIRSORT_NONE = 0x01;
        constexpr static quint8 TNFS_DIRSORT_CASE = 0x02;
        constexpr static quint8 TNFS_DIRSORT_DESCENDING = 0x04;
        constexpr static quint8 TNFS_DIRSORT_MODIFIED = 0x08;
        constexpr static quint8 TNFS_DIRSORT_SIZE = 0x10;

        constexpr static quint8 TNFS_DIRSTATUS_EOF = 0x01;

        constexpr static quint8 TNFS_DIRENTRY_DIR = 0x01;
        constexpr static quint8 TNFS_DIRENTRY_HIDDEN = 0x02;
        constexpr static quint8 TNFS_DIRENTRY_SPECIAL = 0x04;
    };
}
#endif
