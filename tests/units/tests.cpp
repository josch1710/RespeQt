#include "siorecordertest.h"
#include "pclinktest.h"
#include "cpu6502test.h"

#include <QTest>

int main(int argc, char** argv)
{
    int status = 0;
    {
        Tests::SioRecorderTest sioRecorderTest;
        status |= QTest::qExec(&sioRecorderTest, argc, argv);
    }
    {
        Tests::PclinkTest pclinkTest;
        status |= QTest::qExec(&pclinkTest, argc, argv);
    }
    {
        Tests::Cpu6502Test cpu6502Test;
        status |= QTest::qExec(&cpu6502Test, argc, argv);
    }

    return status;
}
