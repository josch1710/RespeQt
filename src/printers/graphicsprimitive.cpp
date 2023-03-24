#include "printers/graphicsprimitive.h"
#include "logdisplaydialog.h"
#include "printers/atariprinter.h"

namespace Printers {
  GraphicsClearPane::GraphicsClearPane()
      : GraphicsPrimitive() {
  }

  void GraphicsClearPane::execute(QGraphicsScene *graphicsScene) {
    graphicsScene->clear();
  }

  GraphicsSetPoint::GraphicsSetPoint(const QPoint point, const QPen pen) : mPoint(point),
                                                                           mPen(pen) {
  }

  GraphicsDrawLine::GraphicsDrawLine(const QPoint srcPoint, const QPen pen, const QPoint destPoint)
      : GraphicsSetPoint(srcPoint, pen),
        mDestPoint(destPoint) {
  }

  void GraphicsDrawLine::execute(QGraphicsScene *graphicsScene) {
    graphicsScene->addLine(mPoint.x(), -mPoint.y(), mDestPoint.x(), -mDestPoint.y(), mPen);
  }

  GraphicsDrawText::GraphicsDrawText(const QPoint point, const QPen pen, const int orientation, const QFont font, QString text)
      : GraphicsSetPoint(point, pen),
        mOrientation(orientation),
        mFont(font),
        mText(text) {
  }

  void GraphicsDrawText::execute(QGraphicsScene *graphicsScene) {
    QGraphicsTextItem *text = graphicsScene->addText(mText, mFont);
    text->setDefaultTextColor(mPen.color());
    text->setRotation(mOrientation);
    QPoint adjusted = computeTextCoordinates();
    text->setPos(adjusted.x(), adjusted.y());
  }

  QPoint GraphicsDrawText::computeTextCoordinates() {
    // fix position because (0,0) on Windows/Linux is top left but on the 1020 it is bottom left.
    QPoint adjusted(mPoint.x(), -mPoint.y());
    QFontMetrics metrics(mFont);
    switch (mOrientation) {

      case 0:
        adjusted.setY(adjusted.y() - metrics.height());
        break;

      case 90:
        adjusted.setX(adjusted.x() + metrics.height());
        break;

      case 180:
        adjusted.setY(adjusted.y() + metrics.height());
        break;

      case 270:
        adjusted.setY(adjusted.x() - metrics.height());
        break;
    }

    return adjusted;
  }
}// namespace Printers
