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
#include <QImageReader>
#include <QMenu>


PicPreview::PicPreview(QWidget* parent) : QLabel(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &PicPreview::customContextMenuRequested, this, &PicPreview::popupMenuReq);
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

void PicPreview::setDiskName(const QString& fileName)
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

void PicPreview::parseName()
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

// findImage() is my first pass at a scheme for preview pics. The pics/images are placed in the folder along side disk images.
//  (note: an "image file" in this context is a digital picture, not an ATR disk!)
// A complex, yet flexible naming convention allows the app to grab corresponding pics and labeling data (for rendering on a blank/built in floppy pic)
// and is organized and prioritized as follows:
// 1. Basename with viable image extension.
//    ex: DiskBaseName.png or *.jpg, etc... any Qt supported format can be used.
// 2. Matching index prefixed image file in the disk folder. Note index B designates the reverse side of a floppy disk.
//    ex: disk name = 12b.Title of Disk.ATR ("Title of Disk" appears on the mock label if no PNG present)
//        image file = 12b.Menu Screen.PNG (screen shot of a disk that boots to a game menu)
//    or: image file = 12b.PNG
// 3. Use the hard-coded/generic name respeqt_db.* for default thumbnail.
//    ex: respeqt_db.png
// 4. Default to loading a built-in image of a 5 1/2-inch floppy disk
//
QString PicPreview::findImage()
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

        // 2. check for matching indexing filename prefix

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

    // 4. TBD: check INI file scheme for an image to load

    // 5. load built-in image of a 5 1/2-inch floppy disk

    if (_isSideA)
        return FLOPPY_INDEXED_PNG;  // has 2 labels: (small) disk no./index & (large) title/content
    if (_isSideB)
        return FLOPPY_BACKSIDE_PNG; // same 2 labels but flip-side of double-sided floppy

    return FLOPPY_DEFAULT_PNG;      // used if all else fails
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

void PicPreview::popupMenuReq(const QPoint& pos)
{
    QMenu menu;
    menu.addAction(QIcon(":/icons/silk-icons/icons/image.png"), "Set Preview Default...", this, &PicPreview::actionSetDefault);
    menu.addAction(QIcon(":/icons/silk-icons/icons/image_add.png"), "Set Disk Preview Pic...", this, &PicPreview::actionSetPic);
    menu.addAction(QIcon(":/icons/silk-icons/icons/image_delete.png"), "Clear Preview", this, &PicPreview::actionSetPic);
    menu.addSeparator();
    menu.addAction(QIcon(":/icons/silk-icons/icons/font.png"), "Set Disk Title", this, &PicPreview::actionSetTitle);
    menu.addAction(QIcon(":/icons/silk-icons/icons/text_list_numbers.png"), "Set Disk Index", this, &PicPreview::actionSetIndex);

    menu.exec(mapToGlobal(pos));
}

void PicPreview::actionSetDefault()
{
    QString fname = browsePic();
    if (!fname.isEmpty())
    {
    }
}

void PicPreview::actionSetPic()
{

}

void PicPreview::actionSetTitle()
{

}

void PicPreview::actionSetIndex()
{

}

void PicPreview::actionClear()
{

}

QString PicPreview::browsePic()
{
    return QString();
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
