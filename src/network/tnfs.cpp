#include "network/tnfs.h"
#include "network/sessioninfo.h"
#include "network/datagram.h"

#include <fcntl.h>
#include <algorithm>
#include <QDateTime>
#include <QStorageInfo>

namespace Network {

    Tnfs::Tnfs(): QObject() {
        socket = new QUdpSocket(this);
        socket->bind(QHostAddress::Any, 16384);

        connect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    }

    Tnfs::~Tnfs() {
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
        delete socket;
        socket = NULL;
    }

    auto Tnfs::removeMountPoint(QDir mountPoint) -> void {
        _mountPoints.removeOne(QDirPtr::create(mountPoint));
    }

    auto Tnfs::addMountPoint(QDir mountPoint) -> void {
        _mountPoints.append(QDirPtr::create(mountPoint));
    }

    void Tnfs::readPendingDatagrams() {
        while (socket->hasPendingDatagrams()) {
            Datagram datagram, answer;
            datagram.resize(socket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            socket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
qDebug() << "!n" << "Incoming command 0x"<<QString::number((unsigned char)datagram.at(3),16);
            switch(datagram.at(3)) {
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

                default:
                {
                    qDebug() << "!n" << "Unknown command 0x"<< QString::number((unsigned char)datagram.at(3),16);
                    //for(auto i=0;i<datagram.size();i++) qDebug()<<"!n" << i << " => "<<(unsigned)datagram.at(i);

                    answer = datagram.createAnswer();
                    answer[4] = EINVAL;
                }
            }
            if (answer.length() > 0) {
                socket->writeDatagram(answer, sender, senderPort);
            }
        }
    }

    /* -------------------- TNFS commands --------------- */
    auto Tnfs::mount(const Datagram &datagram) -> Datagram {
        //auto retry{datagram.at(2)};
        //auto version{getU16At(datagram, 4)};
        /*auto*/QString mountPoint{datagram.getStringAt(6)};
        /*auto*/QString userID{datagram.getStringAt(7 + mountPoint.length())};
        /*auto*/QString password{datagram.getStringAt(7 + mountPoint.length() + userID.length())};
        Datagram answer{};

        /*auto*/QDir root{QDir::homePath().append(mountPoint)};
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
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(datagram.getSessionID()).isNull()) {
            answer[4] = EINVAL;
            return answer;
        }
        sessions.remove(datagram.getSessionID());

        auto isSessionsEmpty = sessions.end() == std::find_if_not(sessions.begin(), sessions.end(),
            [](SessionInfoPtr session) {
              return session.isNull();
            }
        );
        if (isSessionsEmpty)
            emit allSessionsDisconnected(); // Inform the mainwindow, that all session are closed.

        return answer;
    }

    auto Tnfs::opendir(const Datagram &datagram) -> Datagram {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/QString dirName{datagram.getStringAt(4)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).isNull()) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions[sessionID]};
        /*auto*/QDirPtr pathName{sessionInfo->realPath(dirName)};
        if (dirName != "/" && !pathName.isNull() && !pathName->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        /*auto*/QDirIndexVector &openDirs{sessionInfo->openDirectories()};
        qint16 handle{findFreeSlot(openDirs)};
        if(handle < 0) {
            answer[4] = EMFILE;
            return answer;
        }
        answer[5] = handle;

        /*auto*/QDirIndexPtr index{QDirIndexPtr::create()};
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
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        quint8 handle = datagram[4];
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions[sessionID]};
        /*auto*/QDirIndexVector &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull()) {
            answer[4] = ENOENT;
            return answer;
        }

        /*auto*/QDirIndexPtr index{openDirs[handle]};
        if (index->fileListIndex >= index->files.length()) {
            answer[4] = TNFS_EOF;
            return answer;
        }

        answer.setStringAt(index->files[index->fileListIndex], 5);
        index->fileListIndex++;

        return answer;
    }

    auto Tnfs::opendirx(const Datagram &datagram) -> Datagram {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/quint8 diropt{static_cast<quint8>(datagram.at(4))};
        /*auto*/quint8 dirsort{static_cast<quint8>(datagram.at(5))};
        /*auto*/quint16 maxcount{datagram.getU16At(6)};
        /*auto*/QString wildcard{datagram.getStringAt(8)};
        /*auto*/QString dirName{datagram.getStringAt(9 + wildcard.length())};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).isNull()) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions[sessionID]};
        /*auto*/QDirPtr pathName{sessionInfo->realPath(dirName)};
        if (dirName != "/" && !pathName.isNull() && !pathName->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        /*auto*/QDirIndexVector &openDirs{sessionInfo->openDirectories()};
        qint16 handle{findFreeSlot(openDirs)};
        if (handle < 0) {
            answer[4] = EMFILE;
            return answer;
        }

        answer[5] = handle;
qDebug()<<"!n"<<"opendirx "<<dirName.toLatin1().constData()<<endl;
        /*auto*/QDirIndexPtr index{QDirIndexPtr::create()};
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
        /*auto*/ quint16 sessionID{datagram.getSessionID()};
        quint8 handle   = datagram[4];
        quint8 maxCount = datagram[5];
        /*auto*/ Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr)
        {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/ SessionInfoPtr sessionInfo{sessions[sessionID]};
        /*auto*/ QDirIndexVector &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull())
        {
            answer[4] = ENOENT;
            return answer;
        }

        /*auto*/ QDirIndexPtr index{openDirs[handle]};
        if (index->fileListIndex >= index->files.length())
        {
            answer[4] = TNFS_EOF;
            return answer;
        }

        QString fileName{};
        if (index->isVirtualRoot) {
            fileName = "/";
            fileName.append(index->files.at(index->fileListIndex));
            /*auto*/QDirPtr dir{sessionInfo->realPath(fileName)};
            fileName = QDir::toNativeSeparators(dir->absolutePath());
        }
        else {
            fileName = index->actualDir->absoluteFilePath(index->files.at(index->fileListIndex));
        }

qDebug()<<"!n"<<fileName<<" => "<<QDir::toNativeSeparators(fileName)<<endl;
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
qDebug()<<"!n"<<fileInfo.absoluteFilePath()<<" exists"<<fileInfo.exists() <<"entry "<<entry<<endl;
        answer.setU32At(fileInfo.size(), 10);
        answer.setU32At(fileInfo.lastModified().toTime_t(), 14);
        answer.setU32At(fileInfo.lastModified().toTime_t(), 18);
        answer.setStringAt(index->files[index->fileListIndex], 22);
        index->fileListIndex++;

        return answer;
    }

    auto Tnfs::telldir(const Datagram &datagram) -> Datagram {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        quint8 handle = datagram[4];
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions[sessionID]};
        /*auto*/QDirIndexVector &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull() || !openDirs[handle]->actualDir->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        answer.setU32At(openDirs[handle]->fileListIndex, 5);

        return answer;
    }

    auto Tnfs::seekdir(const Datagram &datagram) -> Datagram {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        quint8 handle = datagram[4];
        /*auto*/quint32 seekIndex{datagram.getU32At(5)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }
// TODO openDirs wegmachen
        /*auto*/SessionInfoPtr sessionInfo{sessions[sessionID]};
        /*auto*/QDirIndexVector &openDirs{sessionInfo->openDirectories()};
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
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        quint8 handle = datagram[4];
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions[sessionID]};
        /*auto*/QDirIndexVector &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull()) {
            answer[4] = ENOENT;
            return answer;
        }

        openDirs[handle].clear();
        return answer;
    }

    auto Tnfs::mkdir(const Datagram &datagram) -> Datagram {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/QString dirName{datagram.getStringAt(4)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
        if (dirName.startsWith('/'))
            dirName.remove(0, 1);
        /*auto*/QDirPtr realPath{sessionInfo->realPath(dirName)};
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

    auto Tnfs::rmdir(const Datagram &datagram) -> Datagram {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/QString dirName{datagram.getStringAt(4)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
        if (dirName.startsWith('/'))
            dirName.remove(0, 1);
        /*auto*/QDirPtr realPath{sessionInfo->realPath(dirName)};
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
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/quint16 flags{datagram.getU16At(4)};
        /*auto*/quint16 mode{datagram.getU16At(6)};
        /*auto*/QString fileName{datagram.getStringAt(8)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
        /*auto*/ QFileVector &openFiles{sessionInfo->openFiles()};
        qint16 handle{findFreeSlot(openFiles)};
        answer[5] = handle;
        if(handle < 0) {
            answer[4] = EMFILE;
            return answer;
        }

        /*auto*/QFile* file = new QFile(sessionInfo->realFileName(fileName));
        /*auto*/QIODevice::OpenMode qflags{QIODevice::NotOpen};
        if((flags & O_WRONLY) == 0) // if WRONLY is not set, we want to readonly
            qflags |= QIODevice::ReadOnly;
        if (flags & O_WRONLY)
            qflags |= QIODevice::WriteOnly;
        if (flags & O_RDWR)
            qflags |= QIODevice::ReadWrite;
        if (flags & O_APPEND)
            qflags |= QIODevice::Append;
        if (flags & O_TRUNC)
            qflags |= QIODevice::Truncate;
        // We must simulate ~O_CREAT and O_EXCL
        if (((flags & O_WRONLY) | (flags & O_RDWR)) && (flags & O_CREAT) == 0 && !file->exists()) {
            // Qt always creates a file, when in write mode.
            // So we simulate not to automatically create the file, when O_CREAT is not set.
            answer[4] = ENOENT;
            return answer;
        }
        if (((flags & O_WRONLY) | (flags & O_RDWR)) && (flags & (O_CREAT | O_EXCL)) && file->exists()) {
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

    auto Tnfs::closefile(const Datagram &datagram) -> Datagram {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/quint8 handle{static_cast<quint8>(datagram.at(4))};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
        /*auto*/QDirIndexVector& openFiles{sessionInfo->openDirectories()};
        if (!openFiles[handle].isNull()) {
            answer[4] = ENOENT;
            return answer;
        }

        openFiles[handle].clear();
        return answer;
    }

    auto Tnfs::readfile(const Datagram &datagram) -> Datagram
    {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/quint8 handle{static_cast<quint8>(datagram.at(4))};
        /*auto*/quint16 max{datagram.getU16At(5)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
        /*auto*/QFileVector& openFiles{sessionInfo->openFiles()};
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
        auto buffer = openFiles[handle]->read(max);
        answer.setU16At(buffer.length(), 5);
        answer.setRawBytes(buffer, 7);

        return answer;
    }

    auto Tnfs::writefile(const Datagram &datagram) -> Datagram
    {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/quint8 handle{static_cast<quint8>(datagram.at(4))};
        /*auto*/quint16 length{datagram.getU16At(5)};
        /*auto*/QByteArray data{datagram.getRawBytes(length, 7)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
        /*auto*/QFileVector& openFiles{sessionInfo->openFiles()};
        if (openFiles[handle].isNull() || !openFiles[handle]->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        if (!openFiles[handle]->isWritable()) {
            answer[4] = EACCES;
            return answer;
        }

        /*auto*/qint64 written = openFiles[handle]->write(data);
        answer.setU16At(written, 5);

        return answer;
    }

    auto Tnfs::seekfile(const Datagram &datagram) -> Datagram
    {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/quint8 handle{static_cast<quint8>(datagram.at(4))};
        /*auto*/quint8 type{static_cast<quint8>(datagram.at(5))};
        /*auto*/quint32 position{datagram.getU32At(6)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
        /*auto*/QFileVector& openFiles{sessionInfo->openFiles()};
        if (openFiles[handle].isNull() || !openFiles[handle]->exists()) {
            answer[4] = ENOENT;
            return answer;
        }

        if (!openFiles[handle]->isReadable()) {
            answer[4] = EACCES;
            return answer;
        }

        /*auto*/QFilePtr file{openFiles[handle]};
        bool success{};
        if (type == SEEK_SET)
            success = file->seek(position);
        else if (type == SEEK_CUR)
            success = file->seek(file->pos() + position);
        else if (type == SEEK_END)
            success = file->seek(file->size());

        if (!success) {
            answer[4] = EINVAL;
            return answer;
        }

        answer.setU32At(file->pos(), 5);
        return answer;
    }

    auto Tnfs::statfile(const Datagram &datagram) -> Datagram
    {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/QString fileName{datagram.getStringAt(4)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
        QFileInfo fileInfo{sessionInfo->realFileName(fileName)};
        answer.setU16At(fileInfo.permissions(), 5);
        // On Widnows and other system ownerID and groupID will return -2. TNFS needs 0.
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
        answer.setU32At(fileInfo.created().toTime_t(), 23);
        /*auto*/QString owner{fileInfo.owner()};
        answer.setStringAt(owner, 27);
        answer.setStringAt(fileInfo.group(), 28 + owner.length());

        return answer;
    }

    auto Tnfs::unlinkfile(const Datagram &datagram) -> Datagram
    {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/QString fileName{datagram.getStringAt(4)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
        QFile file{sessionInfo->realFileName(fileName)};
        if (file.exists() && !file.remove()) {
            answer[4] = EACCES;
            return answer;
        }

        return answer;
    }

    auto Tnfs::chmodfile(const Datagram &datagram) -> Datagram
    {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/quint16 permissions{datagram.getU16At(4)};
        /*auto*/QString fileName{datagram.getStringAt(6)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
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

    auto Tnfs::renamefile(const Datagram &datagram) -> Datagram
    {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/QString fileName{datagram.getStringAt(4)};
        /*auto*/QString newName{datagram.getStringAt(5 + fileName.length())};
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(sessionID).data() == nullptr) {
            answer[4] = EINVAL;
            return answer;
        }

        /*auto*/SessionInfoPtr sessionInfo{sessions.at(sessionID)};
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
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/QString fileName{datagram.getStringAt(4)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        answer[4] = 0;
        QStorageInfo disk{*(_mountPoints.first())};
        answer.setU32At(static_cast<quint32>(disk.bytesTotal() / 1024), 5);
        return answer;
    }

    auto Tnfs::fsFree(const Datagram &datagram) -> Datagram
    {
        /*auto*/quint16 sessionID{datagram.getSessionID()};
        /*auto*/QString fileName{datagram.getStringAt(4)};
        /*auto*/Datagram answer{datagram.createAnswer()};

        answer[4] = 0;
        QStorageInfo disk{*(_mountPoints.first())};
        answer.setU32At(static_cast<quint32>(disk.bytesAvailable() / 1024), 5);
        return answer;
    }
}
