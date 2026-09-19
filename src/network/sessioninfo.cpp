
#include "network/sessioninfo.h"
#include "network/tnfs.h"

#include <QStorageInfo>

namespace Network {

    auto SessionInfo::realPath(const QString &path) const -> QDirPtr {
        if (path == "/") {
            return {};
        }
        auto pathList{path.split('/')};
        if (pathList.first() == "")
            pathList.pop_front();

        // TODO Perhaps there is a way to detect case sensitivity for a filesystem
#if defined Q_OS_UNIX && !defined Q_OS_OSX
        constexpr Qt::CaseSensitivity cs {Qt::CaseSensitive};
#else
        constexpr Qt::CaseSensitivity cs {Qt::CaseInsensitive};
#endif
        for(const auto& mountPoint: _parent->mountPoints()) {
            if (mountPoint.isNull()) {
                continue;
            }
            if (mountPoint->isRoot()) {
                if (QStorageInfo info{mountPoint->absolutePath()}; pathList.first() == info.displayName())
                    return QDirPtr::create(mountPoint->absolutePath());
            }
            auto temp{mountPoint->absolutePath()};
            for(auto dir = pathList.rbegin(); dir != pathList.rend(); ++dir) {
                if (temp.endsWith(*dir, cs)) {
                    const auto i = temp.lastIndexOf(*dir, -1, cs);
                    const auto length = dir->length();
                    temp.remove(i, length);
                }
            }

            auto infoPath{temp.append('/').append(path)};
            if (QFileInfo info{infoPath}; info.exists()) {
                return QDirPtr::create(QDir::cleanPath(info.absoluteFilePath()));
            }
        }
        return {};
    }
    auto SessionInfo::realFileName(const QString &fileName) const -> QString {
        const QFileInfo fileinfo(fileName);
        if (const auto dir{realPath(fileinfo.path())}; !dir.isNull()) {
            if (const QFileInfo file{dir->absoluteFilePath(fileinfo.fileName())}; file.exists())
                return file.absoluteFilePath();
        }
        return {};
    }

}  // Network