#ifndef NATIVEOUTPUT_H
#define NATIVEOUTPUT_H

#include <QPainter>
#include <QRect>
#include <QPaintDevice>
#include <QSharedPointer>
#include <QWeakPointer>
#include <cmath>
#include <memory>

#include "sioworker.h"
#include "baseoutput.h"

namespace Printers
{
    class NativeOutput : public BaseOutput
    {
    public:
        NativeOutput();
        virtual ~NativeOutput();

        virtual bool beginOutput();
        virtual bool endOutput();
        virtual void newLine(bool linefeed = false);
        virtual void printChar(const QChar &c);
        virtual void printString(const QString &s);
        virtual void plot(QPoint p, uint8_t dot);
        virtual int width() {
            return static_cast<int>(trunc(mBoundingBox.width()));
        }
        virtual int height() {
            return static_cast<int>(trunc(mBoundingBox.height()));
        }
        virtual int dpiX() { return mDevice->logicalDpiX(); }
        virtual void setFont(const QFontPtr& font);
        QFontPtr font() const { return mFont; }
        QPaintDevicePtr device() const { return mDevice; }
        QPainterPtr painter() const { return mPainter; }
        virtual void calculateFixedFontSize(uint8_t line_char_count);

        static QString typeName()
        {
            throw new std::invalid_argument("Not implemented");
        }

    };
}
#endif // NATIVEOUTPUT_H
