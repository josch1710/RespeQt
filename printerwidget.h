#ifndef PRINTERWIDGET_H
#define PRINTERWIDGET_H

#include "printers/common/baseprinter.h"
#include "sioworker.h"
#include "printers/outputwindow.h"

#include <QFrame>

namespace Ui {
    class PrinterWidget;
}

class PrinterWidget : public QFrame
{
    Q_OBJECT

public:
    explicit PrinterWidget(int printerNum, QWidget *parent = nullptr);
    ~PrinterWidget();

    int getPrinterNumber() const { return printerNo_; }

    Printers::BasePrinterPtr printer() const { return mPrinter; }
    void setPrinter(Printers::BasePrinterPtr printer) { mPrinter = printer; }

    void setSioWorker(SioWorkerPtr sio);

    Printers::OutputWindowPtr outputWindow() { return mOutputWindow; }

    bool connected() const { return mConnected; }

signals:
    void actionEject(int deviceId);
    void actionConnectPrinter(int deviceId);

public slots:
    void disconnectPrinter();

private slots:
    void connectPrinter();
    void outputSelectionChanged(const QString &outputName);
    void printerSelectionChanged(const QString &printerName);

private:
    void setup();
    bool selectPrinter();

    Ui::PrinterWidget *ui;
    int printerNo_;
    Printers::BasePrinterPtr mPrinter;
    Printers::OutputWindowPtr mOutputWindow;
    
    SioWorkerPtr mSio;
    bool mInitialized{false};
    bool mConnected{false};
};

#endif // PRINTERWIDGET_H
