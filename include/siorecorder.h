#ifndef SIORECORDER_H
#define SIORECORDER_H

#include "serialport.h"

#include <QFile>
#include <QJsonArray>
#include <QObject>
#include <memory>
#include <vector>

namespace Tests {
  class SioRecorder;
  using SioRecorderPtr = std::shared_ptr<SioRecorder>;

  class SioRecorder final : public AbstractSerialPortBackend {
  public:
    SioRecorder(QObject *parent = nullptr);
    virtual ~SioRecorder();

    static SioRecorderPtr instance() { return sInstance; }

    /* Snapshot API */
    void startSIOSnapshot();
    QByteArray stopSIOSnapshot();
    bool isSnapshotRunning() const { return mSnapshotRunning; }
    void writeSnapshotCommandFrame(const qint8 no, const qint8 command, const qint8 aux1, const qint8 aux2);
    void writeSnapshotDataFrame(const QByteArray &data);

    void prepareReplaySnapshot(QFile *file, SerialBackend prevSerialBackend);

    void writePauseCommand(int msec);

    /* SerialPortBackend API */
    bool open() override;
    bool isOpen() override;
    void close() override;
    void cancel() override;
    int speedByte() override;
    QByteArray readCommandFrame() override;
    QByteArray readDataFrame(uint size, bool verbose = true) override;
    bool writeDataFrame(const QByteArray &data) override;
    bool writeCommandAck() override;
    bool writeCommandNak() override;
    bool writeDataAck() override;
    bool writeDataNak() override;
    bool writeComplete() override;
    bool writeError() override;
    bool setSpeed(int speed) override;
    bool writeRawFrame(const QByteArray &data) override;
    void setActiveSioDevices(const QByteArray &data) override;
    int speed() override;
    void forceHighSpeed(int speed) override;

  protected:
    bool readPauseTag();
    void forwardXml();

  private:
    bool mSnapshotRunning{false};
    std::unique_ptr<QJsonArray> mSnapshotData{nullptr};
    std::vector<char> mTestData{};
    int mReadIndex{0};
    SerialBackend mPreviousBackend{SerialBackend::NONE};

    static SioRecorderPtr sInstance;
  };
}// namespace Tests
#endif// SIORECORDER_H
