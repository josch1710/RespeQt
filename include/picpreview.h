/*
 * picpreview.h - declaration of DiskBrowserDialog's PicPreview class.
 *
 * Copyright 2023 D.Caputi
 */
#ifndef PICPREVIEW_H
#define PICPREVIEW_H

#include <QLabel>
#include <QString>
#include <QPixmap>
#include <QTextEdit>
#include "folderdisks.h"

class DiskNo : public QLabel
{
public:
    DiskNo(QWidget* parent);
    bool isEmpty() { return text().isEmpty(); }
};

class Title : public QTextEdit
{
public:
    Title(QWidget* parent);
    void setText(const QString& text);
    void setLineHeight(int height);
    bool isEmpty() { return toPlainText().isEmpty(); }

private:
    QString _fontColor {"black"};
#if defined Q_OS_WIN
    const QString _fontFamily {"Ink Free"};
    bool _fontIsBold = true;
    int _lineHeight = 70;
#elif defined Q_OS_MAC
    const QString _fontFamily {"Bradley Hand"};
    bool _fontIsBold = false;
    int _lineHeight = 65;
#else
    const QString _fontFamily {"Comic Sans"};
    bool _fontIsBold = false;
    int _lineHeight = 70;
#endif
};

class PicPreview : public QLabel
{
    Q_OBJECT

public:
    PicPreview(QWidget* parent);
    virtual ~PicPreview();

    void setFileName(const QString& name);
    void setLabel(const DiskLabel& label);
    void setLabel(const QString& title, int diskNo, bool bSide);

    double ratio();
    void clear();

    virtual QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

signals:
    void sigPopupMenuReq(const QPoint& pos);

private:
    QString  _diskName;
    QString  _picPath;
    QString  _picTooltip;
    QPixmap* _pixmap  {nullptr};
    Title    _title   {this};
    DiskNo   _diskNo  {this};
    bool     _isSideB {false};

    void loadPixmap(const QString& picPath);
    void moveLabels();
    void scaleFonts();
    void update();
    QRect padRect();
    QRect scaleRect(const QRectF& rect, const QRectF& rcChild);
    void popupMenuReq(const QPoint& pos);
};

#endif // PICPREVIEW_H
