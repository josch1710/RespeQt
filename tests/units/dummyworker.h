#ifndef DUMMYWORKER_H
#define DUMMYWORKER_H
#include "serialport.h"
#include "sioworker.h"

namespace Tests
{

    class DummyWorker final: public SioWorker
    {
    public:
        DummyWorker(AbstractSerialPortBackend* port): SioWorker(port) {};

        void uninstallDevice(quint8) override {};
    };

}
#endif // DUMMYWORKER_H
