#include "include/picsplitter.h"
#include <QResizeEvent>
#include <QLabel>
#include <QtDebug>

PicSplitter::PicSplitter(QWidget* parent) : QSplitter(parent)
{
    connect(this, &PicSplitter::splitterMoved, this, &PicSplitter::onSplitterMoved);
}

void PicSplitter::setOther(QSplitter* other)
{
    _other = other;
}

void PicSplitter::setRatio(double ratio)
{
    _ratio = ratio;
}

void PicSplitter::onSplitterMoved()
{
    QList<int> otherSizes = _other->sizes();

    int dim1 = sizes().at(1);
    int dim2  = (orientation() == Qt::Horizontal) ? (dim1 / _ratio) : (dim1 * _ratio);
    otherSizes[0] = otherSizes[0] + otherSizes[1] - dim2;
    otherSizes[1] = dim2;

    _other->setSizes(otherSizes);
}
