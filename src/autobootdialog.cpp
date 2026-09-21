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

extern QString g_exefileName;
static bool reload;

[[maybe_unused]] AutoBootDialog::AutoBootDialog(QWidget *parent) : QDialog(parent),
                                                  ui(new Ui::AutoBootDialog) {
  ui->setupUi(this);
  ui->progressBar->setVisible(false);
  connect(ui->reloadButton, &QPushButton::clicked, this, &AutoBootDialog::reject);
  connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &AutoBootDialog::close);
  connect(ui->reloadButton, &QPushButton::clicked, this, &AutoBootDialog::reloadExe);
  reload = false;
  ui->progressBar->setVisible(true);
}

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

void AutoBootDialog::booterStarted() const
{
  ui->label->setText(tr("Atari is loading the booter."));
}

void AutoBootDialog::booterLoaded() const
{
  ui->label->setText(tr("Atari is loading the program.\n\nFor some programs you may have to close this dialog manually when the program starts."));
}

void AutoBootDialog::blockRead(const int current, const int all) const
{
  ui->progressBar->setMaximum(all);
  ui->progressBar->setValue(current);
}

void AutoBootDialog::loaderDone() {
  accept();
}

// TODO Is it used?
// ReSharper disable once CppMemberFunctionMayBeStatic
[[maybe_unused]] void AutoBootDialog::onClick(const QAbstractButton *button) { // NOLINT(*-convert-member-functions-to-static)
  if (button->text() == "Cancel") {
    g_exefileName = "";
  }
}

void AutoBootDialog::reloadExe() {
  reload = true;
  close();
}
