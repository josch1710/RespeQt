#ifndef SIORECORDER_H
#define SIORECORDER_H

#include "sio/serialport.h"

#include <QFile>
#include <QJsonArray>
#include <QObject>
#include <QSharedPointer>
#include <memory>
#include <vector>

namespace SIO {

class SioRecorder;
using SioRecorderPtr = QSharedPointer<SioRecorder>;

class SioRecorder final : public AbstractSerialPortBackend {
public:
  explicit SioRecorder(QObject *parent = nullptr);
  ~SioRecorder() override;

  static SioRecorderPtr instance() { return sInstance; }

  /* Snapshot API */
  void startSIOSnapshot();
  QByteArray stopSIOSnapshot();
  [[nodiscard]] bool isSnapshotRunning() const { return mSnapshotRunning; }
  void writeSnapshotCommandFrame(quint8 no, quint8 command, quint8 aux1, quint8 aux2) const;
  void writeSnapshotDataFrame(const QByteArray &data) const;

  void prepareReplaySnapshot(QFile *file, SerialBackend previousBackend);

  void writePauseCommand(int msec) const;

  /* SerialPortBackend API */
  bool open() override;
  bool isOpen() override;
  void close() override;
  void cancel() override;
  int speedByte() override;
  QByteArray readCommandFrame() override;
  QByteArray readDataFrame(uint size, bool isCommandFrame, bool verbose = true) override;
  bool writeDataFrame(const QByteArray &data) override;
  bool writeCommandAck() override;
  bool writeCommandNak() override;
  bool writeDataAck() override;
  bool writeDataNak() override;
  bool writeComplete() override;
  bool writeError() override;
  bool setSpeed(unsigned long speed) override;
  unsigned long speed() override;
  bool writeRawFrame(const QByteArray &data) override;
  void setActiveSioDevices(const QByteArray &data) override;
  void forceHighSpeed(unsigned int speed) override;

protected:
  bool readPauseTag();

private:
  bool mSnapshotRunning{false};
  // QJsonArray is not a QObject, so we can't use QPointer
  std::unique_ptr<QJsonArray> mSnapshotData{nullptr};
  std::vector<char> mTestData;
  int mReadIndex{0};
  SerialBackend mPreviousBackend{SerialBackend::NONE};

  static SioRecorderPtr sInstance;
};

} // namespace SIO

#endif// SIORECORDER_H
