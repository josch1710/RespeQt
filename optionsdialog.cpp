/*
 * optionsdialog.cpp
 *
 * Copyright 2015 Joseph Zatarski
 * Copyright 2016, 2017 TheMontezuma
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "respeqtsettings.h"
#include "printers/rawoutput.h"
#include <QtSerialPort/QtSerialPort>
#include <QTranslator>
#include <QDir>
#include <QFileDialog>
#include <QFontDialog>
#include <QPrintDialog>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::OptionsDialog)
{
    auto flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);

    m_ui->setupUi(this);

    m_ui->optionSections->expandAll();
    itemStandard = m_ui->optionSections->topLevelItem(0)->child(0);
    itemAtariSio = m_ui->optionSections->topLevelItem(0)->child(1);
    itemEmulation = m_ui->optionSections->topLevelItem(1);
    itemDiskOptions = m_ui->optionSections->topLevelItem(2)->child(0);
    itemDiskOSB = m_ui->optionSections->topLevelItem(2)->child(1);
    itemDiskIcons = m_ui->optionSections->topLevelItem(2)->child(2);
    itemDiskFavorite = m_ui->optionSections->topLevelItem(2)->child(3);
    itemI18n = m_ui->optionSections->topLevelItem(3);
    itemAtari1027 = m_ui->optionSections->topLevelItem(4)->child(0);
    itemPassthrough = m_ui->optionSections->topLevelItem(4)->child(1);
    itemPrinterProtocol = m_ui->optionSections->topLevelItem(4)->child(2);
    item1020Options = m_ui->optionSections->topLevelItem(4)->child(3);

#ifndef Q_OS_LINUX
    m_ui->optionSections->topLevelItem(0)->removeChild(itemAtariSio);
#endif
#ifdef QT_NO_DEBUG
    m_ui->optionSections->topLevelItem(0)->removeChild(itemTestSerialPort);
#endif

    connectSignals();
    setupSettings();
}

void OptionsDialog::setupSettings()
{
    /* Retrieve application settings */
    m_ui->serialPortComboBox->clear();
    const QList<QSerialPortInfo>& infos = QSerialPortInfo::availablePorts();
    for (QList<QSerialPortInfo>::const_iterator it = infos.begin() ; it!=infos.end() ; it++)
    {
        m_ui->serialPortComboBox->addItem(it->portName(),it->systemLocation());
    }
    m_ui->serialPortComboBox->setCurrentText(RespeqtSettings::instance()->serialPortName());
    if(0 != m_ui->serialPortComboBox->currentText().compare(RespeqtSettings::instance()->serialPortName(),Qt::CaseInsensitive))
    {
        m_ui->serialPortComboBox->setEditable(true);
        m_ui->serialPortComboBox->addItem(RespeqtSettings::instance()->serialPortName());
        m_ui->serialPortComboBox->setCurrentText(RespeqtSettings::instance()->serialPortName());
    }
    else
    {
        m_ui->serialPortComboBox->addItem(tr("Custom"));
    }

    m_ui->serialPortHandshakeCombo->setCurrentIndex(RespeqtSettings::instance()->serialPortHandshakingMethod());
    m_ui->serialPortFallingEdge->setChecked(RespeqtSettings::instance()->serialPortTriggerOnFallingEdge());
    m_ui->serialPortDTRControlEnable->setChecked(RespeqtSettings::instance()->serialPortDTRControlEnable());
    m_ui->serialPortWriteDelayCombo->setCurrentIndex(RespeqtSettings::instance()->serialPortWriteDelay());
    m_ui->serialPortBaudCombo->setCurrentIndex(RespeqtSettings::instance()->serialPortMaximumSpeed());
    m_ui->serialPortUseDivisorsBox->setChecked(RespeqtSettings::instance()->serialPortUsePokeyDivisors());
    m_ui->serialPortDivisorEdit->setValue(RespeqtSettings::instance()->serialPortPokeyDivisor());
    m_ui->serialPortCompErrDelayBox->setValue(RespeqtSettings::instance()->serialPortCompErrDelay());
    m_ui->atariSioDriverNameEdit->setText(RespeqtSettings::instance()->atariSioDriverName());
    m_ui->atariSioHandshakingMethodCombo->setCurrentIndex(RespeqtSettings::instance()->atariSioHandshakingMethod());
    m_ui->emulationHighSpeedExeLoaderBox->setChecked(RespeqtSettings::instance()->useHighSpeedExeLoader());
    m_ui->emulationUseCustomCasBaudBox->setChecked(RespeqtSettings::instance()->useCustomCasBaud());
    m_ui->emulationCustomCasBaudSpin->setValue(RespeqtSettings::instance()->customCasBaud());
    m_ui->minimizeToTrayBox->setChecked(RespeqtSettings::instance()->minimizeToTray());
    m_ui->saveWinPosBox->setChecked(RespeqtSettings::instance()->saveWindowsPos());
    m_ui->saveDiskVisBox->setChecked(RespeqtSettings::instance()->saveDiskVis());
    m_ui->filterUscore->setChecked(RespeqtSettings::instance()->filterUnderscore());
    m_ui->capitalLettersPCLINK->setChecked(RespeqtSettings::instance()->capitalLettersInPCLINK());
    m_ui->URLSubmit->setChecked(RespeqtSettings::instance()->isURLSubmitEnabled());
    m_ui->spyMode->setChecked(RespeqtSettings::instance()->isSpyMode());
    m_ui->commandName->setChecked(RespeqtSettings::instance()->isCommandName());
    m_ui->trackLayout->setChecked(RespeqtSettings::instance()->isTrackLayout());
    m_ui->displayTransmission->setChecked(RespeqtSettings::instance()->displayTransmission());
    m_ui->disassembleUploadedCode->setChecked(RespeqtSettings::instance()->disassembleUploadedCode());
    m_ui->translatorDiskImagePath->setText(RespeqtSettings::instance()->translatorDiskImagePath());
    m_ui->translatorAutomaticDetection->setChecked(RespeqtSettings::instance()->translatorAutomaticDetection());
    m_ui->sioAutoReconnect->setChecked(RespeqtSettings::instance()->sioAutoReconnect());
    m_ui->hideChipMode->setChecked(RespeqtSettings::instance()->hideChipMode());
    m_ui->hideHappyMode->setChecked(RespeqtSettings::instance()->hideHappyMode());
    m_ui->hideNextImage->setChecked(RespeqtSettings::instance()->hideNextImage());
    m_ui->hideOSBMode->setChecked(RespeqtSettings::instance()->hideOSBMode());
    m_ui->hideToolDisk->setChecked(RespeqtSettings::instance()->hideToolDisk());
    m_ui->toolDiskImagePath->setText(RespeqtSettings::instance()->toolDiskImagePath());
    m_ui->activateChipModeWithTool->setChecked(RespeqtSettings::instance()->activateChipModeWithTool());
    m_ui->activateHappyModeWithTool->setChecked(RespeqtSettings::instance()->activateHappyModeWithTool());
    m_ui->useLargerFont->setChecked(RespeqtSettings::instance()->useLargeFont());
    m_ui->enableShade->setChecked(RespeqtSettings::instance()->enableShade());
    m_ui->RclNameEdit->setText(RespeqtSettings::instance()->lastRclDir());
    m_ui->printerSpyMode->setChecked(RespeqtSettings::instance()->isPrinterSpyMode());
    m_ui->displayGraphicsInstructions->setChecked(RespeqtSettings::instance()->displayGraphicsInstructions());
    m_ui->clearOnStatus->setChecked(RespeqtSettings::instance()->clearOnStatus());

#ifdef Q_OS_MAC
    m_ui->useNativeMenu->setChecked(RespeqtSettings::instance()->nativeMenu());
    const auto& actualNoMenu = QApplication::testAttribute(Qt::AA_DontUseNativeMenuBar);
    // The meaning of both flags are the opposite (i.e. boolean not) of each other.
    // So, we have to test for equality to get the difference (i.e. a != !b).
    if (actualNoMenu == RespeqtSettings::instance()->nativeMenu())
        m_ui->warning_nativemenu->show();
    else
        m_ui->warning_nativemenu->hide();

    m_ui->minimizeToTrayBox->hide();
#endif

    switch (RespeqtSettings::instance()->backend()) {
        default:
        case SerialBackend::STANDARD:
            itemStandard->setCheckState(0, Qt::Checked);
            itemAtariSio->setCheckState(0, Qt::Unchecked);
            m_ui->optionSections->setCurrentItem(itemStandard);
            break;
        case SerialBackend::SIO_DRIVER:
            itemStandard->setCheckState(0, Qt::Unchecked);
            itemAtariSio->setCheckState(0, Qt::Checked);
            m_ui->optionSections->setCurrentItem(itemAtariSio);
            break;
    }
    m_ui->serialPortBox->setCheckState(itemStandard->checkState(0));
    m_ui->atariSioBox->setCheckState(itemAtariSio->checkState(0));

    /* list available translations */
    QTranslator local_translator;
    m_ui->i18nLanguageCombo->clear();
    m_ui->i18nLanguageCombo->addItem(tr("Automatic"), "auto");
    if (RespeqtSettings::instance()->i18nLanguage().compare("auto") == 0)
      m_ui->i18nLanguageCombo->setCurrentIndex(0);
    m_ui->i18nLanguageCombo->addItem(QT_TR_NOOP("English"), "en");
    if (RespeqtSettings::instance()->i18nLanguage().compare("en") == 0)
      m_ui->i18nLanguageCombo->setCurrentIndex(1);
    QDir dir(":/translations/i18n/");
    QStringList filters;
    filters << "respeqt_*.qm";
    dir.setNameFilters(filters);
    for (int i = 0; i < dir.entryList().size(); ++i) {
        local_translator.load(":/translations/i18n/" + dir.entryList()[i]);
        m_ui->i18nLanguageCombo->addItem(local_translator.translate("OptionsDialog", "English"), dir.entryList()[i].replace("respeqt_", "").replace(".qm", ""));
        if (dir.entryList()[i].replace("respeqt_", "").replace(".qm", "").compare(RespeqtSettings::instance()->i18nLanguage()) == 0) {
            m_ui->i18nLanguageCombo->setCurrentIndex(i+2);
        }
    }

    bool software_handshake = (RespeqtSettings::instance()->serialPortHandshakingMethod()==HANDSHAKE_SOFTWARE);
    m_ui->serialPortWriteDelayLabel->setVisible(software_handshake);
    m_ui->serialPortWriteDelayCombo->setVisible(software_handshake);
    m_ui->serialPortBaudLabel->setVisible(!software_handshake);
    m_ui->serialPortBaudCombo->setVisible(!software_handshake);
    m_ui->serialPortUseDivisorsBox->setVisible(!software_handshake);
    m_ui->serialPortDivisorLabel->setVisible(!software_handshake);
    m_ui->serialPortDivisorEdit->setVisible(!software_handshake);
    m_ui->serialPortCompErrDelayLabel->setVisible(!software_handshake);
    m_ui->serialPortCompErrDelayBox->setVisible(!software_handshake);
#ifdef Q_OS_WIN
    bool no_handshake = (RespeqtSettings::instance()->serialPortHandshakingMethod()==HANDSHAKE_NO_HANDSHAKE);
    m_ui->serialPortFallingEdge->setVisible(!no_handshake && !software_handshake);
    m_ui->serialPortDTRControlEnable->setVisible(no_handshake || software_handshake);
#else
    m_ui->serialPortFallingEdge->setVisible(false);
#endif

    if((SerialBackend::STANDARD == RespeqtSettings::instance()->backend()) && software_handshake)
    {
        m_ui->emulationHighSpeedExeLoaderBox->setVisible(false);
    }
    else
    {
        m_ui->emulationHighSpeedExeLoaderBox->setVisible(true);
    }

    m_ui->label_atarifixed->setText(RespeqtSettings::instance()->atariFixedFontFamily());
    QFont font;
    font.setPointSize(12);
    font.setFamily(m_ui->label_atarifixed->text());
    m_ui->fontSample->setFont(font);

#ifdef Q_OS_MAC
    m_ui->useNativeMenu->show();
    m_ui->useNativeMenu->setChecked(RespeqtSettings::instance()->nativeMenu());
#else
    m_ui->useNativeMenu->hide();
#endif
    m_ui->showDebugMenu->setChecked(RespeqtSettings::instance()->debugMenuVisible());

    // Setup via platform dependent class
    Printers::RawOutput::setupRawPrinters(m_ui->rawPrinterName);
    QString rawPrinterName = RespeqtSettings::instance()->rawPrinterName();
    if (rawPrinterName.length() > 0)
        m_ui->rawPrinterName->setCurrentText(rawPrinterName);
}

// This will connect all necessary signals to slots on this dialog.
void OptionsDialog::connectSignals()
{
    // Dialog
    connect(this, &OptionsDialog::accepted, this, &OptionsDialog::saveSettings);

    // Tree
    connect(m_ui->optionSections, &QTreeWidget::itemClicked, this, &OptionsDialog::sectionClicked);
    connect(m_ui->optionSections, &QTreeWidget::currentItemChanged, this, &OptionsDialog::currentSectionChanged);

    // Serial port section
    void (QComboBox::*serialPortChanged)(int) = &QComboBox::currentIndexChanged;
    connect(m_ui->serialPortComboBox, serialPortChanged, this, &OptionsDialog::serialPortChanged);
    void (QComboBox::*handshakeChanged)(int) = &QComboBox::currentIndexChanged;
    connect(m_ui->serialPortHandshakeCombo, handshakeChanged, this, &OptionsDialog::handshakeChanged);
    connect(m_ui->serialPortUseDivisorsBox, &QCheckBox::toggled, this, &OptionsDialog::useDivisorToggled);

    // Emulation section
    connect(m_ui->emulationUseCustomCasBaudBox, &QCheckBox::toggled, this, &OptionsDialog::useCustomBaudToggled);
    connect(m_ui->buttonRclFolder, &QPushButton::clicked, this, &OptionsDialog::rclFolderClicked);

    // Disk images
#ifdef SHOWFIRMWARE
    connect(m_ui->atari810FirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select810FirmwareTriggered);
    connect(m_ui->atari810HappyFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select810HappyFirmwareTriggered);
    connect(m_ui->atari810ChipFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select810ChipFirmwareTriggered);
    connect(m_ui->atari1050FirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050FirmwareTriggered);
    connect(m_ui->atari1050HappyFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050HappyFirmwareTriggered);
    connect(m_ui->atari1050TurboFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050TurboFirmwareTriggered);
    connect(m_ui->atari1050SpeedyFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050SpeedyFirmwareTriggered);
    connect(m_ui->atari1050ArchiverFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050ArchiverFirmwareTriggered);
    connect(m_ui->atari1050DuplicatorFirmwareButton, &QPushButton::clicked, this, &OptionsDialog::select1050DuplicatorFirmwareTriggered);
#endif
    connect(m_ui->toolDiskSelectButton, &QPushButton::clicked, this, &OptionsDialog::selectToolDiskTriggered);
    connect(m_ui->translatorSelectButton, &QPushButton::clicked, this, &OptionsDialog::selectTranslatorDiskTriggered);

    // printer section
    connect(m_ui->button_atarifixed, &QPushButton::clicked, this, &OptionsDialog::fixedFontClicked);

    // UI section
    connect(m_ui->useNativeMenu, &QCheckBox::toggled, this, &OptionsDialog::useNativeMenuToggled);
}

void OptionsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void OptionsDialog::serialPortChanged(int index)
{
    bool isCustomPath = !m_ui->serialPortComboBox->itemData(index).isValid();
    m_ui->serialPortComboBox->setEditable(isCustomPath);
}

void OptionsDialog::handshakeChanged(int index)
{
    auto software_handshake = (index==HANDSHAKE_SOFTWARE);
    m_ui->serialPortWriteDelayLabel->setVisible(software_handshake);
    m_ui->serialPortWriteDelayCombo->setVisible(software_handshake);
    m_ui->serialPortBaudLabel->setVisible(!software_handshake);
    m_ui->serialPortBaudCombo->setVisible(!software_handshake);
    m_ui->serialPortUseDivisorsBox->setVisible(!software_handshake);
    m_ui->serialPortDivisorLabel->setVisible(!software_handshake);
    m_ui->serialPortDivisorEdit->setVisible(!software_handshake);
    m_ui->serialPortCompErrDelayLabel->setVisible(!software_handshake);
    m_ui->serialPortCompErrDelayBox->setVisible(!software_handshake);
#ifdef Q_OS_WIN
    bool no_handshake = (index==HANDSHAKE_NO_HANDSHAKE);
    m_ui->serialPortFallingEdge->setVisible(!no_handshake && !software_handshake);
    m_ui->serialPortDTRControlEnable->setVisible(no_handshake || software_handshake);
#endif
    if(itemStandard->checkState((0)) == Qt::Checked)
    {
        m_ui->emulationHighSpeedExeLoaderBox->setVisible(!software_handshake);
    }
}

void OptionsDialog::useDivisorToggled(bool checked)
{
    m_ui->serialPortBaudLabel->setEnabled(!checked);
    m_ui->serialPortBaudCombo->setEnabled(!checked);
    m_ui->serialPortDivisorLabel->setEnabled(checked);
    m_ui->serialPortDivisorEdit->setEnabled(checked);
}

// TODO Bug with serial selection
void OptionsDialog::sectionClicked(QTreeWidgetItem* item, int column)
{
    if (item->checkState(column) == Qt::Checked)
    {
        if (item == itemStandard)
        {
            m_ui->emulationHighSpeedExeLoaderBox->setVisible(HANDSHAKE_SOFTWARE != m_ui->serialPortHandshakeCombo->currentIndex());
        }
        else
        {
            itemStandard->setCheckState(column, Qt::Unchecked);
        }
        if (item == itemAtariSio)
        {
            m_ui->emulationHighSpeedExeLoaderBox->setVisible(true);
        }
        else
        {
            itemAtariSio->setCheckState(column, Qt::Unchecked);
        }
    }
    else if ((itemStandard->checkState(column) == Qt::Unchecked) &&
            (itemAtariSio->checkState(column) == Qt::Unchecked))
    {
        item->setCheckState(column, Qt::Checked);
    }
    m_ui->serialPortBox->setCheckState(itemStandard->checkState(column));
    m_ui->atariSioBox->setCheckState(itemAtariSio->checkState(column));
}

void OptionsDialog::currentSectionChanged(QTreeWidgetItem* current, QTreeWidgetItem* /*previous*/)
{
    if (current == itemStandard) {
        m_ui->stackedWidget->setCurrentIndex(0);
    } else if (current == itemAtariSio) {
        m_ui->stackedWidget->setCurrentIndex(1);
    } else if (current == itemEmulation) {
        m_ui->stackedWidget->setCurrentIndex(2);
    } else if (current == itemDiskOptions) {
        m_ui->stackedWidget->setCurrentIndex(3);
    } else if (current == itemDiskOSB) {
        m_ui->stackedWidget->setCurrentIndex(11);
    } else if (current == itemDiskIcons) {
        m_ui->stackedWidget->setCurrentIndex(12);
    } else if (current == itemDiskFavorite) {
        m_ui->stackedWidget->setCurrentIndex(13);
    } else if (current == itemI18n) {
        m_ui->stackedWidget->setCurrentIndex(4);
    } else if (current == itemAtari1027) {
        m_ui->stackedWidget->setCurrentIndex(5);
    } else if (current == itemPassthrough) {
        m_ui->stackedWidget->setCurrentIndex(6);
    } else if (current == itemPrinterProtocol) {
        m_ui->stackedWidget->setCurrentIndex(14);
    } else if (current == item1020Options) {
        m_ui->stackedWidget->setCurrentIndex(15);
    }
}

void OptionsDialog::saveSettings()
{
    RespeqtSettings::instance()->setSerialPortName(m_ui->serialPortComboBox->currentText());
    RespeqtSettings::instance()->setSerialPortHandshakingMethod(m_ui->serialPortHandshakeCombo->currentIndex());
    RespeqtSettings::instance()->setSerialPortTriggerOnFallingEdge(m_ui->serialPortFallingEdge->isChecked());
    RespeqtSettings::instance()->setSerialPortDTRControlEnable(m_ui->serialPortDTRControlEnable->isChecked());
    RespeqtSettings::instance()->setSerialPortWriteDelay(m_ui->serialPortWriteDelayCombo->currentIndex());
    RespeqtSettings::instance()->setSerialPortCompErrDelay(m_ui->serialPortCompErrDelayBox->value());
    RespeqtSettings::instance()->setSerialPortMaximumSpeed(m_ui->serialPortBaudCombo->currentIndex());
    RespeqtSettings::instance()->setSerialPortUsePokeyDivisors(m_ui->serialPortUseDivisorsBox->isChecked());
    RespeqtSettings::instance()->setSerialPortPokeyDivisor(m_ui->serialPortDivisorEdit->value());
    RespeqtSettings::instance()->setAtariSioDriverName(m_ui->atariSioDriverNameEdit->text());
    RespeqtSettings::instance()->setAtariSioHandshakingMethod(m_ui->atariSioHandshakingMethodCombo->currentIndex());
    RespeqtSettings::instance()->setUseHighSpeedExeLoader(m_ui->emulationHighSpeedExeLoaderBox->isChecked());
    RespeqtSettings::instance()->setUseCustomCasBaud(m_ui->emulationUseCustomCasBaudBox->isChecked());
    RespeqtSettings::instance()->setCustomCasBaud(m_ui->emulationCustomCasBaudSpin->value());
    RespeqtSettings::instance()->setMinimizeToTray(m_ui->minimizeToTrayBox->isChecked());
    RespeqtSettings::instance()->setsaveWindowsPos(m_ui->saveWinPosBox->isChecked());
    RespeqtSettings::instance()->setsaveDiskVis(m_ui->saveDiskVisBox->isChecked());
    RespeqtSettings::instance()->setfilterUnderscore(m_ui->filterUscore->isChecked());
    RespeqtSettings::instance()->setCapitalLettersInPCLINK(m_ui->capitalLettersPCLINK->isChecked());
    RespeqtSettings::instance()->setURLSubmit(m_ui->URLSubmit->isChecked());
    RespeqtSettings::instance()->setSpyMode(m_ui->spyMode->isChecked());
    RespeqtSettings::instance()->setCommandName(m_ui->commandName->isChecked());
    RespeqtSettings::instance()->setTrackLayout(m_ui->trackLayout->isChecked());
    RespeqtSettings::instance()->setDisplayTransmission(m_ui->displayTransmission->isChecked());
    RespeqtSettings::instance()->setDisassembleUploadedCode(m_ui->disassembleUploadedCode->isChecked());
    RespeqtSettings::instance()->setTranslatorAutomaticDetection(m_ui->translatorAutomaticDetection->isChecked());
    RespeqtSettings::instance()->setTranslatorDiskImagePath(m_ui->translatorDiskImagePath->text());
    RespeqtSettings::instance()->setSioAutoReconnect(m_ui->sioAutoReconnect->isChecked());
    RespeqtSettings::instance()->setHideChipMode(m_ui->hideChipMode->isChecked());
    RespeqtSettings::instance()->setHideHappyMode(m_ui->hideHappyMode->isChecked());
    RespeqtSettings::instance()->setHideNextImage(m_ui->hideNextImage->isChecked());
    RespeqtSettings::instance()->setHideOSBMode(m_ui->hideOSBMode->isChecked());
    RespeqtSettings::instance()->setHideToolDisk(m_ui->hideToolDisk->isChecked());
    RespeqtSettings::instance()->setToolDiskImagePath(m_ui->toolDiskImagePath->text());
    RespeqtSettings::instance()->setActivateChipModeWithTool(m_ui->activateChipModeWithTool->isChecked());
    RespeqtSettings::instance()->setActivateHappyModeWithTool(m_ui->activateHappyModeWithTool->isChecked());
    RespeqtSettings::instance()->setUseLargeFont(m_ui->useLargerFont->isChecked());
    RespeqtSettings::instance()->setEnableShade(m_ui->enableShade->isChecked());
    RespeqtSettings::instance()->setRclDir(m_ui->RclNameEdit->text());
    RespeqtSettings::instance()->setPrinterSpyMode(m_ui->printerSpyMode->isChecked());
    RespeqtSettings::instance()->setDisplayGraphicsInstructions(m_ui->displayGraphicsInstructions->isChecked());
    RespeqtSettings::instance()->setClearOnStatus(m_ui->clearOnStatus->isChecked());

    SerialBackend backend = SerialBackend::STANDARD;
    if (itemAtariSio->checkState(0) == Qt::Checked)
    {
        backend = SerialBackend::SIO_DRIVER;
    }

    RespeqtSettings::instance()->setBackend(static_cast<SerialBackend>(backend));

    RespeqtSettings::instance()->setI18nLanguage(m_ui->i18nLanguageCombo->itemData(m_ui->i18nLanguageCombo->currentIndex()).toString());
#ifdef Q_OS_MAC
    RespeqtSettings::instance()->setNativeMenu(m_ui->useNativeMenu->isChecked());
#endif
    if (m_ui->rawPrinterName->currentData() != -1)
        RespeqtSettings::instance()->setRawPrinterName(m_ui->rawPrinterName->currentText());
    else
        RespeqtSettings::instance()->setRawPrinterName("");

    RespeqtSettings::instance()->setDebugMenuVisible(m_ui->showDebugMenu->isChecked());
}

void OptionsDialog::useCustomBaudToggled(bool checked)
{
    m_ui->emulationCustomCasBaudSpin->setEnabled(checked);
}

void OptionsDialog::selectFirmware(QLineEdit *edit, QString title, QString filters)
{
    auto dir = edit->text();
    auto lastSlash = dir.lastIndexOf("/");
    auto lastBackslash = dir.lastIndexOf("\\");
    if ((lastSlash != -1) || (lastBackslash != -1))
    {
        auto lastIndex = lastSlash > lastBackslash ? lastSlash : lastBackslash;
        dir = dir.left(lastIndex);
    }
    else
    {
        dir = "";
    }
    auto fileName = QFileDialog::getOpenFileName(this, title, dir, filters);
    if (fileName.isEmpty()) {
        return;
    }
    edit->setText(fileName);
}

#ifdef SHOWFIRMWARE
void OptionsDialog::select810FirmwareTriggered()
{
    selectFirmware(m_ui->atari810FirmwarePath, tr("Select Atari 810 firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select810ChipFirmwareTriggered()
{
    selectFirmware(m_ui->atari810ChipFirmwarePath, tr("Select Atari 810 Chip firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select810HappyFirmwareTriggered()
{
    selectFirmware(m_ui->atari810HappyFirmwarePath, tr("Select Atari 810 Happy firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050FirmwareTriggered()
{
    selectFirmware(m_ui->atari1050FirmwarePath, tr("Select Atari 1050 firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050ArchiverFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050ArchiverFirmwarePath, tr("Select Atari 1050 Archiver firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050HappyFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050HappyFirmwarePath, tr("Select Atari 1050 Happy firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050SpeedyFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050SpeedyFirmwarePath, tr("Select Atari 1050 Speedy firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050TurboFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050TurboFirmwarePath, tr("Select Atari 1050 Turbo firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}

void OptionsDialog::select1050DuplicatorFirmwareTriggered()
{
    selectFirmware(m_ui->atari1050DuplicatorFirmwarePath, tr("Select Atari 1050 Duplicator firmware"), tr("Atari drive firmware (*.rom);;All files (*)"));
}
#endif

void OptionsDialog::selectTranslatorDiskTriggered()
{
    selectFirmware(m_ui->translatorDiskImagePath, tr("Select translator disk image"), tr("Atari disk image (*.atr);;All files (*)"));
}

void OptionsDialog::selectToolDiskTriggered()
{
    selectFirmware(m_ui->toolDiskImagePath, tr("Select tool disk image"), tr("Atari disk image (*.atr);;All files (*)"));
}

void OptionsDialog::fixedFontClicked()
{
    bool ok;
    QFont font;
    font.setFamily(m_ui->label_atarifixed->text());
    QFontDialog::FontDialogOptions options = QFontDialog::MonospacedFonts;
    QFont newFont = QFontDialog::getFont(&ok, font, this, tr("Select Atari fixed width font"), options);
    if (ok)
    {
        newFont.setPointSize(12);
        m_ui->label_atarifixed->setText(newFont.family());
        m_ui->fontSample->setFont(newFont);
        RespeqtSettings::instance()->setAtariFixedFontFamily(newFont.family());
    }
}

void OptionsDialog::rclFolderClicked()
{
     QString dir;
     dir = RespeqtSettings::instance()->lastRclDir();
     QString fileName = QFileDialog::getExistingDirectory(this, tr("Selec RCL image folder"), dir);
     fileName = QDir::fromNativeSeparators(fileName);    //
     if (fileName.isEmpty()) {
         return;
     }
     RespeqtSettings::instance()->setRclDir(fileName);
     m_ui->RclNameEdit->setText(fileName);

}

void OptionsDialog::useNativeMenuToggled()
{
    // The meaning of both flags are the opposite (i.e. boolean not) of each other.
    // So, we have to test for equality to get difference (i.e. a != !b).
    const auto& actualNoMenu = QApplication::testAttribute(Qt::AA_DontUseNativeMenuBar);
    // We reverse the meaning of the checkbox to match the application attribute, see above for reason.
    const auto& checkboxNoMenu = m_ui->useNativeMenu->checkState() == Qt::Unchecked;
    m_ui->warning_nativemenu->setVisible(actualNoMenu != checkboxNoMenu);
}
