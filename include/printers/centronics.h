#ifndef CENTRONICS_H
#define CENTRONICS_H

#include "baseprinter.h"

namespace Printers {
  class Centronics : public BasePrinter {
  public:
    explicit Centronics(const SIO::SioWorkerPtr &sio);

    QChar translateAtascii(unsigned char b) const override;
  };
}// namespace Printers
#endif// CENTRONICS_H
