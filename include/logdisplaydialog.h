/*
 * logdisplaydialog.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef LOGDISPLAYDIALOG_H
#define LOGDISPLAYDIALOG_H

#include <QAbstractButton>
#include <QDebug>
#include <QDialog>
#include <QTextEdit>

namespace Ui {
  class LogDisplayDialog;
}

class LogDisplayDialog : public QDialog {
  Q_OBJECT

public:
  LogDisplayDialog(QWidget *parent = 0);
  ~LogDisplayDialog();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::LogDisplayDialog *l_ui;
  QString savedLog, filter;

public slots:
  void setLogText(QString logText);
  void addLogTextChange(QString logChange);

private slots:
  void diskFilter();
  void onClick(QAbstractButton *button);
};
#endif// LOGDISPLAYDIALOG_H
