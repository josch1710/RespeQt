/*
 * textprinterwindow.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef GRAPHICSPRINTERWINDOW_H
#define GRAPHICSPRINTERWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QString>
#include "nativeoutput.h"

namespace Ui {
    class GraphicsPrinterWindow;
}

namespace Printers {

class GraphicsPrinterWindow : public QMainWindow, public NativeOutput {
    Q_OBJECT
public:
    explicit GraphicsPrinterWindow(QWidget *parent = nullptr);
    ~GraphicsPrinterWindow();

    virtual void newLine(bool = false) override {};
    virtual void newPage(bool) override {}
    virtual void updateBoundingBox() override {}
    virtual bool beginOutput() override { return true; }
    virtual bool endOutput() override { return close(); }
    virtual void printChar(const QChar &c) override;
    virtual void printString(const QString &s) override;
    virtual void setWindow(const QRect &) {}
    virtual void setPen(const QColor &) {}
    virtual void setPen(Qt::PenStyle) {}
    virtual void setPen(const QPen &) {}
    virtual int dpiX() override { return 1; }
    virtual const QPen &pen() const { return mPen; }
    virtual void translate(const QPointF &) {}
    virtual void drawLine(const QPointF &, const QPointF &) {}
    virtual void calculateFixedFontSize(uint8_t) override {}
    virtual bool setupOutput() override;
    virtual void executeGraphicsPrimitive(GraphicsPrimitive *primitive) override;

    static QString typeName()
    {
        return QObject::tr("Graphics printer");
    }

protected:
    void changeEvent(QEvent *e) override;
    void closeEvent(QCloseEvent *e) override;

private:
    Ui::GraphicsPrinterWindow *ui;
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
    void closed(const Printers::GraphicsPrinterWindow* window);
    void textPrint(const QString &text);
    void graphicsPrint(GraphicsPrimitive *primitive);
};

}
#endif // GRAPHICSPRINTERWINDOW_H
