#ifndef GRAPHICSPRIMITIVE_H
#define GRAPHICSPRIMITIVE_H

#include <QFont>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPoint>
#include <QSharedPointer>
#include <vector>
#include <iostream>


namespace Printers
{
    class _GraphicsPrimitive final: public QObject
    {
        Q_OBJECT;
    public:
        _GraphicsPrimitive() = default;
        ~_GraphicsPrimitive() = default;

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

