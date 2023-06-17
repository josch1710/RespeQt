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
};

class PicLabel : public QLabel
{
public:
    PicLabel(QWidget* parent);
    virtual ~PicLabel();

    void setDiskName(const QString& diskName);
    void setPicPath(const QString& picPath);
    void setText(const QString& text);
    void setDiskError(bool error = true);

    double ratio();

    virtual QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    QString  _picPath;
    QPixmap* _pixmap  {nullptr};
    Title    _title   {this};
    DiskNo   _diskNo  {this};
    bool     _diskErr {false};
    bool     _isSideA {false};
    bool     _isSideB {false};

    const QString FLOPPY_336x224_PNG  {":/icons/other-icons/floppy336x224.png"};
    const QString FLOPPY_INDEXED_PNG  {":/icons/other-icons/floppy336x224-no.png"};
    const QString FLOPPY_BACKSIDE_PNG {":/icons/other-icons/floppy336x224-back-no.png"};

    bool isFloppyPng();

    void moveLabels();
    void scaleFonts();
    void update();

    QRect scaleRect(const QRectF& rect, const QSizeF& szChild, const QSizeF& szFrame);
};

#endif // PICLABEL_H
