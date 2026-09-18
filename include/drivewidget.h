/*
 * drivewidget.h
 *
 * Copyright 2017 blind
 *
 */

#ifndef DRIVEWIDGET_H
#define DRIVEWIDGET_H

#include <QFrame>

namespace Ui {
  class DriveWidget;
}

namespace DiskImages {
  class SimpleDiskImage;
}

class DriveWidget : public QWidget {
  Q_OBJECT

public:
  explicit DriveWidget(int driveNum, QWidget *parent = nullptr);
  ~DriveWidget() override;

  [[nodiscard]] [[maybe_unused]] int getDriveNumber() const { return driveNo_; }
  void setup(bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden);


  void showAsEmpty(bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden);
  void showAsFolderMounted(const QString &fileName, const QString &description, bool editEnabled);
  void showAsImageMounted(const QString &fileName, const QString &description, bool editEnabled, bool enableSave, bool leverOpen, bool happyEnabled, bool chipOpen,
                          bool translatorActive, bool toolDiskActive, bool severalSides, bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden);

  void updateFromImage(DiskImages::SimpleDiskImage *diskImage, bool happyHidden, bool chipHidden, bool nextSideHidden, bool OSBHidden, bool toolDiskHidden);
  [[nodiscard]] bool isAutoSaveEnabled() const;
  [[nodiscard]] bool isHappyEnabled() const;
  [[nodiscard]] bool isChipEnabled() const;
  void setLabelToolTips(const QString &one, const QString &two, const QString &three) const;
  void setDropTarget(bool target) const;

  void triggerAutoSaveClickIfEnabled() const;
  void triggerHappyClickIfEnabled() const;
  void triggerChipClickIfEnabled() const;

signals:
  void actionMountDisk(int deviceId);
  void actionMountFolder(int deviceId);
  void actionEject(int deviceId);
  void actionNextSide(int deviceId);
  void actionToggleHappy(int deviceId, bool enabled);
  void actionToggleChip(int deviceId, bool open);
  void actionToggleOSB(int deviceId, bool open);
  void actionToolDisk(int deviceId, bool open);
  void actionWriteProtect(int deviceId, bool state);
  [[maybe_unused]] void actionMountRecent(int deviceId, const QString &fileName);
  void actionEditDisk(int deviceId);
  void actionSave(int deviceId);
  void actionAutoSave(int deviceId, bool enabled);
  void actionSaveAs(int deviceId);
  void actionRevert(int deviceId);
  void actionBootOptions(int deviceId);

public slots:
  void setLabelFont(const QFont &font) const;

private slots:
  void mountFolderTriggered();
  void mountDiskTriggered();
  void ejectTriggered();
  void nextSideTriggered();
  void happyToggled(bool open);
  void chipToggled(bool open);
  void osbToggled(bool open);
  void toolDiskToggled(bool open);
  void writeProtectToggled(bool state);
  void editDiskTriggered();
  void saveTriggered();
  void revertTriggered();
  void saveAsTriggered();
  void autoSaveToggled(bool state);
  void bootOptionTriggered();

protected:
  void changeEvent(QEvent *e) override;

private:
  void setFileNameHighlight(bool highlighted);
  void applyPaletteColors();

  Ui::DriveWidget *ui;
  int driveNo_;
  bool fileNameHighlighted_{false};
};

#endif// DRIVEWIDGET_H
