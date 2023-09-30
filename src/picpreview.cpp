/*
 * picpreview.cpp - implementation of DiskBrowserDialog's PicPreview class.
 *
 * Copyright 2023 D.Caputi
 */
#include "include/picpreview.h"
#include <math.h>
#include <QPaintEvent>
#include <QPainter>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QFileInfo>


PicPreview::PicPreview(QWidget* parent) : QLabel(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &PicPreview::customContextMenuRequested, this, &PicPreview::popupMenuReq);
}

void PicPreview::popupMenuReq(const QPoint& pos)
{
    emit sigPopupMenuReq(pos);
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
    _diskName.clear();
    _picPath.clear();
    _picTooltip.clear();
    _title.clear();
    _diskNo.clear();
    _isSideA = false;
    _isSideB = false;

    if (_pixmap)
    {
        delete _pixmap;
        _pixmap = nullptr;
    }
}

void PicPreview::setFileName(const QString& picPath)
{
    loadPixmap(picPath);            // load the pixmap from the new path name/resource

    if (picPath[0] != ':')          // custom pic?...
    {
        _diskNo.clear();            // no number overlay
        _title.clear();             // no title overlay
    }

    update();
}

void PicPreview::setLabel(const QString& title, int index, bool bSide)
{
    _title.setText(title);
    _diskNo.setText(QString::number(index));
    _isSideA = !bSide && index;
    _isSideB = bSide;

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

    const QRectF LABEL_RECT   {QPointF{95,24}, QSizeF{105,49}};
    const QRectF LABEL_RECT_A {QPointF{57,24}, QSizeF{142,49}};
    const QRectF LABEL_RECT_B {QPointF{23,24}, QSizeF{142,49}};

    QRectF labelRect = LABEL_RECT;

    if (_isSideA)
        labelRect = LABEL_RECT_A;
    if (_isSideB)
        labelRect = LABEL_RECT_B;

    QRect paddedRect = padRect();
    QRect scaledRect = scaleRect(labelRect, paddedRect);

    _title.setGeometry(scaledRect);

    if (_isSideA || _isSideB)
    {
        // move the index rect
        const double indexX = (_isSideA ? 25 : 175);
        const QRectF INDEX_RECT {QPointF{indexX,25}, QSizeF{20,20}};
        const QRect  indexRect = scaleRect(INDEX_RECT, paddedRect);

        _diskNo.setGeometry(indexRect);
        _diskNo.setVisible(true);
    }
    else
        _diskNo.setVisible(false);
}

void PicPreview::scaleFonts()
{
    QFont font = _title.font();
    double pix = round((double)_title.size().height() / 3.5);

    font.setPixelSize((int)pix);

    _title.setFont(font);

    font = _diskNo.font();
    pix = round((double)_diskNo.size().height() / 2.0);

    font.setPixelSize((int)pix);

    _diskNo.setFont(font);
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


// Title class -
// Derived from QTextEdit, this class encapsulates the floppy disk title rendered on the label (if no preview is defined).
// I'm using a Rich Text widget Due to the buggy nature specifically on macOS with line spacing in a QLabel.

Title::Title(QWidget* parent) : QTextEdit(parent)
{
    QString fmt
    {
        "color: %1;"
        "font-family: \"%2\";"
    };

    QString style = fmt.arg(_fontColor, _fontFamily);
    if (_fontIsBold)
        style += "font-weight: bold";
    setStyleSheet(style);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
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

void Title::setLineHeight(int height)
{
    _lineHeight = height;
}

void Title::setText(const QString& text)
{
    setPlainText(text);

    QTextBlockFormat blockFmt;
    blockFmt.setLineHeight(_lineHeight, QTextBlockFormat::ProportionalHeight);

    auto theCursor = textCursor();
    theCursor.clearSelection();
    theCursor.select(QTextCursor::Document);
    theCursor.mergeBlockFormat(blockFmt);
}

DiskNo::DiskNo(QWidget* parent) : QLabel(parent)
{
    setStyleSheet("color: black; font-family: \"Courier New\"");
}
