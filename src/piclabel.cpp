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

void PicLabel::setDiskName(const QString& fileName)
{
    if ((_diskName == fileName) || !QFileInfo(fileName).exists())
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
    clear();                        // clear parent label text (not used)
    _title.clear();
    _diskNo.clear();
    _isSideA = _isSideB = false;    // either of these implies the disk is number indexed

    auto fileInfo = QFileInfo {_diskName};

    Q_ASSERT(fileInfo.exists());    // validated prior to this call

    QString baseName = fileInfo.completeBaseName();

    QRegularExpression re("(^\\d+)([b|B]?)(\\.?)(.*)");
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
    auto pathName = fileInfo.absolutePath();

    // 1. check for same index prefixed image file in the disk folder
    // ex: disk name = 12b.Title of Disk.ATR
    //     image file = 12b.Menu Screen.PNG (TBD: use mid string as tooltip?)
    // or: image file = 12b.PNG

    if (!_diskNo.isEmpty())     // check if current disk has index prefix NN. or NNb.
    {
        QDir dir {pathName};
        auto formats = QImageReader::supportedImageFormats();
        auto entries = dir.entryList(toFileTypes(formats));
        auto bsidexp = _isSideB ? QString("[b|B]") : QString();
        auto sregexp = QString("^%1%2\\..*").arg(_diskNo.text()).arg(bsidexp);
        auto qregexp = QRegularExpression {sregexp};

        for (QString entry : entries)
        {
            auto matcher = qregexp.match(entry);
            if (matcher.hasMatch())
                return pathName + "/" + entry;
        }
    }

    // 2. use generic name for default thumbnail

    auto imagePath = pathName + "/FolderDisks.png"; // TBD: support any image type
    auto fiPreview = QFileInfo(imagePath);

    if (fiPreview.exists())
        return imagePath;

    // 3. load built-in image of a 5 1/2-inch floppy disk

    if (_isSideA)
        return FLOPPY_INDEXED_PNG;  // has 2 labels: (small) disk no./index & (large) title/content
    if (_isSideB)
        return FLOPPY_BACKSIDE_PNG; // same 2 labels but flip-side of double-sided floppy

    return FLOPPY_336x224_PNG;      // used if all else fails
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
    bool useSmallLabel = !(_isSideA || _isSideB); // && (_title.text().length() < 20);

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
//  setFrameStyle(QFrame::Box);
}

DiskNo::DiskNo(QWidget* parent) : QLabel(parent)
{
    QFont font("Courier New");
    font.setPointSize(12);
    setFont(font);
}
