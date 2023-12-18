/*
 * picpreview.cpp - implementation of DiskBrowserDialog's PicPreview class.
 *
 * Copyright 2023 D.Caputi
 */
#include "include/diskbrowser/picpreview.h"
#include <math.h>
#include <QPaintEvent>
#include <QPainter>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>


PicPreview::PicPreview(QWidget* parent) : QLabel(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &PicPreview::customContextMenuRequested, this, &PicPreview::popupMenuReq);
    connect(&_title, &Label::sigEditDone, this, &PicPreview::slotEditDone);
    connect(&_index, &Label::sigEditDone, this, &PicPreview::slotEditDone);

//  _title.hide();
//  _index.hide();
    _title.setReadOnly(true);
    _index.setReadOnly(true);
}

void PicPreview::popupMenuReq(const QPoint& pos)
{
    emit sigPopupMenuReq(mapToGlobal(pos));
}

void PicPreview::slotEditDone(bool canceled)
{
    if (canceled)
    {
        setFocus();
        return;
    }

    if (sender() == qobject_cast<QObject*>(&_title))
        emit sigTitleChanged(_title.toPlainText());
    else
        emit sigIndexChanged(_index.toPlainText());
}

PicPreview::~PicPreview()
{
    if (_pixmap)
    {
        delete _pixmap;
        _pixmap = nullptr;
    }
}

void PicPreview::clear()
{
    QLabel::clear();
    _picPath.clear();
    _picTooltip.clear();
    _title.clear();
    _index.clear();
    _sideB = false;

    if (_pixmap)
    {
        delete _pixmap;
        _pixmap = nullptr;
    }

//  _title.hide();
//  _index.hide();
    _title.setEditMode(false);
    _index.setEditMode(false);
}

void PicPreview::setFileName(const QString& picPath)
{
    loadPixmap(picPath);            // load the pixmap from the new path name/resource

    if (picPath[0] != ':')          // custom pic?...
    {
        _index.clear();             // no number overlay
        _title.clear();             // no title overlay
    }

    update();
}

void PicPreview::setLabel(const QString& title, const QString& index, bool bSide)
{
    _title.setText(title);
    _sideB = bSide;
    _index.setText(index);

    update();
}

void PicPreview::setLabel(const DiskLabel& label)
{
    setLabel(label.title, label.index, label.sideB);
}

void PicPreview::loadPixmap(const QString& picPath)
{
    if (_picPath == picPath)
        return;

    _picPath = picPath;

    if (_pixmap)
        delete _pixmap;

    _pixmap = new QPixmap(_picPath);

    Q_ASSERT(_pixmap && !_pixmap->isNull());
}

QRect PicPreview::scaleRect(const QRectF& rect, const QRectF& rcChild)
{
    const QSizeF szPic = _pixmap->size();

    int X = (int)round(rect.x() * rcChild.width() / szPic.width());
    int Y = (int)round(rect.y() * rcChild.height() / szPic.height());
    int W = (int)round(rect.width() * rcChild.width() / szPic.width());
    int H = (int)round(rect.height() * rcChild.height() / szPic.height());

    X += rcChild.x();
    Y += rcChild.y();

    return QRect {QPoint{X,Y}, QSize{W,H}};
}

// returns the current widget rect padded either horizontally or vertically
// to maintain the current aspect ratio (property defined by the content)
//
QRect PicPreview::padRect()
{
    QRect lblRect(rect());
    double ratioNow = static_cast<double>(width()) / height();

    if (ratio() < ratioNow)
    {
        // pillarbox (extra space on left/right)
        int maxWidth = static_cast<int>(round(ratio() * height()));
        lblRect.setX((width() - maxWidth) / 2);
        lblRect.setWidth(maxWidth);
    }
    else if (ratio() > ratioNow)
    {
        // letterbox (extra space on top/bottom)
        int maxHeight = static_cast<int>(round(width() / ratio()));
        lblRect.setY((height() - maxHeight) / 2);
        lblRect.setHeight(maxHeight);
    }

    return lblRect;
}

void PicPreview::moveLabels()
{
    if (!_pixmap || _pixmap->isNull())
        return;

    const QRectF LABEL_RECT_A {QPointF{57,24}, QSizeF{142,49}};
    const QRectF LABEL_RECT_B {QPointF{23,24}, QSizeF{142,49}};

    QRectF labelRect;

    if (_sideB)
        labelRect = LABEL_RECT_B;
    else
        labelRect = LABEL_RECT_A;

    QRect paddedRect = padRect();
    QRect scaledRect = scaleRect(labelRect, paddedRect);

    _title.setGeometry(scaledRect);

    // move the index rect
    const double indexX = (_sideB ? 170 : 20);
    const QRectF INDEX_RECT {QPointF{indexX,20}, QSizeF{30,30}};
    const QRect  indexRect = scaleRect(INDEX_RECT, paddedRect);

    _index.setGeometry(indexRect);
}

void PicPreview::scaleFonts()
{
    QFont font = _title.font();
    double pix = round((double)_title.size().height() / 3.5);

    font.setPixelSize((int)pix);

    _title.setFont(font);

    font = _index.font();
    pix = round((double)_index.size().height() / 2.5);

    font.setPixelSize((int)pix);

    _index.setFont(font);
}

void PicPreview::update()
{
    moveLabels();
    scaleFonts();
    QLabel::update();
}

double PicPreview::ratio()
{
    double aspectRatio = 0.0;

    if (_pixmap)
        aspectRatio = static_cast<double>(_pixmap->width()) / _pixmap->height();

    return aspectRatio;
}

void PicPreview::paintEvent(QPaintEvent* event)
{
    if (_pixmap)
    {
        QPainter painter(this);
        QRect lblRect = padRect();
        painter.drawPixmap(lblRect, _pixmap->scaled(lblRect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    QLabel::paintEvent(event);
}

void PicPreview::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    update();
}

QSize PicPreview::sizeHint() const
{
    if (_pixmap == nullptr)
        return QSize(200,130);  // TBD: why isn't this 1:1 with pixel ruler? (on a 4k monitor)

    return QSize(_pixmap->width(), _pixmap->height());
}

void PicPreview::editTitle()
{
    _title.setEditMode();
}

void PicPreview::editIndex()
{
    _index.setEditMode();
}


// Title class -
// Derived from QTextEdit, this class encapsulates the floppy disk title rendered on the label (if no preview is defined).
// I'm using a Rich Text widget Due to the buggy nature specifically on macOS with line spacing in a QLabel.

Label::Label(QWidget* parent, const QString& fontFamily, bool isIndex) : QTextEdit(parent)
{
    setContextMenuPolicy(Qt::NoContextMenu);

    if (!fontFamily.isEmpty())
        _fontFamily = fontFamily;

    _isIndex = isIndex;

    if (_isIndex)
    {
        setAlignment(Qt::AlignCenter);
        setLineWrapMode(QTextEdit::NoWrap);
        document()->setDocumentMargin(qreal(height())/2.0);
    }
    else
    {
        setAlignment(Qt::AlignLeft | Qt::AlignTop);
        setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    }

    QString fmt
    {
        "color: %1;"
        "font-family: \"%2\";"
    };
    QString style = fmt.arg(_fontColor, _fontFamily);
    if (_fontIsBold)
        style += "font-weight: bold";
    setStyleSheet(style);
    auto pal = palette();
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    setPalette(pal);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setReadOnly(true);

#ifndef QT_NO_DEBUG
    ensurePolished();
    Q_ASSERT(font().family() == _fontFamily);
#endif
}

void Label::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);

    if (_isIndex)
        document()->setDocumentMargin(qreal(height())/4.0);
}

void Label::setLineHeight(int height)
{
    _lineHeight = height;
}

void Label::setText(const QString& text)
{
    setPlainText(text);

    QTextBlockFormat blockFmt;
    blockFmt.setLineHeight(_lineHeight, QTextBlockFormat::ProportionalHeight);

    if (_isIndex)
        blockFmt.setAlignment(Qt::AlignCenter);

    auto theCursor = textCursor();
    theCursor.clearSelection();
    theCursor.select(QTextCursor::Document);
    theCursor.mergeBlockFormat(blockFmt);
}

void Label::setEditMode(bool edit)
{
    if (edit == _editMode)
        return;

    _editMode = edit;

    setReadOnly(!edit);

    if (edit)
    {
        _lastTitle = toPlainText();
        setFocus();
        selectAll();
        QFrame::setFrameStyle(QFrame::Box | QFrame::Plain);
        QFrame::setLineWidth(1);
    }
    else
    {
        QFrame::setFrameStyle(QFrame::NoFrame);
    }
}

void Label::keyPressEvent(QKeyEvent* evt)
{
    if (_editMode)
    {
        switch (evt->key())
        {
            case Qt::Key_Return:
            case Qt::Key_Enter:
            {
                // if the shift key is down, process the return/enter key
                auto mods = QGuiApplication::queryKeyboardModifiers();
                if (mods & Qt::ShiftModifier)
                    break;

                // return/enter completes edit mode
                setEditMode(false);
                emit sigEditDone(false); // not canceled
                return;
            }
            case Qt::Key_Escape:
            {
                setEditMode(false);
                setText(_lastTitle);
                emit sigEditDone(true); // canceled
                return;
            }
            default:
                break;
        }
    }

    QTextEdit::keyPressEvent(evt);
}

void Label::mousePressEvent(QMouseEvent* evt)
{
    if (!_editMode && (evt->button() == Qt::LeftButton))
        setEditMode();
    else
        QTextEdit::mousePressEvent(evt);
}
