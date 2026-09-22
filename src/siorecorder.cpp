#include "siorecorder.h"
#include "ui/mainwindow.h"
#include "respeqtsettings.h"

#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

SioRecorderPtr SioRecorder::sInstance = SioRecorderPtr(new SioRecorder());

SioRecorder::SioRecorder(QObject *parent)
    : AbstractSerialPortBackend(parent) {}

SioRecorder::~SioRecorder() {
  if (isOpen()) {
    close();
  }
}

void SioRecorder::startSIOSnapshot() {
  if (!mSnapshotData) {
    mSnapshotData = std::make_unique<QJsonArray>();
  }
  mSnapshotRunning = true;
}

QByteArray SioRecorder::stopSIOSnapshot() {
  QByteArray result{};
  if (mSnapshotData) {
    const auto document = QJsonDocument(*mSnapshotData);
    result = document.toJson();
    mSnapshotRunning = false;
    mSnapshotData.reset();
  }
  return result;
}

void SioRecorder::writeSnapshotCommandFrame(const quint8 no, const quint8 command, const quint8 aux1, const quint8 aux2) const
{
  // Record the command frame, if the snapshot is running
  if (mSnapshotRunning && mSnapshotData) {
    QJsonObject commandframe{};
    commandframe["device"] = QJsonValue(no);
    commandframe["command"] = QJsonValue(command);
    commandframe["aux1"] = QJsonValue(aux1);
    commandframe["aux2"] = QJsonValue(aux2);
    mSnapshotData->push_back(commandframe);
    qDebug() << "!n" << "command frame";
  }
}

void SioRecorder::writeSnapshotDataFrame(const QByteArray &data) const
{
  // Record the command frame, if the snapshot is running
  if (mSnapshotRunning && mSnapshotData) {
    QJsonObject dataframe{};
    QString text{};
    text.reserve(data.size());
    for (const auto byte: data)
      text.push_back(QChar{byte});
    dataframe["data"] = QJsonValue(text);
    mSnapshotData->push_back(dataframe);
  }
}

void SioRecorder::writePauseCommand(const int msec) const
{
  if (mSnapshotRunning && mSnapshotData) {
    QJsonObject pause{};
    pause["pause"] = QJsonValue(msec);
    mSnapshotData->push_back(pause);
  }
}

void SioRecorder::prepareReplaySnapshot(QFile *file, const SerialBackend previousBackend) {
  mPreviousBackend = previousBackend;
  if (file->isOpen() && file->isReadable()) {
    // Now we read the whole file to memory.
    // ReSharper disable once CppRedundantCastExpression
    const auto size = static_cast<size_t>(file->size());
    mTestData.resize(size + 1);// Don't forget the 0 terminator
    QDataStream data(file);
    data.readRawData(mTestData.data(), static_cast<int>(size));
    mTestData[size] = 0;
  } else
    mTestData.clear();
}

bool SioRecorder::open() {
  if (isOpen()) {
    close();
  }
  // Open Json and parse it
  if (mTestData.empty())
    return false;

  const auto document = QJsonDocument::fromJson(QByteArray{mTestData.data()});

  mSnapshotData.reset();
  if (document.isNull() || !document.isArray())
    return false;

  mSnapshotData = std::make_unique<QJsonArray>(document.array());
  mReadIndex = 0;

  return true;
}

bool SioRecorder::isOpen() {
  return !mSnapshotData;
}

void SioRecorder::close() {
  cancel();
}

void SioRecorder::cancel() {
  mSnapshotData.reset();
  if (mPreviousBackend != SerialBackend::NONE) {
    RespeqtSettings::instance()->setBackend(mPreviousBackend);
    mPreviousBackend = SerialBackend::NONE;
  }
}

int SioRecorder::speedByte() {
  return 0x28;// standard speed (19200)
}

unsigned long SioRecorder::speed() {
  return 19200;
}

bool SioRecorder::setSpeed(unsigned long /*speed*/) {
  return true;
}

void SioRecorder::forceHighSpeed(unsigned int /*speed*/) {
}

bool SioRecorder::readPauseTag() {
  // How about more than one pause command?
  const auto data = mSnapshotData->at(mReadIndex);
  if (data == QJsonValue::Undefined || !data.isObject())
    return false;

  if (const auto object = data.toObject(); object.contains("pause")) {
    if (const auto value = object.value("pause").toInt(); value > 0) {
      qDebug() << "!d" << tr("Sleeping %1 milliseconds").arg(value);
      QThread::msleep(static_cast<unsigned long>(value));
    }
    mReadIndex++;
  }
  return true;
}

QByteArray SioRecorder::readCommandFrame() {
  // We look whether there is a pause tag
  if (!readPauseTag())
    return {};

  const auto data = mSnapshotData->at(mReadIndex);
  if (data == QJsonValue::Undefined || !data.isObject())
    return {};

  auto object = data.toObject();
  if (!object.contains("command"))
    return {};

  QByteArray result{4, 0};
  auto temp = object["device"].toInt(-1);
  if (temp == -1)
    return {};
  result[0] = static_cast<char>(temp);

  temp = object["command"].toInt(-1);
  if (temp == -1)
    return {};
  result[1] = static_cast<char>(temp);

  temp = object["aux1"].toInt(-1);
  if (temp == -1)
    return {};
  result[2] = static_cast<char>(temp);

  temp = object["aux2"].toInt(-1);
  if (temp == -1)
    return {};
  result[3] = static_cast<char>(temp);

  mReadIndex++;
  return result;
}

/// TODO Be more verbose about failures.
QByteArray SioRecorder::readDataFrame(const uint size, bool, const bool verbose) {
  // We look whether there is a pause tag
  if (!readPauseTag())
    return {};

  const auto data = mSnapshotData->at(mReadIndex);
  if (data == QJsonValue::Undefined || !data.isObject())
    return {};

  auto object = data.toObject();
  if (!object.contains("data"))
    return {};

  const auto framedata = object["data"].toString("");
  if (framedata.isEmpty() || static_cast<int>(size) != framedata.size())
    return {};

  if (verbose)
    qDebug() << "!d" << tr("Read data frame with size %1").arg(size);

  mReadIndex++;
  return framedata.toLatin1();
}

// The playback never writeDataFrame, we just want to feed RespeQt some data and watch the reaction.
bool SioRecorder::writeDataFrame(const QByteArray & /*data*/) {
  return true;
}

bool SioRecorder::writeCommandAck() {
  return true;
}

bool SioRecorder::writeCommandNak() {
  return true;
}

bool SioRecorder::writeDataAck() {
  return true;
}

bool SioRecorder::writeDataNak() {
  return true;
}

bool SioRecorder::writeComplete() {
  return true;
}

bool SioRecorder::writeError() {
  return true;
}

bool SioRecorder::writeRawFrame(const QByteArray & /*data*/) {
  return true;
}

void SioRecorder::setActiveSioDevices(const QByteArray & /*data*/) {}
