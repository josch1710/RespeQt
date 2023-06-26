#ifndef PICLABEL_H
#define PICLABEL_H

#include <QLabel>
#include <QString>
#include <QPixmap>

class DiskNo : public QLabel
{
public:
    DiskNo(QWidget* parent);
    bool isEmpty() { return text().isEmpty(); }
};

class Title : public QLabel
{
public:
    Title(QWidget* parent);
    bool isEmpty() { return text().isEmpty(); }
    void setText(const QString& text);
    void setLineHeight(int height);

private:
    int _lineHeight = 70;
    QString _fontColor {"black"};
#if defined Q_OS_WIN
    const QString _fontFamily {"Ink Free"};
#elif defined Q_OS_MAC
    const QString _fontFamily {"Marker Felt"};
#else
    const QString _fontFamily {"Comic Sans"};
#endif
};

class PicLabel : public QLabel
{
public:
    PicLabel(QWidget* parent);
    virtual ~PicLabel();

    void setDiskName(const QString& fileName);
    double ratio();
    void clear();

    virtual QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    QString  _diskName;
    QString  _picPath;
    QString  _picTooltip;
    QPixmap* _pixmap  {nullptr};
    Title    _title   {this};
    DiskNo   _diskNo  {this};
    bool     _isSideA {false};
    bool     _isSideB {false};

    const QString FLOPPY_DEFAULT_PNG  {":/icons/other-icons/floppy.png"};
    const QString FLOPPY_INDEXED_PNG  {":/icons/other-icons/floppy_front.png"};
    const QString FLOPPY_BACKSIDE_PNG {":/icons/other-icons/floppy_back.png"};

    void loadPixmap(const QString& picPath);
    void parseName();
    QString findImage();
    void moveLabels();
    void scaleFonts();
    void update();
    QRect scaleRect(const QRectF& rect, const QSizeF& szChild, const QSizeF& szFrame);
};

#endif // PICLABEL_H
