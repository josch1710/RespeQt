/*
 * autobootdialog.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "autobootdialog.h"
#include "mainwindow.h"
#include "ui_autobootdialog.h"

#include <QTime>

extern QString g_exefileName;
bool reload;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
AutoBootDialog::AutoBootDialog(QWidget *parent) : QDialog(parent),
                                                  ui(new Ui::AutoBootDialog) {
  ui->setupUi(this);
  ui->progressBar->setVisible(false);
  connect(ui->reloadButton, &QPushButton::clicked, this, &AutoBootDialog::reject);
  connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &AutoBootDialog::close);
  connect(ui->reloadButton, &QPushButton::clicked, this, &AutoBootDialog::reloadExe);
  reload = false;
  ui->progressBar->setVisible(true);
}
#pragma clang diagnostic pop

AutoBootDialog::~AutoBootDialog() {
  delete ui;
}

void AutoBootDialog::changeEvent(QEvent *e) {
  QDialog::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

void AutoBootDialog::closeEvent(QCloseEvent *) {
  if (!reload) g_exefileName = "";
}

void AutoBootDialog::booterStarted() {
  ui->label->setText(tr("Atari is loading the booter."));
}

void AutoBootDialog::booterLoaded() {
  ui->label->setText(tr("Atari is loading the program.\n\nFor some programs you may have to close this dialog manually when the program starts."));
}

void AutoBootDialog::blockRead(int current, int all) {
  ui->progressBar->setMaximum(all);
  ui->progressBar->setValue(current);
}

void AutoBootDialog::loaderDone() {
  accept();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
// TODO Is it used?
void AutoBootDialog::onClick(QAbstractButton *button) {
  if (button->text() == "Cancel") {
    g_exefileName = "";
    return;
  }
}
#pragma clang diagnostic pop

void AutoBootDialog::reloadExe() {
  reload = true;
  close();
}
