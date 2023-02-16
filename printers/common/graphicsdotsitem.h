#ifndef GRAPHICSDOTSITEM_H
#define GRAPHICSDOTSITEM_H

#include <QGraphicsObject>
#include <QObject>
#include <QPainter>

#include <bitset>

namespace Printers {
    namespace Common {

        template <std::size_t N, unsigned char type_n>
        class GraphicsDotsItem : public QGraphicsObject
        {
        public:
            enum { Type = UserType + type_n };

            GraphicsDotsItem(QPointF point, unsigned long val, QGraphicsItem *parent = Q_NULLPTR);

            int type() const override { return Type; }
            virtual QRectF boundingRect() const override;
            virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR) override;


        private:
            std::bitset<N> mDots;
            QPointF mPoint;
        };

        // ---- Implementation -----
        template<std::size_t N, unsigned char type_n>
        GraphicsDotsItem<N, type_n>::GraphicsDotsItem(QPointF point, unsigned long val, QGraphicsItem *parent)
           : QGraphicsObject(parent),
             mDots(val), mPoint(point)
        {}

        template<std::size_t N, unsigned char type_n>
        QRectF GraphicsDotsItem<N, type_n>::boundingRect() const
        {
            QSizeF size(1, N);
            QRectF rect(mPoint, size);
            return rect;
        }

        template<std::size_t N, unsigned char type_n>
        void GraphicsDotsItem<N, type_n>::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget *widget)
        {
            if (!painter || !widget)
                return;

            auto point(mPoint);
            painter->setPen(Qt::black);
            for(std::size_t i = 0; i < mDots.size(); i++) {
                if (mDots[mDots.size() - 1 - i])
                    painter->drawPoint(point);

                point.setY(point.y() + 1);
            }
        }

    }
}
#endif // GRAPHICSDOTSITEM_H
