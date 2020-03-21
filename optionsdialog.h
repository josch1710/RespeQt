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

#include <QtWidgets/QDialog>
#include <QFileDialog>
#include <QTreeWidget>
#include <QtDebug>

#include "serialport.h"

namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog {
    Q_OBJECT

public:
    OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog() = default;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::OptionsDialog *m_ui;
    QTreeWidgetItem *itemTestSerialPort, *itemPassthrough, *itemAtari1027,
        *itemStandard, *itemAtariSio, *itemEmulation, *itemDiskOptions, *itemDiskOSB, *itemDiskIcons, *itemDiskFavorite, *itemI18n,
        *itemFirmware810Path, *itemFirmware1050Path, *itemFirmwareEmulation, *itemTraceOptions, *itemDiskImages;

    void selectFirmware(QLineEdit *edit, QString title, QString filters);

    void connectSignals();
    void setupSettings();

private slots:
    void serialPortChanged(int index);
    void handshakeChanged(int index);
    void useDivisorToggled(bool checked);
    void sectionClicked(QTreeWidgetItem* item, int column);
    void currentSectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void saveSettings();
    void useCustomBaudToggled(bool checked);
    void testFileClicked();
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
};

#endif // OPTIONSDIALOG_H
