#include "printerfactory.h"

namespace Printers {
std::shared_ptr<PrinterFactory> PrinterFactory::sInstance = std::shared_ptr<PrinterFactory>(new PrinterFactory);
}
