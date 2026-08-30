/*
 * textprinterwindow.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include <QString>
#include <QGraphicsItem>
#include <memory>

namespace Ui {
  class OutputWindow;
}

namespace Printers {

  class OutputWindow : public QMainWindow /*, public NativeOutput*/ {
    Q_OBJECT

  public:
    explicit OutputWindow(QWidget *parent = nullptr);
    ~OutputWindow();

    virtual void setWindow(const QRect &) {}
    //virtual void translate(const QPointF &) {}
    //virtual void drawLine(const QPointF &, const QPointF &) {}
    //virtual void calculateFixedFontSize(uint8_t) override {}

  public slots:
    void setSceneRect(const QRectF &sceneRect);
    void addItem(QGraphicsItem *item);
    void playScene();
    // Set printer dimension for scaling operations
    void updatePrinterDimension(const QRectF &dimension);


  protected:
    void changeEvent(QEvent *e) override;
    void closeEvent(QCloseEvent *e) override;
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent *e) override;
    qreal calculateScaleFactor() const;


  private:
    Ui::OutputWindow *ui;
    QPen mPen;
    QGraphicsScene mGraphicsScene;
    QRectF printerDimension{0, 0, 0, 0};

  protected slots:
    void saveTriggered();
    void clearTriggered();
    void printTriggered();

    // To manipulate fonts and ascii/atascii windows  //
    void print(const QString &text);

  signals:
    void closed(const Printers::OutputWindow *window);
    void textPrint(const QString &text);
  };

  using OutputWindowPtr = QSharedPointer<OutputWindow>;

}// namespace Printers
#endif// OUTPUTWINDOW_H
