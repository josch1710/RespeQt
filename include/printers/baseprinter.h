#ifndef BASEPRINTER_H
#define BASEPRINTER_H

#include <QPainter>
#include <QPrinter>
#include <QRect>

// We need a forward class definition,
//because we reference BasePrinter in NativeOutput
namespace Printers {
  class BasePrinter;
  using BasePrinterPtr = QSharedPointer<BasePrinter>;
}// namespace Printers

#include "atascii.h"
#include "sio/sioworker.h"
#include "outputwindow.h"

namespace Printers {
  class BasePrinter : public SIO::SioDevice {
    Q_OBJECT
  public:
    explicit BasePrinter(const SIO::SioWorkerPtr &worker);
    ~BasePrinter() override;

    void handleCommand(quint8 command, quint8 aux1, quint8 aux2) override;
    virtual bool handleBuffer(const QByteArray &buffer, unsigned int len) = 0;

    virtual QChar translateAtascii(unsigned char b) const;

    OutputWindowPtr outputWindow() const { return mOutputWindow; }
    void setOutputWindow(OutputWindowPtr outputWindow);
    void resetOutputWindow();
    virtual void setupFont() {}
    virtual QRectF getSceneRect() const;

    static QString typeName() {
      throw std::invalid_argument("Not implemented");
    }

  signals:
    void setSceneRect(const QRect &rect);
    void addItem(QGraphicsItem *item);
    void setPrinterDimension(const QRectF &dimension);

  protected:
    Atascii mAtascii;
    OutputWindowPtr mOutputWindow;

    QByteArray readDataFrame(uint size, bool isCommandFrame, bool verbose = true);
    bool writeDataFrame(QByteArray data);
    void dumpBuffer(const unsigned char *buf, int len);
    void fillBuffer(char* line, const unsigned char* buf, int len, int ofs, bool dumpAscii);
    virtual QRectF printerDimension() const = 0;

    QFont mFont{};
    bool mIsFontInitialized{false};
    QString buffer{};
    bool mClearPane{false};
    QPointF mPenPoint;
    QPen mPen;
    qreal mLineHeight{0};
    bool mESC;
    bool mElongatedMode;
    uint16_t mGraphicsColumns{0};


  private:
    char m_lastOperation{};
  };
}// namespace Printers
#endif// BASEPRINTER_H
