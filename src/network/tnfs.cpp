#include "network/tnfs.h"
#include "network/sessioninfo.h"
#include "network/datagram.h"
#include "mainwindow.h"

#include <fcntl.h>
#include <algorithm>
#include <QDateTime>
#include <QStorageInfo>

namespace Network {
    Tnfs::Tnfs() : QObject() {
        commandTexts[0x00] = QString("Mount");
        commandTexts[0x01] = QString("Unmount");
        commandTexts[0x10] = QString("Open Dir");
        commandTexts[0x11] = QString("Read Dir");
        commandTexts[0x12] = QString("Close Dir");
        commandTexts[0x13] = QString("Make Dir");
        commandTexts[0x14] = QString("Remove Dir");
        commandTexts[0x15] = QString("Tell Dir");
        commandTexts[0x16] = QString("Seek Dir");
        commandTexts[0x17] = QString("Open Dir Extended");
        commandTexts[0x18] = QString("Read Dir Extended");
        commandTexts[0x21] = QString("Read Block");
        commandTexts[0x22] = QString("Write Block");
        commandTexts[0x23] = QString("Close File");
        commandTexts[0x24] = QString("Stat File");
        commandTexts[0x25] = QString("Seek File");
        commandTexts[0x26] = QString("Unlink File");
        commandTexts[0x27] = QString("Chmod File");
        commandTexts[0x28] = QString("Rename File");
        commandTexts[0x29] = QString("Open File");
        commandTexts[0x30] = QString("Filesystem Size");
        commandTexts[0x31] = QString("Filesystem Free");
    };

// TODO Make static
    auto Tnfs::removeMountPoint(QDir mountPoint) -> void {
        _mountPoints.removeOne(QDirPtr::create(mountPoint));
    }

    auto Tnfs::addMountPoint(QDir mountPoint) -> void {
        _mountPoints.append(QDirPtr::create(mountPoint));
    }

    auto Tnfs::handleDatagram(const Network::Datagram &datagram) -> Datagram
    {
        Datagram answer{};
        QString commandname{};

        if (commandTexts.contains(datagram.at(3)))
        {
            commandname = " (";
            commandname.append(commandTexts[datagram.at(3)]);
            commandname.append(')');
        }

        qDebug() << "!e" << "Incoming command 0x" << QString::number((unsigned char) datagram.at(3), 16) << commandname;
        switch (datagram.at(3))
        {
            case TNFS_MOUNT:
                answer = mount(datagram);
                break;

            case TNFS_UMOUNT:
                answer = unmount(datagram);
                break;

            case TNFS_OPENDIR:
                answer = opendir(datagram);
                break;

            case TNFS_READDIR:
                answer = readdir(datagram);
                break;

            case TNFS_OPENDIRX:
                answer = opendirx(datagram);
                break;

            case TNFS_READDIRX:
                answer = readdirx(datagram);
                break;

            case TNFS_TELLDIR:
                answer = telldir(datagram);
                break;

            case TNFS_SEEKDIR:
                answer = seekdir(datagram);
                break;

            case TNFS_CLOSEDIR:
                answer = closedir(datagram);
                break;

            case TNFS_MKDIR:
                answer = mkdir(datagram);
                break;

            case TNFS_RMDIR:
                answer = rmdir(datagram);
                break;

            case TNFS_OPENFILE:
                answer = openfile(datagram);
                break;

            case TNFS_READBLOCK:
                answer = readfile(datagram);
                break;

            case TNFS_WRITEBLOCK:
                answer = writefile(datagram);
                break;

            case TNFS_CLOSEFILE:
                answer = closefile(datagram);
                break;

            case TNFS_STATFILE:
                answer = statfile(datagram);
                break;

            case TNFS_SEEKFILE:
                answer = seekfile(datagram);
                break;

            case TNFS_UNLINKFILE:
                answer = unlinkfile(datagram);
                break;

            case TNFS_CHMODFILE:
                answer = chmodfile(datagram);
                break;

            case TNFS_RENAMEFILE:
                answer = renamefile(datagram);
                break;

            case TNFS_FSFREE:
                answer = fsFree(datagram);
                break;

            case TNFS_FSSIZE:
                answer = fsSize(datagram);
                break;

            default: {
                qDebug() << "!n" << "Unknown command 0x" << QString::number((unsigned char) datagram.at(3), 16);
                //for(auto i=0;i<datagram.size();i++) qDebug()<<"!n" << i << " => "<<(unsigned)datagram.at(i);

                answer    = datagram.createAnswer();
                answer[4] = EINVAL;
            }
        }
        return answer;
    }

    auto Tnfs::reset() -> void
    {
        emit allSessionsDisconnected();
        _sessionID = 1;
        sessions.clear();
    }

    /* -------------------- TNFS commands --------------- */
    auto Tnfs::mount(const Datagram &datagram) -> Datagram {
        //auto retry{datagram.at(2)};
        //auto version{getU16At(datagram, 4)};
        auto mountPoint{datagram.getStringAt(6)};
        auto userID{datagram.getStringAt(7 + mountPoint.length())};
        auto password{datagram.getStringAt(7 + mountPoint.length() + userID.length())};
        Datagram answer{};

        const QDir root{QDir::homePath().append(mountPoint)};
        if (!root.exists()) {
            answer.setU16At(0, 0);
            answer.setU16At(ENOENT, 2);
            answer.setU16At(versionSupported, 4);
            answer.setU16At(timeout, 6);
            return answer;
        }

        SessionInfoPtr session{SessionInfoPtr::create(_sessionID, this)};
        sessions[_sessionID] = session;

        answer.setU16At(session.data()->sessionID(), 0);
        // Header fill
        answer.append(static_cast<char>(0));
        answer.append(static_cast<char>(0));
        answer.append(static_cast<char>(0));
        // Version number 1.2
        answer.append(static_cast<char>(2));
        answer.append(static_cast<char>(1));
        // 1000 (0x03E8) ms timeout
        answer.append(static_cast<char>(0xE8));
        answer.append(static_cast<char>(0x03));

        emit sessionConnected(); // Inform the main window about session

        _sessionID++;
        return answer;
    }

    auto Tnfs::unmount(const Datagram &datagram) -> Datagram {
        auto answer{datagram.createAnswer()};

        if (sessions.at(datagram.getSessionID()).isNull()) {
            answer[4] = EINVAL;
            return answer;
        }
        sessions.remove(datagram.getSessionID());

        auto isSessionsEmpty {
            sessions.end() == std::find_if_not(sessions.begin(), sessions.end(),
                [](SessionInfoPtr session) {
                    return session.isNull();
                })
        };
        if (isSessionsEmpty)
            emit allSessionsDisconnected(); // Inform the mainwindow, that all session are closed.

        return answer;
    }

    auto Tnfs::opendir(const Datagram &datagram) -> Datagram {
        auto sessionID{datagram.getSessionID()};
        auto dirName{datagram.getStringAt(4)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).isNull()) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions[sessionID]};
        auto pathName{sessionInfo->realPath(dirName)};
        if (dirName != "/" && !pathName.isNull() && !pathName->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        auto &openDirs{sessionInfo->openDirectories()};
        qint16 handle{findFreeSlot(openDirs)};
        if(handle < 0) {
            answer[4] = EMFILE;
            return answer;
        }
        answer[5] = handle;

        auto index{QDirIndexPtr::create()};
        index->isVirtualRoot = dirName == "/";
        index->actualDir = pathName;
        index->virtualDir = QDirPtr::create(dirName);
        if (index->isVirtualRoot) {
            for(auto mountPoint: mountPoints()){
                if (mountPoint.isNull()){
                    continue;
                }
                if (mountPoint->isRoot()){
                    QStorageInfo info{mountPoint->absolutePath()};
                    index->files.append(info.displayName());
                }
                else
                    index->files.append(mountPoint->dirName());
            }
        }
        else {
            index->files = index->actualDir->entryList();
        }
        index->fileListIndex = 0;
        openDirs[handle] = index;

        return answer;
    }

    auto Tnfs::readdir(const Datagram &datagram) -> Datagram {
        auto sessionID{datagram.getSessionID()};
        quint8 handle = datagram[4];
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions[sessionID]};
        auto &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull()) {
            answer[4] = ENOENT;
            return answer;
        }

        auto index{openDirs[handle]};
        if (index->fileListIndex >= index->files.length()) {
            answer[4] = TNFS_EOF;
            return answer;
        }

        answer.setStringAt(index->files[index->fileListIndex], 5);
        index->fileListIndex++;

        return answer;
    }

    auto Tnfs::opendirx(const Datagram &datagram) -> Datagram {
        auto sessionID{datagram.getSessionID()};
        auto diropt{static_cast<quint8>(datagram.at(4))};
        auto dirsort{static_cast<quint8>(datagram.at(5))};
        auto maxcount{datagram.getU16At(6)};
        auto wildcard{datagram.getStringAt(8)};
        auto dirName{datagram.getStringAt(9 + wildcard.length())};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).isNull()) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions[sessionID]};
        auto pathName{sessionInfo->realPath(dirName)};
        if (dirName != "/" && !pathName.isNull() && !pathName->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        auto &openDirs{sessionInfo->openDirectories()};
        qint16 handle{findFreeSlot(openDirs)};
        if (handle < 0) {
            answer[4] = EMFILE;
            return answer;
        }

        answer[5] = handle;
        auto index{QDirIndexPtr::create()};
        index->isVirtualRoot = dirName == "/";
        index->actualDir = pathName;
        index->virtualDir = QDirPtr::create(dirName);
        if (index->isVirtualRoot) {
            for(const auto& mountPoint: mountPoints()){
                if (mountPoint.isNull()){
                    continue;
                }
                if (mountPoint->isRoot()){
                    QStorageInfo info{mountPoint->absolutePath()};
                    index->files.append(info.displayName());
                }
                else
                    index->files.append(mountPoint->dirName());
            }
        }
        else {
            QDir::Filters filter{QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs};
            QDir::SortFlags sorting{QDir::DirsFirst | QDir::IgnoreCase | QDir::Name};
            QStringList nameFilter{};
            if (diropt & TNFS_DIROPT_NO_FOLDERSFIRST) {
                sorting &= ~QDir::DirsFirst;
                sorting |= QDir::DirsLast;
            }
            if (diropt & TNFS_DIROPT_NO_SKIPHIDDEN) {
                filter &= ~QDir::NoDotAndDotDot;
            }
            if (diropt & TNFS_DIROPT_NO_SKIPSPECIAL) {
                filter &= ~QDir::NoDotAndDotDot;
                filter |= QDir::System;
            }
            if (diropt & TNFS_DIROPT_DIR_PATTERN) {
                filter &= ~QDir::AllDirs;
                filter |= QDir::Dirs;
            }
            if (dirsort & TNFS_DIRSORT_NONE) {
                sorting &= ~(QDir::DirsFirst | QDir::Name);
            }
            if (dirsort & TNFS_DIRSORT_CASE) {
                sorting &= ~QDir::IgnoreCase;
            }
            if (dirsort & TNFS_DIRSORT_DESCENDING) {
                sorting |= QDir::Reversed;
            }
            if (dirsort & TNFS_DIRSORT_MODIFIED) {
                sorting &= ~QDir::Name;
                sorting |= QDir::Time;
            }
            if (dirsort & TNFS_DIRSORT_SIZE) {
                sorting &= ~QDir::Name;
                sorting |= QDir::Size;
            }

            if (wildcard.isEmpty())
                wildcard = "*";
            nameFilter.append(wildcard);
            index->files = index->actualDir->entryList(nameFilter, filter, sorting);
        }
        index->fileListIndex = 0;
        openDirs[handle] = index;

        if (maxcount > 0 && maxcount < index->files.count()) {
            // More files than it's asked for. Drop the remaining.
            while (!index->files.isEmpty() && maxcount < index->files.count()) {
                index->files.removeLast();
            }
        }
        answer.setU16At(index->files.count(), 6);

        return answer;
    }

    auto Tnfs::readdirx(const Datagram &datagram) -> Datagram
    {
        auto sessionID{datagram.getSessionID()};
        quint8 handle   = datagram[4];
        quint8 maxCount = datagram[5];
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr)
        {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions[sessionID]};
        auto &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull())
        {
            answer[4] = ENOENT;
            return answer;
        }

        auto index{openDirs[handle]};
        if (index->fileListIndex >= index->files.length())
        {
            answer[4] = TNFS_EOF;
            return answer;
        }

        QString fileName{};
        if (index->isVirtualRoot) {
            fileName = "/";
            fileName.append(index->files.at(index->fileListIndex));
            auto dir{sessionInfo->realPath(fileName)};
            fileName = QDir::toNativeSeparators(dir->absolutePath());
        }
        else {
            fileName = index->actualDir->absoluteFilePath(index->files.at(index->fileListIndex));
        }

        QFileInfo fileInfo{fileName};
        answer[5] = 1;
        answer[6] = 0;
        if (index->fileListIndex + 1 == index->files.count()) { // Last entry
            answer[6] = TNFS_DIRSTATUS_EOF;
        }
        answer.setU16At(index->fileListIndex, 7);
        quint8 entry{0};
        if (fileInfo.isDir())
            entry |= TNFS_DIRENTRY_DIR;
        if (fileInfo.isHidden())
            entry |= TNFS_DIRENTRY_HIDDEN;
        answer[9] = entry;
        answer.setU32At(fileInfo.size(), 10);
        answer.setU32At(fileInfo.lastModified().toTime_t(), 14);
        answer.setU32At(fileInfo.lastModified().toTime_t(), 18);
        answer.setStringAt(index->files[index->fileListIndex], 22);
        index->fileListIndex++;

        return answer;
    }

    auto Tnfs::telldir(const Datagram &datagram) -> Datagram {
        auto sessionID{datagram.getSessionID()};
        quint8 handle = datagram[4];
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions[sessionID]};
        auto &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull() || !openDirs[handle]->actualDir->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        answer.setU32At(openDirs[handle]->fileListIndex, 5);

        return answer;
    }

    auto Tnfs::seekdir(const Datagram &datagram) -> Datagram {
        auto sessionID{datagram.getSessionID()};
        quint8 handle = datagram[4];
        auto seekIndex{datagram.getU32At(5)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }
// TODO remove openDirs
        auto sessionInfo{sessions[sessionID]};
        auto &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull() ||
            (!openDirs[handle]->isVirtualRoot && !openDirs[handle]->actualDir->exists())
        ) {
            answer[4] = ENOENT;
            return answer;
        }

        if (seekIndex >= openDirs[handle]->files.count()) {
            answer[4] = EINVAL;
            return answer;
        }

        openDirs[handle]->fileListIndex = seekIndex;
        return answer;
    }

    auto Tnfs::closedir(const Datagram &datagram) -> Datagram {
        auto sessionID{datagram.getSessionID()};
        quint8 handle = datagram[4];
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions[sessionID]};
        auto &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull()) {
            answer[4] = ENOENT;
            return answer;
        }

        openDirs[handle].clear();
        return answer;
    }

    auto Tnfs::mkdir(const Datagram &datagram) const -> Datagram {
        auto sessionID{datagram.getSessionID()};
        auto dirName{datagram.getStringAt(4)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        if (dirName.startsWith('/'))
            dirName.remove(0, 1);
        auto realPath{sessionInfo->realPath(dirName)};
        if (realPath->exists()) {
            answer[4] = EEXIST;
            return answer;
        }

        if (!realPath->mkdir("")) {
            answer[4] = EACCES;
            return answer;
        }

        return answer;
    }

    auto Tnfs::rmdir(const Datagram &datagram) const -> Datagram {
        auto sessionID{datagram.getSessionID()};
        auto dirName{datagram.getStringAt(4)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        if (dirName.startsWith('/'))
            dirName.remove(0, 1);
        auto realPath{sessionInfo->realPath(dirName)};
        if (!realPath->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        if (!realPath->rmdir("")) {
            answer[4] = EACCES;
            return answer;
        }

        return answer;
    }

    auto Tnfs::openfile(const Datagram &datagram) -> Datagram {
        auto sessionID{datagram.getSessionID()};
        auto flags{datagram.getU16At(4)};
        auto mode{datagram.getU16At(6)};
        auto fileName{datagram.getStringAt(8)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        auto &openFiles{sessionInfo->openFiles()};
        qint16 handle{findFreeSlot(openFiles)};
        answer[5] = handle;
        if(handle < 0) {
            answer[4] = EMFILE;
            return answer;
        }

        auto file = new QFile(sessionInfo->realFileName(fileName));
        QIODevice::OpenMode qflags{QIODevice::NotOpen};
        if (flags & 0x0001)
            qflags |= QIODevice::ReadOnly;
        if (flags & 0x0002)
            qflags |= QIODevice::WriteOnly;
        if (flags & 0x0003)
            qflags |= QIODevice::ReadWrite;
        if (flags & O_APPEND)
            qflags |= QIODevice::Append;
        if (flags & O_TRUNC)
            qflags |= QIODevice::Truncate;
        // We must simulate ~O_CREAT and O_EXCL
        if (((flags & 0x0002) | (flags & 0x0003)) && (flags & O_CREAT) == 0 && !file->exists()) {
            // Qt always creates a file, when in write mode.
            // So we simulate not to automatically create the file, when O_CREAT is not set.
            answer[4] = ENOENT;
            return answer;
        }
        if (((flags & 0x0002) | (flags & 0x0003)) && (flags & (O_CREAT | O_EXCL)) && file->exists()) {
            // O_EXCL with O_CREAT means that, that a file is only created, if it does not exist.
            answer[4] = EEXIST;
            return answer;
        }

        file->setPermissions(static_cast<QFileDevice::Permissions>(mode));
        if (!file->open(qflags)) {
            answer[4] = EACCES;
            return answer;
        }
        openFiles[handle] = QFilePtr(file);

        return answer;
    }

    auto Tnfs::closefile(const Datagram &datagram) const -> Datagram {
        auto sessionID{datagram.getSessionID()};
        quint8 handle{static_cast<quint8>(datagram.at(4))};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        auto& openFiles{sessionInfo->openDirectories()};
        if (!openFiles[handle].isNull()) {
            answer[4] = ENOENT;
            return answer;
        }

        openFiles[handle].clear();
        return answer;
    }

    auto Tnfs::readfile(const Datagram &datagram) const -> Datagram
    {
        auto sessionID{datagram.getSessionID()};
        auto handle{static_cast<quint8>(datagram.at(4))};
        auto max{datagram.getU16At(5)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        auto& openFiles{sessionInfo->openFiles()};
        if (openFiles[handle].isNull() || !openFiles[handle]->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        if (!openFiles[handle]->isReadable()) {
            answer[4] = EACCES;
            return answer;
        }

        if (openFiles[handle]->atEnd()) {
            answer[4] = TNFS_EOF;
            return answer;
        }

        max = std::min(max, MAX_PACKET_SIZE); // Clamp the datagram size
        auto buffer {openFiles[handle]->read(max)};
        answer.setU16At(buffer.length(), 5);
        answer.setRawBytes(buffer, 7);

        return answer;
    }

    auto Tnfs::writefile(const Datagram &datagram) const -> Datagram
    {
        auto sessionID{datagram.getSessionID()};
        auto handle{static_cast<quint8>(datagram.at(4))};
        auto length{datagram.getU16At(5)};
        auto data{datagram.getRawBytes(length, 7)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        auto& openFiles{sessionInfo->openFiles()};
        if (openFiles[handle].isNull() || !openFiles[handle]->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        if (!openFiles[handle]->isWritable()) {
            answer[4] = EACCES;
            return answer;
        }

        auto written = openFiles[handle]->write(data);
        answer.setU16At(written, 5);

        return answer;
    }

    auto Tnfs::seekfile(const Datagram &datagram) const -> Datagram
    {
        auto sessionID{datagram.getSessionID()};
        auto handle{static_cast<quint8>(datagram.at(4))};
        auto type{static_cast<quint8>(datagram.at(5))};
        unsigned char a = datagram.at(6);
        unsigned char b = datagram.at(7);
        unsigned char c = datagram.at(8);
        unsigned char d = datagram.at(9);
        qint32 position = a | b << 8 | c << 16 | d << 24;
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        auto& openFiles{sessionInfo->openFiles()};
        if (openFiles[handle].isNull() || !openFiles[handle]->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        /*if (!openFiles[handle]->isReadable()) {
            answer[4] = EACCES;
            return answer;
        }*/

        auto file{openFiles[handle]};
        bool success{};
        if (type == SEEK_SET)
            success = file->seek(position);
        else if (type == SEEK_CUR)
            success = file->seek(file->pos() + position);
        else if (type == SEEK_END)
            success = file->seek(file->size() + position);

        if (!success) {
            answer[4] = EINVAL;
            return answer;
        }

        answer.setU32At(file->pos(), 5);
        return answer;
    }

    auto Tnfs::statfile(const Datagram &datagram) const -> Datagram
    {
        quint16 sessionID{datagram.getSessionID()};
        auto fileName{datagram.getStringAt(4)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        QFileInfo fileInfo{sessionInfo->realFileName(fileName)};
        answer.setU16At(fileInfo.permissions(), 5);
        // On Windows and other system ownerID and groupID will return -2. TNFS needs 0.
        quint16 id{static_cast<quint16>(fileInfo.ownerId())};
        if (id < 0)
            id = 0;
        answer.setU16At(id, 7);
        id = fileInfo.groupId();
        if (id < 0)
            id = 0;
        answer.setU16At(id, 9);
        answer.setU32At(fileInfo.size(), 11);
        answer.setU32At(fileInfo.lastRead().toTime_t(), 15);
        answer.setU32At(fileInfo.lastModified().toTime_t(), 19);
        answer.setU32At(fileInfo.birthTime().toTime_t(), 23);
        answer.setStringAt(fileInfo.owner(), 27);
        answer.setStringAt(fileInfo.group(), 28 + fileInfo.owner().length());

        return answer;
    }

    auto Tnfs::unlinkfile(const Datagram &datagram) const -> Datagram
    {
        auto sessionID{datagram.getSessionID()};
        auto fileName{datagram.getStringAt(4)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        QFile file{sessionInfo->realFileName(fileName)};
        if (file.exists() && !file.remove()) {
            answer[4] = EACCES;
            return answer;
        }

        return answer;
    }

    auto Tnfs::chmodfile(const Datagram &datagram) const -> Datagram
    {
        auto sessionID{datagram.getSessionID()};
        auto permissions{datagram.getU16At(4)};
        auto fileName{datagram.getStringAt(6)};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        QFile file{sessionInfo->realFileName(fileName)};
        if (!file.exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        if (!file.setPermissions(static_cast<QFileDevice::Permissions>(permissions))) {
            answer[4] = EACCES;
            return answer;
        }

        return answer;
    }

    auto Tnfs::renamefile(const Datagram &datagram) const -> Datagram
    {
        auto sessionID{datagram.getSessionID()};
        auto fileName{datagram.getStringAt(4)};
        auto newName{datagram.getStringAt(5 + fileName.length())};
        auto answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        auto sessionInfo{sessions.at(sessionID)};
        QFile file{sessionInfo->realFileName(fileName)};
        if (!file.exists()) {
            answer[4] = ENOENT;
            return answer;
        }
        if (!file.rename(newName)) {
            answer[4] = EACCES;
            return answer;
        }

        return answer;
    }


    auto Tnfs::fsSize(const Datagram &datagram) -> Datagram
    {
        //auto sessionID{datagram.getSessionID()};
        auto fileName{datagram.getStringAt(4)};
        auto answer{datagram.createAnswer()};

        answer[4] = 0;
        QStorageInfo disk{*(_mountPoints.first())};
        answer.setU32At(static_cast<quint32>(disk.bytesTotal() / 1024), 5);
        return answer;
    }

    auto Tnfs::fsFree(const Datagram &datagram) -> Datagram
    {
        //auto sessionID{datagram.getSessionID()};
        auto fileName{datagram.getStringAt(4)};
        auto answer{datagram.createAnswer()};

        answer[4] = 0;
        QStorageInfo disk{*(_mountPoints.first())};
        answer.setU32At(static_cast<quint32>(disk.bytesAvailable() / 1024), 5);
        return answer;
    }
}
