#include "include/piclabel.h"
#include <QPaintEvent>
#include <QPainter>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDebug>


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
}

bool PicLabel::isFloppyPng()
{
    return (!_picPath.isEmpty() && (_picPath.at(0) == ':'));
}

void PicLabel::setDiskName(const QString &diskName)
{
    QFileInfo fiDisk = QFileInfo(diskName);
    if (!fiDisk.exists())
        return;

    // 1. look for a preview/thumbnail with diskname.png
    QString   fileName  = fiDisk.completeBaseName();
    QString   imagePath = fiDisk.absolutePath() + "/" + fileName + ".png";
    QFileInfo fiPreview = QFileInfo(imagePath);

    setText(fileName);

    if (!fiPreview.exists())
    {
        // 2. use a generic name for default thumbnail
        imagePath = fiDisk.absolutePath() + "/FolderDisks.png";
        fiPreview = QFileInfo(imagePath);
    }
    if (!fiPreview.exists())
    {
        // 3. load built-in image of a 5 1/2-inch floppy disk
        if (_isSideA)
            imagePath = FLOPPY_INDEXED_PNG;
        else if (_isSideB)
            imagePath = FLOPPY_BACKSIDE_PNG;
        else
            imagePath = FLOPPY_336x224_PNG;
    }
    else
    {
        _title.clear();
        _diskNo.clear();
    }

    setPicPath(imagePath);  // load the thumbnail/preview into the pixmap

    if (isFloppyPng() && _diskErr)
        QLabel::setText("?");

    update();
}

void PicLabel::setDiskError(bool error)
{
    _diskErr = error;
}

void PicLabel::setPicPath(const QString& picPath)
{
    if (_picPath == picPath)
        return;

    _picPath = picPath;

    if (_pixmap)
        delete _pixmap;

    _pixmap = new QPixmap(_picPath);

    Q_ASSERT(_pixmap && !_pixmap->isNull());
}

void PicLabel::setText(const QString& text)
{
    clear();
    _title.clear();
    _diskNo.clear();

    QRegularExpression re("(^\\d+)( )(.*)");
    auto rem = re.match(text); 
    _isSideA = rem.hasMatch();
    _isSideB = false;

    if (!_isSideA)
    {
        re.setPattern("(^\\d+)([b|B] )(.*)");
        rem = re.match(text);
        _isSideB = rem.hasMatch();
    }
    if (rem.hasMatch())
    {
        _diskNo.setText(rem.captured(1));
        _title.setText(rem.captured(3));
    }
    if (_title.isEmpty())
        _title.setText(text);
}

QRect PicLabel::scaleRect(const QRectF& rect, const QSizeF& szChild, const QSizeF& szFrame)
{
    int X = (int)round(rect.x() * szChild.width() / szFrame.width());
    int Y = (int)round(rect.y() * szChild.height() / szFrame.height());
    int W = (int)round(rect.width() * szChild.width() / szFrame.width());
    int H = (int)round(rect.height() * szChild.height() / szFrame.height());

    return QRect {QPoint{X,Y}, QSize{W,H}};
}

void PicLabel::moveLabels()
{
    bool useSmallLabel = !(_isSideA || _isSideB) && (_title.text().length() < 20);

    QSizeF szPic = (_pixmap ? _pixmap->size() : QSizeF(336,224));

    const int lblPtX = useSmallLabel ? 150 : 117;
    const int lblSzW = useSmallLabel ? 110 : 140;
    const QRectF LABEL_RECT {QPointF{lblPtX,25}, QSizeF{lblSzW,48}};
    const QRect scaledRect = scaleRect(LABEL_RECT, size(), QSizeF{szPic.width(),szPic.height()});

    _title.setGeometry(scaledRect);

    const QRectF INDEX_RECT {QPointF{80,25}, QSizeF{20,20}};
    const QRect indexRect = scaleRect(INDEX_RECT, size(), QSizeF{szPic.width(),szPic.height()});

    _diskNo.setGeometry(indexRect);
}

void PicLabel::scaleFonts()
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

void PicLabel::update()
{
    moveLabels();
    scaleFonts();
    QLabel::update();
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
    update();
}

QSize PicLabel::sizeHint() const
{
    if (_pixmap == nullptr)
        return QSize(200,130);  // TBD: why isn't this 1:1 with pixel ruler? (on a 4k monitor)

    return QSize(_pixmap->width(), _pixmap->height());
}

Title::Title(QWidget* parent) : QLabel(parent)
{
    QFont font("Ink Free");
    font.setPointSize(12);
    setFont(font);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setWordWrap(true);
}

DiskNo::DiskNo(QWidget* parent) : QLabel(parent)
{
    QFont font("Courier New");
    font.setPointSize(12);
    setFont(font);
}
