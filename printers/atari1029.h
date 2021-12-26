#ifndef ATARI1029_H
#define ATARI1029_H

#include "atariprinter.h"
#include "common/graphicsdotsitem.h"

#include <QFont>
#include <QFontMetrics>
#include <QPrinter>
#include <QRect>
#include <QPoint>

namespace Printers
{
    enum class GraphicsMode {
        NOT_GRAPHICS,
        FETCH_LSB, FETCH_MSB,
        PLOT_DOTS
    };

    using GraphicsDots7Item = Common::GraphicsDotsItem<7, 1>;

    class Atari1029 : public AtariPrinter
    {
        Q_OBJECT
    public:
        Atari1029(SioWorkerPtr worker);

        virtual bool handleBuffer(const QByteArray &buffer, const unsigned int len) override;

        static QString typeName()
        {
            return "Atari 1029";
        }

    private:
        QPoint mPoint;
        bool mESC;
        bool mElongatedMode;
        GraphicsMode mGraphicsMode{GraphicsMode::NOT_GRAPHICS};
        uint16_t mGraphicsColumns;

        bool handleEscapedCodes(const unsigned char b);
        bool handlePrintableCodes(const unsigned char b);
        bool elongatedMode() { return mElongatedMode; }
        void setElongatedMode(bool elongatedMode);
        bool handleGraphicsMode(const unsigned char b);
        bool flushTextBuffer();
    };
}
#endif // ATARI1029_H
