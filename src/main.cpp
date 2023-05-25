/*
 * main.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "mainwindow.h"
#include "RespeQtApplication.h"
#include <QTextCodec>
#include <memory>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN
  timeBeginPeriod(1);
#endif
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  RespeQtApplication a(argc, argv);

  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  auto w = new MainWindow;
  w->show();
  auto ret = a.exec();
#ifdef Q_OS_WIN
  timeEndPeriod(1);
#endif
  return ret;
}
