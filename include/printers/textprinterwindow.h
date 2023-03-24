/*
 * textprinterwindow.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef TEXTPRINTERWINDOW_H
#define TEXTPRINTERWINDOW_H

#include "nativeoutput.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QString>

namespace Ui {
  class TextPrinterWindow;
}

namespace Printers {

  class TextPrinterWindow : public QMainWindow, public NativeOutput {
    Q_OBJECT
  public:
    explicit TextPrinterWindow(QWidget *parent = nullptr);
    ~TextPrinterWindow();

    virtual void newLine(bool linefeed = false) override;
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
    //virtual bool setupOutput() override;

    static QString typeName() {
      return QObject::tr("Text printer");
    }

  protected:
    void changeEvent(QEvent *e) override;
    void closeEvent(QCloseEvent *e) override;

  private:
    Ui::TextPrinterWindow *ui;
    QPen mPen;
    int effAtasciiFont;
    int effFontSize;
    bool showAscii;
    bool showAtascii;
    int fontSize;
    QString atasciiFont;

  protected slots:
    void saveTriggered();
    void clearTriggered();
    void wordwrapTriggered();
    void printTriggered();

    // To manipulate fonts and ascii/atascii windows  //
    void atasciiFontTriggered();
    void fontSizeTriggered();
    void hideshowAsciiTriggered();
    void hideshowAtasciiTriggered();
    void stripLineNumbersTriggered();
    void asciiFontChanged(const QFont &);
    void print(const QString &text);

  signals:
    void closed(const Printers::TextPrinterWindow *window);
    void textPrint(const QString &text);
  };

}// namespace Printers
#endif// TEXTPRINTERWINDOW_H
