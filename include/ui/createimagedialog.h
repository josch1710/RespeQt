/*
 * createimagedialog.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef CREATEIMAGEDIALOG_H
#define CREATEIMAGEDIALOG_H

#include <QDialog>

namespace Ui {
  class CreateImageDialog;
}

namespace UI {
  class CreateImageDialog : public QDialog {
    Q_OBJECT

  public:
    explicit CreateImageDialog(QWidget *parent = nullptr);
    ~CreateImageDialog() override;
    [[nodiscard]] int sectorCount() const;
    [[nodiscard]] int sectorSize() const;

  protected:
    void changeEvent(QEvent *e) override;

  private:
    ::Ui::CreateImageDialog *m_ui;

  private slots:
    void recalculate() const;
    void harddiskToggled(bool checked) const;
    void customToggled(bool checked) const;
    void doubleDoubleToggled(bool checked) const;
    void standardDoubleToggled(bool checked) const;
    void standardSingleToggled(bool checked) const;
    void standardEnhancedToggled(bool checked) const;
  };
} // namespace UI

#endif// CREATEIMAGEDIALOG_H
