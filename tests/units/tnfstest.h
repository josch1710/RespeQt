#ifndef TNFSTEST_H
#define TNFSTEST_H

#include <QObject>
#include "network/tnfs.h"

namespace Tests
{

    class TnfsTest: public QObject
    {
        Q_OBJECT

    public:
        TnfsTest() = default;
        virtual ~TnfsTest() = default;

    private slots:
        // Setup methods
        void initTestCase();
        void cleanupTestCase();

        // Test methods
        void openSession();

    private:
        Network::Tnfs tnfs{};
        QUdpSocket socket{Andy};
    };

}  // namespace Tests

#endif  //TNFSTEST_H
