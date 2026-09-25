#include "pclinktest.h"
#include "dummyworker.h"
#include "mockserialport.h"
#include "respeqtsettings.h"
#include "sio/pclink.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSharedPointer>
#include <QTest>

namespace Tests {

#pragma pack(push, 1)
struct TestParBuf {
  quint8 fno{0};
  quint8 handle{0};
  quint8 f1{0}, f2{0}, f3{0}, f4{0};
  quint8 f5{0}, f6{0};
  quint8 fmode{0};
  quint8 fatr1{0};
  quint8 fatr2{0};
  char name[12]{0};
  char names[12]{0};
  char path[65]{0};
};

struct TestPclDbf {
  quint8 handle;
  quint8 dirbuf[23];
};
#pragma pack(pop)

static void set83Name(char dst[12], const char *src) {
  memset(dst, ' ', 11);
  dst[11] = 0;
  const char *dot = strchr(src, '.');
  int namelen = dot ? static_cast<int>(dot - src) : static_cast<int>(strlen(src));
  if (namelen > 8) namelen = 8;
  for (int i = 0; i < namelen; ++i) dst[i] = static_cast<char>(toupper(src[i]));
  if (dot) {
    const char *ext = dot + 1;
    int extlen = static_cast<int>(strlen(ext));
    if (extlen > 3) extlen = 3;
    for (int i = 0; i < extlen; ++i) dst[8 + i] = static_cast<char>(toupper(ext[i]));
  }
}

static QByteArray createParBufData(const TestParBuf &pbuf) {
  return QByteArray(reinterpret_cast<const char *>(&pbuf), sizeof(TestParBuf));
}

void PclinkTest::initTestCase() {
  QVERIFY(mTempDir.isValid());
}

void PclinkTest::cleanupTestCase() {}

void PclinkTest::init() {
  // Ensure temp dir is clean before each test
  QDir dir(mTempDir.path());
  const auto entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries | QDir::Hidden);
  for (const auto &entry : entries) {
    if (entry.isDir()) {
      QDir(entry.absoluteFilePath()).removeRecursively();
    } else {
      QFile::remove(entry.absoluteFilePath());
    }
  }
}

void PclinkTest::cleanup() {}

void PclinkTest::testLinkManagement() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();

  // Initially link 1 is not mounted
  QVERIFY(!pclink.hasLink(1));
  QVERIFY(!pclink.hasLink(2));

  // Mount link 1
  pclink.setLink(1, canonicalPath.toUtf8().constData());
  QVERIFY(pclink.hasLink(1));
  QVERIFY(!pclink.hasLink(2));

  // Swap link 1 and 2
  pclink.swapLinks(1, 2);
  QVERIFY(!pclink.hasLink(1));
  QVERIFY(pclink.hasLink(2));

  // Reset link 2
  pclink.resetLink(2);
  QVERIFY(!pclink.hasLink(2));
}

void PclinkTest::testSpeedPoll() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  mockPort->setSpeedByte(0x28);
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();
  pclink.setLink(1, canonicalPath.toUtf8().constData());

  mockPort->clearLog();
  pclink.handleCommand('?', 0, 1);

  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::CmdAck));
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QCOMPARE(mockPort->sentDataFrames().size(), 1);
  QCOMPARE(static_cast<quint8>(mockPort->sentDataFrames().first().at(0)), static_cast<quint8>(0x28));
}

void PclinkTest::testInit() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();
  pclink.setLink(1, canonicalPath.toUtf8().constData());

  TestParBuf pbuf;
  pbuf.fno = static_cast<quint8>(SIO::PclinkFunction::Init);
  mockPort->queueDataFrame(createParBufData(pbuf));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);

  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::CmdAck));
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::DataAck));
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
}

void PclinkTest::testDFree() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();
  pclink.setLink(1, canonicalPath.toUtf8().constData());

  TestParBuf pbuf;
  pbuf.fno = static_cast<quint8>(SIO::PclinkFunction::DFree);
  mockPort->queueDataFrame(createParBufData(pbuf));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::CmdAck));
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // 'R' command fetches DFREE data block (64 bytes)
  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::CmdAck));
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QCOMPARE(mockPort->sentDataFrames().size(), 1);
  QCOMPARE(mockPort->sentDataFrames().first().size(), 64);
}

void PclinkTest::testGetCwdAndChdir() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();
  pclink.setLink(1, canonicalPath.toUtf8().constData());

  // Create a subfolder
  QDir(canonicalPath).mkdir("SUBDIR");

  // Get current CWD
  TestParBuf pbufCwd;
  pbufCwd.fno = static_cast<quint8>(SIO::PclinkFunction::GetCwd);
  mockPort->queueDataFrame(createParBufData(pbufCwd));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QVERIFY(!mockPort->sentDataFrames().isEmpty());

  // Chdir into SUBDIR
  TestParBuf pbufCd;
  pbufCd.fno = static_cast<quint8>(SIO::PclinkFunction::Chdir);
  qstrncpy(pbufCd.path, "SUBDIR", sizeof(pbufCd.path));
  mockPort->queueDataFrame(createParBufData(pbufCd));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // Chdir back up with '<'
  TestParBuf pbufUp;
  pbufUp.fno = static_cast<quint8>(SIO::PclinkFunction::Chdir);
  qstrncpy(pbufUp.path, "<", sizeof(pbufUp.path));
  mockPort->queueDataFrame(createParBufData(pbufUp));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
}

void PclinkTest::testMkdirAndRmdir() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();
  pclink.setLink(1, canonicalPath.toUtf8().constData());

  // Create directory NEWDIR
  TestParBuf pbufMkdir;
  pbufMkdir.fno = static_cast<quint8>(SIO::PclinkFunction::Mkdir);
  set83Name(pbufMkdir.name, "NEWDIR");
  mockPort->queueDataFrame(createParBufData(pbufMkdir));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QVERIFY(QDir(canonicalPath + "/newdir").exists() || QDir(canonicalPath + "/NEWDIR").exists());

  // Remove directory NEWDIR
  TestParBuf pbufRmdir;
  pbufRmdir.fno = static_cast<quint8>(SIO::PclinkFunction::Rmdir);
  set83Name(pbufRmdir.name, "NEWDIR");
  mockPort->queueDataFrame(createParBufData(pbufRmdir));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QVERIFY(!QDir(canonicalPath + "/newdir").exists() && !QDir(canonicalPath + "/NEWDIR").exists());
}

void PclinkTest::testFileCreateWriteRead() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();
  pclink.setLink(1, canonicalPath.toUtf8().constData());

  // 1. Open file for writing (fmode = 0x08)
  TestParBuf pbufOpenWrite;
  pbufOpenWrite.fno = static_cast<quint8>(SIO::PclinkFunction::FOpen);
  pbufOpenWrite.fmode = 0x08; // Write mode
  set83Name(pbufOpenWrite.name, "TESTFILE.TXT");
  mockPort->queueDataFrame(createParBufData(pbufOpenWrite));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // Receive open response frame via 'R'
  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QCOMPARE(mockPort->sentDataFrames().size(), 1);

  TestPclDbf openResp{};
  memcpy(&openResp, mockPort->sentDataFrames().first().constData(), sizeof(TestPclDbf));
  const quint8 handle = openResp.handle;
  QVERIFY(handle <= 15);

  // 2. Write data to file
  const QByteArray testPayload = "Hello SpartaDOS PCLink!";
  TestParBuf pbufWrite;
  pbufWrite.fno = static_cast<quint8>(SIO::PclinkFunction::FWrite);
  pbufWrite.handle = handle;
  pbufWrite.f1 = static_cast<quint8>(testPayload.size() & 0xFF);
  pbufWrite.f2 = static_cast<quint8>((testPayload.size() >> 8) & 0xFF);
  pbufWrite.f3 = 0;
  mockPort->queueDataFrame(createParBufData(pbufWrite));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // Send payload via 'R'
  mockPort->queueDataFrame(testPayload);
  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // 3. Close write handle
  TestParBuf pbufClose;
  pbufClose.fno = static_cast<quint8>(SIO::PclinkFunction::FClose);
  pbufClose.handle = handle;
  mockPort->queueDataFrame(createParBufData(pbufClose));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  const auto list = QDir(canonicalPath).entryList(QDir::NoDotAndDotDot | QDir::Files);
  QVERIFY(!list.isEmpty());

  QFile writtenFile(canonicalPath + "/" + list.first());
  QVERIFY(writtenFile.open(QIODevice::ReadOnly));
  QCOMPARE(writtenFile.readAll(), testPayload);
  writtenFile.close();

  // 4. Open file for reading (fmode = 0x04)
  TestParBuf pbufOpenRead;
  pbufOpenRead.fno = static_cast<quint8>(SIO::PclinkFunction::FOpen);
  pbufOpenRead.fmode = 0x04; // Read mode
  set83Name(pbufOpenRead.name, "TESTFILE.TXT");
  mockPort->queueDataFrame(createParBufData(pbufOpenRead));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  TestPclDbf readResp{};
  memcpy(&readResp, mockPort->sentDataFrames().first().constData(), sizeof(TestPclDbf));
  const quint8 readHandle = readResp.handle;

  // 5. Query file length (FLen)
  TestParBuf pbufLen;
  pbufLen.fno = static_cast<quint8>(SIO::PclinkFunction::FLen);
  pbufLen.handle = readHandle;
  mockPort->queueDataFrame(createParBufData(pbufLen));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // Query status 'S' to get file length in status
  mockPort->clearLog();
  pclink.handleCommand('S', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // 6. Read payload (FRead)
  TestParBuf pbufRead;
  pbufRead.fno = static_cast<quint8>(SIO::PclinkFunction::FRead);
  pbufRead.handle = readHandle;
  pbufRead.f1 = static_cast<quint8>(testPayload.size() & 0xFF);
  pbufRead.f2 = static_cast<quint8>((testPayload.size() >> 8) & 0xFF);
  pbufRead.f3 = 0;
  mockPort->queueDataFrame(createParBufData(pbufRead));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QCOMPARE(mockPort->sentDataFrames().size(), 1);
  QCOMPARE(mockPort->sentDataFrames().first(), testPayload);

  // 7. Close read handle
  TestParBuf pbufCloseRead;
  pbufCloseRead.fno = static_cast<quint8>(SIO::PclinkFunction::FClose);
  pbufCloseRead.handle = readHandle;
  mockPort->queueDataFrame(createParBufData(pbufCloseRead));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
}

void PclinkTest::testDirectoryListing() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();
  pclink.setLink(1, canonicalPath.toUtf8().constData());

  // Create two sample files
  QFile file1(canonicalPath + "/file1.txt");
  file1.open(QIODevice::WriteOnly);
  file1.write("DATA1");
  file1.close();

  QFile file2(canonicalPath + "/file2.txt");
  file2.open(QIODevice::WriteOnly);
  file2.write("DATA2");
  file2.close();

  // FFirst (directory stream mode: 0x10)
  TestParBuf pbufFirst;
  pbufFirst.fno = static_cast<quint8>(SIO::PclinkFunction::FFirst);
  pbufFirst.fmode = 0x10; // Directory mode
  set83Name(pbufFirst.name, "*.*");
  mockPort->queueDataFrame(createParBufData(pbufFirst));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QVERIFY(!mockPort->sentDataFrames().isEmpty());

  TestPclDbf firstResp{};
  memcpy(&firstResp, mockPort->sentDataFrames().first().constData(), sizeof(TestPclDbf));
  const quint8 dirHandle = firstResp.handle;

  // FNext to get the second file entry
  TestParBuf pbufNext;
  pbufNext.fno = static_cast<quint8>(SIO::PclinkFunction::FNext);
  pbufNext.handle = dirHandle;
  mockPort->queueDataFrame(createParBufData(pbufNext));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // Close directory handle
  TestParBuf pbufCloseDir;
  pbufCloseDir.fno = static_cast<quint8>(SIO::PclinkFunction::FClose);
  pbufCloseDir.handle = dirHandle;
  mockPort->queueDataFrame(createParBufData(pbufCloseDir));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
}

void PclinkTest::testRenameAndRemove() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();
  pclink.setLink(1, canonicalPath.toUtf8().constData());

  // Create initial file
  QFile file(canonicalPath + "/oldname.txt");
  file.open(QIODevice::WriteOnly);
  file.write("Content");
  file.close();

  // Rename OLDNAME.TXT -> NEWNAME.TXT
  TestParBuf pbufRename;
  pbufRename.fno = static_cast<quint8>(SIO::PclinkFunction::Rename);
  set83Name(pbufRename.name, "OLDNAME.TXT");
  set83Name(pbufRename.names, "NEWNAME.TXT");
  mockPort->queueDataFrame(createParBufData(pbufRename));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QVERIFY(!QFile::exists(canonicalPath + "/oldname.txt") && !QFile::exists(canonicalPath + "/OLDNAME.TXT"));
  QVERIFY(QFile::exists(canonicalPath + "/newname.txt") || QFile::exists(canonicalPath + "/NEWNAME.TXT"));

  // Remove NEWNAME.TXT
  TestParBuf pbufRemove;
  pbufRemove.fno = static_cast<quint8>(SIO::PclinkFunction::Remove);
  set83Name(pbufRemove.name, "NEWNAME.TXT");
  mockPort->queueDataFrame(createParBufData(pbufRemove));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QVERIFY(!QFile::exists(canonicalPath + "/newname.txt") && !QFile::exists(canonicalPath + "/NEWNAME.TXT"));
}

void PclinkTest::testErrorHandling() {
  auto mockPort = QSharedPointer<MockSerialPortBackend>::create();
  auto worker = QSharedPointer<DummyWorker>::create(mockPort);
  SIO::PCLINK pclink(worker);

  const QString canonicalPath = QDir(mTempDir.path()).canonicalPath();
  pclink.setLink(1, canonicalPath.toUtf8().constData());

  // 1. Try to open non-existent file for reading
  TestParBuf pbufNonExistent;
  pbufNonExistent.fno = static_cast<quint8>(SIO::PclinkFunction::FOpen);
  pbufNonExistent.fmode = 0x04; // Read mode
  set83Name(pbufNonExistent.name, "NONEXIST.TXT");
  mockPort->queueDataFrame(createParBufData(pbufNonExistent));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // FOpen executes on 'R' frame
  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  // Query status to verify error code
  mockPort->clearLog();
  pclink.handleCommand('S', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));
  QCOMPARE(mockPort->sentDataFrames().size(), 1);
  // Status frame: byte 1 is err
  const quint8 errCode = static_cast<quint8>(mockPort->sentDataFrames().first().at(1));
  QCOMPARE(errCode, static_cast<quint8>(SIO::PclinkError::FileNotFound));

  // 2. Access invalid handle
  TestParBuf pbufInvalidHandle;
  pbufInvalidHandle.fno = static_cast<quint8>(SIO::PclinkFunction::FRead);
  pbufInvalidHandle.handle = 14; // Unopened handle
  pbufInvalidHandle.f1 = 10;
  mockPort->queueDataFrame(createParBufData(pbufInvalidHandle));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  mockPort->clearLog();
  pclink.handleCommand('S', 0, 1);
  const quint8 errBadHandle = static_cast<quint8>(mockPort->sentDataFrames().first().at(1));
  QCOMPARE(errBadHandle, static_cast<quint8>(SIO::PclinkError::BadHandle));

  // 3. Invalid path traversal (attempting to go above mounted root)
  TestParBuf pbufPathTraversal;
  pbufPathTraversal.fno = static_cast<quint8>(SIO::PclinkFunction::FOpen);
  pbufPathTraversal.fmode = 0x04;
  set83Name(pbufPathTraversal.name, "TEST.TXT");
  qstrncpy(pbufPathTraversal.path, "<<<", sizeof(pbufPathTraversal.path));
  mockPort->queueDataFrame(createParBufData(pbufPathTraversal));

  mockPort->clearLog();
  pclink.handleCommand('P', sizeof(TestParBuf), 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  mockPort->clearLog();
  pclink.handleCommand('R', 0, 1);
  QVERIFY(mockPort->hasOp(MockSerialPortBackend::Operation::Complete));

  mockPort->clearLog();
  pclink.handleCommand('S', 0, 1);
  const quint8 errInvalidPath = static_cast<quint8>(mockPort->sentDataFrames().first().at(1));
  QCOMPARE(errInvalidPath, static_cast<quint8>(SIO::PclinkError::InvalidPath));
}

} // namespace Tests
