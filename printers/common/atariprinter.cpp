#include "atariprinter.h"

#include <utility>

namespace Printers
{
    AtariPrinter::AtariPrinter(SioWorkerPtr worker)
        : BasePrinter(std::move(worker)),
        mPrintText("")
    {
        mEscState = new QState();
        mEscState->assignProperty(this, "esc", false);
        mCombinedState = new QState(QState::ParallelStates);
        mInternationalState = new QState(mCombinedState);
        mInternationalState->assignProperty(this, "international", false);
        mInternationalState = new QState(mCombinedState);
        mInternationalState->assignProperty(this, "international", false);

        mPrinterMachine = new QStateMachine();
        mPrinterMachine->addState(mCombinedState);
        mPrinterMachine->addState(mEscState);
        mPrinterMachine->setInitialState(mCombinedState);
    }

    AtariPrinter::~AtariPrinter()
    {
        delete mEscState;
        delete mInternationalState;
        delete mCombinedState;
        delete mPrinterMachine;
    }

    bool AtariPrinter::internationalMode() const
    {
        return mInternationalState->property("international").toBool();
    }

    void AtariPrinter::setInternationalMode(bool internationalMode)
    {
        mInternationalState->setProperty("international", internationalMode);
    }


    const QChar AtariPrinter::translateAtascii(const unsigned char b) const
    {
        if (internationalMode()) {
            return mAtasciiInternational(b);
        }
        return BasePrinter::translateAtascii(b);
    }
}
