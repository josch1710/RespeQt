#ifndef ATARI1027_H
#define ATARI1027_H

#include "atariprinter.h"

#include <QPrinter>
#include <QRect>

namespace Printers {
  class Atari1027 : public AtariPrinter {
  protected:
    QRectF printerDimension() const override;

  private:
    Q_OBJECT

  public:
    explicit Atari1027(const SioWorkerPtr &worker);

    bool handleBuffer(const QByteArray &buffer, unsigned int len) override;
    void setupFont() override;

    static QString typeName() {
      return "Atari 1027";
    }

  private:
    bool mESC;

    bool handleEscapedCodes(unsigned char b);
    bool handlePrintableCodes(unsigned char b);
  };
}// namespace Printers
#endif// ATARI1027_H
