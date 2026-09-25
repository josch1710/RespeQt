#include "siorecordertest.h"
#include "pclinktest.h"

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

    return status;
}
