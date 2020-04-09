#ifndef SIORECORDER_H
#define SIORECORDER_H

#include <QObject>
#include <QXmlStreamWriter>
#include <QFile>
#include <memory>

namespace Tests
{
    class SioRecorder;
    using SioRecorderPtr = std::shared_ptr<SioRecorder>;
    using QXmlStreamWriterPtr = std::unique_ptr<QXmlStreamWriter>;

    class SioRecorder
    {
    public:
        SioRecorder() = default;
        virtual ~SioRecorder() = default;

        static SioRecorderPtr instance() { return sInstance; }

        void startSIOSnapshot(QFile* file);
        void stopSIOSnapshot();
        bool isSnapshotRunning() const { return mSnapshotRunning; }
        void writeSnapshotCommandFrame(const qint8 no, const qint8 command, const qint8 aux1, const qint8 aux2);
        void writeSnapshotDataFrame(const QByteArray &data);

        void replaySIOSnaphot();

    private:
        bool mSnapshotRunning{false};
        QXmlStreamWriterPtr mSnapshotWriter{nullptr};
        static SioRecorderPtr sInstance;

    };
}
#endif // SIORECORDER_H
