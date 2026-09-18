/*
 * autobootdialog.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef AUTOBOOTDIALOG_H
#define AUTOBOOTDIALOG_H

#include <QAbstractButton>
#include <QDialog>

namespace Ui {
  class AutoBootDialog;
}

class AutoBootDialog : public QDialog {
  Q_OBJECT

public:
  explicit AutoBootDialog(QWidget *parent = nullptr);
  ~AutoBootDialog() override;


protected:
  void changeEvent(QEvent *e) override;
  void closeEvent(QCloseEvent *) override;

private:
  Ui::AutoBootDialog *ui;

public slots:
  void booterStarted() const;
  void booterLoaded() const;
  void blockRead(int current, int all) const;
  void loaderDone();

private slots:
  [[maybe_unused]] void onClick(const QAbstractButton *button);
  void reloadExe();
};

#endif// AUTOBOOTDIALOG_H
