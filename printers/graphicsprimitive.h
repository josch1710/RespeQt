#ifndef GRAPHICSPRIMITIVE_H
#define GRAPHICSPRIMITIVE_H

#include <QFont>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPoint>
#include <vector>
#include <iostream>

using QPainterPtr = std::shared_ptr<QPainter>;

namespace Printers
{
    class GraphicsPrimitive final: public QObject
    {
        Q_OBJECT;
    public:
        GraphicsPrimitive() = default;
        ~GraphicsPrimitive() = default;

        void executeQueue(QGraphicsScene *);
        void addItem(QGraphicsItem*);
        void clearScene();
        void setTransform(QTransform transform);

        static QString typeName()
        {
            return "GraphicsPrimitive";
        }

    protected:
        std::vector<QGraphicsItem*> mGraphicsItems{};
        QPoint mPoint;
        int mColor;
        QTransform mTransform;

    protected slots:
        void changed(const QList<QRectF> &)
            {
                std::cout << "changed\n";
            }
    };
}
#endif // GRAPHICSPRIMITIVE_H

