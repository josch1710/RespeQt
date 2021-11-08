#ifndef ATARIPRINTER_H
#define ATARIPRINTER_H

#include "baseprinter.h"
#include "atasciiinternational.h"
#include "sioworker.h"

#include <QState>
#include <QStateMachine>

namespace Printers
{
    class AtariPrinter : public BasePrinter
    {
        Q_OBJECT
    public:
        AtariPrinter(SioWorkerPtr worker);
        virtual ~AtariPrinter();

        bool internationalMode() const;
        void setInternationalMode(bool internationalMode);

        virtual const QChar translateAtascii(const unsigned char b) const override;
    protected:
        QState *mInternationalState;
        QState *mEscState, *mCombinedState;
        QStateMachine *mPrinterMachine;
        AtasciiInternational mAtasciiInternational;
        QByteArray mPrintText;
    };
}
#endif // ATARIPRINTER_H
