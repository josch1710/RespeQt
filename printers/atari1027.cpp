#include "atari1027.h"
#include "respeqtsettings.h"

#include <cstdlib>
#include <utility>

#include <QState>
#include <QStateMachine>
#include <QSharedPointer>

namespace Printers
{
    Atari1027::Atari1027(SioWorkerPtr worker)
        : AtariPrinter(std::move(worker)),
          mESC(false)
    {
        mUnderlinedState = new QState(mCombinedState);
        mUnderlinedState->assignProperty(this, "underlined", false);
    }

    Atari1027::~Atari1027()
    {
        delete mUnderlinedState;
    }

    void Atari1027::setupFont()
    {
        mFont = QSharedPointer<QFont>(new QFont(RespeqtSettings::instance()->atariFixedFontFamily(), 12));
        mFont->setUnderline(false);
        // TODO calculateFixedFontSize(80);
    }

    bool Atari1027::handleBuffer(const QByteArray &buffer, const unsigned int len)
    {
        for(unsigned int i = 0; i < len; i++)
        {
            auto b = static_cast<unsigned char>(
                        buffer.at(static_cast<int>(i)));
            switch(b) {
                case 15: // CTRL+O starts underline mode
                {
                    if (mFont)
                    {
                        mFont->setUnderline(true);
                    }
                    mUnderlinedState->setProperty("underlined", true);
                    qDebug() << "!n" << "Underline on";
                }
                break;

                case 14: // CTRL+N ends underline mode
                {
                    if (mFont)
                    {
                        mFont->setUnderline(false);
                    }
                    mUnderlinedState->setProperty("underlined", false);
                    qDebug() << "!d" << "Underline off";
                }
                break;

                case 23: // CTRL+W could be ESC code
                case 24: // CTRL+X could be ESC code
                case 25: // CTRL+Y could be ESC code
                case 26: // CTRL+Z could be ESC code
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
                    mESC = false;
                    mEscState->setProperty("esc", false);
                    mUnderlinedState->setProperty("underlined", false);
                    if (mFont)
                    {
                        mFont->setUnderline(false);
                    }
                    //mOutput->newLine();
                    // Drop the rest of the buffer
                    return true;
                }
                // no break needed

                case 27: // ESC could be starting something
                    if (mEscState->property("esc").toBool()) { // ESC from last buffer
                        mESC = false;
                        mEscState->setProperty("esc", false);
                        handlePrintableCodes(27);
                    } else { // No ESC codes from last buffer
                        mESC = true;
                        mEscState->setProperty("esc", true);
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
        }
        return true;
    }

    bool Atari1027::handleEscapedCodes(const unsigned char b)
    {
        // At this time we have seen an ESC.
        switch(b) {
            case 25: // CTRL+Y starts underline mode
            {
                if (mFont)
                {
                    mFont->setUnderline(true);
                }
                mESC = false;
                mEscState->setProperty("esc", false);
                mUnderlinedState->setProperty("underlined", true);
                qDebug() << "!d" << "ESC Underline on";
                return true;
            }
            case 26: // CTRL+Z ends underline mode
            {
                if (mFont)
                {
                    mFont->setUnderline(false);
                }
                mESC = false;
                mEscState->setProperty("esc", false);
                mUnderlinedState->setProperty("underlined", false);
                qDebug() << "!d" << "ESC Underline off";
                return true;
            }
            case 23: // CTRL+W starts international mode
                setInternationalMode(true);

                mESC = false;
                mEscState->setProperty("esc", false);
                return true;

            case 24: // CTRL+X ends international mode
                setInternationalMode(false);
                mESC = false;
                mEscState->setProperty("esc", false);
                return true;

                // TODO Check
            /*default: // Not known control codes are consumed
                mESC = false;
                return true;*/
        }
        return false;
    }

    bool Atari1027::handlePrintableCodes(const unsigned char b)
    {
        QChar qb = translateAtascii(b & 127); // Masking inverse characters.
        mPrintText.append(qb);
        return true;
    }
}
