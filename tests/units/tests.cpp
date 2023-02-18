#include <QCoreApplication>

#include "siorecordertest.h"

#include <QTest>
#include <unistd.h>

int main(int, char**)
{
    Tests::SioRecorderTest sioRecorderTest;
    QTest::qExec(&sioRecorderTest);

    return 0;
}
