/*
 * main.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <memory>
#include <QStyleFactory>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN
  timeBeginPeriod(1);
#endif
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
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
