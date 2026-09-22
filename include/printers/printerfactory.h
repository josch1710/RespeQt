#ifndef PRINTERFACTORY_H
#define PRINTERFACTORY_H

#include "baseprinter.h"
#include "sio/sioworker.h"
#include <QString>
#include <QVector>
#include <memory>

namespace Printers {

  class PrinterFactory {
  public:
    static std::shared_ptr<PrinterFactory> instance() {
      return sInstance;
    }

    template<class TDerived>
    void registerPrinter(const QString label) {
      static_assert(std::is_base_of_v<BasePrinter, TDerived>, "PrinterFactory::registerPrinter doesn't accept this type because it doesn't derive from base class");
      creatorFunctions.push_back(CreatorPair(label, &creator<TDerived>));
    }

    [[nodiscard]] BasePrinterPtr createPrinter(const QString& label, const SIO::SioWorkerPtr& worker) const {
      for (const auto & [printerName, printerFactory]: creatorFunctions) {
        if (printerName == label) {
          // This is a call to the function pointed to by it.second.
          return printerFactory(worker);
        }
      }
      throw std::invalid_argument("Unknown printer label given.");
    }

    [[nodiscard]] QVector<QString> getPrinterNames() const {
      QVector<QString> names;
      for (const auto & [printerName, _]: creatorFunctions) {
        names.append(printerName);
      }
      return names;
    }

  private:
    template<class TDerived>
    static BasePrinterPtr creator(const SIO::SioWorkerPtr &worker) {
      return QSharedPointer<TDerived>::create(worker);
    }

    // Instanciation maps
    using Creator = BasePrinterPtr(const SIO::SioWorkerPtr &worker);
    using CreatorPair = std::pair<QString, Creator *>;
    using CreatorVector = std::vector<CreatorPair>;
    CreatorVector creatorFunctions;

    static std::shared_ptr<PrinterFactory> sInstance;
    PrinterFactory() {}

  };
}// namespace Printers
#endif// PRINTERFACTORY_H
