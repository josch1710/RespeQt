#ifndef CENTRONICS_H
#define CENTRONICS_H

#include "baseprinter.h"

namespace Printers {
  class Centronics : public BasePrinter {
  public:
    Centronics(SioWorkerPtr sio);

    virtual const QChar translateAtascii(const unsigned char b) const override;
  };
}// namespace Printers
#endif// CENTRONICS_H
