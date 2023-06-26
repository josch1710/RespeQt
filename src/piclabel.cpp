#include "include/piclabel.h"
#include <QPaintEvent>
#include <QPainter>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImageReader>


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

void PicLabel::clear()
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

void PicLabel::setDiskName(const QString& fileName)
{
    if ((_diskName == fileName) || !QFileInfo::exists(fileName))
        return;

    _diskName = fileName;

    parseName();    // splits index | b-side | title (TBD: re-think side effects)

    QString picPath = findImage();  // find a custom or built-in pic
    loadPixmap(picPath);            // load the pixmap from the new path name/resource

    if (picPath[0] != ':')          // custom pic?...
    {
        _diskNo.clear();            // no number overlay
        _title.clear();             // no title overlay
    }

    update();
}

void PicLabel::loadPixmap(const QString& picPath)
{
    if (_picPath == picPath)
        return;

    _picPath = picPath;

    if (_pixmap)
        delete _pixmap;

    _pixmap = new QPixmap(_picPath);

    Q_ASSERT(_pixmap && !_pixmap->isNull());
}

void PicLabel::parseName()
{
    QLabel::clear();                // clear parent label text (not used)
    _title.clear();                 // side-effect #1 is to set this
    _diskNo.clear();                // side-effect #2 is to set this
    _isSideA = _isSideB = false;    // either of these implies the disk is number indexed

    auto fileInfo = QFileInfo {_diskName};

    Q_ASSERT(fileInfo.exists());    // validated prior to this call

    QString baseName = fileInfo.completeBaseName();

    static QRegularExpression re("(^\\d+)([b|B]?)(\\.?)(.*)");
    auto rem = re.match(baseName);

    if (rem.hasMatch())
    {
        _diskNo.setText(rem.captured(1));
        _title.setText(rem.captured(4));

        _isSideB = rem.captured(2).toUpper() == "B";
        _isSideA = !_isSideB;
    }
    else
    {
        _title.setText(baseName);
    }
}

static QStringList toFileTypes(const QList<QByteArray>& list)
{
    QStringList strings;

    foreach (const QByteArray& item, list)
    {
        QString fileSpec = "*." + QString::fromLocal8Bit(item);
        strings.append(fileSpec);
    }

    return strings;
}

QString PicLabel::findImage()
{
    auto fileInfo = QFileInfo {_diskName};
    auto diskBase = fileInfo.completeBaseName();
    QDir dir {fileInfo.absolutePath()};
    auto formats = QImageReader::supportedImageFormats();
    auto entries = dir.entryInfoList(toFileTypes(formats));
    auto bsidexp = _isSideB ? QString("[b|B]") : QString();
    auto sregexp = QString("^(%1)(%2)(\\.)(.*)").arg(_diskNo.text(), bsidexp);
    auto qregexp = QRegularExpression {sregexp};

    foreach (const QFileInfo& entry, entries)
    {
        // 1. check for basename with viable image extension

        const QString entryName {entry.completeBaseName()};
        if (entryName == diskBase)
        {
            setToolTip(entryName);
            return entry.absoluteFilePath();
        }

        // 2. check for same index prefixed image file in the disk folder
        // ex: disk name = 12b.Title of Disk.ATR
        //     image file = 12b.Menu Screen.PNG
        // or: image file = 12b.PNG

        if (!_diskNo.isEmpty())     // check if current disk has index prefix NN. or NNb.
        {
            auto basename = entry.completeBaseName();
            auto matcher = qregexp.match(basename);
            if (matcher.hasMatch())
            {
                QString tip = matcher.captured(4);  // use mid string as tooltip
                setToolTip(tip);
                return entry.absoluteFilePath();
            }
        }

        // 3. use generic name for default thumbnail

        auto imagePath = fileInfo.path() + "/respeqt_db." + entry.suffix();
        if (QFileInfo::exists(imagePath))
        {
            setToolTip("");
            return imagePath;
        }
    }

    // 3. load built-in image of a 5 1/2-inch floppy disk

    if (_isSideA)
        return FLOPPY_INDEXED_PNG;  // has 2 labels: (small) disk no./index & (large) title/content
    if (_isSideB)
        return FLOPPY_BACKSIDE_PNG; // same 2 labels but flip-side of double-sided floppy

    return FLOPPY_DEFAULT_PNG;      // used if all else fails
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

    const QSizeF szPic {_pixmap->size()};
    const QRect scaledRect = scaleRect(labelRect, size(), szPic);

    _title.setGeometry(scaledRect);

    if (_isSideA || _isSideB)
    {
        // move the index rect
        const double indexX = (_isSideA ? 25 : 175);
        const QRectF INDEX_RECT {QPointF{indexX,25}, QSizeF{20,20}};
        const QRect  indexRect = scaleRect(INDEX_RECT, size(), szPic);

        _diskNo.setGeometry(indexRect);
        _diskNo.setVisible(true);
    }
    else
        _diskNo.setVisible(false);
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
    double aspectRatio = 0.0;

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
    QString fmt
    {
        "color: %1;"
        "font-family: \"%2\";"
        "font-weight: bold;"
    };

    QString style = fmt.arg(_fontColor, _fontFamily);
    setStyleSheet(style);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setWordWrap(true);
//  setFrameStyle(QFrame::Box);

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
    QString html
    {
        "<html><head/><body>"
        "<p style=\"line-height:%1\">"
        "<span>%2</span></p>"
        "</body></html>"
    };

    QString fmtStr = html.arg(_lineHeight).arg(text);
    QLabel::setText(fmtStr);
}

DiskNo::DiskNo(QWidget* parent) : QLabel(parent)
{
    setStyleSheet("color: black; font-family: \"Courier New\"");
}
