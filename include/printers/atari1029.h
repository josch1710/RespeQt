#ifndef ATARI1029_H
#define ATARI1029_H

#include "atariprinter.h"

#include <QPrinter>
#include <QRect>

namespace Printers {
  enum class GraphicsMode {
    NOT_GRAPHICS,
    FETCH_LSB,
    FETCH_MSB,
    PLOT_DOTS
  };

  class Atari1029 : public AtariPrinter {
    Q_OBJECT
  public:
    Atari1029(SioWorkerPtr worker);

    bool handleBuffer(const QByteArray &buffer, const unsigned int len) override;
    void setupFont() override;

    static QString typeName() {
      return "Atari 1029";
    }

  protected:
    QRectF printerDimension() const override;

  private:
    bool handleEscapedCodes(const unsigned char b);
    bool handlePrintableCodes(const unsigned char b);
    bool elongatedMode() const { return mElongatedMode; }
    void setElongatedMode(bool elongatedMode);
    bool handleGraphicsMode(const unsigned char b);

    GraphicsMode mGraphicsMode{GraphicsMode::NOT_GRAPHICS};
  };
}// namespace Printers
#endif// ATARI1029_H
