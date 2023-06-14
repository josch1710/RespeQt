#ifndef PICLABEL_H
#define PICLABEL_H

#include <QLabel>
#include <QString>
#include <QPixmap>

class PicLabel : public QLabel
{
public:
    PicLabel(QWidget* parent);
    virtual ~PicLabel();

    void setPicPath(const QString& picPath);
    void setText(const QString& text, bool center = true);

    double ratio();

    virtual QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    QString  _picPath;
    QPixmap* _pixmap    = nullptr;
    QLabel*  _lblText   = nullptr;
    QLabel*  _lblNoText = nullptr;

    void moveLabels();
    void scaleFonts();
    void update();
};

#endif // PICLABEL_H
