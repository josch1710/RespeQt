#ifndef SVGOUTPUT_H
#define SVGOUTPUT_H

#include "nativeoutput.h"
#include <memory>
#include <QSvgGenerator>
#include <QSharedPointer>

using QSvgGeneratorPtr = std::shared_ptr<QSvgGenerator>;

namespace Printers
{
    class SVGOutput : public NativeOutput
    {
    public:
        SVGOutput();
        virtual ~SVGOutput();
        QSvgGeneratorPtr svg() {
            return std::dynamic_pointer_cast<QSvgGenerator>(mDevice);
        }

        void setFileName(const QString &filename) { svg()->setFileName(filename); }
        virtual void updateBoundingBox() override;
        virtual void newPage(bool linefeed = false) override;
        virtual bool setupOutput() override;

        static QString typeName()
        {
            return "SVG";
        }

    };
}
#endif // SVGOUTPUT_H
