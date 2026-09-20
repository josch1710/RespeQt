#include "printers/escp.h"

namespace Printers {
  Escp::Escp(const SioWorkerPtr& sio)
      : Centronics(sio) {
    initPrinter();
  }

  void Escp::initPrinter() {
    mEsc = false;
  }

  bool Escp::handleBuffer(const QByteArray & /*buffer*/, const unsigned int len) {
    for (unsigned int i = 0; i < len; i++) {
      //unsigned char b = buffer.at(i);
    }
    return true;
  }

  // ReSharper disable once CppMemberFunctionMayBeStatic
  void Escp::handlePrintableCodes(unsigned char /*b*/) { // NOLINT(*-convert-member-functions-to-static)
  }

  // ReSharper disable once CppMemberFunctionMayBeStatic
  void Escp::handleEscapableCodes(unsigned char /*b*/) { // NOLINT(*-convert-member-functions-to-static)
  }

}// namespace Printers
