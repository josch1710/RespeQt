#include "printers/passthrough.h"
#include "printers/rawoutput.h"
#include "respeqtsettings.h"

namespace Printers {

  Passthrough::Passthrough(const SioWorkerPtr& sio)
      : BasePrinter(sio) {}

  Passthrough::~Passthrough() = default;

  /*void Passthrough::setupOutput()
    {
        BasePrinter::setupOutput();
    }*/

  void Passthrough::setupFont() {}

  bool Passthrough::handleBuffer(const QByteArray &buffer, const unsigned int len) {
    QSharedPointer<RawOutput> output;
    try {
      /*output = qSharedPointerDynamicCast<RawOutput>(mOutput);
            if (output == nullptr)
                return false;*/
    } catch (...) {
      return false;
    }

    const auto lenmin = std::min(static_cast<unsigned int>(buffer.count()), len);
    auto tempbuffer = buffer;
    for (unsigned int i = 0; i < lenmin; i++) {
      if (const auto b = static_cast<unsigned char>(buffer.at(static_cast<int>(i))); b == 155)// EOL
      {
        constexpr char lf = 13;
        tempbuffer.replace(static_cast<int>(i), 1, &lf);
        tempbuffer.resize(static_cast<int>(i + 1));
        break;// Drop the rest of the buffer
      }
    }

    //return output->sendBuffer(tempbuffer, static_cast<unsigned int>(tempbuffer.size()));
    return false;
  }
}// namespace Printers
