#ifndef GRAPHICSPRIMITIVE_H
#define GRAPHICSPRIMITIVE_H

#include <QFont>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
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

        static QString typeName()
        {
            return "GraphicsPrimitive";
        }

    protected:
        std::vector<QGraphicsItem*> mGraphicsItems{};
        QPoint mPoint;
        int mColor;

    protected slots:
        void changed(const QList<QRectF> &)
            {
                std::cout << "changed\n";
            }
    };


/*    class GraphicsClearPane : public GraphicsPrimitive
    {
    public:
        GraphicsClearPane();
        virtual ~GraphicsClearPane() = default;

        virtual void execute(QGraphicsScene *graphicsScene) override;

        static QString typeName()
        {
            return "GraphicsClearPane";
        }
    };

    class GraphicsSetPoint : public GraphicsPrimitive
    {
    public:
        GraphicsSetPoint(const QPoint point, const QPen);
        virtual ~GraphicsSetPoint() = default;

        static QString typeName()
        {
            return "GraphicsSetPoint";
        }

    protected:
        QPoint mPoint;
        QPen mPen;
    };

    class GraphicsDrawLine : public GraphicsSetPoint
    {
    public:
        GraphicsDrawLine(const QPoint srcPoint, const QPen pen, const QPoint destPoint);
        virtual ~GraphicsDrawLine() = default;

        virtual void execute(QGraphicsScene *graphicsScene) override;

        static QString typeName()
        {
            return "GraphicsDrawLine";
        }

    protected:
        QPoint mDestPoint;
    };

    class GraphicsDrawText : public GraphicsSetPoint
    {
    public:
        GraphicsDrawText(const QPoint point, const QPen pen, const int orientation, const QFont font, QString text);
        virtual ~GraphicsDrawText() = default;

        virtual void execute(QGraphicsScene *graphicsScene) override;

        static QString typeName()
        {
            return "GraphicsDrawText";
        }

    protected:
        int mOrientation;
        QFont mFont;
        QString mText;

        QPoint computeTextCoordinates();
    }; */
}
#endif // GRAPHICSPRIMITIVE_H

