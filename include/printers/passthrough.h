#ifndef PASSTHROUGH_H
#define PASSTHROUGH_H

#include "baseprinter.h"

namespace Printers {
  class Passthrough : public BasePrinter {
  public:
    explicit Passthrough(const SioWorkerPtr& sio);
    ~Passthrough() override;

    bool handleBuffer(const QByteArray &buffer, unsigned int len) override;
    void setupFont() override;
    //void setupOutput() override;

    static QString typeName() {
      return tr("Passthrough");
    }
  };
}// namespace Printers
#endif// PASSTHROUGH_H
