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

    protected slots:
        // No auto -> type notation, because moc.
        void newConnection();
        void readPendingData(QTcpSocket *const socket);

    private:
        QTcpServer *tcpServer;
        QVector<QTcpSocketPtr> tcpConnections{10};
    };

}  // namespace Network

#endif  //RESPEQT_TNFSTCP_H
