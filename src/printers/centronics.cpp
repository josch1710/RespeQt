#include "printers/centronics.h"

namespace Printers {
  Centronics::Centronics(const SIO::SioWorkerPtr& sio)
      : BasePrinter(sio) {}

  QChar Centronics::translateAtascii(const unsigned char b) const {
    if (static_cast<unsigned char>(b) == 155)// Translate EOL to CR
    {
      return {13};
    }
    return BasePrinter::translateAtascii(b);
  }
}// namespace Printers
