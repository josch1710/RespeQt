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

class LabelFont : public QFont
{
//  Q_OBJECT
public:
    LabelFont() {}

    LabelFont(const QFont& font)
    {
        setFamily(font.family());
        setBold(font.bold());
        setItalic(font.italic());
        setPixelSize(font.pixelSize());
        _color = QRgb(0);
        _scale = 3.0;
    }

    LabelFont(const QString& family, bool bold = false, bool italic = false, QColor color = QColor("black"), double scale = 3.0)
    {
        setFamily(family);
        setBold(bold);
        setItalic(italic);
        _color = color;
        _scale = scale;
    }
    virtual ~LabelFont() {}

    QColor color() const { return _color; }
    void setColor(const QColor& color)  { _color = color; }
    void setColor(const QString& color) { _color = QColor(color); }

    double scale() const { return _scale; }
    void setScale(double scale) { _scale = scale; }

//  QFont qfont() { return QFont {family(), pointSize(), weight(), italic()}; }  // TBD: override cast operator? omit?

private:
    QColor _color = QColor("black");
    double _scale = 3.0;
};

class Label : public QTextEdit
{
    Q_OBJECT
public:
    Label(QWidget* parent, bool isIndex = false);
    void setText(const QString& text);
    void setLineHeight(int height);
    bool isEmpty() { return toPlainText().isEmpty(); }
    void setEditMode(bool edit = true);
    void setFont(const LabelFont& font);

    const LabelFont& font() { return _font; }

    static const QString DEF_INDEX_FNT;
    static const QString DEF_TITLE_FNT;
    static const bool    DEF_TITLE_BOLD;
    static const double  DEF_INDEX_SCALE;
    static const double  DEF_TITLE_SCALE;

signals:
    void sigEditDone(bool canceled);

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    bool      _isIndex  = false;
    bool      _editMode = false;
    QString   _lastTitle;
    LabelFont _font;

#if defined Q_OS_WIN
    int _lineHeight = 70;
#elif defined Q_OS_MAC
    int _lineHeight = 65;
#else
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
    Label    _index  {this, true};
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
