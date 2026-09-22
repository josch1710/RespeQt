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
#include "ui/uiscale.h"

class LabelFont : public QFont
{
//  Q_OBJECT
public:
    LabelFont() = default;

    // Copying the four attributes by hand used to lose the size: a font that
    // carries a point size has pixelSize() == -1, so setPixelSize(-1) was
    // rejected by Qt with a warning and the size fell back to the default. The
    // base class copy keeps the size in whichever unit the source uses.
    explicit LabelFont(const QFont& font) : QFont(font)
    {
        _color = QColor(Qt::black);
        _scale = 3.0;
    }

    explicit LabelFont(const QString& family, const bool bold = false, const bool italic = false, QColor color = QColor("black"), const double scale = 3.0)
    {
        setFamily(family);
        setBold(bold);
        setItalic(italic);
        // PicPreview::scaleFonts() derives the size that actually gets painted
        // from the label rectangle, but until then the font has to have a valid
        // size, otherwise it reaches the settings as -1.
        setPointSize(UiScale::defaultPointSize());
        _color = std::move(color);
        _scale = scale;
    }
    ~LabelFont() = default;

    [[nodiscard]] QColor color() const { return _color; }
    void setColor(const QColor& color)  { _color = color; }
    void setColor(const QString& color) { _color = QColor(color); }

    [[nodiscard]] double scale() const { return _scale; }
    void setScale(const double scale) { _scale = scale; }

//  QFont qfont() { return QFont {family(), pointSize(), weight(), italic()}; }  // TBD: override cast operator? omit?

private:
    QColor _color = QColor("black");
    double _scale = 3.0;
};

class Label : public QTextEdit
{
    Q_OBJECT
public:
    explicit Label(QWidget* parent, bool isIndex = false);
    void setLabelText(const QString& text);
    void setLineHeight(int height);
    [[nodiscard]] bool isEmpty() const { return toPlainText().isEmpty(); }
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
    void keyPressEvent(QKeyEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

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
    explicit PicPreview(QWidget* parent);
    ~PicPreview() override;

    void setFileName(const QString& picPath);
    void setLabel(const DiskLabel& label);
    void setLabel(const QString& title, const QString& index, bool bSide);

    void editTitle();
    void editIndex();

    [[nodiscard]] double ratio() const;
    void clearAll();

    [[nodiscard]] QSize sizeHint() const override;

signals:
    void sigTitleChanged(QString title);
    void sigIndexChanged(QString index);
    void sigPopupMenuReq(const QPoint& pos);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

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
    void updateLabel();
    [[nodiscard]] QRectF padRect() const;
    [[nodiscard]] QRectF scaleRect(const QRectF& rect, const QRectF& rcChild) const;
    void popupMenuReq(const QPoint& pos);
};

#endif // PICPREVIEW_H
