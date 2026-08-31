#ifndef PRINTERWIDGET_H
#define PRINTERWIDGET_H

#include "printers/baseprinter.h"
#include "sioworker.h"
//#include "printers/nativeoutput.h"
#include "printers/outputwindow.h"

#include <QFrame>
#include <utility>

namespace Ui {
  class PrinterWidget;
}

class PrinterWidget : public QFrame {
  Q_OBJECT

public:
  explicit PrinterWidget(int printerNum, QWidget *parent = nullptr);
  ~PrinterWidget() override;

  int getPrinterNumber() const { return printerNo_; }

  Printers::BasePrinterPtr printer() const { return mPrinter; }
  void setPrinter(Printers::BasePrinterPtr printer) { mPrinter = std::move(printer); }

  void setSioWorker(SioWorkerPtr sio);

  Printers::OutputWindowPtr outputWindow() { return mOutputWindow; }

  bool connected() const { return mConnected; }

signals:
  void actionEject(int deviceId);
  void actionConnectPrinter(int deviceId);
  void printerActivated(int deviceId, Printers::BasePrinterPtr printer);
  void printerDeactivated(int deviceId, Printers::BasePrinterPtr printer);

public slots:
  void disconnectPrinter();

protected:
  void changeEvent(QEvent *e) override;

private slots:
  void connectPrinter();
  void printerSelectionChanged(const QString &printerName);

private:
  void setup();
  bool selectPrinter();
  void applyPaletteColors();


  Ui::PrinterWidget *ui;
  int printerNo_;
  Printers::BasePrinterPtr mPrinter;
  Printers::OutputWindowPtr mOutputWindow;

  SioWorkerPtr mSio;
  bool mInitialized{false};
  bool mConnected{false};
};

#endif// PRINTERWIDGET_H
