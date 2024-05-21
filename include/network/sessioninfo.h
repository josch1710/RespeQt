
#ifndef RESPEQT_SESSIONINFO_H
#define RESPEQT_SESSIONINFO_H

#include <QSharedPointer>
#include <QDir>
#include <QVector>

using QDirPtr = QSharedPointer<QDir>;
using QDirVector = QVector<QDirPtr>;
using QFilePtr = QSharedPointer<QFile>;
using QFileVector = QVector<QFilePtr>;

namespace Network {

    struct OpenDirInfo
    {
        QDirPtr virtualDir;
        QDirPtr actualDir;
        QStringList files;
        quint32 fileListIndex;
        bool isVirtualRoot;
    };

    using QDirIndexPtr = QSharedPointer<OpenDirInfo>;
    using QDirIndexVector = QVector<QDirIndexPtr>;

    class SessionInfo
    {
    public:
        SessionInfo(quint16 sessionID)
            : _sessionID(sessionID) {}

        // TODO mountpoints should not be in sessioninfo
        auto mountPoints() const -> const QDirVector& { return _mountPoints; }
        auto addMountPoint(QDir mountPoint) -> void { return _mountPoints.append(QDirPtr::create(mountPoint)); }
        auto realPath(const QString &path) const -> QDirPtr;
        auto realFileName(const QString &fileName) const -> QString;
        auto sessionID() const -> const quint16 { return _sessionID; }
        auto openDirectories() -> QDirIndexVector& { return _openDirs; }
        auto openFiles() -> QFileVector& { return _openFiles; }

    private:
        quint16 _sessionID{0};
        QDirVector _mountPoints{};
        QDirIndexVector _openDirs{10};
        QVector<quint32> _dirListIndex{10};
        QFileVector _openFiles{10};
    };

    using SessionInfoPtr = QSharedPointer<SessionInfo>;
}  // Network

#endif  //RESPEQT_SESSIONINFO_H
