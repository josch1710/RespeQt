#include <QCoreApplication>

#include "siorecordertest.h"
#include "tnfstest.h"

#include <QTest>
#include <unistd.h>

int main(int, char**)
{
    Tests::SioRecorderTest sioRecorderTest;
    QTest::qExec(&sioRecorderTest);

    Tests::TnfsTest tnfsTest;
    QTest::qExec(&tnfsTest);

    return 0;
}
