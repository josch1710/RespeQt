#ifndef MOCKSERIALPORT_H
#define MOCKSERIALPORT_H

#include "sio/serialport.h"
#include <QByteArray>
#include <QList>
#include <QQueue>

namespace Tests {

class MockSerialPortBackend final : public SIO::AbstractSerialPortBackend {
  Q_OBJECT

public:
  enum class Operation {
    CmdAck,
    CmdNak,
    DataAck,
    DataNak,
    Complete,
    Error,
    WriteData,
    WriteRaw
  };

  struct LoggedOp {
    Operation op;
    QByteArray data;
  };

  explicit MockSerialPortBackend(QObject *parent = nullptr)
      : SIO::AbstractSerialPortBackend(parent), mOpen(true), mSpeed(19200), mSpeedByte(0x28) {}

  ~MockSerialPortBackend() override = default;

  bool open() override {
    mOpen = true;
    return true;
  }

  bool isOpen() override {
    return mOpen;
  }

  void close() override {
    mOpen = false;
  }

  void cancel() override {}

  int speedByte() override {
    return mSpeedByte;
  }

  void setSpeedByte(int byte) {
    mSpeedByte = byte;
  }

  QByteArray readCommandFrame() override {
    if (!mIncomingCommandFrames.isEmpty()) {
      return mIncomingCommandFrames.dequeue();
    }
    return {};
  }

  QByteArray readDataFrame(uint size, bool isCommandFrame, bool verbose = true) override {
    Q_UNUSED(isCommandFrame);
    Q_UNUSED(verbose);
    if (!mIncomingDataFrames.isEmpty()) {
      QByteArray data = mIncomingDataFrames.dequeue();
      if (data.size() > static_cast<int>(size)) {
        return data.left(size);
      }
      return data;
    }
    return {};
  }

  bool writeDataFrame(const QByteArray &data) override {
    mLog.append({Operation::WriteData, data});
    mSentDataFrames.append(data);
    return true;
  }

  bool writeCommandAck() override {
    mLog.append({Operation::CmdAck, {}});
    return true;
  }

  bool writeCommandNak() override {
    mLog.append({Operation::CmdNak, {}});
    return true;
  }

  bool writeDataAck() override {
    mLog.append({Operation::DataAck, {}});
    return true;
  }

  bool writeDataNak() override {
    mLog.append({Operation::DataNak, {}});
    return true;
  }

  bool writeComplete() override {
    mLog.append({Operation::Complete, {}});
    return true;
  }

  bool writeError() override {
    mLog.append({Operation::Error, {}});
    return true;
  }

  unsigned long speed() override {
    return mSpeed;
  }

  bool setSpeed(unsigned long speed) override {
    mSpeed = speed;
    return true;
  }

  bool writeRawFrame(const QByteArray &data) override {
    mLog.append({Operation::WriteRaw, data});
    return true;
  }

  void setActiveSioDevices(const QByteArray &data) override {
    Q_UNUSED(data);
  }

  void forceHighSpeed(unsigned int speed) override {
    mSpeed = speed;
  }

  // Test helpers
  void queueDataFrame(const QByteArray &data) {
    mIncomingDataFrames.enqueue(data);
  }

  void queueCommandFrame(const QByteArray &frame) {
    mIncomingCommandFrames.enqueue(frame);
  }

  void clearLog() {
    mLog.clear();
    mSentDataFrames.clear();
  }

  const QList<LoggedOp> &log() const {
    return mLog;
  }

  const QList<QByteArray> &sentDataFrames() const {
    return mSentDataFrames;
  }

  bool hasOp(Operation op) const {
    for (const auto &item : mLog) {
      if (item.op == op) return true;
    }
    return false;
  }

  int countOp(Operation op) const {
    int count = 0;
    for (const auto &item : mLog) {
      if (item.op == op) ++count;
    }
    return count;
  }

private:
  bool mOpen;
  unsigned long mSpeed;
  int mSpeedByte;
  QQueue<QByteArray> mIncomingCommandFrames;
  QQueue<QByteArray> mIncomingDataFrames;
  QList<LoggedOp> mLog;
  QList<QByteArray> mSentDataFrames;
};

} // namespace Tests

#endif // MOCKSERIALPORT_H
