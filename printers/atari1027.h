#ifndef ATARI1027_H
#define ATARI1027_H

#include "atariprinter.h"

#include <QFont>
#include <QFontMetrics>
#include <QPrinter>
#include <QRect>
#include <QState>
#include <QPoint>

namespace Printers
{
    class Atari1027 : public AtariPrinter
    {
        Q_OBJECT
    public:
        Atari1027(SioWorkerPtr worker);
        ~Atari1027();

        virtual bool handleBuffer(const QByteArray &buffer, const unsigned int len) override;

        static QString typeName()
        {
            return "Atari 1027";
        }

    private:
        QPoint mPoint;
        bool mESC;
        QState *mUnderlinedState;

        bool handleEscapedCodes(const unsigned char b);
        bool handlePrintableCodes(const unsigned char b);
    };
}
#endif // ATARI1027_H
