#ifndef PICSPLITTER_H
#define PICSPLITTER_H

#include <QSplitter>
#include <QLabel>

class PicSplitter : public QSplitter
{
public:
    PicSplitter(QWidget* parent);

    void setOther(QSplitter* other);
    void setRatio(double ratio, bool moveOther = false);

private slots:
    void onSplitterMoved();

private:
    QSplitter* _other = nullptr;
    double     _ratio = 4.0/3.0;    // default 4:3 aspect ratio
};

#endif // PICSPLITTER_H
