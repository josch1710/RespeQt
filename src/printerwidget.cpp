#include "printerwidget.h"
#include "ui_printerwidget.h"
#include "respeqtsettings.h"
#include "printers/printers.h"
#include "printers/printerfactory.h"

#include <QVector>
#include <QString>
#include <QMessageBox>
#include <memory>
#include <utility> 
PrinterWidget::PrinterWidget(int printerNum, QWidget *parent)
   : QFrame(parent)
   , ui(new Ui::PrinterWidget)
   , printerNo_(printerNum)
   , mPrinter(nullptr)
   //, mDevice(nullptr)
   , mSio(nullptr)
{
    ui->setupUi(this);

    // Connect the printer selection combobox
    void (QComboBox::*printerSignal)(const QString&) = &QComboBox::currentIndexChanged;
    connect(ui->atariPrinters, printerSignal, this, &PrinterWidget::printerSelectionChanged);
    // Connect the connect and disconnect button
    connect(ui->buttonConnectPrinter, &QToolButton::triggered, this, &PrinterWidget::connectPrinter);
    connect(ui->buttonDisconnectPrinter, &QToolButton::triggered, this, &PrinterWidget::disconnectPrinter);

    // Connect widget actions to buttons
    ui->buttonDisconnectPrinter->setDefaultAction(ui->actionDisconnectPrinter);
    ui->buttonConnectPrinter->setDefaultAction(ui->actionConnectPrinter);

    setup();
}

PrinterWidget::~PrinterWidget()
{
    disconnectPrinter();
    delete ui;
}

void PrinterWidget::setup()
{
    QString printerTxt = QString("P%1").arg(printerNo_ + 1);
    ui->printerLabel->setText(printerTxt);

    RespeqtSettings::PrinterSettings ps = RespeqtSettings::instance()->printerSettings(printerNo_);

    ui->atariPrinters->clear();
    std::map<QString, int> list;
    ui->atariPrinters->addItem(tr("None"), -1);
    auto factory = Printers::PrinterFactory::instance();
    const QVector<QString> pnames = factory->getPrinterNames();
    QVector<QString>::const_iterator it;
    for(it = pnames.begin(); it != pnames.end(); ++it)
    {
        ui->atariPrinters->addItem(*it);
    }

    // Set to default (none) and then look whether we have a settings
    ui->atariPrinters->setCurrentIndex(0);
    if (ps.printerName != "")
    {
        int index = ui->atariPrinters->findText(ps.printerName);
        if (index != -1)
        {
            ui->atariPrinters->setCurrentIndex(index);
        }
    }

    ui->atariPrinters->setEnabled(true);
    ui->actionDisconnectPrinter->setEnabled(false);
    ui->actionConnectPrinter->setEnabled(true);
}

void PrinterWidget::setSioWorker(SioWorkerPtr sio)
{
    mSio = std::move(sio);
    if (!mInitialized)
    {
        mInitialized = true;
    }
}

bool PrinterWidget::selectPrinter()
{
    if (ui->atariPrinters->currentText() == tr("None"))
    {
        return false;
    }
    // If we select a new printer, end the printing job of the old printer
    if (mPrinter)
    {
        // TBD Stop the printer.
        if (mSio)
            mSio->uninstallDevice(static_cast<quint8>(PRINTER_BASE_CDEVIC + printerNo_));
        mPrinter.reset();
    }
    if (mSio) {
       auto newPrinter = Printers::PrinterFactory::instance()->createPrinter(ui->atariPrinters->currentText(), mSio);
       if (newPrinter)
       {
           mSio->installDevice(static_cast<quint8>(PRINTER_BASE_CDEVIC + printerNo_), newPrinter.data());
           mPrinter = newPrinter;
           RespeqtSettings::instance()->setPrinterName(printerNo_, ui->atariPrinters->currentText());
           return true;
       }
    }
    return false;
}

/*bool PrinterWidget::selectOutput()
{
    if (ui->outputSelection->currentText() == tr("None"))
    {
        return false;
    }
    if (!mDevice.isNull())
    {
        mDevice.reset();
    }

    QString label = ui->outputSelection->currentText();
    auto output = Printers::OutputFactory::instance()->createOutput(label);
    // Try to cast to NativePrinter, if successful set printer name;
    auto printoutput = qSharedPointerDynamicCast<Printers::NativePrinter>(output);
    if (printoutput)
        printoutput->printer()->setPrinterName(label);

    if (output)
    {
        if (output->setupOutput())
        {
            mDevice = output;
            RespeqtSettings::instance()->setOutputName(printerNo_, label);
        }

        return true;
    }
    return false;
}*/

void PrinterWidget::connectPrinter()
{
    if (/*ui->outputSelection->currentIndex() == 0
            ||*/ ui->atariPrinters->currentIndex() == 0)
    {
        QMessageBox::warning(this, tr("Printers"), tr("Please select a printer emulation."));
        return;
    }

    if (!selectPrinter()/* || !selectOutput()*/)
    {
         disconnectPrinter();
         return;
    }

    if (mPrinter/* && mDevice*/)
    {
        /* TBD Raw Printer disabled for now
         * try {
            auto ptemp = qSharedPointerDynamicCast<Printers::Passthrough>(mPrinter);
            //auto otemp = qSharedPointerDynamicCast<Printers::RawOutput>(mDevice);
            if ((!ptemp.isNull() && otemp.isNull()) || (!otemp.isNull() && ptemp.isNull()))
            {
                QMessageBox::critical(this, tr("Printer emulation"), tr("You are not allowed to use the passthrough emulation without an raw output."));
                disconnectPrinter();
                return;
            }
        } catch(...) {}*/

        mOutputWindow = std::make_shared<Printers::OutputWindow>(new Printers::OutputWindow(this));
        connect(mOutputWindow.get(), &Printers::OutputWindow::closed, this, &PrinterWidget::disconnectPrinter);

        mOutputWindow->show();
        mPrinter->setOutputWindow(mOutputWindow);
        /*mDevice->setPrinter(mPrinter);
        if (!mDevice->beginOutput())
        {
            QMessageBox::critical(this, tr("Beginning output"), tr("The output device couldn't start."));
            return;
        }*/
        mConnected = true;
        ui->atariPrinters->setEnabled(false);
        ui->actionDisconnectPrinter->setEnabled(true);
        ui->actionConnectPrinter->setEnabled(false);
    }
}

void PrinterWidget::disconnectPrinter()
{
/*    if (mDevice)
    {
        mDevice->endOutput();
        mDevice.reset();
    }*/
    if (mOutputWindow)
    {
        mOutputWindow->close();
        mOutputWindow.reset();
        mPrinter->resetOutputWindow();
    }
    mConnected = false;
    ui->atariPrinters->setEnabled(true);
    ui->actionDisconnectPrinter->setEnabled(false);
    ui->actionConnectPrinter->setEnabled(true);
}

void PrinterWidget::outputSelectionChanged(const QString &outputName)
{
    RespeqtSettings::instance()->setOutputName(printerNo_, outputName);
}

void PrinterWidget::printerSelectionChanged(const QString &printerName)
{
    RespeqtSettings::instance()->setPrinterName(printerNo_, printerName);
}
