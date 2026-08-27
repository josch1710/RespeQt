/*
 * uiscale.h
 *
 * Sizing helpers that derive from the active style instead of hardcoded pixel
 * values, so the GUI follows the platform's icon size and style settings.
 */

#ifndef UISCALE_H
#define UISCALE_H

#include <QAbstractButton>
#include <QApplication>
#include <QFont>
#include <QSize>
#include <QStyle>
#include <QToolButton>
#include <QWidget>

namespace UiScale {

  // The active style's own small icon size: 16 on the common desktop styles,
  // larger on styles built for touch or high density output.
  inline int iconExtent(const QWidget *reference) {
    return reference->style()->pixelMetric(QStyle::PM_SmallIconSize, nullptr, reference);
  }

  // Gives every tool button below root an explicit icon size so that its own
  // sizeHint() can size the box. The forms used to pin minimumSize ==
  // maximumSize and never set an icon size at all, which left the box and the
  // icon inside it free to drift apart whenever the style changed.
  inline void applyToolButtonIconSizes(QWidget *root) {
    const int extent = iconExtent(root);
    for (QToolButton *button : root->findChildren<QToolButton *>()) {
      button->setIconSize(QSize(extent, extent));
    }
  }

  // Squares off a button that carries a single glyph or icon instead of a
  // label. The forms pinned these to 22 or 35 pixels, but a push button asks
  // the style how tall it has to be for the current font -- 32 pixels under
  // QMacStyle at 13 pt -- and refuses to shrink below that, so the pin cut the
  // glyph off. Ask for that height and use it for both sides.
  inline void applySquareSize(QAbstractButton *button) {
    const int extent = button->sizeHint().height();
    button->setFixedSize(extent, extent);
  }

  // A font size in points that follows the system font. A default constructed
  // QFont carries no size of its own -- pointSize() stays -1 until something
  // sets one -- so anything that has to write a concrete number needs a source
  // for it. The literal is only the last resort for a system font that is
  // itself specified in pixels, in which case pointSize() is -1 as well.
  inline int defaultPointSize() {
    const int points = QApplication::font().pointSize();
    return points > 0 ? points : 10;
  }

}// namespace UiScale

#endif// UISCALE_H
