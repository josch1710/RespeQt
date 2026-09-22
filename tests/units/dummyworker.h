#ifndef DUMMYWORKER_H
#define DUMMYWORKER_H
#include "sio/serialport.h"
#include "sio/sioworker.h"

namespace Tests
{

    class DummyWorker final: public SIO::SioWorker
    {
    public:
        DummyWorker(SIO::AbstractSerialPortBackend* port): SIO::SioWorker(port) {};

        void uninstallDevice(quint8) override {};
    };

}
#endif // DUMMYWORKER_H
