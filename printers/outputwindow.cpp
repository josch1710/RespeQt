/*
 * textprinterwindow.cpp
 *
 * Copyright 2015 Joseph Zatarski
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "outputwindow.h"
#include "ui_outputwindow.h"
#include "respeqtsettings.h"

#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QString>
#include <QFontComboBox>
#include <QMessageBox>
#include <QSharedPointer>
#include <QPainter>
#include <QSvgGenerator>

#include <typeinfo>
#include <memory>


namespace Printers {

OutputWindow::OutputWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OutputWindow)
{
    ui->setupUi(this);

    mGraphicsScene = QGraphicsScenePtr::create(new QGraphicsScene());
    mGraphicsScene->setSceneRect(0, -499, 480, 1499);
    ui->printerGraphics->setScene(mGraphicsScene.data());
    mGraphicsScene->addRect(0, 0, 499, 999, QPen(QColor("magenta")));

    connect(mGraphicsScene.data(), &QGraphicsScene::changed, this, &OutputWindow::sceneChanged);
    connect(mGraphicsScene.data(), &QGraphicsScene::sceneRectChanged, this, &OutputWindow::sceneRectChanged);

    connect(this, &OutputWindow::textPrint, this, &OutputWindow::print);
    connect(this, &OutputWindow::graphicsPrint, this, &OutputWindow::printGraphics);
    connect(ui->actionSave, &QAction::triggered, this, &OutputWindow::saveTriggered);
    connect(ui->actionClear, &QAction::triggered, this, &OutputWindow::clearTriggered);
    connect(ui->actionPrint, &QAction::triggered, this, &OutputWindow::printTriggered);
    connect(ui->actionUp, &QAction::triggered, this, &OutputWindow::upTriggered);
    connect(ui->actionDown, &QAction::triggered, this, &OutputWindow::downTriggered);
    connect(ui->actionAutoZoom, &QAction::triggered, this, &OutputWindow::autoZoomTriggered);
    connect(ui->actionZoomIn, &QAction::triggered, this, &OutputWindow::zoomInTriggered);
    connect(ui->actionZoomOut, &QAction::triggered, this, &OutputWindow::zoomOutTriggered);

    mGraphicsScene->addItem(createCursor());
    mPrinterGroup = new QGraphicsItemGroup();
    mGraphicsScene->addItem(mPrinterGroup);
}

QGraphicsPolygonItem* OutputWindow::createCursor()
{
    mCursor = new QGraphicsPolygonItem();
    mCursor->setPos(0.0, 0.0);
    mCursor->setPen(QPen(Qt::black));
    mCursor->setBrush(QBrush(Qt::black));
    QPolygonF polygon;
    polygon << QPointF(0.0, 0.0);
    polygon << QPointF(-5.0, -5.0);
    polygon << QPointF(5.0, -5.0);
    mCursor->setPolygon(polygon);

    return mCursor;
}

OutputWindow::~OutputWindow()
{
    delete ui;
}

void OutputWindow::changeEvent(QEvent *e)
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

void OutputWindow::closeEvent(QCloseEvent *e)
{
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

void OutputWindow::resizeEvent(QResizeEvent *e)
{
    //qDebug() << "!n" << "old size " << e->oldSize() << " new size " << e->size();
    emit resized(e);
}

void OutputWindow::print(const QString&)
{
    // TODO Print text
}

void OutputWindow::printGraphics()
{
    if (ui->printerGraphics->scene() == nullptr)
        return;

    for(auto item: mGraphicsItems)
    {
        mPrinterGroup->addToGroup(item);
    }
    mGraphicsItems.clear();
}

void OutputWindow::clearTriggered()
{
    clearScene();
}

void OutputWindow::clearScene()
{
    if (ui->printerGraphics->scene()) {
        mCursor = nullptr;
        mGraphicsScene->clear();
        mGraphicsScene->addItem(createCursor());
    }
}

// Send to Printer Action   // 
void OutputWindow::printTriggered()
{
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
    painter.translate(printer.paperRect().x() + printer.pageRect().width()/2,
        printer.paperRect().y() + printer.pageRect().height()/2);
    painter.scale(scale, scale);
    painter.translate(-width()/2, -height()/2);
    // Now render the scene on the printer.
    ui->printerGraphics->render(&painter);
    painter.end();
}

void OutputWindow::saveTriggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save printer output"),
            RespeqtSettings::instance()->lastPrinterTextDir(),
            tr("SVG files (*.svg);;Text files (*.txt);;All files (*)"), nullptr);
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    file.open(QFile::WriteOnly | QFile::Truncate);
    QFileInfo info (fileName);
    auto extension = info.suffix();

    if (extension.toLower() == "svg") {
        QSvgGenerator svg;
        svg.setSize(QSize(width(), height()));
        svg.setOutputDevice(&file);
        QPainter painter;
        painter.begin(&svg);
        ui->printerGraphics->render(&painter);
        painter.end();
    }
    else if (extension.toLower() == "txt") {
        QTextStream out(&file);
        for(const auto item: ui->printerGraphics->items()) {
            if (typeid(item) == typeid(QGraphicsTextItem)) {
                auto textItem = dynamic_cast<QGraphicsTextItem*>(item);
                out << textItem->toPlainText() << "\n";
            }
        }
    }

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

// Needed for cross-thread execution
void OutputWindow::executeGraphicsItems()
{
    emit graphicsPrint();
}

void OutputWindow::addGraphicsItem(QGraphicsItem *item)
{
    mGraphicsItems.push_back(item);
}

void OutputWindow::setCursorPosition(const QPoint &position, qreal xscale, qreal yscale)
{
    //qDebug() << "!u" << "setCursorPosition " << position << " Transform " << transform;
    if (mCursor) {
        // TODO REMOVE ui->printerGraphics->scale(xscale, yscale);
        //TODO Remove mCursor->setTransform(transform);
        mCursor->setPos(position);
        ui->printerGraphics->ensureVisible(mCursor);
    }
}

void OutputWindow::setCursorColor(const QColor &color)
{
    if (mCursor) {
        mCursor->setPen(color);
        mCursor->setBrush(color);
    }
}

void OutputWindow::decorateToolbar(QWidgetList buttons)
{
    for(auto button: buttons) {
        ui->toolBar->addWidget(button);
    }
}

void OutputWindow::downTriggered()
{
    emit paperDown();
}

void OutputWindow::upTriggered()
{
    emit paperUp();
}


void OutputWindow::sceneChanged(const QList<QRectF> &regions) {
    //qDebug() << "!n" << regions;
}

void OutputWindow::redrawAll() {
    QList<QRectF> regions;
    QRectF region{};

    regions.append(region);
    sceneChanged(regions);
}

void OutputWindow::sceneRectChanged(const QRectF &rect) {
    //qDebug() << "!n" << "Scene changed" << rect;
}


void OutputWindow::autoZoomTriggered()
{
    auto autoZoomEnabled = ui->actionAutoZoom->isChecked();
    ui->actionZoomOut->setDisabled(autoZoomEnabled);
    ui->actionZoomIn->setDisabled(autoZoomEnabled);

}


void OutputWindow::zoomInTriggered()
{
    mXZoom *= 1.2;
    if (mXZoom > maxZoom)
        mXZoom = maxZoom;

    mYZoom *= 1.2;
    if (mYZoom > maxZoom)
        mYZoom = maxZoom;

    qDebug() << "!n" << "Zoom In " << fabs(mXZoom) << " " << fabs(mYZoom);
    ui->actionZoomIn->setDisabled(fabs(mXZoom) >= maxZoom || fabs(mYZoom) >= maxZoom);
    ui->actionZoomOut->setDisabled(false);
    ui->printerGraphics->scale(mXZoom, mYZoom);
    ui->printerGraphics->ensureVisible(mCursor);
}


void OutputWindow::zoomOutTriggered() {
    mXZoom /= 1.2;
    if (mXZoom < minZoom)
        mXZoom = minZoom;

    mYZoom /= 1.2;
    if (mYZoom < minZoom)
        mYZoom = minZoom;

    qDebug() << "!n" << "Zoom Out " << fabs(mXZoom) << " " << fabs(mYZoom);
    ui->actionZoomOut->setDisabled(fabs(mXZoom) <= minZoom || fabs(mYZoom) <= minZoom);
    ui->actionZoomIn->setDisabled(false);
    ui->printerGraphics->scale(mXZoom, mYZoom);
    ui->printerGraphics->ensureVisible(mCursor);
}

void OutputWindow::setScale(qreal xScale, qreal yScale)
{
    if (ui->actionAutoZoom->isChecked()) {
        if (xScale < minZoom)
            xScale = minZoom;
        if (xScale > maxZoom)
            xScale = maxZoom;
        if (yScale < minZoom)
            yScale = minZoom;
        if (yScale > maxZoom)
            yScale = maxZoom;
        ui->printerGraphics->scale(xScale, yScale);
        qDebug() << "!n" << xScale << " : " << yScale;
    }
}

} // End of namespace
