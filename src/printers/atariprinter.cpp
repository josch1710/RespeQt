#include "printers/atariprinter.h"

namespace Printers {
  AtariPrinter::AtariPrinter(const SIO::SioWorkerPtr &worker)
      : BasePrinter(worker),
        mInternational(false) {}

  QChar AtariPrinter::translateAtascii(const unsigned char b) const {
    if (internationalMode()) {
      return mAtasciiInternational(b);
    }
    return BasePrinter::translateAtascii(b);
  }
}// namespace Printers
