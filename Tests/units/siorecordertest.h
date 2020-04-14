#ifndef SIORECORDERTEST_H
#define SIORECORDERTEST_H

#include <QObject>
#include <QFile>


namespace Tests
{
    class SioRecorderTest: public QObject
    {
        Q_OBJECT
    public:
        SioRecorderTest() = default;
        virtual ~SioRecorderTest() = default;


    private slots:
        // Setup methods
        void initTestCase();
        void cleanupTestCase();

        // Test methods
        void writeSioCapture();
        void readSioCapture();
    };
}
#endif // SIORECORDERTEST_H
