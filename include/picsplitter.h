#ifndef PICSPLITTER_H
#define PICSPLITTER_H

#include <QSplitter>

class PicSplitter : public QSplitter
{
public:
    PicSplitter(QWidget* parent);

    void resizeEvent(QResizeEvent* event) override;
};

#endif // PICSPLITTER_H
