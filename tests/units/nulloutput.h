#ifndef NULLOUTPUT_H
#define NULLOUTPUT_H

#include "printers/nativeoutput.h"

namespace Tests {

    class NullOutput final : public Printers::NativeOutput
    {
    public:
        NullOutput() = default;
        virtual ~NullOutput() = default;

        bool setupOutput() override { return true; }
        void executeGraphicsPrimitive(Printers::GraphicsPrimitive *primitive) override {}
        bool beginOutput() override { return true; }
        bool endOutput() override { return true; }
        void newPage(bool) override {}
        void newLine(bool) override {}
        void printChar(const QChar&) override {}
        void printString(const QString&) override {}
        void plot(QPoint, uint8_t) override {}
        int width() override { return 0; }
        int height() override { return 0; }
        int dpiX() override { return 0; }
        void setFont(QFontPtr) override {}
        void applyFont() override {}
        void calculateFixedFontSize(uint8_t) override {}

        static QString typeName()
        {
            return "Null Output";
        }

    protected:
        void updateBoundingBox() override {}
    };
}

#endif // NULLOUTPUT_H
