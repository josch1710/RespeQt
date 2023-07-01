#ifndef PICLABEL_H
#define PICLABEL_H

#include <QLabel>
#include <QString>
#include <QPixmap>
#include <QTextEdit>

class DiskNo : public QLabel
{
public:
    DiskNo(QWidget* parent);
    bool isEmpty() { return text().isEmpty(); }
};

class Title : public QTextEdit
{
public:
    Title(QWidget* parent);
    void setText(const QString& text);
    void setLineHeight(int height);

private:
    QString _fontColor {"black"};
#if defined Q_OS_WIN
    const QString _fontFamily {"Ink Free"};
    bool _fontIsBold = true;
    int _lineHeight = 70;
#elif defined Q_OS_MAC
    const QString _fontFamily {"Bradley Hand"};
    bool _fontIsBold = false;
    int _lineHeight = 65;
#else
    const QString _fontFamily {"Comic Sans"};
    bool _fontIsBold = false;
    int _lineHeight = 70;
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
    QRect padRect();
    QRect scaleRect(const QRectF& rect, const QRectF& rcChild);
};

#endif // PICLABEL_H
