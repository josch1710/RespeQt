#include "printers/atari1029.h"
#include "respeqtsettings.h"
#include <qglobal.h>
#include <utility>

namespace Printers
{
    Atari1029::Atari1029(SioWorkerPtr worker)
        : AtariPrinter(std::move(worker)),
          mESC(false),
          mElongatedMode(false) {}

    void Atari1029::setupFont()
    {
        mFont = QFont(RespeqtSettings::instance()->atariFixedFontFamily(), 12);
        mFont.setUnderline(false);
    }

    bool Atari1029::handleBuffer(const QByteArray &buffer, const unsigned int len)
    {
        for (unsigned int i = 0; i < len; i++)
        {
            if (mTotalColums > 480)
            {
                newLine();
            }
            auto b = static_cast<uint8_t>(buffer[i]);
            if (mGraphicsMode == GraphicsMode::NOT_GRAPHICS)
            {
                switch (b)
                {
                    case 13:  // CTRL+N could be ESC code
                    case 14:  // CTRL+O could be ESC code
                    case 23:  // CTRL+W could be ESC code
                    case 24:  // CTRL+X could be ESC code
                    case 25:  // CTRL+Y could be ESC code
                    case 26:  // CTRL+Z could be ESC code
                    case 54:  // 6 could be ESC code
                    case 57:  // 9 could be ESC code
                    case 65:  // A could be ESC code
                        if (mESC)
                        {
                            if (!handleEscapedCodes(b))
                            {
                                handlePrintableCodes(b);
                            }
                        }
                        else
                        {
                            handlePrintableCodes(b);
                        }
                        break;

                    case 155:  // EOL
                        newLine();
                        return true;

                    case 27:  // ESC could be starting something
                        if (mESC)
                        {  // ESC from last buffer
                            mESC = false;
                            handlePrintableCodes(b);
                        }
                        else
                        {  // No ESC codes from last buffer
                            mESC = true;
                            if (i + 1 < len)
                            {
                                i++;
                                b = static_cast<unsigned char>(buffer.at(static_cast<int>(i)));
                                if (!handleEscapedCodes(b))
                                {
                                    handlePrintableCodes(b);
                                }
                            }
                        }
                        break;

                    default:  // Everythings else
                        handlePrintableCodes(b);
                        break;
                }
            }
            else
                handleGraphicsMode(b);
        }
        return true;
    }

    bool Atari1029::handleEscapedCodes(const unsigned char b)
    {
        // At this time we have seen an ESC.
        switch (b)
        {
            case 25:  // CTRL+Y starts underline mode
            {
                mESC = false;
                qDebug() << "!d"
                         << "ESC Underline on";
                return true;
            }
            case 26:  // CTRL+Z ends underline mode
            {
                mESC = false;
                qDebug() << "!d"
                         << "ESC Underline off";
                return true;
            }
            case 23:  // CTRL+W starts international mode
                setInternationalMode(true);
                mESC = false;
                return true;

            case 24:  // CTRL+X ends international mode
                setInternationalMode(false);
                mESC = false;
                return true;

            case 13:  // CTRL+N starts elongated mode
                setElongatedMode(true);
                mESC = false;
                return true;

            case 14:  // CTRL+O ends elongated mode
                setElongatedMode(false);
                mESC = false;
                return true;

            case 54:  // 6 sets LPI to 6
                mESC = false;
                mLineHeightPixels = 10.5;
                return true;

            case 57:  // 9 sets LPI to 9
                mESC = false;
                mLineHeightPixels = 7;
                return true;

            case 65:  // A starts graphics mode
                mGraphicsMode = GraphicsMode::FETCH_MSB;
                mESC          = false;
                return true;
        }
        return false;
    }

    bool Atari1029::handlePrintableCodes(const unsigned char b)
    {
        /*auto*/ QChar qb{translateAtascii(b & 127)};  // Masking inverse characters.
        /*auto*/ GraphicsDrawText *text{new GraphicsDrawText(mPenPoint, mPen, 0, mFont, qb)};
        executeGraphicsPrimitive(text);
        mTotalColums += mElongatedMode ? 12 : 6;
        mPenPoint.setX(mPenPoint.x() + (mElongatedMode ? 12 : 6));
        return true;
    }

    void Atari1029::setElongatedMode(bool elongatedMode)
    {
        mElongatedMode = elongatedMode;
    }

    bool Atari1029::handleGraphicsMode(const unsigned char b)
    {
        switch (mGraphicsMode)
        {
            case GraphicsMode::FETCH_MSB:
                // b is the MSB of the count of following columns
                mGraphicsColumns = static_cast<uint16_t>(b << 8);
                mGraphicsMode    = GraphicsMode::FETCH_LSB;
                break;

            case GraphicsMode::FETCH_LSB:
                // b is the LSB of the count of following columns
                mGraphicsColumns += b;
                mGraphicsMode = GraphicsMode::PLOT_DOTS;
                break;

            case GraphicsMode::PLOT_DOTS: {
                // Now we fetch the graphics data, until mGraphicsColumns is 0
                // Paint the dots;
                QPointF point{this->mPenPoint};
                for (int i = 0; i < 7; i++)
                {
                    // Mask the point we want to draw.
                    /*auto*/ QPen pen{Qt::PenStyle::SolidLine};
                    pen.setColor((b & (1 << i)) ? Qt::black : Qt::white);
                    /*auto*/ GraphicsSetPoint *graphics{new GraphicsSetPoint(point, pen)};
                    executeGraphicsPrimitive(graphics);
                    point.setY(point.y() - 1);
                }
                mPenPoint.setX(mPenPoint.x() + 1);  // Move to next column;
                mGraphicsColumns--;
                mTotalColums++;

                if (mGraphicsColumns == 0 || mTotalColums > 480)
                {
                    mGraphicsMode = GraphicsMode::NOT_GRAPHICS;
                    if (mTotalColums > 480)
                    {
                        qDebug()  << "!n" << mTotalColums;
                        newLine();
                    }
                }
            }
            break;

            case GraphicsMode::NOT_GRAPHICS:  //Should not happen.
                Q_ASSERT(false);
        }

        return true;
    }

    void Atari1029::newLine()
    {
        mESC = false;
        setElongatedMode(false);
        mTotalColums = 0;
        mGraphicsMode = GraphicsMode::NOT_GRAPHICS;
        mPenPoint.setX(0);
        mPenPoint.setY(mPenPoint.y() + mLineHeightPixels);
    }
}  // namespace Printers