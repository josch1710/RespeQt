#ifndef RESPEQT_TNFSTCP_H
#define RESPEQT_TNFSTCP_H

#include "network/tnfs.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QSharedPointer>

using QTcpSocketPtr = QSharedPointer<QTcpSocket>;

namespace Network
{

    class TnfsTcp: public Tnfs
    {
        Q_OBJECT
    public:
        TnfsTcp();
        ~TnfsTcp() override;

    public slots:
        // No auto -> type notation, because moc.
        void start();
        void stop();

    protected slots:
        // No auto -> type notation, because moc.
        void newConnection();
        void readPendingData(QTcpSocket *const socket);

    private:
        QTcpServer *tcpServer{nullptr};
        QVector<QTcpSocketPtr> tcpConnections{10};
    };

}  // namespace Network

#endif  //RESPEQT_TNFSTCP_H
