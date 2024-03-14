/*
 * main.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "mainwindow.h"
#include "respeqtapplication.h"
#include <QTextCodec>
#include <memory>
#include <QStyleFactory>

#if defined(Q_OS_LINUX) && defined(QT_X11EXTRAS_LIB)
#include <QX11Info>
#endif
#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN
  timeBeginPeriod(1);
#endif
#if false && defined(Q_OS_LINUX) && defined(QT_X11EXTRAS_LIB)
  auto dpi{QX11Info::appDpiX()};

  if (dpi != 0 && dpi > 90)
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#else
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  QApplication a(argc, argv);
  a.setFont(a.font("QMenu"));
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  auto w = new MainWindow;
  w->show();
  auto ret = a.exec();
#ifdef Q_OS_WIN
  timeEndPeriod(1);
#endif
  return ret;
}
