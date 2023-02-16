/*
 * textprinterwindow.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QString>
#include <QSharedPointer>
#include <QTransform>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>

using QGraphicsScenePtr = QSharedPointer<QGraphicsScene>;
using QGraphicsItemsVector = QVector<QGraphicsItem*>;

namespace Ui {
    class OutputWindow;
}

namespace Printers {

class OutputWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit OutputWindow(QWidget *parent = nullptr);
    ~OutputWindow();

    void setWindow(const QRect &) {}
    void translate(const QPointF &) {}
    void drawLine(const QPointF &, const QPointF &) {}
    //void calculateFixedFontSize(uint8_t) override {}
    void executeGraphicsItems();
    void addGraphicsItem(QGraphicsItem *item);
    void clearScene();
    QRectF sceneRect() { return mGraphicsScene->sceneRect(); }

protected:
    void changeEvent(QEvent *e) override;
    void closeEvent(QCloseEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:
    Ui::OutputWindow *ui;
    QPen mPen{};
    QGraphicsItemsVector mGraphicsItems{};
    QGraphicsItemGroup* mPrinterGroup{nullptr};
    QGraphicsScenePtr mGraphicsScene{nullptr};
    QGraphicsPolygonItem* mCursor{nullptr};
    QGraphicsPolygonItem* createCursor();
    qreal mXZoom{1.0}, mYZoom{1.0};
    const qreal maxZoom{2.0};
    const qreal minZoom{0.5};

public slots:
    // Manipulate cursor and toolbar of output window
    void setCursorPosition(const QPoint &position, qreal xscale, qreal yscale);
    void setCursorColor(const QColor &color);
    void decorateToolbar(QWidgetList buttons);
    void sceneChanged(const QList<QRectF> &regions);
    void redrawAll();
    void setScale(qreal xScale, qreal yScale);

protected slots:
    void saveTriggered();
    void clearTriggered();
    void printTriggered();
    void upTriggered();
    void downTriggered();
    void autoZoomTriggered();
    void zoomInTriggered();
    void zoomOutTriggered();

    // To manipulate fonts and ascii/atascii windows  // 
    void print(const QString &text);
    void printGraphics();

    void sceneRectChanged(const QRectF &rect);

signals:
    void closed(const Printers::OutputWindow* window);
    void textPrint(const QString &text);
    void graphicsPrint();
    void paperUp();
    void paperDown();
    void resized(QResizeEvent *event);
};

using OutputWindowPtr = QSharedPointer<OutputWindow>;

}
#endif // OUTPUTWINDOW_H
