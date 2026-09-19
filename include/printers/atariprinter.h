#ifndef ATARIPRINTER_H
#define ATARIPRINTER_H

#include "atasciiinternational.h"
#include "baseprinter.h"
#include "sioworker.h"

namespace Printers {
  class AtariPrinter : public BasePrinter {
    Q_OBJECT
  public:
    explicit AtariPrinter(const SioWorkerPtr &worker);

    bool internationalMode() const { return mInternational; }
    void setInternationalMode(const bool internationalMode) { mInternational = internationalMode; }

    QChar translateAtascii(unsigned char b) const override;

  protected:
    bool mInternational;
    AtasciiInternational mAtasciiInternational;
  };
}// namespace Printers
#endif// ATARIPRINTER_H
