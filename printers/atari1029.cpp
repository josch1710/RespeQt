#include "atari1029.h"
#include "respeqtsettings.h"
#include "common/graphicsdotsitem.h"

#include <cstdlib>
#include <utility> 
#include <qglobal.h>
#include <QGraphicsLineItem> // TODO Create GraphicsDotsItem template
#include <QLineF>

namespace Printers
{
    Atari1029::Atari1029(SioWorkerPtr worker)
        : AtariPrinter(std::move(worker)),
          mESC(false),
          mElongatedMode(false)
    {
        auto font = new QFont(RespeqtSettings::instance()->atariFixedFontFamily(), 12);
        mFont = QSharedPointer<QFont>(font);
        if (mFont)
            mFont->setUnderline(false);
    }

    void Atari1029::applyResizing(QResizeEvent */*e*/)
    {

    }

    bool Atari1029::handleBuffer(const QByteArray &buffer, const unsigned int len)
    {
        for(unsigned int i = 0; i < len; i++)
        {
            auto b = static_cast<unsigned char>(buffer.at(static_cast<int>(i)));
            if (mGraphicsMode == GraphicsMode::NOT_GRAPHICS)
            {
                switch(b) {
                    case 13: // CTRL+N could be ESC code
                    case 14: // CTRL+O could be ESC code
                    case 23: // CTRL+W could be ESC code
                    case 24: // CTRL+X could be ESC code
                    case 25: // CTRL+Y could be ESC code
                    case 26: // CTRL+Z could be ESC code
                    case 54: // 6 could be ESC code
                    case 57: // 9 could be ESC code
                    case 65: // A could be ESC code
                        if (mESC)
                        {
                            if (!handleEscapedCodes(b))
                            {
                                handlePrintableCodes(b);
                            }
                        } else {
                            handlePrintableCodes(b);
                        }
                    break;

                    case 155: // EOL
                    {
                        flushTextBuffer();
                        mESC = false;
                        setElongatedMode(false);
                        if (mFont)
                            mFont->setUnderline(false);

                        // Linefeed

                        // Drop the rest of the buffer
                        return true;
                    }

                    case 27: // ESC could be starting something
                        if (mESC) { // ESC from last buffer
                            mESC = false;
                            handlePrintableCodes(b);
                        } else { // No ESC codes from last buffer
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

                    default: // Everythings else
                        handlePrintableCodes(b);
                    break;
                }
            } else
                handleGraphicsMode(b);
        }
        return true;
    }

    bool Atari1029::handleEscapedCodes(const unsigned char b)
    {
        // At this time we have seen an ESC.
        switch(b) {
            case 25: // CTRL+Y starts underline mode
            {
                if (mFont)
                    mFont->setUnderline(true);

                mESC = false;
                qDebug() << "!d" << "ESC Underline on";
                return true;
            }
            case 26: // CTRL+Z ends underline mode
            {
                if (mFont)
                    mFont->setUnderline(false);

                mESC = false;
                qDebug() << "!d" << "ESC Underline off";
                return true;
            }
            case 23: // CTRL+W starts international mode
                setInternationalMode(true);
                mESC = false;
                return true;

            case 24: // CTRL+X ends international mode
                setInternationalMode(false);
                mESC = false;
                return true;

            case 13: // CTRL+N starts elongated mode
                setElongatedMode(true);
                mESC = false;
                return true;

            case 14: // CTRL+O ends elongated mode
                setElongatedMode(false);
                mESC = false;
                return true;

            case 54: // 6 sets LPI to 6
                mLPI = 6;
                mESC = false;
                return true;

            case 57: // 9 sets LPI to 9
                mLPI = 9;
                mESC = false;
                return true;

            case 65: // A starts graphics mode
                flushTextBuffer();
                mGraphicsMode = GraphicsMode::FETCH_MSB;
                mESC = false;
                return true;
        }
        return false;
    }

    bool Atari1029::flushTextBuffer()
    {
        auto item = new QGraphicsTextItem{QString{mPrintText}};
        item->setFont(*mFont);
        item->setDefaultTextColor(QColor(0, 0, 0));
        item->setRotation(0);
        item->setPos(position());
        mOutputWindow->addGraphicsItem(item);
        executeGraphicsItems();
        // clear text buffer
        mPrintText.clear();

        return true;
    }

    bool Atari1029::handlePrintableCodes(const unsigned char b)
    {
        QChar qb = translateAtascii(b & 127); // Masking inverse characters.
        mPrintText.append(qb);
        return true;
    }

    void Atari1029::setElongatedMode(bool elongatedMode)
    {
        mElongatedMode = elongatedMode;
        if (mElongatedMode)
        {
            // TODO mOutput->calculateFixedFontSize(40);
        } else {
            // TODO mOutput->calculateFixedFontSize(80);
        }
    }

    bool Atari1029::handleGraphicsMode(const unsigned char b)
    {
        switch(mGraphicsMode)
        {
            case GraphicsMode::FETCH_MSB:
                // b is the MSB of the count of following columns
                mGraphicsColumns = static_cast<uint16_t>(b << 8);
                mGraphicsMode = GraphicsMode::FETCH_LSB;
            break;

            case GraphicsMode::FETCH_LSB:
                // b is the LSB of the count of following columns
                mGraphicsColumns += b;
                mGraphicsMode = GraphicsMode::PLOT_DOTS;
            break;

            case GraphicsMode::PLOT_DOTS:
            {
                // Now we fetch the graphics data, until mGraphicsColumns is 0
                // Paint the dots;
                auto _position = position();
                auto dots = new GraphicsDots7Item(_position, b);
                mGraphicsColumns--;
                _position.setX(_position.x()+1); // Move to next column;
                setPosition(_position);
                mOutputWindow->addGraphicsItem(dots);
                executeGraphicsItems();

                if (mGraphicsColumns == 0)
                    mGraphicsMode = GraphicsMode::NOT_GRAPHICS;
            }
            break;

            case GraphicsMode::NOT_GRAPHICS: //Should not happen.
                Q_ASSERT(false);
            break;
        }

        return true;
    }
}
