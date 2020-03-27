#include "outputfactory.h"

namespace Printers {
    std::shared_ptr<OutputFactory> OutputFactory::sInstance = std::shared_ptr<OutputFactory>(new OutputFactory);
}
