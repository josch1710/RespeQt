/*
 * uicolors.h
 *
 * Colour helpers that derive from the active palette instead of naming literal
 * colours, so the GUI stays readable on a light and on a dark palette alike.
 */

#ifndef UICOLORS_H
#define UICOLORS_H

#include <QColor>
#include <QPalette>
#include <QWidget>

namespace UiColors {

  // Everything below the middle of the lightness range counts as a dark
  // palette. Only the window background is looked at, since that is what the
  // text has to stand out against.
  inline bool isDark(const QWidget *reference) {
    return reference->palette().color(QPalette::Window).lightness() < 128;
  }

  // Mixes fg into bg, ignoring gamma -- close enough for muting text.
  inline QColor blend(const QColor &fg, const QColor &bg, int percent) {
    const int rest = 100 - percent;
    return QColor((fg.red() * percent + bg.red() * rest) / 100,
                  (fg.green() * percent + bg.green() * rest) / 100,
                  (fg.blue() * percent + bg.blue() * rest) / 100);
  }

  // The colour for secondary text such as the drive number, the image details
  // or a hint below an option. The forms named fixed greys between #686868 and
  // #808080 for those, which drop to about 3:1 on a dark window. 60% of the
  // text colour over the window colour stays above the 4.5:1 that text needs
  // on every palette tried: 5.5:1 on a light window, 5.0:1 on a dark one.
  inline QColor mutedText(const QWidget *reference) {
    const QPalette pal = reference->palette();
    return blend(pal.color(QPalette::WindowText), pal.color(QPalette::Window), 60);
  }

  // QMacStyle, like the common style, takes the text colour of both labels and
  // radio buttons from QPalette::WindowText; ButtonText and Text have no effect
  // there. Going through the palette rather than a style sheet also leaves the
  // native rendering of, say, a radio indicator untouched.
  inline void setTextColor(QWidget *widget, const QColor &color) {
    QPalette pal = widget->palette();
    pal.setColor(QPalette::WindowText, color);
    widget->setPalette(pal);
  }

  inline void setButtonColor(QWidget *widget, const QColor &color) {
    QPalette pal = widget->palette();
    pal.setColor(QPalette::ButtonText, color);
    widget->setPalette(pal);
  }

  inline void setBaseColor(QWidget *widget, const QColor &color) {
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Base, color);
    widget->setPalette(pal);
  }

}// namespace UiColors

#endif// UICOLORS_H
