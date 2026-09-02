#include "printerwidget.h"
#include "printers/printerfactory.h"
#include "printers/printers.h"
#include "respeqtsettings.h"
#include "ui_printerwidget.h"
#include "uiscale.h"
#include "uicolors.h"

#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QPalette>
#include <memory>
#include <utility>

PrinterWidget::PrinterWidget(int printerNum, QWidget *parent)
    : QFrame(parent), ui(new Ui::PrinterWidget), printerNo_(printerNum), mPrinter(nullptr)
      //, mDevice(nullptr)
      ,
      mSio(nullptr) {
  ui->setupUi(this);
  // See DriveWidget: the form pinned the buttons and set no icon size.
  UiScale::applyToolButtonIconSizes(this);
  setup();

  // Connect the printer selection combobox
  connect(ui->atariPrinters, &QComboBox::currentTextChanged, this, &PrinterWidget::printerSelectionChanged);
  // Connect the connect and disconnect button
  connect(ui->buttonConnectPrinter, &QToolButton::triggered, this, &PrinterWidget::connectPrinter);
  connect(ui->buttonDisconnectPrinter, &QToolButton::triggered, this, &PrinterWidget::disconnectPrinter);

  // Connect widget actions to buttons
  ui->buttonDisconnectPrinter->setDefaultAction(ui->actionDisconnectPrinter);
  ui->buttonConnectPrinter->setDefaultAction(ui->actionConnectPrinter);

  applyPaletteColors();
}

PrinterWidget::~PrinterWidget() {
  disconnectPrinter();
  delete ui;
}

void PrinterWidget::setup() {
  QString printerTxt = QString("P%1").arg(printerNo_ + 1);
  ui->printerLabel->setText(printerTxt);

  // Keep the printer number column aligned without pinning the label to 26x26.
  ui->printerLabel->setMinimumWidth(
      ui->printerLabel->fontMetrics().horizontalAdvance(QStringLiteral("P9")));

  ui->atariPrinters->clear();
  std::map<QString, int> list;
  ui->atariPrinters->addItem(tr("None"), -1);
  auto factory = Printers::PrinterFactory::instance();
  const QVector<QString> pnames = factory->getPrinterNames();
  QVector<QString>::const_iterator it;
  for (it = pnames.begin(); it != pnames.end(); ++it) {
    ui->atariPrinters->addItem(*it);
  }

  // Set to default (none) and then look whether we have a settings
  ui->atariPrinters->setCurrentIndex(0);
  if (RespeqtSettings::instance()->printerName(printerNo_) != "") {
    int index = ui->atariPrinters->findText(RespeqtSettings::instance()->printerName(printerNo_));
    if (index != -1) {
      ui->atariPrinters->setCurrentIndex(index);
    }
  }

  ui->atariPrinters->setEnabled(true);
  ui->actionDisconnectPrinter->setEnabled(false);
  ui->actionConnectPrinter->setEnabled(true);
}

// The drive number and the image details are secondary information and were
// dimmed by the form through a literal palette override, rgb(104, 104, 104)
// and rgb(128, 128, 128). Both were chosen for a light window and drop to
// about 3:1 on a dark one, so derive the dimming from the palette instead.
void PrinterWidget::applyPaletteColors() {
#ifdef Q_OS_MAC
  const auto color{QColor(UiColors::isDark(ui->atariPrinters) ? Qt::black : Qt::white)};
#else
  const auto color{QColor(UiColors::isDark(ui->atariPrinters) ? Qt::white : Qt::black)};
#endif
  UiColors::setButtonColor(ui->atariPrinters, color);
}

// The colours above are derived once, so they would go stale when the user
// switches the system to dark mode while the window is open.
void PrinterWidget::changeEvent(QEvent *e) {
  QFrame::changeEvent(e);
  if (e->type() == QEvent::PaletteChange) {
    applyPaletteColors();
  }
}


void PrinterWidget::setSioWorker(SioWorkerPtr sio) {
  mSio = std::move(sio);
  if (!mInitialized) {
    mInitialized = true;
  }
}

bool PrinterWidget::selectPrinter() {
  if (ui->atariPrinters->currentText() == tr("None")) {
    return false;
  }
  // If we select a new printer, end the printing job of the old printer
  if (mPrinter) {
    // TBD Stop the printer.
    if (mSio)
      mSio->uninstallDevice(static_cast<quint8>(PRINTER_BASE_CDEVIC + printerNo_));
    mPrinter.reset();
  }
  if (mSio) {
    auto newPrinter = Printers::PrinterFactory::instance()->createPrinter(ui->atariPrinters->currentText(), mSio);
    if (newPrinter) {
      mSio->installDevice(static_cast<quint8>(PRINTER_BASE_CDEVIC + printerNo_), newPrinter.data());
      mPrinter = newPrinter;
      RespeqtSettings::instance()->setPrinterName(printerNo_, ui->atariPrinters->currentText());
      return true;
    }
  }
  return false;
}

void PrinterWidget::connectPrinter() {
  if (ui->atariPrinters->currentIndex() == 0) {
    QMessageBox::warning(this, tr("Printers"), tr("Please select a printer emulation."));
    return;
  }

  if (!selectPrinter() ) {
    disconnectPrinter();
    return;
  }

  if (mPrinter) {
    mOutputWindow = QSharedPointer<Printers::OutputWindow>(new Printers::OutputWindow(this));
    connect(mOutputWindow.data(), &Printers::OutputWindow::closed, this, &PrinterWidget::disconnectPrinter);

    mOutputWindow->show();
    mPrinter->setOutputWindow(mOutputWindow);
    mConnected = true;
    ui->atariPrinters->setEnabled(false);
    ui->actionDisconnectPrinter->setEnabled(true);
    ui->actionConnectPrinter->setEnabled(false);
    emit printerActivated(printerNo_, mPrinter);
  }
}

void PrinterWidget::disconnectPrinter() {
  if (mOutputWindow) {
    mOutputWindow->close();
    mOutputWindow.reset();
    mPrinter->resetOutputWindow();
  }
  mConnected = false;
  ui->atariPrinters->setEnabled(true);
  ui->actionDisconnectPrinter->setEnabled(false);
  ui->actionConnectPrinter->setEnabled(true);
  emit printerDeactivated(printerNo_, mPrinter);
}

void PrinterWidget::printerSelectionChanged(const QString &printerName) {
  RespeqtSettings::instance()->setPrinterName(printerNo_, printerName);
}
