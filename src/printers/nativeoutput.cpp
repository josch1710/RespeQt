#include "printers/nativeoutput.h"
#include "ui/logdisplaydialog.h"
#include <cmath>

template<typename T>
class TD;

namespace Printers {
  NativeOutput::NativeOutput() {
    mCharMode = true;
    NativeOutput::calculateFixedFontSize(mCharsPerLine);
  }

  NativeOutput::~NativeOutput() {
    NativeOutput::endOutput();
  }

  bool NativeOutput::beginOutput() {
    if (!mDevice)
      return false;

    mPainter = std::make_shared<QPainter>();
    mPainter->setRenderHint(QPainter::Antialiasing);
    mPainter->begin(mDevice.get());
    setFont(mFont);
    updateBoundingBox();
    /*if (mPrinter)
        {
            BasePrinterPtr temp = mPrinter.lock();
            if (temp)
            {
                temp->setupOutput();
                temp->setupFont();
            }
        }*/
    return true;
  }

  bool NativeOutput::endOutput() {
    if (mPainter) {
      mPainter->end();
    }
    return true;
  }

  void NativeOutput::calculateFixedFontSize(const uint8_t charsPerLine) {
    if (!mFont) {
      return;
    }
    const qreal painterWidth = mBoundingBox.right() - mBoundingBox.left();
    qreal oldFontSize = mFont->pointSizeF();

    // Loop to approximate correct font size
    for (int i = 0; i < 3; i++) {
      QFontMetrics metrics{*mFont};
      auto bounds = metrics.boundingRect('M');
      const int oldWidth = bounds.width();
      const qreal scale = painterWidth / (oldWidth * charsPerLine);
      mFont->setPointSizeF(bounds.height() * scale);
      applyFont();
      oldFontSize = bounds.height() * scale;
    }

    // End
    mFont->setPointSizeF(oldFontSize);
    applyFont();
    mCharsPerLine = charsPerLine;
  }

  void NativeOutput::setFont(const QFontPtr &font) {
    if (font != mFont) {
      mFont = font;
    }
    applyFont();
  }

  void NativeOutput::printChar(const QChar &c) {
    const QFontMetrics metrics(*mFont);
    if (metrics.boundingRect(c).width() + mX > mBoundingBox.right() || mCharCount + 1 > mCharsPerLine) {
      // Char has to go on next line
      newLine();
    }
    if (mPainter) {
      constexpr QColor color(255, 0, 0);
      mPainter->setPen(color);
      mPainter->drawText(mX, mY + metrics.height(), c);
    }
    mX += metrics.boundingRect(c).width();
    mCharCount++;
  }

  void NativeOutput::printString(const QString &s) {
    for (auto cit: s) {
      printChar(cit);
    }
  }

  void NativeOutput::newLine(const bool linefeed) {
    const QFontMetrics metrics(*mFont);

    int lineSpacing = metrics.lineSpacing();
    if (mLPIMode > 0) {
      lineSpacing = metrics.height();
    }
    if (!linefeed) {
      mX = static_cast<int>(trunc(mBoundingBox.left()));
      mCharCount = 0;
    }
    if (mY + metrics.height() > mBoundingBox.bottom()) {
      newPage(linefeed);
      mY = static_cast<int>(trunc(mBoundingBox.top()));
    } else {
      mY += lineSpacing;
    }
  }

  void NativeOutput::plot(const QPoint p, const uint8_t dot) {
    if (!mPainter)
      return;

    if (dot > 0)
      mPainter->setPen(QColor("black"));
    else
      mPainter->setPen(QColor("white"));
    mPainter->drawPoint(p);
  }

  void NativeOutput::applyFont() {
    if (mPainter && mFont)
      mPainter->setFont(*mFont);
  }
}// namespace Printers
