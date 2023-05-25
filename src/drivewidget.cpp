/*
 * drivewidget.cpp
 *
 * Copyright 2017 blind
 *
 */

#include "drivewidget.h"
#include "include/diskimages/diskimage.h"
#include "ui_drivewidget.h"

DriveWidget::DriveWidget(int driveNum, QWidget *parent)
    : QFrame(parent), ui(new Ui::DriveWidget), driveNo_(driveNum) {
  ui->setupUi(this);
}

DriveWidget::~DriveWidget() {
  delete ui;
}


static void FormatStatusTip(QAction *action, QString &driveNum) {
  const QString &tip = action->statusTip().arg(driveNum);
  action->setStatusTip(tip);
}

void DriveWidget::setup(bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden) {
  QString driveTxt;
  if (driveNo_ < 9) {
    driveTxt = QString("%1").arg(driveNo_ + 1);
    //ui->driveLabel->setText(driveTxt);
  } else {
    driveTxt = QString("%1").arg((char) ((char) (driveNo_ - 9) + 'J'));
  }

  //ui->driveLabel->setText(QString("%1:").arg(driveTxt));

  // Fixup status tips (Note: not all of these have an %1 in the status tip
  FormatStatusTip(ui->actionBootOption, driveTxt);
  FormatStatusTip(ui->actionSave, driveTxt);
  FormatStatusTip(ui->actionAutoSave, driveTxt);
  FormatStatusTip(ui->actionSaveAs, driveTxt);
  FormatStatusTip(ui->actionRevert, driveTxt);
  FormatStatusTip(ui->actionMountDisk, driveTxt);
  FormatStatusTip(ui->actionMountFolder, driveTxt);
  FormatStatusTip(ui->actionEject, driveTxt);
  FormatStatusTip(ui->actionNextSide, driveTxt);
  FormatStatusTip(ui->actionToggleHappy, driveTxt);
  FormatStatusTip(ui->actionToggleChip, driveTxt);
  FormatStatusTip(ui->actionToggleOSB, driveTxt);
  FormatStatusTip(ui->actionToolDisk, driveTxt);
  FormatStatusTip(ui->actionWriteProtect, driveTxt);
  FormatStatusTip(ui->actionEditDisk, driveTxt);

  // Add actions to context menu
  if (driveNo_ == 0)
    insertAction(0, ui->actionBootOption);
  insertAction(0, ui->actionSave);
  insertAction(0, ui->actionAutoSave);//
  insertAction(0, ui->actionSaveAs);
  insertAction(0, ui->actionRevert);
  insertAction(0, ui->actionMountDisk);
  insertAction(0, ui->actionMountFolder);
  insertAction(0, ui->actionEject);
  insertAction(0, ui->actionNextSide);
  insertAction(0, ui->actionToggleHappy);
  insertAction(0, ui->actionToggleChip);
  insertAction(0, ui->actionToggleOSB);
  insertAction(0, ui->actionToolDisk);
  insertAction(0, ui->actionWriteProtect);
  insertAction(0, ui->actionEditDisk);

  // Connect widget actions to buttons
  ui->buttonMountDisk->setDefaultAction(ui->actionMountDisk);
  ui->buttonMountFolder->setDefaultAction(ui->actionMountFolder);
  ui->buttonEject->setDefaultAction(ui->actionEject);
  ui->buttonNextSide->setDefaultAction(ui->actionNextSide);
  ui->buttonToggleHappy->setDefaultAction(ui->actionToggleHappy);
  ui->buttonToggleChip->setDefaultAction(ui->actionToggleChip);
  ui->buttonToggleOSB->setDefaultAction(ui->actionToggleOSB);
  ui->buttonToolDisk->setDefaultAction(ui->actionToolDisk);
  ui->buttonSave->setDefaultAction(ui->actionSave);
  ui->autoSave->setDefaultAction(ui->actionAutoSave);
  ui->buttonEditDisk->setDefaultAction(ui->actionEditDisk);

  ui->buttonNextSide->setVisible(!nextSideHidden);
  ui->buttonToggleHappy->setVisible(!happyHidden);
  ui->buttonToggleChip->setVisible(!chipHidden);
  ui->buttonToggleOSB->setVisible((driveNo_ == 0) && (!OSBHidden));
  ui->buttonToolDisk->setVisible((driveNo_ == 0) && (!toolDiskHidden));


  // do the connections.
  connect(ui->actionSave, &QAction::triggered, this, &DriveWidget::saveTriggered);
  connect(ui->actionEject, &QAction::triggered, this, &DriveWidget::ejectTriggered);
  connect(ui->actionNextSide, &QAction::triggered, this, &DriveWidget::nextSideTriggered);
  connect(ui->actionToggleHappy, &QAction::toggled, this, &DriveWidget::happyToggled);
  connect(ui->actionToggleOSB, &QAction::toggled, this, &DriveWidget::osbToggled);
  connect(ui->actionToggleChip, &QAction::toggled, this, &DriveWidget::chipToggled);
  connect(ui->actionMountDisk, &QAction::triggered, this, &DriveWidget::mountDiskTriggered);
  connect(ui->actionMountFolder, &QAction::triggered, this, &DriveWidget::mountFolderTriggered);
  connect(ui->actionToolDisk, &QAction::toggled, this, &DriveWidget::toolDiskToggled);
  connect(ui->actionWriteProtect, &QAction::toggled, this, &DriveWidget::writeProtectToggled);
  connect(ui->actionEditDisk, &QAction::triggered, this, &DriveWidget::editDiskTriggered);
  connect(ui->actionRevert, &QAction::triggered, this, &DriveWidget::revertTriggered);
  connect(ui->actionSaveAs, &QAction::triggered, this, &DriveWidget::saveAsTriggered);
  connect(ui->actionAutoSave, &QAction::toggled, this, &DriveWidget::autoSaveToggled);
  connect(ui->actionBootOption, &QAction::triggered, this, &DriveWidget::bootOptionTriggered);
}

void DriveWidget::updateFromImage(DiskImages::DiskImagePtr diskImage, bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden) {
  if (diskImage == nullptr) {
    showAsEmpty(happyHidden, chipHidden, nextSideHidden, OSBHidden, toolDiskHidden);
    return;
  }

  const QString &fileName = diskImage->originalFileName();
  ui->labelFileName->setText(fileName);
  ui->labelImageProperties->setText(diskImage->description());
  ui->actionEject->setEnabled(true);
  ui->buttonNextSide->setVisible(!nextSideHidden);
  ui->actionNextSide->setEnabled(diskImage->hasSeveralSides());
  if (diskImage->hasSeveralSides()) {
    ui->actionNextSide->setToolTip(diskImage->getNextSideLabel());
  }
  ui->buttonToggleHappy->setVisible(!happyHidden);
  ui->actionToggleHappy->setEnabled(true);
  ui->actionToggleHappy->setChecked(diskImage->isHappyEnabled());
  ui->buttonToggleChip->setVisible(!chipHidden);
  ui->actionToggleChip->setEnabled(true);
  ui->actionToggleChip->setChecked(diskImage->isChipOpen());
  ui->buttonToggleOSB->setVisible((driveNo_ == 0) && (!OSBHidden));
  ui->actionToggleOSB->setEnabled(true);
  ui->actionToggleOSB->setChecked(diskImage->isTranslatorActive());
  ui->buttonToolDisk->setVisible((driveNo_ == 0) && (!toolDiskHidden));
  ui->actionToolDisk->setEnabled(true);
  ui->actionToolDisk->setChecked(diskImage->isToolDiskActive());

  bool enableEdit = diskImage->editDialog() != nullptr;
  ui->actionEditDisk->setChecked(enableEdit);

  // Update save/revert
  bool modified = diskImage->isModified();
  ui->actionSave->setEnabled(!modified);
  ui->actionRevert->setEnabled(!modified);

  // This was not set in default image mounted!
  ui->actionWriteProtect->setChecked(diskImage->isReadOnly());
  ui->actionWriteProtect->setEnabled(!diskImage->isUnmodifiable());
}


void DriveWidget::triggerAutoSaveClickIfEnabled() {
  if (ui->autoSave->isEnabled()) ui->autoSave->click();
}

void DriveWidget::triggerHappyClickIfEnabled() {
  if (ui->buttonToggleHappy->isEnabled()) ui->buttonToggleHappy->click();
}

void DriveWidget::triggerChipClickIfEnabled() {
  if (ui->buttonToggleChip->isEnabled()) ui->buttonToggleChip->click();
}

void DriveWidget::showAsEmpty(bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden) {
  ui->actionSave->setEnabled(false);
  //ui->labelFileName->clear();
  //ui->labelImageProperties->clear();
  ui->actionEject->setEnabled(false);
  ui->buttonNextSide->setVisible(!nextSideHidden);
  ui->actionNextSide->setEnabled(false);
  ui->actionNextSide->setChecked(false);
  ui->buttonToggleHappy->setVisible(!happyHidden);
  ui->actionToggleHappy->setChecked(false);
  ui->actionToggleHappy->setEnabled(false);
  ui->buttonToggleChip->setVisible(!chipHidden);
  ui->actionToggleChip->setChecked(false);
  ui->actionToggleChip->setEnabled(false);
  ui->buttonToggleOSB->setVisible((driveNo_ == 0) && (!OSBHidden));
  ui->actionToggleOSB->setChecked(false);
  ui->actionToggleOSB->setEnabled(false);
  ui->buttonToolDisk->setVisible((driveNo_ == 0) && (!toolDiskHidden));
  ui->actionToolDisk->setChecked(false);
  ui->actionToolDisk->setEnabled(false);
  ui->actionEditDisk->setEnabled(false);
  ui->actionEditDisk->setChecked(false);
  ui->actionAutoSave->setEnabled(false);
  ui->actionAutoSave->setChecked(false);
  ui->actionRevert->setEnabled(false);
  ui->actionSaveAs->setEnabled(false);

  if (driveNo_ == 0) ui->actionBootOption->setEnabled(false);
  QString empty = "";
  setLabelToolTips(empty, empty, empty);
}

void DriveWidget::showAsFolderMounted(const QString &fileName, const QString &description, bool editEnabled) {
  // code dupe
  ui->labelFileName->setText(fileName);
  ui->labelImageProperties->setText(description);
  ui->actionEject->setEnabled(true);
  ui->buttonNextSide->setVisible(false);
  ui->actionNextSide->setEnabled(false);
  ui->buttonToggleHappy->setVisible(false);
  ui->actionToggleHappy->setEnabled(false);
  ui->buttonToggleChip->setVisible(false);
  ui->actionToggleChip->setEnabled(false);
  ui->buttonToggleOSB->setVisible(false);
  ui->actionToggleOSB->setEnabled(false);
  ui->buttonToolDisk->setVisible(false);
  ui->actionToolDisk->setEnabled(false);
  ui->actionEditDisk->setChecked(editEnabled);

  ui->labelFileName->setStyleSheet("color: rgb(54, 168, 164); font-weight: bold");
  ui->actionEditDisk->setEnabled(true);

  ui->actionSave->setEnabled(false);
  ui->actionAutoSave->setEnabled(false);
  ui->actionSaveAs->setEnabled(false);
  ui->actionRevert->setEnabled(false);

  if (driveNo_ == 0) ui->actionBootOption->setEnabled(true);
}

void DriveWidget::showAsImageMounted(const QString &fileName, const QString &description, bool editEnabled, bool enableSave, bool leverOpen, bool happyEnabled, bool chipOpen,
                                     bool translatorActive, bool toolDiskActive, bool severalSides, bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden) {
  ui->labelFileName->setText(fileName);
  ui->labelImageProperties->setText(description);
  ui->actionEject->setEnabled(true);

  ui->buttonToggleHappy->setVisible(!happyHidden);
  ui->actionToggleHappy->setEnabled(true);
  ui->actionToggleHappy->setChecked(happyEnabled);
  ui->buttonToggleChip->setVisible(!chipHidden);
  ui->actionToggleChip->setEnabled(true);
  ui->actionToggleChip->setChecked(chipOpen);
  ui->buttonToggleOSB->setVisible((driveNo_ == 0) && (!OSBHidden));
  ui->actionToggleOSB->setEnabled(true);
  ui->actionToggleOSB->setChecked(translatorActive);
  ui->buttonToolDisk->setVisible((driveNo_ == 0) && (!toolDiskHidden));
  ui->actionToolDisk->setEnabled(true);
  ui->actionToolDisk->setChecked(toolDiskActive);
  ui->buttonNextSide->setVisible(!nextSideHidden);
  ui->actionNextSide->setEnabled(severalSides);
  ui->actionNextSide->setChecked(leverOpen);

  ui->actionEditDisk->setChecked(editEnabled);

  ui->labelFileName->setStyleSheet("color: rgb(0, 0, 0); font-weight: normal");//

  ui->actionEditDisk->setEnabled(true);

  ui->actionSaveAs->setEnabled(true);
  ui->actionSave->setEnabled(false);

  ui->actionAutoSave->setEnabled(false);
  ui->actionRevert->setEnabled(false);

  ui->actionAutoSave->setEnabled(true);

  if (driveNo_ == 0) ui->actionBootOption->setEnabled(false);

  ui->actionSave->setEnabled(enableSave);
  ui->actionRevert->setEnabled(enableSave);
}

bool DriveWidget::isAutoSaveEnabled() {
  return ui->actionAutoSave->isChecked();
}

bool DriveWidget::isHappyEnabled() {
  return ui->actionToggleHappy->isChecked();
}

bool DriveWidget::isChipEnabled() {
  return ui->actionToggleChip->isChecked();
}


void DriveWidget::setLabelToolTips(const QString &one, const QString &two, const QString &three) {
  ui->labelFileName->setToolTip(one);
  ui->labelFileName->setStatusTip(two);
  ui->labelImageProperties->setToolTip(three);
}


void DriveWidget::setLabelFont(const QFont &font) {
  ui->labelFileName->setFont(font);
}

void DriveWidget::mountFolderTriggered() { emit actionMountFolder(driveNo_); }
void DriveWidget::mountDiskTriggered() { emit actionMountDisk(driveNo_); }
void DriveWidget::ejectTriggered() { emit actionEject(driveNo_); }
void DriveWidget::nextSideTriggered() { emit actionNextSide(driveNo_); }
void DriveWidget::happyToggled(bool open) { emit actionToggleHappy(driveNo_, open); }
void DriveWidget::chipToggled(bool open) { emit actionToggleChip(driveNo_, open); }
void DriveWidget::osbToggled(bool open) { emit actionToggleOSB(driveNo_, open); }
void DriveWidget::toolDiskToggled(bool open) { emit actionToolDisk(driveNo_, open); }
void DriveWidget::writeProtectToggled(bool state) { emit actionWriteProtect(driveNo_, state); }
void DriveWidget::editDiskTriggered() { emit actionEditDisk(driveNo_); }
void DriveWidget::saveTriggered() { emit actionSave(driveNo_); }
void DriveWidget::revertTriggered() { emit actionRevert(driveNo_); }
void DriveWidget::saveAsTriggered() { emit actionSaveAs(driveNo_); }
void DriveWidget::autoSaveToggled(bool state) { emit actionAutoSave(driveNo_, state); }
void DriveWidget::bootOptionTriggered() { emit actionBootOptions(driveNo_); }
