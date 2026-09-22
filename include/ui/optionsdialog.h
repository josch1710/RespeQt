/*
 * optionsdialog.h
 *
 * Copyright 2016 TheMontezuma
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QFileDialog>
#include <QTreeWidget>

#include "sio/serialport.h"

namespace Ui {
  class OptionsDialog;
}

namespace UI {

class OptionsDialog : public QDialog {
  Q_OBJECT

public:
  explicit OptionsDialog(QWidget *parent = nullptr);
  ~OptionsDialog() override = default;

protected:
  void changeEvent(QEvent *event) override;
  void showEvent(QShowEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

private:
  ::Ui::OptionsDialog *m_ui;
  QTreeWidgetItem
    *itemPassthrough{nullptr},
    *itemAtari1027{nullptr},
    *itemPrinterProtocol{nullptr},
    *item1020Options{nullptr},
    *itemStandard{nullptr},
    *itemAtariSio{nullptr},
    *itemNetSIO{nullptr},
    *itemEmulation{nullptr},
    *itemDiskBrowser{nullptr},
    *itemDiskOptions{nullptr},
    *itemDiskOSB{nullptr},
    *itemDiskIcons{nullptr},
    *itemDiskFavorite{nullptr},
    *itemI18n{nullptr},
    *itemFirmware810Path{nullptr},
    *itemFirmware1050Path{nullptr},
    *itemFirmwareEmulation{nullptr},
    *itemTraceOptions{nullptr},
    *itemDiskImages{nullptr};

  void selectFirmware(QLineEdit* edit, const QString& title, const QString& filters);
  void connectSignals();
  void setupSettings() const;
  void setHorzSplitPos(int pos) const;

private slots:
  void serialPortChanged(int index) const;
  void handshakeChanged(int index) const;
  void useDivisorToggled(bool checked) const;
  void sectionClicked(QTreeWidgetItem *item, int column) const;
  void currentSectionChanged(const QTreeWidgetItem *current, QTreeWidgetItem *previous) const;
  void saveSettings() const;
  void onClosed() const;
  void useCustomBaudToggled(bool checked) const;
  void appDataDirToggled() const;
  void diskSubDirToggled() const;
  void appSettingsToggled() const;
  void browseForAppDir();
  void indexColorClicked();
  void titleColorClicked();
  void indexBoldToggled() const;
  void titleBoldToggled() const;
  void indexItalicToggled() const;
  void titleItalicToggled() const;

#ifdef SHOWFIRMWARE
  void select810FirmwareTriggered();
  void select810ChipFirmwareTriggered();
  void select810HappyFirmwareTriggered();
  void select1050FirmwareTriggered();
  void select1050ArchiverFirmwareTriggered();
  void select1050HappyFirmwareTriggered();
  void select1050SpeedyFirmwareTriggered();
  void select1050TurboFirmwareTriggered();
  void select1050DuplicatorFirmwareTriggered();
#endif
  void selectTranslatorDiskTriggered();
  void selectToolDiskTriggered();
  void fixedFontClicked();
  void rclFolderClicked();
  void useNativeMenuToggled() const;
};

} // namespace UI

#endif// OPTIONSDIALOG_H
