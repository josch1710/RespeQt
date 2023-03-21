#ifndef GRAPHICSPRIMITIVE_H
#define GRAPHICSPRIMITIVE_H

#include <QFont>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <memory>

using QPainterPtr = std::shared_ptr<QPainter>;

namespace Printers
{
    class GraphicsPrimitive : public QObject
    {
        Q_OBJECT

    public:
        GraphicsPrimitive() = default;
        virtual ~GraphicsPrimitive() = default;

        virtual void execute(QGraphicsScene *graphicsScene) = 0;

        static QString typeName()
        {
            return "GraphicsPrimitive";
        }

    protected:
        QPoint mPoint;
        int mColor;
    };

    class GraphicsClearPane : public GraphicsPrimitive
    {
        Q_OBJECT

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
        Q_OBJECT

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
        Q_OBJECT

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
        Q_OBJECT

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
    };
}
#endif // GRAPHICSPRIMITIVE_H
