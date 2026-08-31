/*
 * textprinterwindow.cpp
 *
 * Copyright 2015 Joseph Zatarski
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "printers/outputwindow.h"
#include "respeqtsettings.h"
#include "ui_outputwindow.h"

#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>

// Includes, Globals and various additional class declarations //
#include <QPainter>
#include <QString>
#include <QSvgGenerator>

#include "uicolors.h"

namespace Printers {

  OutputWindow::OutputWindow(QWidget *parent) : QMainWindow(parent),
                                                ui(new Ui::OutputWindow) {
    ui->setupUi(this);

    ui->printerGraphics->setScene(&mGraphicsScene);
    
    // Force the view to align the scene to the top-left corner
    ui->printerGraphics->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    connect(this, &OutputWindow::textPrint, this, &OutputWindow::print);
    //connect(this, &OutputWindow::graphicsPrint, this, &OutputWindow::printGraphics);
    connect(ui->actionSave, &QAction::triggered, this, &OutputWindow::saveTriggered);
    connect(ui->actionClear, &QAction::triggered, this, &OutputWindow::clearTriggered);
    connect(ui->actionPrint, &QAction::triggered, this, &OutputWindow::printTriggered);

    applyPaletteColors();
  }

  OutputWindow::~OutputWindow() {
    delete ui;
  }

  void OutputWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
      case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;

    case QEvent::PaletteChange:
        applyPaletteColors();
        break;

      default:
        break;
    }
  }

  void OutputWindow::showEvent(QShowEvent* e)
  {
      QWidget::showEvent(e);

      if (e->type() == QEvent::Show && RespeqtSettings::instance()->saveWindowsPos())
      {
          // Restore last widget geometry
          auto parent = qobject_cast<PrinterWidget*>(parentWidget());
          QString name = QString("Printer%1").arg(parent->getPrinterNumber()+1);
          RespeqtSettings::instance()->restoreWidgetGeometry(this, name);
      }
      playScene();
  }

  void OutputWindow::closeEvent(QCloseEvent *e)
  {
    // Save Current Window Position and size //
    if (RespeqtSettings::instance()->saveWindowsPos())
    {
      auto parent = qobject_cast<PrinterWidget*>(parentWidget());
      QString name = QString("Printer%1").arg(parent->getPrinterNumber()+1);
      RespeqtSettings::instance()->saveWidgetGeometry(this, name);
    }
    emit closed(this);
    e->accept();
  }

  void OutputWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    playScene();
  }

  void OutputWindow::print(const QString &) {
    // TODO Print
  }


  void OutputWindow::clearTriggered() {
    // auto primitive = new GraphicsClearPane;
    // primitive->execute(ui->printerGraphics->scene());
    // delete primitive;
  }

  // Send to Printer Action   //
  void OutputWindow::printTriggered() {
    QPrinter printer;
    auto dialog = new QPrintDialog(&printer, this);
    if (dialog->exec() != QDialog::Accepted)
      return;

    QPainter painter;
    painter.begin(&printer);
    // Scale the contents of the window to the printer.
    auto xscale = printer.pageRect(QPrinter::DevicePixel).width() / static_cast<double>(width());
    auto yscale = printer.pageRect(QPrinter::DevicePixel).height() / static_cast<double>(height());
    auto scale = qMin(xscale, yscale);
    painter.translate(printer.paperRect(QPrinter::DevicePixel).x() + printer.pageRect(QPrinter::DevicePixel).width() / 2,
                      printer.paperRect(QPrinter::DevicePixel).y() + printer.pageRect(QPrinter::DevicePixel).height() / 2);
    painter.scale(scale, scale);
    painter.translate(-width() / 2.0, -height() / 2.0);
    // Now render the scene on the printer.
    ui->printerGraphics->render(&painter);
    painter.end();
  }

  void OutputWindow::saveTriggered() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save printer text output"),
        RespeqtSettings::instance()->lastPrinterTextDir(),
        tr("SVG files (*.svg);;All files (*)"), nullptr);
    if (fileName.isEmpty()) {
      return;
    }
    QFile file(fileName);
    file.open(QFile::WriteOnly | QFile::Truncate);
    QSvgGenerator svg;
    svg.setSize(QSize(width(), height()));
    svg.setOutputDevice(&file);
    QPainter painter;
    painter.begin(&svg);
    ui->printerGraphics->render(&painter);
    painter.end();
    file.close();
  }

  void OutputWindow::setSceneRect(const QRectF &sceneRect) {
    mGraphicsScene.setSceneRect(sceneRect);
  }

  void OutputWindow::addItem(QGraphicsItem *item)
  {
      // Ensure items are positioned at top-left corner (0, 0) 
      mGraphicsScene.addItem(item);
  }
  
  void OutputWindow::playScene()
  {
    auto scale{calculateScaleFactor()};

    setSceneRect(QRectF(0, 0, printerDimension.width(), ui->printerGraphics->height()));
    ui->printerGraphics->setTransform(QTransform().scale(scale, scale));

    // Ensure scene is properly updated and positioned
    mGraphicsScene.update();
    ui->printerGraphics->update(); // Force update of the graphics view
  }

  void OutputWindow::updatePrinterDimension(const QRectF &dimension)
  {
    printerDimension = dimension;
  }

  qreal OutputWindow::calculateScaleFactor() const
  {
    if (printerDimension.width() == 0)
    {
      return 1.0;
    }

    return static_cast<qreal>(ui->printerGraphics->width()) / printerDimension.width();
  }

  void OutputWindow::applyPaletteColors() const
  {
    UiColors::setBaseColor(ui->printerGraphics, Qt::white);
  }
}  // namespace Printers
