/*
 * drivewidget.cpp
 *
 * Copyright 2017 blind
 *
 */

#include "drivewidget.h"
#include "include/diskimages/diskimage.h"
#include "ui_drivewidget.h"
#include "uicolors.h"
#include "uiscale.h"

#include <QEvent>

DriveWidget::DriveWidget(int driveNum, QWidget *parent)
    : QWidget(parent), ui(new Ui::DriveWidget), driveNo_(driveNum) {
  ui->setupUi(this);
  // The form pinned every button to 28x28 or 21x25 and set no icon size at
  // all. Now the style decides how large an icon is and the buttons size
  // themselves around it.
  UiScale::applyToolButtonIconSizes(this);
  applyPaletteColors();
}

DriveWidget::~DriveWidget() {
  delete ui;
}

// The drive number and the image details are secondary information and were
// dimmed by the form through a literal palette override, rgb(104, 104, 104)
// and rgb(128, 128, 128). Both were chosen for a light window and drop to
// about 3:1 on a dark one, so derive the dimming from the palette instead.
void DriveWidget::applyPaletteColors() {
  const QColor muted = UiColors::mutedText(this);
  UiColors::setTextColor(ui->driveLabel, muted);
  UiColors::setTextColor(ui->labelImageProperties, muted);
  setFileNameHighlight(fileNameHighlighted_);
}

// The colours above are derived once, so they would go stale when the user
// switches the system to dark mode while the window is open.
void DriveWidget::changeEvent(QEvent *e) {
  QWidget::changeEvent(e);
  if (e->type() == QEvent::PaletteChange) {
    applyPaletteColors();
  }
}

// The folder mounted state is highlighted in teal. A literal colour cannot
// serve both palettes: the plain rgb(0, 0, 0) formerly used for the plain
// state turned into invisible text on a dark palette, and the teal
// rgb(54, 168, 164) only reached a 2.9:1 contrast ratio on white. So pick a
// teal tuned for the actual background, and let the palette supply the colour
// for the unhighlighted state instead of naming one. The two tones keep the
// hue the folder state has always had, 177 degrees, and reach 5.3:1 on a light
// window and 9.8:1 on a dark one, so the colour carries the state on its own.
// A darker tone would score better still, but on a light window it starts to
// read as plain dark grey text rather than as a colour.
void DriveWidget::setFileNameHighlight(bool highlighted) {
  fileNameHighlighted_ = highlighted;

  if (!highlighted) {
    ui->labelFileName->setStyleSheet(QString());
    return;
  }

  const QColor accent = UiColors::isDark(this) ? QColor(98, 217, 212)
                                              : QColor(13, 110, 106);
  ui->labelFileName->setStyleSheet(
      QStringLiteral("color: %1; font-weight: bold").arg(accent.name()));
}

static void FormatStatusTip(QAction *action, QString &driveNum) {
  QString tip = action->statusTip();
  if (tip.contains("%1"))
    tip = tip.arg(driveNum);
  action->setStatusTip(tip);
}

void DriveWidget::setup(bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden) {
  QString driveTxt;
  if (driveNo_ < 9) {
    driveTxt = QString("%1").arg(driveNo_ + 1);
    ui->driveLabel->setText(driveTxt);
  } else {
    driveTxt = QString("%1").arg((char) ((char) (driveNo_ - 9) + 'J'));
  }

  ui->driveLabel->setText(QString("%1:").arg(driveTxt));

  // Keep the drive number column aligned across all slots without pinning the
  // label to 26x26. Slots are numbered 1 to 9 and then J onwards, so "M:" is
  // the widest label any slot can show.
  ui->driveLabel->setMinimumWidth(
      ui->driveLabel->fontMetrics().horizontalAdvance(QStringLiteral("M:")));

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

void DriveWidget::updateFromImage(DiskImages::SimpleDiskImage *diskImage, bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden) {
  if (diskImage == nullptr) {
    showAsEmpty(happyHidden, chipHidden, nextSideHidden, OSBHidden, toolDiskHidden);
    return;
  }

  const QString &fileName = diskImage->originalFileName();
  ui->labelFileName->setText(fileName);
  // This path never reset the highlight, so an image replacing a mounted
  // folder in the same slot kept the folder's teal bold styling.
  setFileNameHighlight(false);
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
  ui->labelFileName->clear();
  setFileNameHighlight(false);// reset the styling along with the text
  ui->labelImageProperties->clear();
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

  setFileNameHighlight(true);
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

  setFileNameHighlight(false);

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

void DriveWidget::setDropTarget(bool target) {
  ui->driveFrame->setFrameShadow(target ? QFrame::Sunken : QFrame::Raised);
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
