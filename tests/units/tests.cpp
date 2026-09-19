#include "siorecordertest.h"

#include <QTest>

int main(int, char**)
{
    Tests::SioRecorderTest sioRecorderTest;
    QTest::qExec(&sioRecorderTest);

    return 0;
}
