/*
 * bootoptionsdialog.h
 *
 * Copyright 2017 blind
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef BOOTOPTIONSDIALOG_H
#define BOOTOPTIONSDIALOG_H

#include <QDebug>
#include <QDialog>

namespace Ui {
  class BootOptionsDialog;
}

namespace UI {
  class BootOptionsDialog : public QDialog {
    Q_OBJECT

  public:
    explicit BootOptionsDialog(const QString &bootFolderPath, QWidget *parent = nullptr);
    ~BootOptionsDialog() override;

  protected:
    void changeEvent(QEvent *e) override;

  private:
    const QString &bootFolderPath_;
    ::Ui::BootOptionsDialog *m_ui;

  public slots:
    void accept() override;

  private slots:
    void picoDOSToggled() const;
  };
} // namespace UI
#endif// BOOTOPTIONSDIALOG_H
