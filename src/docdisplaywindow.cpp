/*
 * docdisplaywindow.cpp
 *
 * Copyright 2015 Joseph Zatarski
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "docdisplaywindow.h"
#include "respeqtsettings.h"
#include "ui_docdisplaywindow.h"

#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>

DocDisplayWindow::DocDisplayWindow(QWidget *parent) : QMainWindow(parent),
                                                      ui(new Ui::DocDisplayWindow) {
  ui->setupUi(this);
  QFont f;
  f.setFixedPitch(true);
  f.setFamily("monospace");
  ui->docDisplay->setFont(f);

  connect(ui->actionPrint, &QAction::triggered, this, &DocDisplayWindow::printTriggered);
}

DocDisplayWindow::~DocDisplayWindow() {
  delete ui;
}

void DocDisplayWindow::changeEvent(QEvent *e) {
  QMainWindow::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

void DocDisplayWindow::closeEvent(QCloseEvent *e) {
  emit closed();
  e->accept();
}

void DocDisplayWindow::printTriggered() {
  QPrinter printer;
  auto *dialog = new QPrintDialog(&printer, this);
  if (dialog->exec() != QDialog::Accepted)
    return;
  ui->docDisplay->print(&printer);
}
