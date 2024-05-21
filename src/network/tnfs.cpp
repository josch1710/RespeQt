#include "network/tnfs.h"
#include "network/sessioninfo.h"
#include "network/datagram.h"

#include <fcntl.h>
#include <algorithm>
#include <QDateTime>
#include <QStorageInfo>
#include <iostream>

using std::cout;
using std::endl;

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

    void Tnfs::readPendingDatagrams() {
        while (socket->hasPendingDatagrams()) {
            Datagram datagram, answer;
            datagram.resize(socket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            socket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
            switch(datagram.at(3)) {
                case 0x00: // MOUNT
                    answer = mount(datagram);
                    break;

                case 0x01: // UMOUNT
                    answer = unmount(datagram);
                    break;

                case 0x10: // OPENDIR
                    answer = opendir(datagram);
                    break;

                case 0x11: // READDIR
                    answer = readdir(datagram);
                    break;

                case 0x15: // TELLDIR
                    answer = telldir(datagram);
                    break;

                case 0x16: // SEEKDIR
                    answer = seekdir(datagram);
                    break;

                case 0x12: // CLOSEDIR
                    answer = closedir(datagram);
                    break;

                case 0x13: // MKDIR
                    answer = mkdir(datagram);
                    break;

                case 0x14: // RMDIR
                    answer = rmdir(datagram);
                    break;

                case 0x29: // OPEN
                    answer = open(datagram);
                    break;

                case 0x21: // READ
                    answer = read(datagram);
                    break;

                case 0x23: // CLOSE
                    answer = close(datagram);
                    break;

                case 0x24: // STAT
                    answer = stat(datagram);
                    break;

                default:
                {
                    qDebug() << "!n" << "Unknown command "<< datagram.at(3);

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
            // TODO Version and Timeout => centralize
            // Version number 1.2
            answer.append(static_cast<char>(2));
            answer.append(static_cast<char>(1));
            // 1000 (0x03E8) ms timeout
            answer.append(static_cast<char>(0xE8));
            answer.append(static_cast<char>(0x03));
            return answer;
        }

        SessionInfoPtr session{SessionInfoPtr::create(sessionID)};
        session->addMountPoint(root.absolutePath());
        session->addMountPoint(QString("D:/"));
        sessions[sessionID] = session;

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

        sessionID++;
        return answer;
    }

    auto Tnfs::unmount(const Datagram &datagram) -> Datagram {
        /*auto*/Datagram answer{datagram.createAnswer()};

        if (sessions.at(datagram.getSessionID()).isNull()) {
            answer[4] = EINVAL;
            return answer;
        }
        sessions.remove(sessionID);

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
        quint8 handle = 0;
        // Find the free slot for the handle
        while(handle < openDirs.length() && !openDirs[handle].isNull()) {
            handle++;
        }

        /*auto*/QDirIndexPtr index{QDirIndexPtr::create()};
cout << "Read dir "<<dirName.toLatin1().constData()<<endl;
        index->isVirtualRoot = dirName == "/";
        index->actualDir = pathName;
        index->virtualDir = QDirPtr::create(dirName);
        if (index->isVirtualRoot) {
cout << "Read root" << endl;
            for(auto mountPoint: sessionInfo->mountPoints()){
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
        answer[5] = handle;

        // TODO Cache file entries

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
//cout<<"vdir "<<index->virtualDir->absolutePath().toLatin1().constData()<<endl;
//for(auto file: index->files) cout<<"file "<<file.toLatin1().constData()<<endl;
        if (index->fileListIndex >= index->files.length()) {
            answer[4] = TNFS_EOF;
            return answer;
        }

        answer.setStringAt(index->files[index->fileListIndex++], 5);

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

        /*auto*/SessionInfoPtr sessionInfo{sessions[sessionID]};
        /*auto*/QDirIndexVector &openDirs{sessionInfo->openDirectories()};
        if (openDirs[handle].isNull() || !openDirs[handle]->actualDir->exists()) {
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

    auto Tnfs::open(const Datagram &datagram) -> Datagram {
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

        /*auto*/QFileVector& openFiles{sessionInfo->openFiles()};
        quint8 handle = 0;
        // Find the three slot for the handle
        while(handle < openFiles.length() && !openFiles[handle].isNull()) {
            handle++;
        }

        openFiles[handle] = QFilePtr(file);
        answer[5] = handle;
        return answer;
    }

    auto Tnfs::close(const Datagram &datagram) -> Datagram {
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

    auto Tnfs::read(const Datagram &datagram) -> Datagram
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
        answer.insert(7, buffer);

        return answer;
    }

    auto Tnfs::stat(const Datagram &datagram) -> Datagram
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


}
