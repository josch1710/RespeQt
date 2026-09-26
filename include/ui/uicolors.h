/*
 * uicolors.h
 *
 * Colour helpers that derive from the active palette instead of naming literal
 * colours, so the GUI stays readable on a light and on a dark palette alike.
 */

#ifndef UICOLORS_H
#define UICOLORS_H

#include <QColor>
#include <QGuiApplication>
#include <QPalette>
#include <QWidget>

namespace UI::Colors {

  // Everything below the middle of the lightness range counts as a dark
  // palette. Only the window background is looked at, since that is what the
  // text has to stand out against.
  inline bool isDark(const QWidget *reference = nullptr, const QPalette::ColorRole role = QPalette::Window) {
    const QPalette &pal = reference ? reference->palette() : QGuiApplication::palette();
    return pal.color(role).lightness() < 128;
  }

  // Mixes fg into bg, ignoring gamma -- close enough for muting text.
  inline QColor blend(const QColor &fg, const QColor &bg, const int percent) {
    const int rest = 100 - percent;
    return {(fg.red() * percent + bg.red() * rest) / 100,
                  (fg.green() * percent + bg.green() * rest) / 100,
                  (fg.blue() * percent + bg.blue() * rest) / 100};
  }

  // The colour for secondary text such as the drive number, the image details
  // or a hint below an option. The forms named fixed greys between #686868 and
  // #808080 for those, which drop to about 3:1 on a dark window. 60% of the
  // text colour over the window colour stays above the 4.5:1 that text needs
  // on every palette tried: 5.5:1 on a light window, 5.0:1 on a dark one.
  inline QColor mutedText(const QWidget *reference = nullptr) {
    const QPalette &pal = reference ? reference->palette() : QGuiApplication::palette();
    return blend(pal.color(QPalette::WindowText), pal.color(QPalette::Window), 60);
  }

  // QMacStyle, like the common style, takes the text colour of both labels and
  // radio buttons from QPalette::WindowText; ButtonText and Text have no effect
  // there. Going through the palette rather than a style sheet also leaves the
  // native rendering of, say, a radio indicator untouched.
  inline void setTextColor(QWidget *widget, const QColor &color) {
    if (!widget) return;
    QPalette pal = widget->palette();
    pal.setColor(QPalette::WindowText, color);
    widget->setPalette(pal);
  }

  // QMacStyle, like the common style, takes the highlighted text colour of both labels and
  // radio buttons from QPalette::WindowText; ButtonText and Text have no effect
  // there. Going through the palette rather than a style sheet also leaves the
  // native rendering of, say, a radio indicator untouched.
  inline void setHighlightedTextColor(QWidget *widget, const QColor &color) {
    if (!widget) return;
    QPalette pal = widget->palette();
    pal.setColor(QPalette::HighlightedText, color);
    widget->setPalette(pal);
  }

  inline void setButtonColor(QWidget *widget, const QColor &color) {
    if (!widget) return;
    QPalette pal = widget->palette();
    pal.setColor(QPalette::ButtonText, color);
    widget->setPalette(pal);
  }

  inline void setBaseColor(QWidget *widget, const QColor &color) {
    if (!widget) return;
    QPalette pal = widget->palette();
    pal.setColor(QPalette::Base, color);
    widget->setPalette(pal);
  }

  inline QPalette darkPalette() {
    QPalette darkPal;
    darkPal.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPal.setColor(QPalette::WindowText, Qt::white);
    darkPal.setColor(QPalette::Base, QColor(35, 35, 35));
    darkPal.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPal.setColor(QPalette::ToolTipBase, QColor(25, 25, 25));
    darkPal.setColor(QPalette::ToolTipText, Qt::white);
    darkPal.setColor(QPalette::Text, Qt::white);
    darkPal.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPal.setColor(QPalette::ButtonText, Qt::white);
    darkPal.setColor(QPalette::BrightText, Qt::red);
    darkPal.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPal.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPal.setColor(QPalette::HighlightedText, Qt::white);
    darkPal.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
    darkPal.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    darkPal.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
    darkPal.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));
    return darkPal;
  }

}// namespace UI::Colors

namespace UiColors = UI::Colors;

#endif// UICOLORS_H
