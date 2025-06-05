#ifndef PRINTERFACTORY_H
#define PRINTERFACTORY_H

#include "baseprinter.h"
#include "sioworker.h"
#include <QPair>
#include <QString>
#include <QVector>
#include <memory>

namespace Printers {

  class PrinterFactory {
  private:
    template<class TDerived>
    static BasePrinterPtr creator(SioWorkerPtr worker) {
      return QSharedPointer<TDerived>::create(worker);
    }

    // Instanciation maps
    using Creator = BasePrinterPtr(SioWorkerPtr worker);
    using CreatorPair = std::pair<QString, Creator *>;
    using CreatorVector = std::vector<CreatorPair>;
    CreatorVector creatorFunctions;

    static std::shared_ptr<PrinterFactory> sInstance;
    PrinterFactory() {}

  public:
    static std::shared_ptr<PrinterFactory> instance() {
      return sInstance;
    }

    template<class TDerived>
    void registerPrinter(QString label) {
      static_assert(std::is_base_of<BasePrinter, TDerived>::value, "PrinterFactory::registerPrinter doesn't accept this type because it doesn't derive from base class");
      creatorFunctions.push_back(CreatorPair(label, &creator<TDerived>));
    }

    BasePrinterPtr createPrinter(QString label, SioWorkerPtr worker) const {
      for (const auto &it: creatorFunctions) {
        if (it.first == label) {
          // This is a call to the function pointed to by it.second.
          return it.second(worker);
        }
      }
      throw new std::invalid_argument("Unknown printer label given.");
    }

    const QVector<QString> getPrinterNames() const {
      QVector<QString> names;
      for (const auto &it: creatorFunctions) {
        names.append(it.first);
      }
      return names;
    }
  };
}// namespace Printers
#endif// PRINTERFACTORY_H
