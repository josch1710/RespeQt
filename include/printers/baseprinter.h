#ifndef BASEPRINTER_H
#define BASEPRINTER_H

#include <QByteArray>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QPrinter>
#include <QRect>
#include <QSharedData>

// We need a forward class definition,
//because we reference BasePrinter in NativeOutput
namespace Printers {
  class BasePrinter;
  using BasePrinterPtr = QSharedPointer<BasePrinter>;
}// namespace Printers

#include "atascii.h"
#include "sioworker.h"
#include "outputwindow.h"

namespace Printers {
  class BasePrinter : public SioDevice {
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
    virtual const QRectF getSceneRect() const;

    static QString typeName() {
      throw new std::invalid_argument("Not implemented");
    }

  signals:
    void setSceneRect(const QRect &rect);

  protected:
    Atascii mAtascii;
    OutputWindowPtr mOutputWindow;

    QByteArray readDataFrame(uint size, bool isCommandFrame, bool verbose = true);
    bool writeDataFrame(QByteArray data);
    void dumpBuffer(unsigned char *buf, int len);
    void fillBuffer(char *line, unsigned char *buf, int len, int ofs, bool dumpAscii);
    void executeGraphicsPrimitive(GraphicsPrimitive *primitive);

    QFont mFont;
    QString buffer{};
    bool mClearPane;
    QPoint mPenPoint;
    QPen mPen;

  private:
    char m_lastOperation;
  };
}// namespace Printers
#endif// BASEPRINTER_H
