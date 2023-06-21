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

void PicSplitter::setRatio(double ratio, bool moveOther)
{
    _ratio = ratio;

    if (moveOther && _other)
        onSplitterMoved();
}

void PicSplitter::onSplitterMoved()
{
    QList<int> otherSizes = _other->sizes();

    if (!((otherSizes.size() == 2) && otherSizes[0] && otherSizes[1]))
    {
        //Q_ASSERT(0);  // edge case resize failure
        return;         // program aborted on first run after selecting a disk (?)
    }

    int dim1 = sizes().at(1);
    int dim2  = (orientation() == Qt::Horizontal) ? (dim1 / _ratio) : (dim1 * _ratio);
    otherSizes[0] = otherSizes[0] + otherSizes[1] - dim2;
    otherSizes[1] = dim2;

    _other->setSizes(otherSizes);
}
