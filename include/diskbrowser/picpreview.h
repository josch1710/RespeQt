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


class Label : public QTextEdit
{
    Q_OBJECT
public:
    Label(QWidget* parent, const QString& fontFamily = QString(), bool isIndex = false);
    void setText(const QString& text);
    void setLineHeight(int height);
    bool isEmpty() { return toPlainText().isEmpty(); }
    void setEditMode(bool edit = true);

signals:
    void sigEditDone(bool canceled);

protected:
    virtual void keyPressEvent(QKeyEvent* evt) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    bool    _isIndex  = false;
    bool    _editMode = false;
    QString _lastTitle;
    QString _fontColor {"black"};
#if defined Q_OS_WIN
    QString _fontFamily {"Ink Free"};
    bool _fontIsBold = true;
    int _lineHeight = 70;
#elif defined Q_OS_MAC
    QString _fontFamily {"Bradley Hand"};
    bool _fontIsBold = false;
    int _lineHeight = 65;
#else
    QString _fontFamily {"Comic Sans"};
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
    void setLabel(const QString& title, const QString& index, bool bSide);

    void editTitle();
    void editIndex();

    double ratio();
    void clear();

signals:
    void sigTitleChanged(QString title);
    void sigIndexChanged(QString index);
    void sigPopupMenuReq(const QPoint& pos);

protected:
    virtual QSize sizeHint() const override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private slots:
    void slotEditDone(bool canceled);

private:
    QString  _picPath;
    QString  _picTooltip;
    QPixmap* _pixmap {nullptr};
    Label    _title  {this};
    Label    _index  {this, "Courier New", true};
    bool     _sideB  {false};

    void loadPixmap(const QString& picPath);
    void moveLabels();
    void scaleFonts();
    void update();
    QRect padRect();
    QRect scaleRect(const QRectF& rect, const QRectF& rcChild);
    void popupMenuReq(const QPoint& pos);
};

#endif // PICPREVIEW_H
