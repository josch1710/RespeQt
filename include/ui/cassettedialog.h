/*
 * cassettedialog.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef CASSETTEDIALOG_H
#define CASSETTEDIALOG_H

#include "sio/sioworker.h"
#include <QDialog>
#include <QMovie>

namespace Ui {
  class CassetteDialog;
}

namespace UI {
  class CassetteDialog : public QDialog {
    Q_OBJECT
  public:
    CassetteDialog(QWidget *parent, const QString &fileName);
    ~CassetteDialog() override;

  protected:
    void changeEvent(QEvent *e) override;

  private:
    ::Ui::CassetteDialog *ui;
    SIO::CassetteWorker *worker;
    QTimer *mTimer{};
    int mTotalDuration;
    int mRemainingTime;
    QString mFileName;
    QMovie *mCassMovie{};

  public slots:
    int exec() override;
    void accept() override;
    void tick();

  private slots:
    void progress(int remainingTime);
  };
} // namespace UI

#endif// CASSETTEDIALOG_H
