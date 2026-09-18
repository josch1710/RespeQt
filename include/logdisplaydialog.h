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

constexpr int maxLogSize = 1 * 1024 * 1024; // 1 MB max size.

namespace Ui {
  class LogDisplayDialog;
}

class LogDisplayDialog : public QDialog {
  Q_OBJECT

public:
  explicit LogDisplayDialog(QWidget *parent = nullptr);
  ~LogDisplayDialog() override;

protected:
  void changeEvent(QEvent *e) override;

private:
  Ui::LogDisplayDialog *l_ui;
  QString savedLog, filter;

public slots:
  void setLogText(const QString& logText);
  void addLogTextChange(const QString& logChange);

private slots:
  void diskFilter();
  void onClick(QAbstractButton *button);
};
#endif// LOGDISPLAYDIALOG_H
