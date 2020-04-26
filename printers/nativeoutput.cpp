#include <cmath>
#include <utility>
#include "nativeoutput.h"
#include "logdisplaydialog.h"

template <typename T>
class TD;

namespace Printers
{
    NativeOutput::NativeOutput()
    {
        mCharMode = true;
        calculateFixedFontSize(mCharsPerLine);
    }

    NativeOutput::~NativeOutput()
    {
        endOutput();
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
        if (mPainter)
        {
            mPainter->end();
        }
        return true;
    }

    void NativeOutput::calculateFixedFontSize(uint8_t charsPerLine)
    {
        if (!mFont)
        {
            return;
        }
        qreal painterWidth = mBoundingBox.right() - mBoundingBox.left();
        qreal oldFontSize = mFont->pointSizeF();
        int oldWidth;

        // Loop to approximate correct font size
        for (int i=0 ; i<3 ; i++)
        {
            QFontMetrics metrics{*mFont};
            auto bounds = metrics.boundingRect('M');
            oldWidth = bounds.width();
            qreal scale = painterWidth / (oldWidth * charsPerLine);
            mFont->setPointSizeF(bounds.height() * scale);
            applyFont();
            oldFontSize = bounds.height() * scale;
        }

        // End
        mFont->setPointSizeF(oldFontSize);
        applyFont();
        mCharsPerLine = charsPerLine;
    }

    void NativeOutput::setFont(QFontPtr font)
    {
        if (font != mFont)
        {
            mFont = font;
        }
        applyFont();
    }

    void NativeOutput::printChar(const QChar &c)
    {
        QFontMetrics metrics(*mFont);
        if (metrics.width(c) + mX > mBoundingBox.right()
            || mCharCount + 1 > mCharsPerLine) {
            // Char has to go on next line
            newLine();
        }
        if (mPainter)
        {
            QColor color(255, 0, 0);
            mPainter->setPen(color);
            mPainter->drawText(mX, mY + metrics.height(), c);
        }
        mX += metrics.width(c);
        mCharCount++;
    }

    void NativeOutput::printString(const QString &s)
    {
        for(auto cit: s)
        {
            printChar(cit);
        }
    }

    void NativeOutput::newLine(bool linefeed)
    {
        QFontMetrics metrics(*mFont);

        int lineSpacing = metrics.lineSpacing();
        if (mLPIMode > 0)
        {
            lineSpacing = metrics.height();
        }
        if (!linefeed)
        {
            mX = static_cast<int>(trunc(mBoundingBox.left()));
            mCharCount = 0;
        }
        if (mY + metrics.height() > mBoundingBox.bottom())
        {
            newPage(linefeed);
            mY = static_cast<int>(trunc(mBoundingBox.top()));
        } else {
            mY += lineSpacing;
        }
    }

    void NativeOutput::plot(QPoint p, uint8_t dot)
    {
        if (!mPainter)
            return;

        if (dot > 0)
            mPainter->setPen(QColor("black"));
        else
            mPainter->setPen(QColor("white"));
        mPainter->drawPoint(p);
    }

    void NativeOutput::applyFont()
    {
        if (mPainter && mFont)
            mPainter->setFont(*mFont);
    }
}
