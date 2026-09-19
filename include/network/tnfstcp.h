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
        auto start() -> void;
        auto stop() -> void;

    protected slots:
        auto newConnection() -> void;
        auto readPendingData(QTcpSocket * socket) -> void;

    private:
        QTcpServer *tcpServer{nullptr};
        QVector<QTcpSocketPtr> tcpConnections{10};
    };

}  // namespace Network

#endif  //RESPEQT_TNFSTCP_H
