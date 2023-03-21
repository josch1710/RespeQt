/*
 * logdisplaydialog.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef LOGDISPLAYDIALOG_H
#define LOGDISPLAYDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QAbstractButton>
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
    void closeEvent(QCloseEvent *);

private:
    Ui::LogDisplayDialog *l_ui;

public slots:
    void getLogText(QString logText);
    void getLogTextChange(QString logChange);

private slots:
    void diskFilter();
    void onClick(QAbstractButton* button);

signals:
};
#endif // LOGDISPLAYDIALOG_H
