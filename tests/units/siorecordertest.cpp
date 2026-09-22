#include "siorecordertest.h"
#include "sio/sioworker.h"
#include "sio/siorecorder.h"
#include "respeqtsettings.h"
#include <QTemporaryFile>
#include <QTest>
#include <QSharedPointer>
#include <iostream>

using std::cout;
using std::endl;

namespace Tests {
    // ReSharper disable once CppMemberFunctionMayBeStatic
    void SioRecorderTest::initTestCase()
    {}

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void SioRecorderTest::cleanupTestCase()
    {}

    // ReSharper disable once CppMemberFunctionMayBeStatic
    void SioRecorderTest::writeSioCapture() // NOLINT(*-convert-member-functions-to-static)
    {
        // Some test data
        // Atari OS fills left pads the printer buffer with spaces, so pre-set the spaces.
        QByteArray data{40, ' '};

        // We open a test file
        QTemporaryFile file("testWriteSioCapture");
        file.open();
        file.setAutoRemove(true);
        const auto recorder = SIO::SioRecorder::instance();
        recorder->startSIOSnapshot();

        // Pause
        recorder->writePauseCommand(500);
        // Get Status command
        recorder->writeSnapshotCommandFrame(SIO::PRINTER_BASE_CDEVIC, 0x53, 0, 0);
        // Pause
        recorder->writePauseCommand(500);
        // Write data command
        recorder->writeSnapshotCommandFrame(SIO::PRINTER_BASE_CDEVIC, 0x57, 0x4e, 0);
        // Pause
        recorder->writePauseCommand(500);
        // Some random test data.
        for(unsigned int i = 0; i < 10; i++)
            data[i] = static_cast<char>(i * 3);
        recorder->writeSnapshotDataFrame(data);
        // Pause
        recorder->writePauseCommand(500);

        // We close the test file
        const auto snapshot = recorder->stopSIOSnapshot();
        file.write(snapshot);
        file.close();

        // Now fetch the generated file and the base line file and compare them
        file.open();
        const auto data_test = file.readAll();
        file.close();

        QFile baseline("tests/units/testdata/writeSioCapture.json");
        baseline.open(QIODevice::ReadOnly);
        const auto data_baseline = baseline.readAll();
        QVERIFY2(data_baseline.size() > 0, tr("Baseline file couldn't be read.").toLatin1());
        baseline.close();

        QCOMPARE(data_test, data_baseline);
    }

    // void SioRecorderTest::readSioCapture()
    // {
    //     auto file {new QFile("tests/units/testdata/writeSioCapture.json")};
    //     file->open(QIODevice::ReadOnly);
    //
    //     auto recorder {SIO::SioRecorder::instance()};
    //     recorder->prepareReplaySnapshot(file, SIO::SerialBackend::NONE);
    //
    //     // The test only involves a printer device, so we instanciate one.
    //     SIO::SioWorkerPtr dummyworker{QSharedPointer<DummyWorker>::create(recorder.data())};
    //     Printers::Atari1027 printer(dummyworker);
    //     printer.setDeviceNo(SIO::PRINTER_BASE_CDEVIC);
    //     Printers::NativeOutputPtr nulloutput(new NullOutput);
    //     RespeqtSettings::instance()->setPrinterEmulation(true);
    //
    //     /* Open serial port */
    //     QVERIFY2(recorder->open(), tr("Recorder could not be opened.").toLatin1());
    //
    //     // We simulate what SioWorker::run does just for the printer.
    //     /* Process two SIO commands */
    //     QByteArray cmd = recorder->readCommandFrame();
    //     QVERIFY2(!cmd.isEmpty(), tr("The first command frame should not be empty.").toLatin1());
    //
    //     /* Decode the command */
    //     auto command {static_cast<quint8>(cmd[1])};
    //     auto aux {static_cast<quint16>(static_cast<quint8>(cmd[2]) + static_cast<quint8>(cmd[3]) * 256)};
    //
    //     /* Redirect the command to the printer */
    //     printer.handleCommand(command, aux, 0);
    //     cmd.clear();
    //
    //     cmd = recorder->readCommandFrame();
    //     QVERIFY2(!cmd.isEmpty(), tr("The second command frame should not be empty.").toLatin1());
    //
    //     /* Decode the command */
    //     command = static_cast<quint8>(cmd[1]);
    //     aux = static_cast<quint16>(static_cast<quint8>(cmd[2]) + static_cast<quint8>(cmd[3]) * 256);
    //
    //     /* Redirect the command to the printer */
    //     printer.handleCommand(command, aux, 0);
    //     cmd.clear();
    //
    //     cmd = recorder->readCommandFrame();
    //     QVERIFY2(cmd.isEmpty(), tr("The third command frame must be empty.").toLatin1());
    //
    //     recorder->close();
    // }
}
