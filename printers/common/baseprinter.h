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
#include <QResizeEvent>

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
#include "../outputwindow.h"

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
        virtual void setOutputWindow(OutputWindowPtr outputWindow);
        void resetOutputWindow();

        void setPosition(const QPoint &position);
        void setPosition(int x, int y);
        const QPoint position() const { return mPosition; };
        void movePositionX(int x);
        void movePositionY(int y);
        void movePosition(QPoint p);

        static QString typeName()
        {
            throw new std::invalid_argument("Not implemented");
        }

    public slots:
        void paperUp();
        void paperDown();
        void outputWindowResized(QResizeEvent *e) { applyResizing(e); };

    protected:
        bool mClearPane;
        Atascii mAtascii;
        OutputWindowPtr mOutputWindow;
        QSharedPointer<QFont> mFont;

        QByteArray readDataFrame(uint size);
        bool writeDataFrame(QByteArray data);
        void dumpBuffer(unsigned char *buf, int len);
        void fillBuffer(char *line, unsigned char *buf, int len, int ofs, bool dumpAscii);
        void executeGraphicsItems();
        virtual void createOutputButtons() {}
        virtual void applyResizing(QResizeEvent *e) = 0;

    private:
        char m_lastOperation;
        QPoint mPosition{0, 0};

    signals:
        void setCursorPosition(const QPoint &position, qreal xscale, qreal yscale);
        void decorateOutputToolbar(QWidgetList buttons);
        void setScale(qreal xscale, qreal yscale);
    };
}
#endif // BASEPRINTER_H
