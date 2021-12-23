#include "siorecorder.h"
#include "mainwindow.h"
#include "make_unique.h"
#include "respeqtsettings.h"

#include <QFileDialog>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace Tests {
    SioRecorderPtr SioRecorder::sInstance = std::make_shared<SioRecorder>();

    SioRecorder::SioRecorder(QObject *parent)
        : AbstractSerialPortBackend(parent)
    {}

    SioRecorder::~SioRecorder()
    {
        if (isOpen()) {
            close();
        }
    }

    void SioRecorder::startSIOSnapshot()
    {
        if (!mSnapshotData)
        {
            mSnapshotData = std::make_unique<QJsonArray>();
        }
        mSnapshotRunning = true;
    }

    QByteArray SioRecorder::stopSIOSnapshot()
    {
        QByteArray result{};
        if (mSnapshotData)
        {
            QJsonDocument document = QJsonDocument(*mSnapshotData);
            result = document.toJson();
            mSnapshotRunning = false;
            mSnapshotData.reset();
        }
        return result;
    }

    void SioRecorder::writeSnapshotCommandFrame(const qint8 no, const qint8 command, const qint8 aux1, const qint8 aux2)
    {
        // Record the command frame, if the snapshot is running
        if (mSnapshotRunning && mSnapshotData)
        {
            QJsonObject commandframe{};
            commandframe["device"] = QJsonValue(no);
            commandframe["command"] = QJsonValue(command);
            commandframe["aux1"] = QJsonValue(aux1);
            commandframe["aux2"] = QJsonValue(aux2);
            mSnapshotData->push_back(commandframe);
        }
    }

    void SioRecorder::writeSnapshotDataFrame(const QByteArray &data, bool reading)
    {
        // Record the command frame, if the snapshot is running
        if (mSnapshotRunning && mSnapshotData)
        {
            QJsonObject dataframe{};
            dataframe["direction"] = reading ? "Read" : "Write";

            QString text{};
            text.reserve(data.size());
            for(auto byte: data)
                text.push_back(QChar{byte});
            dataframe["data"] = QJsonValue(text);
            mSnapshotData->push_back(dataframe);
        }
    }

    void SioRecorder::writePauseCommand(int msec)
    {
        if (mSnapshotRunning && mSnapshotData)
        {
            QJsonObject pause{};
            pause["pause"] = QJsonValue(msec);
            mSnapshotData->push_back(pause);
        }
    }

    void SioRecorder::prepareReplaySnapshot(QFile *file, SerialBackend previousBackend)
    {
        mPreviousBackend = previousBackend;
        if (file->isOpen() && file->isReadable())
        {
            // Now we read the whole file to memory.
            auto size = file->size();
            mTestData.resize(size+1); // Don't forget the 0 terminator
            QDataStream data(file);
            data.readRawData(reinterpret_cast<char*>(mTestData.data()), size);
            mTestData[size] = 0;
        } else
            mTestData.clear();
    }

    bool SioRecorder::open()
    {
        if (isOpen()) {
            close();
        }
        // Open Json and parse it
        if (mTestData.size() == 0)
            return false;

        auto document = QJsonDocument::fromJson(QByteArray{mTestData.data()});

        mSnapshotData.reset();
        if (document.isNull() || !document.isArray())
            return false;

        mSnapshotData = std::make_unique<QJsonArray>(document.array());
        mReadIndex = 0;

        return true;
    }

    bool SioRecorder::isOpen()
    {
        return !mSnapshotData;
    }

    void SioRecorder::close()
    {
        cancel();
    }

    void SioRecorder::cancel()
    {
        mSnapshotData.reset();
        if (mPreviousBackend != SerialBackend::NONE)
        {
            RespeqtSettings::instance()->setBackend(mPreviousBackend);
            mPreviousBackend = SerialBackend::NONE;
        }
    }

    int SioRecorder::speedByte()
    {
        return 0x28; // standard speed (19200)
    }

    int SioRecorder::speed()
    {
        return 19200;
    }

    bool SioRecorder::setSpeed(int /*speed*/)
    {
        return true;
    }

    void SioRecorder::forceHighSpeed(int /*speed*/)
    {
    }

    bool SioRecorder::readPauseTag()
    {
        // How about more than one pause command?
        auto data = mSnapshotData->at(mReadIndex);
        if (data == QJsonValue::Undefined || !data.isObject())
            return false;

        auto object = data.toObject();
        if (object.contains("pause"))
        {
            auto value = object.value("pause").toInt();
            if (value > 0)
            {
                qDebug() << "!d" << tr("Sleeping %1 milliseconds").arg(value);
                QThread::currentThread()->msleep(value);
            }
            mReadIndex++;
        }
        return true;
    }

    QByteArray SioRecorder::readCommandFrame()
    {
        // We look whether there is a pause tag
        if (!readPauseTag())
            return {};

        auto data = mSnapshotData->at(mReadIndex);
        if (data == QJsonValue::Undefined || !data.isObject())
            return {};

        auto object = data.toObject();
        if (!object.contains("command"))
            return {};

        QByteArray result{4, 0};
        auto temp = object["device"].toInt(-1);
        if (temp == -1)
            return {};
        result[0] = temp;

        temp = object["command"].toInt(-1);
        if (temp == -1)
            return {};
        result[1] = temp;

        temp = object["aux1"].toInt(-1);
        if (temp == -1)
            return {};
        result[2] = temp;

        temp = object["aux2"].toInt(-1);
        if (temp == -1)
            return {};
        result[3] = temp;

        mReadIndex++;
        return result;
    }

    /// TODO Be more verbose about failures.
    QByteArray SioRecorder::readDataFrame(uint size, bool /*isCommandFrame*/, bool verbose)
    {
        // We look whether there is a pause tag
        if (!readPauseTag())
            return {};

        auto data = mSnapshotData->at(mReadIndex);
        if (data == QJsonValue::Undefined || !data.isObject())
            return {};

        auto object = data.toObject();
        if (!object.contains("data") && !object.contains("direction"))
            return {};

        if (object["direction"].toString("") != "Read")
            return {};

        auto framedata = object["data"].toString("");
        if (framedata.isEmpty() || static_cast<int>(size) != framedata.size())
            return {};

        if (verbose)
            qDebug() << "!d" << tr("Read data frame with size %1").arg(size);

        mReadIndex++;
        return framedata.toLatin1();
    }

    // The playback never writeDataFrame, we just want to feed RespeQt some data and watch the reaction.
    bool SioRecorder::writeDataFrame(const QByteArray &/*data*/)
    {
        mReadIndex++; // Since we write out write frames, we have to jump over them
        return true;
    }

    bool SioRecorder::writeCommandAck()
    {
        return true;
    }

    bool SioRecorder::writeCommandNak()
    {
        return true;
    }

    bool SioRecorder::writeDataAck()
    {
        return true;
    }

    bool SioRecorder::writeDataNak()
    {
        return true;
    }

    bool SioRecorder::writeComplete()
    {
        return true;
    }

    bool SioRecorder::writeError()
    {
        return true;
    }

    bool SioRecorder::writeRawFrame(const QByteArray &/*data*/)
    {
        return true;
    }

    void SioRecorder::setActiveSioDevices(const QByteArray &/*data*/)
    {}

}
