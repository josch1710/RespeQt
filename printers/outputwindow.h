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
#include <memory>
//#include "nativeoutput.h"
#include "graphicsprimitive.h"

namespace Ui {
    class OutputWindow;
}

namespace Printers {

class OutputWindow : public QMainWindow/*, public NativeOutput*/ {
    Q_OBJECT
public:
    explicit OutputWindow(QWidget *parent = nullptr);
    ~OutputWindow();

    virtual void setWindow(const QRect &) {}
    virtual void translate(const QPointF &) {}
    virtual void drawLine(const QPointF &, const QPointF &) {}
    //virtual void calculateFixedFontSize(uint8_t) override {}
    //virtual bool setupOutput() override;
    virtual void executeGraphicsPrimitive(GraphicsPrimitive *primitive);
    virtual void clearScene() { mGraphicsScene.clear(); }
    virtual QRectF getSceneRect() { return mGraphicsScene.sceneRect(); }

    /*static QString typeName()
    {
        return QObject::tr("Graphics printer");
    }*/

protected:
    void changeEvent(QEvent *e) override;
    void closeEvent(QCloseEvent *e) override;

private:
    Ui::OutputWindow *ui;
    QPen mPen;
    QGraphicsScene mGraphicsScene;

protected slots:
    void saveTriggered();
    void clearTriggered();
    void printTriggered();

    // To manipulate fonts and ascii/atascii windows  // 
    void print(const QString &text);
    void printGraphics(GraphicsPrimitive *primitive);

signals:
    void closed(const Printers::OutputWindow* window);
    void textPrint(const QString &text);
    void graphicsPrint(GraphicsPrimitive *primitive);
};

using OutputWindowPtr = std::shared_ptr<OutputWindow>;

}
#endif // OUTPUTWINDOW_H
