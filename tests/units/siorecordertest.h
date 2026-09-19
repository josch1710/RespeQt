#ifndef SIORECORDERTEST_H
#define SIORECORDERTEST_H

#include <QFile>


namespace Tests
{
    class SioRecorderTest: public QObject
    {
        Q_OBJECT
    public:
        SioRecorderTest() = default;
        ~SioRecorderTest() override = default;


    private slots:
        // Setup methods
        void initTestCase();
        void cleanupTestCase();

        // Test methods
        void writeSioCapture();
        //void readSioCapture();
    };
}
#endif // SIORECORDERTEST_H
