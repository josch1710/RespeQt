/*
 * picpreview.cpp - implementation of DiskBrowserDialog's PicPreview class.
 *
 * Copyright 2023 D.Caputi
 */
#include "include/diskbrowser/picpreview.h"
#include "respeqtsettings.h"
#include <cmath>
#include <QPaintEvent>
#include <QPainter>
#include <QGuiApplication>

const QString Label::DEF_INDEX_FNT { "Courier New" };
const double  Label::DEF_INDEX_SCALE { 2.5 };
const double  Label::DEF_TITLE_SCALE { 3.5 };

#if defined Q_OS_WIN
const QString Label::DEF_TITLE_FNT  { "Ink Free" };
const bool    Label::DEF_TITLE_BOLD { true };
#elif defined Q_OS_MAC
const QString Label::DEF_TITLE_FNT  { "Bradley Hand" };
const bool    Label::DEF_TITLE_BOLD { false };
#else
const QString Label::DEF_TITLE_FNT  { "Comic Sans" };       // Ubuntu only (TBD: redhat?)
const bool    Label::DEF_TITLE_BOLD { false };
#endif

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

void PicPreview::slotEditDone(const bool canceled)
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

void PicPreview::clearAll()
{
    clear();
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

    updateLabel();
}

void PicPreview::setLabel(const QString& title, const QString& index, const bool bSide)
{
    _title.setLabelText(title);
    _sideB = bSide;
    _index.setLabelText(index);

    updateLabel();
}

void PicPreview::setLabel(const DiskLabel& label)
{
    setLabel(label.title, label.index, label.sideB);
}

void PicPreview::loadPixmap(const QString& picPath)
{
    if (_picPath == picPath)
        return;

    delete _pixmap;

    _picPath = picPath;
    _pixmap = new QPixmap(_picPath);

    Q_ASSERT(_pixmap && !_pixmap->isNull());
}

QRectF PicPreview::scaleRect(const QRectF& rect, const QRectF& rcChild) const
{
    const QSizeF szPic = _pixmap->size();

    qreal X = round(rect.x() * rcChild.width() / szPic.width());
    qreal Y = round(rect.y() * rcChild.height() / szPic.height());
    const qreal W = round(rect.width() * rcChild.width() / szPic.width());
    const qreal H = round(rect.height() * rcChild.height() / szPic.height());

    X += rcChild.x();
    Y += rcChild.y();

    return QRectF {QPointF{X, Y}, QSizeF{W, H}};
}

// returns the current widget rect padded either horizontally or vertically
// to maintain the current aspect ratio (property defined by the content)
//
QRectF PicPreview::padRect() const
{
    QRectF lblRect(rect());

    if (const auto ratioNow = static_cast<qreal>(width()) / height(); ratio() < ratioNow)
    {
        // pillarbox (extra space on left/right)
        const auto maxWidth = round(ratio() * height());
        lblRect.setX((width() - maxWidth) / 2.0f);
        lblRect.setWidth(maxWidth);
    }
    else if (ratio() > ratioNow)
    {
        // letterbox (extra space on top/bottom)
        const auto maxHeight = round(width() / ratio());
        lblRect.setY((height() - maxHeight) / 2);
        lblRect.setHeight(maxHeight);
    }

    return lblRect;
}

void PicPreview::moveLabels()
{
    if (!_pixmap || _pixmap->isNull())
        return;

    constexpr QRectF LABEL_RECT_A {QPointF{57,24}, QSizeF{142,49}};
    constexpr QRectF LABEL_RECT_B {QPointF{23,24}, QSizeF{142,49}};

    QRectF labelRect;

    if (_sideB)
        labelRect = LABEL_RECT_B;
    else
        labelRect = LABEL_RECT_A;

    const auto paddedRect = padRect();
    const auto scaledRect = scaleRect(labelRect, paddedRect);

    _title.setGeometry(scaledRect.toRect());

    // move the index rect
    const double indexX = _sideB ? 170 : 20;
    const QRectF INDEX_RECT {QPointF{indexX,20}, QSizeF{30,30}};
    const auto indexRect = scaleRect(INDEX_RECT, paddedRect);

    _index.setGeometry(indexRect.toRect());
}

void PicPreview::scaleFonts()
{
    // The label is painted into the disk picture, so its size follows the label
    // rectangle rather than the system font -- but the quotient can round down
    // to zero before the picture is loaded and the labels have a geometry, and
    // setPixelSize(0) is rejected by Qt with a warning.
    LabelFont font = RespeqtSettings::instance()->dbTitleFont();
    double scl = font.scale();
    double pix = round(static_cast<double>(_title.size().height()) / scl);

    font.setPixelSize(qMax(1, static_cast<int>(pix)));

    _title.setFont(font);

    font = RespeqtSettings::instance()->dbIndexFont();
    scl = font.scale();
    pix = round(static_cast<double>(_index.size().height()) / scl);

    font.setPixelSize(qMax(1, static_cast<int>(pix)));

    _index.setFont(font);
}

void PicPreview::updateLabel()
{
    moveLabels();
    scaleFonts();
    update();
}

double PicPreview::ratio() const
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
        const auto lblRect{padRect()};
        const auto lblSize{lblRect.size().toSize()};
        const auto scaledPixmap{_pixmap->scaled(lblSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)};
        painter.drawPixmap(lblRect.toRect(), scaledPixmap);
    }

    QLabel::paintEvent(event);
}

void PicPreview::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    updateLabel();
}

QSize PicPreview::sizeHint() const
{
    if (_pixmap == nullptr)
        return {200,130};  // TBD: why isn't this 1:1 with pixel ruler? (on a 4k monitor)

    return {_pixmap->width(), _pixmap->height()};
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

Label::Label(QWidget* parent, const bool isIndex) : QTextEdit(parent)
{
    setContextMenuPolicy(Qt::NoContextMenu);

    _isIndex = isIndex;

    if (_isIndex)
    {
        setFont(RespeqtSettings::instance()->dbIndexFont());
        setAlignment(Qt::AlignCenter);
        setLineWrapMode(NoWrap);
        document()->setDocumentMargin(static_cast<qreal>(height())/2.0);
    }
    else
    {
        setFont(RespeqtSettings::instance()->dbTitleFont());
        setAlignment(Qt::AlignLeft | Qt::AlignTop);
        setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    }

    setFrameStyle(NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setReadOnly(true);
}

void Label::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);

    if (_isIndex)
        document()->setDocumentMargin(static_cast<qreal>(height())/4.0);
}

void Label::setLineHeight(const int height)
{
    _lineHeight = height;
}

void Label::setLabelText(const QString& text)
{
    setPlainText(text);

    QTextBlockFormat blockFmt;
    blockFmt.setLineHeight(_lineHeight, QTextBlockFormat::ProportionalHeight);

    if (_isIndex)
        blockFmt.setAlignment(Qt::AlignCenter);

    auto theCursor {textCursor()};
    theCursor.clearSelection();
    theCursor.select(QTextCursor::Document);
    theCursor.mergeBlockFormat(blockFmt);
}

void Label::setEditMode(const bool edit)
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
        setFrameStyle(Box | Plain);
        setLineWidth(1);
    }
    else
    {
        setFrameStyle(NoFrame);
    }
}

void Label::keyPressEvent(QKeyEvent* event)
{
    if (_editMode)
    {
        switch (event->key())
        {
            case Qt::Key_Return:
            case Qt::Key_Enter:
            {
                // if the shift key is down, process the return/enter key
                if (const auto mods {QGuiApplication::queryKeyboardModifiers()}; mods & Qt::ShiftModifier)
                    break;

                // return/enter completes edit mode
                setEditMode(false);
                emit sigEditDone(false); // not canceled
                return;
            }
            case Qt::Key_Escape:
            {
                setEditMode(false);
                setLabelText(_lastTitle);
                emit sigEditDone(true); // canceled
                return;
            }
            default:
                break;
        }
    }

    QTextEdit::keyPressEvent(event);
}

void Label::mousePressEvent(QMouseEvent* event)
{
    if (!_editMode && event->button() == Qt::LeftButton)
        setEditMode();
    else
        QTextEdit::mousePressEvent(event);
}

void Label::setFont(const LabelFont& font)
{
    _font = font;
    QTextEdit::setFont(font);

    const QString fmt
    {
        "color: %1;"
        "font-family: \"%2\";"
    };
    QString style = fmt.arg(_font.color().name(), _font.family());
    if (_font.bold())
        style += "font-weight: bold;";
    if (_font.italic())
        style += "font-style: italic;";
    setStyleSheet(style);
    auto pal {palette()};
    pal.setColor(QPalette::Base, QColor(0,0,0,0));
    setPalette(pal);

#ifndef QT_NO_DEBUG
    ensurePolished();
    Q_ASSERT(this->font().family() == _font.family());
#endif
}

void Label::focusOutEvent(QFocusEvent* event)
{
    QTextEdit::focusOutEvent(event);

    if (_editMode)
    {
        // select nothing (work-around for strange background color glitch)
        auto cursor {textCursor()};
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);

        setEditMode(false);
        emit sigEditDone(false);
    }
}
