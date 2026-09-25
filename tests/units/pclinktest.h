#ifndef PCLINKTEST_H
#define PCLINKTEST_H

#include <QObject>
#include <QTemporaryDir>

namespace Tests {

class PclinkTest : public QObject {
  Q_OBJECT

public:
  PclinkTest() = default;
  ~PclinkTest() override = default;

private slots:
  void initTestCase();
  void cleanupTestCase();
  void init();
  void cleanup();

  // Test methods
  void testLinkManagement();
  void testSpeedPoll();
  void testInit();
  void testDFree();
  void testGetCwdAndChdir();
  void testMkdirAndRmdir();
  void testFileCreateWriteRead();
  void testDirectoryListing();
  void testRenameAndRemove();
  void testErrorHandling();

private:
  QTemporaryDir mTempDir;
};

} // namespace Tests

#endif // PCLINKTEST_H
