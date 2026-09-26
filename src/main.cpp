/*
 * main.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "ui/mainwindow.h"
#ifdef HAS_QT_DBUS
#include "ui/portalthemelistener.h"
#endif
#include <QApplication>
#include <QTextCodec>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN
  timeBeginPeriod(1);
#endif
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication a(argc, argv);
#ifdef HAS_QT_DBUS
  UI::PortalThemeListener::init(&a);
#endif
  QIcon::setThemeName("Classic");
  QApplication::setFont(QApplication::font("QMenu"));
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  const auto w {new UI::MainWindow};
  w->show();
  const auto ret {QApplication::exec()};
#ifdef Q_OS_WIN
  timeEndPeriod(1);
#endif
  return ret;
}
