#ifndef BASEPRINTER_H
#define BASEPRINTER_H

#include <QByteArray>
#include <QPainter>
#include <QPrinter>
#include <QRect>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QSharedData>
#include <QSharedPointer>

// We need a forward class definition,
//because we reference BasePrinter in other classes
namespace Printers
{
    class BasePrinter;
    using BasePrinterPtr = QSharedPointer<BasePrinter>;
    //using BasePrinterWPtr = QWeakPointer<BasePrinter>;
}

#include "sioworker.h"
#include "atascii.h"
#include "outputwindow.h"

namespace Printers
{
    class BasePrinter : public SioDevice
    {
        Q_OBJECT
    public:
        BasePrinter(SioWorkerPtr worker);
        virtual ~BasePrinter();

        void handleCommand(const quint8 command, const quint8 aux1, const quint8 aux2) override;
        virtual bool handleBuffer(const QByteArray &buffer, const unsigned int len) = 0;

        virtual const QChar translateAtascii(const unsigned char b) const;

        OutputWindowPtr outputWindow() const { return mOutputWindow; }
        void setOutputWindow(OutputWindowPtr outputWindow);
        void resetOutputWindow();
        virtual void setupFont() {}

        static QString typeName()
        {
            throw new std::invalid_argument("Not implemented");
        }

    protected:
        bool mClearPane;
        Atascii mAtascii;
        OutputWindowPtr mOutputWindow;
        QSharedPointer<QFont> mFont;

        QByteArray readDataFrame(uint size);
        bool writeDataFrame(QByteArray data);
        void dumpBuffer(unsigned char *buf, int len);
        void fillBuffer(char *line, unsigned char *buf, int len, int ofs, bool dumpAscii);
        void executeGraphicsPrimitive(GraphicsPrimitive *primitive);

    private:
        char m_lastOperation;

    };
}
#endif // BASEPRINTER_H
