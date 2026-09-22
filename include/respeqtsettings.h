/*
 * respeqtsettings.h
 *
 * Copyright 2015 Joseph Zatarski
 * Copyright 2016, 2017 TheMontezuma
 * Copyright 2017 blind
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef RESPEQTSETTINGS_H
#define RESPEQTSETTINGS_H

#include "ui/mainwindow.h"
#include "sio/serialport.h"
#include "sio/sioworker.h"
#include "diskbrowser/picsourcetype.h"
#include "diskbrowser/picpreview.h"
#include "diskbrowser/dbsettings.h"

#include <QPrinterInfo>
#include <QSettings>
#include <memory>

#define NUM_RECENT_FILES 10

// There should always only be one instance, so this is a singleton.
class RespeqtSettings final {
public:
  class ImageSettings {
  public:
    QString fileName{};
    bool isWriteProtected{false};
  };

  ~RespeqtSettings();

  [[nodiscard]] bool isFirstTime() const;

  bool saveMainWinGeometry(const QMainWindow* window, bool isMiniMode) const;
  bool restoreMainWinGeometry(QMainWindow* window, bool isMiniMode) const;

  [[nodiscard]] QString serialPortName() const;
  void setSerialPortName(const QString &name) const;

  [[nodiscard]] int serialPortHandshakingMethod() const;
  void setSerialPortHandshakingMethod(int method) const;

  [[nodiscard]] bool serialPortTriggerOnFallingEdge() const;
  void setSerialPortTriggerOnFallingEdge(bool use) const;

  [[nodiscard]] bool serialPortDTRControlEnable() const;
  void setSerialPortDTRControlEnable(bool use) const;

  [[nodiscard]] int serialPortMaximumSpeed() const;
  void setSerialPortMaximumSpeed(int speed) const;

  [[nodiscard]] bool serialPortUsePokeyDivisors() const;
  void setSerialPortUsePokeyDivisors(bool use) const;

  [[nodiscard]] int serialPortPokeyDivisor() const;
  void setSerialPortPokeyDivisor(int divisor) const;

  [[nodiscard]] int serialPortWriteDelay() const;
  void setSerialPortWriteDelay(int delay) const;

  [[nodiscard]] unsigned int serialPortCompErrDelay() const;
  void setSerialPortCompErrDelay(unsigned int delay) const;

  [[nodiscard]] QString atariSioDriverName() const;
  void setAtariSioDriverName(const QString &name) const;

  [[nodiscard]] int atariSioHandshakingMethod() const;
  void setAtariSioHandshakingMethod(int method) const;

  [[nodiscard]] SIO::SerialBackend backend() const;
  void setBackend(SIO::SerialBackend backend) const;

  [[nodiscard]] bool useHighSpeedExeLoader() const;
  void setUseHighSpeedExeLoader(bool use) const;

  [[nodiscard]] bool printerEmulation() const;
  void setPrinterEmulation(bool status) const;

  [[nodiscard]] bool useCustomCasBaud() const;
  void setUseCustomCasBaud(bool use) const;

  [[nodiscard]] int customCasBaud() const;
  void setCustomCasBaud(int baud) const;

  [[nodiscard]] ImageSettings getImageSettingsFromName(const QString &fileName) const;
  [[nodiscard]] ImageSettings mountedImageSetting(int no) const;
  [[nodiscard]] ImageSettings recentImageSetting(int no) const;

  void setMountedImageSetting(int no, const QString &fileName, bool prot) const;
  void setMountedImageProtection(int no, bool prot) const;

  void mountImage(int no, const QString &fileName, bool isWriteProtected) const;

  void unmountImage(int no) const;

  void swapImages(int no1, int no2) const;

  [[nodiscard]] QString lastDiskImageDir() const;
  void setLastDiskImageDir(const QString &dir) const;

  [[nodiscard]] QString lastFolderImageDir() const;
  void setLastFolderImageDir(const QString &dir) const;

  [[nodiscard]] QString lastSessionDir() const;
  void setLastSessionDir(const QString &dir) const;

  [[nodiscard]] QString lastExeDir() const;
  void setLastExeDir(const QString &dir) const;

  [[nodiscard]] QString lastExtractDir() const;
  void setLastExtractDir(const QString &dir) const;

  [[nodiscard]] QString lastPrinterTextDir() const;
  void setLastPrinterTextDir(const QString &dir) const;

  [[nodiscard]] QString lastCasDir() const;
  void setLastCasDir(const QString &dir) const;

  [[nodiscard]] QString lastRclDir() const;
  void setRclDir(const QString &dir) const;

  [[nodiscard]] bool  showLogWindow() const;
  void  setShowLogWindow(bool show = true) const;

  // Set and restore last mainwindow position and size //
  const QPoint DefaultFullModePos  = { 100, 100 };
  const QSize  DefaultFullModeSize = { 800, 650 };
  const QRect  DefaultFullModeRect = { DefaultFullModePos, DefaultFullModeSize };

  // Set and restore last mini-window position //
  const QPoint DefaultMiniModePos  = { 8, 50 };
  const QSize  DefaultMiniModeSize = { 600, 100 };
  const QRect  DefaultMiniModeRect = { DefaultMiniModePos, DefaultMiniModeSize };

  [[nodiscard]] bool miniMode() const;
  void setMiniMode(bool miniMode) const;

  [[nodiscard]] QString i18nLanguage() const;
  void setI18nLanguage(const QString &lang) const;

  [[nodiscard]] bool minimizeToTray() const;
  void setMinimizeToTray(bool tray) const;

  void setOptionsDlgSplitPos(int pos) const;
  [[nodiscard]] int  optionsDialogSplitPos() const;

  // Save window positions and sizes option //
  [[nodiscard]] bool saveWindowsPos() const;
  void setSaveWindowsPos(bool saveMwp) const;

  // Save drive visibility option //
  [[nodiscard]] bool saveDiskVis() const;
  void setSaveDiskVis(bool saveDvis) const;

  // To pass session file name/path  //
  void setSessionFile(const QString &g_sessionFile, const QString &g_sessionFilePath);

  // To manipulate session files  //
  void copySettings(QSettings& setFrom, QSettings& setTo);
  void saveSessionToFile(const QString &fileName);
  void loadSessionFromFile(const QString &fileName);

  // To manipulate Main Window Title for Session file names    //
  void setMainWindowTitle(const QString &g_mainWindowTitle);

  // Hide/Show drives D9-DO   //
  [[nodiscard]] bool D9DOVisible() const;
  void setD9DOVisible(bool dVis) const;

  // Filter special characters from file names in Folder Images
  [[nodiscard]] bool filterUnderscore() const;
  void setfilterUnderscore(bool filter) const;

  // Limit number of entries in Folder Images to 64 or not.
  [[nodiscard]] bool limitFileEntries() const;
  void setlimitFileEntries(bool limit) const;

  // CAPITAL letters in file names for PCLINK
  [[nodiscard]] bool capitalLettersInPCLINK() const;
  void setCapitalLettersInPCLINK(bool caps) const;

  // URL Submit feature
  [[nodiscard]] bool isURLSubmitEnabled() const;
  void setURLSubmit(bool enabled) const;

  // Spy Mode
  [[nodiscard]] bool isSpyMode() const;
  void setSpyMode(bool enabled) const;

  // Command Name
  [[nodiscard]] bool isCommandName() const;
  void setCommandName(bool enabled) const;

  // Track Layout
  [[nodiscard]] bool isTrackLayout() const;
  void setTrackLayout(bool enabled) const;

  // Enable Shade Mode //
  [[nodiscard]] bool enableShade() const;
  void setEnableShade(bool shade) const;

  // Explorer Window On Top
  [[nodiscard]] bool explorerOnTop() const;
  void setExplorerOnTop(bool expOnTop) const;

  // save/restore top-level widget geometry
  bool saveWidgetGeometry(const QWidget* widget, const QString& name = QString()) const;
  bool restoreWidgetGeometry(QWidget* widget, const QString& name = QString(), const QRect& defRect = QRect()) const;
  bool windowPosSaved(const QWidget* widget, const QString& name = QString()) const;

  // Printer Spy Mode
  [[nodiscard]] bool isPrinterSpyMode() const;
  void setPrinterSpyMode(bool enabled) const;

  // 1020 Graphics Instructions
  [[nodiscard]] bool displayGraphicsInstructions() const;
  void setDisplayGraphicsInstructions(bool enabled) const;

  // Clear 1020 Graphics pane on STATUS command
  [[nodiscard]] bool clearOnStatus() const;
  void setClearOnStatus(bool enabled) const;

  // Methods for setting and getting the Printer emulation settings.
  void setPrinterName(int no, const QString &printerName) const;
  [[nodiscard]] QString printerName(int no) const;

  [[nodiscard]] QString atariFixedFontFamily() const;
  void setAtariFixedFontFamily(const QString& fontFamily) const;

  // Drive emulation (SIO or firmware)
  [[nodiscard]] bool displayTransmission() const;
  void setDisplayTransmission(bool displayTransmission) const;
  [[nodiscard]] bool displayDriveHead() const;
  void setDisplayDriveHead(bool displayDriveHead) const;
  [[nodiscard]] bool displayFdcCommands() const;
  void setDisplayFdcCommands(bool displayFdcCommands) const;
  [[nodiscard]] bool displayIndexPulse() const;
  void setDisplayIndexPulse(bool displayIndexPulse) const;
  [[nodiscard]] bool displayMotorOnOff() const;
  void setDisplayMotorOnOff(bool displayMotorOnOff) const;
  [[nodiscard]] bool displayIDAddressMarks() const;
  void setDisplayIDAddressMarks(bool displayIDAddressMarks) const;
  [[nodiscard]] bool displayTrackInformation() const;
  void setDisplayTrackInformation(bool displayTrackInformation) const;
  [[nodiscard]] bool disassembleUploadedCode() const;
  void setDisassembleUploadedCode(bool disassembleUploadedCode) const;
  [[nodiscard]] bool translatorAutomaticDetection() const;
  void setTranslatorAutomaticDetection(bool translatorAutomaticDetection) const;
  [[nodiscard]] QString translatorDiskImagePath() const;
  void setTranslatorDiskImagePath(const QString &diskImage) const;
  [[nodiscard]] QString toolDiskImagePath() const;
  void setToolDiskImagePath(const QString &diskImage) const;
  [[nodiscard]] bool sioAutoReconnect() const;
  void setSioAutoReconnect(bool sioAutoReconnect) const;
  [[nodiscard]] bool hideChipMode() const;
  void setHideChipMode(bool hidden) const;
  [[nodiscard]] bool hideHappyMode() const;
  void setHideHappyMode(bool hidden) const;
  [[nodiscard]] bool hideNextImage() const;
  void setHideNextImage(bool hidden) const;
  [[nodiscard]] bool hideOSBMode() const;
  void setHideOSBMode(bool hidden) const;
  [[nodiscard]] bool hideToolDisk() const;
  void setHideToolDisk(bool hidden) const;
  [[nodiscard]] bool activateChipModeWithTool() const;
  void setActivateChipModeWithTool(bool activate) const;
  [[nodiscard]] bool activateHappyModeWithTool() const;
  void setActivateHappyModeWithTool(bool activate) const;
  [[nodiscard]] bool displayCpuInstructions() const;
  void setDisplayCpuInstructions(bool displayCpuInstructions) const;
  [[nodiscard]] QString traceFilename() const;
  void setTraceFilename(const QString &filename) const;
#ifdef Q_OS_MAC
  void setNativeMenu(bool nativeMenu) const;
  [[nodiscard]] bool nativeMenu() const;
#endif
  void setRawPrinterName(const QString &name) const;
  [[nodiscard]] QString rawPrinterName() const;

  auto setTnfsEnabled(bool enable) const -> void;
  [[nodiscard]] auto isTnfsEnabled() const -> bool;

  // Disk Collection Browser
  [[nodiscard]] QString mostRecentBrowserFolder() const;
  [[nodiscard]] QStringList recentBrowserFolders() const;   // raw from QSettings map
  QStringList buildBrowserFolders();    // remove selected disk names from paths and validates exists
  void setMostRecentBrowserFolder(const QString& name) const;
  void delMostRecentBrowserFolder(const QString& name) const;
  bool isDiskImage(const QString& name);
  [[nodiscard]] bool showDiskBrowser() const;
  void setShowDiskBrowser(bool show = true) const;
  [[nodiscard]] int  diskBrowserHorzSplitPos() const;
  [[nodiscard]] int  diskBrowserVertSplitPos() const;
  void setDiskBrowserHorzSplitPos(int pos) const;
  void setDiskBrowserVertSplitPos(int pos) const;
  void setDiskPic(const QString& pic) const;
  [[nodiscard]] QString diskPic() const;

  // Disk Collection browser options page
  [[nodiscard]] DbDataSource dbDataSource() const;
  void setDbDataSource(DbDataSource newDbSource);
  void setDbFileNames(bool useFileNames, bool favorJson = false) const;
  [[nodiscard]] bool dbFavorJson() const;
  [[nodiscard]] bool dbUseFileNames() const;
  [[nodiscard]] bool dbCopyPics() const;
  void setDbCopyPics(bool copy) const;
  [[nodiscard]] LabelFont dbTitleFont() const;
  void setDbTitleFont(const LabelFont& font) const;
  [[nodiscard]] LabelFont dbIndexFont() const;
  void setDbIndexFont(const LabelFont& font) const;
  [[nodiscard]] QString appDataFolder() const;
  void setAppFolderDir(const QString& appDataDir) const;

  // Disk Collection Browser artwork settings -
  // These settings are kept seperately from above application global settings
  // when dbDataSource is JSON (DbData_subDirJson and DbData_appFolderJson).
  static const std::unique_ptr<DbSettings>& dbSettings();

  [[nodiscard]] bool debugMenuVisible() const;
  void setDebugMenuVisible(bool menuVisible) const;

  static const std::unique_ptr<RespeqtSettings> &instance() {
    if (!sInstance) {
      sInstance.reset(new RespeqtSettings);
    }
    return sInstance;
  }

private:
  RespeqtSettings();

  static std::unique_ptr<RespeqtSettings> sInstance;

public:
  QSettings *mSettings;

private:
  static std::unique_ptr<DbSettings> sDbSettings;

  void writeRecentBrowserFolders(const QStringList& folders) const;
//  const int maxRecentBrowserFolders = 10;

  bool mIsFirstTime;

  QString mSessionFileName;
  QString mSessionFilePath;
  QString mMainWindowTitle;
  //
};

Q_DECLARE_METATYPE(RespeqtSettings::ImageSettings)

#endif// RESPEQTSETTINGS_H
