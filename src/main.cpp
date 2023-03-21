/*
 * main.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include <QApplication>
#include <QTextCodec>
#include <QLibraryInfo>
#include <memory>
#include "mainwindow.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <Mmsystem.h>
#endif

int main(int argc, char *argv[])
{
    int ret = 0;
#ifdef Q_OS_WIN
    timeBeginPeriod(1);
#endif
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    auto w = new MainWindow;
    w->show();
    ret = a.exec();
#ifdef Q_OS_WIN
    timeEndPeriod(1);
#endif
    return ret;
}
