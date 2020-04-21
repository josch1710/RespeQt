/*
 * textprinterwindow.cpp
 *
 * Copyright 2015 Joseph Zatarski
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "graphicsprinterwindow.h"
#include "ui_graphicsprinterwindow.h"
#include "respeqtsettings.h"

#include <memory>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QGraphicsEllipseItem>

// Includes, Globals and various additional class declarations // 
#include <QString>
#include <QFontComboBox>
#include <QMessageBox>
#include <QSharedPointer>

namespace Printers {

GraphicsPrinterWindow::GraphicsPrinterWindow(QWidget *parent) :
    QMainWindow(parent), NativeOutput(),
    ui(new Ui::GraphicsPrinterWindow)
{
    ui->setupUi(this);

    mGraphicsScene.setSceneRect(0, -499, 480, 1499);
    ui->printerGraphics->setScene(&mGraphicsScene);

    connect(this, &GraphicsPrinterWindow::textPrint, this, &GraphicsPrinterWindow::print);
    connect(this, &GraphicsPrinterWindow::closed, MainWindow::instance(), &MainWindow::closeGraphicsPrinterWindow);
    connect(this, &GraphicsPrinterWindow::graphicsPrint, this, &GraphicsPrinterWindow::printGraphics);
    connect(ui->actionSave, &QAction::triggered, this, &GraphicsPrinterWindow::saveTriggered);
    connect(ui->actionClear, &QAction::triggered, this, &GraphicsPrinterWindow::clearTriggered);
    connect(ui->actionPrint, &QAction::triggered, this, &GraphicsPrinterWindow::printTriggered);

    mFont.reset();
    mDevice.reset();
    mPainter.reset();
}

GraphicsPrinterWindow::~GraphicsPrinterWindow()
{
    delete ui;
}

void GraphicsPrinterWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void GraphicsPrinterWindow::closeEvent(QCloseEvent *e)
{
    // Save Current TexPrinterWindow Position and size // 
    if (RespeqtSettings::instance()->saveWindowsPos()) {
        // TODO Correct settings
        RespeqtSettings::instance()->setLastPrtHorizontalPos(GraphicsPrinterWindow::geometry().x());
        RespeqtSettings::instance()->setLastPrtVerticalPos(GraphicsPrinterWindow::geometry().y());
        RespeqtSettings::instance()->setLastPrtWidth(GraphicsPrinterWindow::geometry().width());
        RespeqtSettings::instance()->setLastPrtHeight(GraphicsPrinterWindow::geometry().height());
    }
    emit closed(this);
    e->accept();
}

void GraphicsPrinterWindow::print(const QString&)
{
    // TODO Print
}

void GraphicsPrinterWindow::printGraphics(GraphicsPrimitive *primitive)
{
    primitive->execute(mGraphicsScene);
    delete primitive;
}


void GraphicsPrinterWindow::clearTriggered()
{
    // TODO Clear Graphics pane
}

// Send to Printer Action   // 
void GraphicsPrinterWindow::printTriggered()
{
    QPrinter printer;
    auto dialog = new QPrintDialog(&printer, this);
    if (dialog->exec() != QDialog::Accepted)
        return;

    // TODO Print the scene
}

void GraphicsPrinterWindow::saveTriggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save printer text output"), RespeqtSettings::instance()->lastPrinterTextDir(),
                                                    tr("Text files (*.txt);;All files (*)"), nullptr);
    if (fileName.isEmpty()) {
        return;
    }
    RespeqtSettings::instance()->setLastPrinterTextDir(QFileInfo(fileName).absolutePath());
    QFile file(fileName);
    file.open(QFile::WriteOnly | QFile::Truncate);

    // TODO Save the scene
}

void GraphicsPrinterWindow::printChar(const QChar &c)
{
    emit textPrint(QString(c));
}

void GraphicsPrinterWindow::printString(const QString &s)
{
    emit textPrint(s);
}

bool GraphicsPrinterWindow::setupOutput()
{
    this->setGeometry(RespeqtSettings::instance()->lastPrtHorizontalPos(), RespeqtSettings::instance()->lastPrtVerticalPos(), RespeqtSettings::instance()->lastPrtWidth(), RespeqtSettings::instance()->lastPrtHeight());
    this->show();

    return true;
}

 void GraphicsPrinterWindow::executeGraphicsPrimitive(GraphicsPrimitive *primitive)
 {
    emit graphicsPrint(primitive);
 }

} // End of namespace
