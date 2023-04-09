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
#include <QGraphicsEllipseItem>
#include <QPrintDialog>
#include <QPrinter>
#include <memory>

// Includes, Globals and various additional class declarations //
#include <QFontComboBox>
#include <QMessageBox>
#include <QPainter>
#include <QSharedPointer>
#include <QString>
#include <QSvgGenerator>

namespace Printers {

  OutputWindow::OutputWindow(QWidget *parent) : QMainWindow(parent), /* NativeOutput(),*/
                                                ui(new Ui::OutputWindow) {
    ui->setupUi(this);

    mGraphicsScene.setSceneRect(0, -499, 480, 1499);
    ui->printerGraphics->setScene(&mGraphicsScene);

    connect(this, &OutputWindow::textPrint, this, &OutputWindow::print);
    connect(this, &OutputWindow::graphicsPrint, this, &OutputWindow::printGraphics);
    connect(ui->actionSave, &QAction::triggered, this, &OutputWindow::saveTriggered);
    connect(ui->actionClear, &QAction::triggered, this, &OutputWindow::clearTriggered);
    connect(ui->actionPrint, &QAction::triggered, this, &OutputWindow::printTriggered);
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
      default:
        break;
    }
  }

  void OutputWindow::closeEvent(QCloseEvent *e) {
    // Save Current TexPrinterWindow Position and size //
    if (RespeqtSettings::instance()->saveWindowsPos()) {
      // TODO Correct settings
      RespeqtSettings::instance()->setLastPrtHorizontalPos(OutputWindow::geometry().x());
      RespeqtSettings::instance()->setLastPrtVerticalPos(OutputWindow::geometry().y());
      RespeqtSettings::instance()->setLastPrtWidth(OutputWindow::geometry().width());
      RespeqtSettings::instance()->setLastPrtHeight(OutputWindow::geometry().height());
    }
    emit closed(this);
    e->accept();
  }

  void OutputWindow::print(const QString &) {
    // TODO Print
  }

  void OutputWindow::printGraphics(GraphicsPrimitive *primitive) {
    primitive->execute(ui->printerGraphics->scene());
    delete primitive;
  }


  void OutputWindow::clearTriggered() {
    auto primitive = new GraphicsClearPane;
    primitive->execute(ui->printerGraphics->scene());
    delete primitive;
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
    auto xscale = printer.pageRect().width() / static_cast<double>(width());
    auto yscale = printer.pageRect().height() / static_cast<double>(height());
    auto scale = qMin(xscale, yscale);
    painter.translate(printer.paperRect().x() + printer.pageRect().width() / 2,
                      printer.paperRect().y() + printer.pageRect().height() / 2);
    painter.scale(scale, scale);
    painter.translate(-width() / 2, -height() / 2);
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

  /*void OutputWindow::printChar(const QChar &c)
{
    emit textPrint(QString(c));
}

void OutputWindow::printString(const QString &s)
{
    emit textPrint(s);
}

bool OutputWindow::setupOutput()
{
    this->setGeometry(RespeqtSettings::instance()->lastPrtHorizontalPos(), RespeqtSettings::instance()->lastPrtVerticalPos(), RespeqtSettings::instance()->lastPrtWidth(), RespeqtSettings::instance()->lastPrtHeight());
    this->show();

    return true;
}*/

  void OutputWindow::executeGraphicsPrimitive(GraphicsPrimitive *primitive) {
    emit graphicsPrint(primitive);
  }

}// namespace Printers
