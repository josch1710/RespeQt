#include "include/picsplitter.h"

PicSplitter::PicSplitter(QWidget* parent) : QSplitter(parent)
{

}

void PicSplitter::resizeEvent(QResizeEvent* event)
{
    QSplitter::resizeEvent(event);
}
