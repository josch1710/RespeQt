
#include "network/sessioninfo.h"
#include "network/tnfs.h"

#include <QStorageInfo>

namespace Network {

    auto SessionInfo::realPath(const QString &path) const -> QDirPtr {
        if (path == "/") {
            return QDirPtr();
        }
        /*auto*/QStringList pathList{path.split('/')};
        if (pathList.first() == "")
            pathList.pop_front();

        // TODO Perhaps there is a way to detect case sensitivity for a filesystem
#if defined Q_OS_UNIX && !defined Q_OS_OSX
        Qt::CaseSensitivity cs = Qt::CaseSensitive;
#else
        Qt::CaseSensitivity cs = Qt::CaseInsensitive;
#endif
        for(auto mountPoint: _parent->mountPoints()) {
            if (mountPoint.isNull()) {
                continue;
            }
            if (mountPoint->isRoot()) {
                QStorageInfo info{mountPoint->absolutePath()};
                if (pathList.first() == info.displayName())
                    return QDirPtr::create(mountPoint->absolutePath());
            }
            /*auto*/QString temp{mountPoint->absolutePath()};
            for(auto dir= pathList.rbegin(); dir != pathList.rend(); dir++) {
                if (temp.endsWith(*dir, cs)) {
                    auto i = temp.lastIndexOf(*dir, -1, cs);
                    auto length = dir->length();
                    if (false && i > 0 && length > 0)
                        i--, length++;

                    temp.remove(i, length);
                }
            }

            /*auto*/QString infoPath{temp.append('/').append(path)};
            /*auto*/QFileInfo info{infoPath};
            if (info.exists()) {
                return QDirPtr::create(QDir::cleanPath(info.absoluteFilePath()));
            }
        }
        return QDirPtr();
    }
    auto SessionInfo::realFileName(const QString &fileName) const -> QString {
        QFileInfo fileinfo(fileName);
        /*auto*/QDirPtr dir{realPath(fileinfo.path())};
        if (!dir.isNull()) {
            QFileInfo file{dir->absoluteFilePath(fileinfo.fileName())};
            if (file.exists())
                return file.absoluteFilePath();
        }
        return QString();
    }

}  // Network