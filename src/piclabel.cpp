#include "include/piclabel.h"
#include <QPaintEvent>
#include <QPainter>

PicLabel::PicLabel(QWidget* parent) : QLabel(parent)
{
}

PicLabel::~PicLabel()
{
    if (_pixmap)
    {
        delete _pixmap;
        _pixmap = nullptr;
    }
    if (_lblText)
    {
        delete _lblText;
        _lblText = nullptr;
    }
}

void PicLabel::setPicPath(const QString& picPath)
{
    if (_picPath == picPath)
        return;

    _picPath = picPath;

    if (_pixmap)
        delete _pixmap;

    _pixmap = new QPixmap(_picPath);

    update();
}

void PicLabel::setText(const QString& text, bool center)
{
    if (center)
    {
        QLabel::setText(text);

        if (_lblText)
            _lblText->clear();
    }
    else
    {
        QLabel::setText("");

        if (_lblText == nullptr)
        {
            _lblText = new QLabel(this);
            QFont font("Ink Free");
            font.setPointSize(12);
            _lblText->setFont(font);
            _lblText->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            _lblText->setWordWrap(true);
        }
        _lblText->setText(text);
        moveText();
    }
}

void PicLabel::moveText()
{
    if (_lblText == nullptr)
        return;

    QSizeF szPic = (_pixmap ? _pixmap->size() : QSizeF(336, 224));

    int X = (int)round(150.0 * width() / szPic.width());
    int Y = (int)round(25.0 * height() / szPic.height());
    int W = (int)round(110.0 * width() / szPic.width());
    int H = (int)round(48.0 * height() / szPic.height());

    _lblText->setGeometry(X, Y, W, H);

    QFont font = _lblText->font();
    font.setPixelSize((int)round((double)H / 3.5));
    _lblText->setFont(font);
}

double PicLabel::ratio()
{
    double aspectRatio = 4.0 / 3.0;     // default is 4:3

    if (_pixmap)
        aspectRatio = static_cast<double>(_pixmap->width()) / _pixmap->height();

    return aspectRatio;
}

void PicLabel::paintEvent(QPaintEvent* event)
{
    if (_pixmap)
    {
        QPainter painter(this);
        painter.drawPixmap(rect(), _pixmap->scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    QLabel::paintEvent(event);
}

void PicLabel::resizeEvent(QResizeEvent *event)
{
    QLabel::resizeEvent(event);
    moveText();
}

QSize PicLabel::sizeHint() const
{
    if (_pixmap == nullptr)
        return QSize(200,130);  // TBD: why isn't this 1:1 with pixel ruler? (on a 4k monitor)

    return QSize(_pixmap->width(), _pixmap->height());
}
