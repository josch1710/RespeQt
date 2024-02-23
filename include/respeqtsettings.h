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

#include "mainwindow.h"
#include "diskbrowser/picsourcetype.h"
#include "diskbrowser/picpreview.h"

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

  bool isFirstTime();

  bool saveMainWinGeometry(QMainWindow* window, bool miniMode);
  bool restoreMainWinGeometry(QMainWindow* window, bool miniMode);

  QString serialPortName();
  void setSerialPortName(const QString &name);

  int serialPortHandshakingMethod();
  void setSerialPortHandshakingMethod(int method);

  bool serialPortTriggerOnFallingEdge();
  void setSerialPortTriggerOnFallingEdge(bool use);

  bool serialPortDTRControlEnable();
  void setSerialPortDTRControlEnable(bool use);

  int serialPortMaximumSpeed();
  void setSerialPortMaximumSpeed(int speed);

  bool serialPortUsePokeyDivisors();
  void setSerialPortUsePokeyDivisors(bool use);

  int serialPortPokeyDivisor();
  void setSerialPortPokeyDivisor(int divisor);

  int serialPortWriteDelay();
  void setSerialPortWriteDelay(int delay);

  int serialPortCompErrDelay();
  void setSerialPortCompErrDelay(int delay);

  QString atariSioDriverName();
  void setAtariSioDriverName(const QString &name);

  int atariSioHandshakingMethod();
  void setAtariSioHandshakingMethod(int method);

  SerialBackend backend();
  void setBackend(SerialBackend backend);

  bool useHighSpeedExeLoader();
  void setUseHighSpeedExeLoader(bool use);

  bool printerEmulation();
  void setPrinterEmulation(bool status);

  bool useCustomCasBaud();
  void setUseCustomCasBaud(bool use);

  int customCasBaud();
  void setCustomCasBaud(int baud);

  ImageSettings getImageSettingsFromName(const QString &fileName);
  ImageSettings mountedImageSetting(int no);
  ImageSettings recentImageSetting(int no);

  void setMountedImageSetting(int no, const QString &fileName, bool prot);
  void setMountedImageProtection(int no, bool prot);

  void mountImage(int no, const QString &fileName, bool prot);

  void unmountImage(int no);

  void swapImages(int no1, int no2);

  QString lastDiskImageDir();
  void setLastDiskImageDir(const QString &dir);

  QString lastFolderImageDir();
  void setLastFolderImageDir(const QString &dir);

  QString lastSessionDir();
  void setLastSessionDir(const QString &dir);

  QString lastExeDir();
  void setLastExeDir(const QString &dir);

  QString lastExtractDir();
  void setLastExtractDir(const QString &dir);

  QString lastPrinterTextDir();
  void setLastPrinterTextDir(const QString &dir);

  QString lastCasDir();
  void setLastCasDir(const QString &dir);

  QString lastRclDir();
  void setRclDir(const QString &dir);

  bool  showLogWindow();
  void  setShowLogWindow(bool show = true);

  // Set and restore last mainwindow position and size //
  const QPoint DefaultFullModePos  = { 100, 100 };
  const QSize  DefaultFullModeSize = { 800, 650 };
  const QRect  DefaultFullModeRect = { DefaultFullModePos, DefaultFullModeSize };

  // Set and restore last mini-window position //
  const QPoint DefaultMiniModePos  = { 8, 50 };
  const QSize  DefaultMiniModeSize = { 600, 100 };
  const QRect  DefaultMiniModeRect = { DefaultMiniModePos, DefaultMiniModeSize };

  bool miniMode();
  void setMiniMode(bool miniMode);

  QString i18nLanguage();
  void setI18nLanguage(const QString &lang);

  bool minimizeToTray();
  void setMinimizeToTray(bool tray);

  // Save window positions and sizes option //
  bool saveWindowsPos();
  void setSaveWindowsPos(bool saveMwp);

  // Save drive visibility option //
  bool saveDiskVis();
  void setSaveDiskVis(bool saveDvis);

  // To pass session file name/path  //
  void setSessionFile(const QString &g_sessionFile, const QString &g_sessionFilePath);

  // To manipulate session files  //
  void copySettings(QSettings& setFrom, QSettings& setTo);
  void saveSessionToFile(const QString &fileName);
  void loadSessionFromFile(const QString &fileName);

  // To manipulate Main Window Title for Session file names    //
  void setMainWindowTitle(const QString &g_mainWindowTitle);

  // Hide/Show drives D9-DO   //
  bool D9DOVisible();
  void setD9DOVisible(bool dVis);

  // Filter special characters from file names in Folder Images
  bool filterUnderscore();
  void setfilterUnderscore(bool filter);

  // Limit number of entries in Folder Images to 64 or not.
  bool limitFileEntries();
  void setlimitFileEntries(bool limit);

  // CAPITAL letters in file names for PCLINK
  bool capitalLettersInPCLINK();
  void setCapitalLettersInPCLINK(bool caps);

  // URL Submit feature
  bool isURLSubmitEnabled();
  void setURLSubmit(bool enabled);

  // Spy Mode
  bool isSpyMode();
  void setSpyMode(bool enabled);

  // Command Name
  bool isCommandName();
  void setCommandName(bool enabled);

  // Track Layout
  bool isTrackLayout();
  void setTrackLayout(bool enabled);

  // Enable Shade Mode //
  bool enableShade();
  void setEnableShade(bool shade);

  // Use Large Font //
  bool useLargeFont();
  void setUseLargeFont(bool largeFont);

  // Explorer Window On Top
  bool explorerOnTop();
  void setExplorerOnTop(bool expOnTop);

  // save/restore top-level widget geometry
  bool saveWidgetGeometry(QWidget* widget, const QString& name = QString());
  bool restoreWidgetGeometry(QWidget* widget, const QString& name = QString(), const QRect& defRect = QRect());

  // Printer Spy Mode
  bool isPrinterSpyMode();
  void setPrinterSpyMode(bool enabled);

  // 1020 Graphics Instructions
  bool displayGraphicsInstructions();
  void setDisplayGraphicsInstructions(bool enabled);

  // Clear 1020 Graphics pane on STATUS command
  bool clearOnStatus();
  void setClearOnStatus(bool enabled);

  // Methods for setting and getting the Printer emulation settings.
  void setPrinterName(int no, const QString &printerInfo);
  QString printerName(int no) const;

  QString atariFixedFontFamily();
  void setAtariFixedFontFamily(QString fontFamily);

  // Drive emulation (SIO or firmware)
  bool displayTransmission();
  void setDisplayTransmission(bool displayTransmission);
  bool displayDriveHead();
  void setDisplayDriveHead(bool displayDriveHead);
  bool displayFdcCommands();
  void setDisplayFdcCommands(bool displayFdcCommands);
  bool displayIndexPulse();
  void setDisplayIndexPulse(bool displayIndexPulse);
  bool displayMotorOnOff();
  void setDisplayMotorOnOff(bool displayMotorOnOff);
  bool displayIDAddressMarks();
  void setDisplayIDAddressMarks(bool displayIDAddressMarks);
  bool displayTrackInformation();
  void setDisplayTrackInformation(bool displayTrackInformation);
  bool disassembleUploadedCode();
  void setDisassembleUploadedCode(bool disassembleUploadedCode);
  bool translatorAutomaticDetection();
  void setTranslatorAutomaticDetection(bool translatorAutomaticDetection);
  QString translatorDiskImagePath();
  void setTranslatorDiskImagePath(const QString &diskImage);
  QString toolDiskImagePath();
  void setToolDiskImagePath(const QString &diskImage);
  bool sioAutoReconnect();
  void setSioAutoReconnect(bool sioAutoReconnect);
  bool hideChipMode();
  void setHideChipMode(bool hidden);
  bool hideHappyMode();
  void setHideHappyMode(bool hidden);
  bool hideNextImage();
  void setHideNextImage(bool hidden);
  bool hideOSBMode();
  void setHideOSBMode(bool hidden);
  bool hideToolDisk();
  void setHideToolDisk(bool hidden);
  bool activateChipModeWithTool();
  void setActivateChipModeWithTool(bool activate);
  bool activateHappyModeWithTool();
  void setActivateHappyModeWithTool(bool activate);
  bool displayCpuInstructions();
  void setDisplayCpuInstructions(bool displayCpuInstructions);
  QString traceFilename();
  void setTraceFilename(const QString &filename);
#ifdef Q_OS_MAC
  void setNativeMenu(bool nativeMenu);
  bool nativeMenu();
#endif
  void setRawPrinterName(const QString &name);
  QString rawPrinterName() const;

  // Disk Collection Browser
  QString mostRecentBrowserFolder();
  QStringList recentBrowserFolders();   // raw from QSettings map
  QStringList buildBrowserFolders();    // remove selected disk names from paths and validates exists
  void setMostRecentBrowserFolder(const QString& name);
  void delMostRecentBrowserFolder(const QString& name);
  bool isDiskImage(const QString& name);
  bool showDiskBrowser();
  void setShowDiskBrowser(bool show = true);
  int  diskBrowserHorzSplitPos();
  int  diskBrowserVertSplitPos();
  void setDiskBrowserHorzSplitPos(int pos);
  void setDiskBrowserVertSplitPos(int pos);
  void setDiskPic(const QString& pic);
  QString diskPic();

  // Disk Collection browser options page
  DbDataSource dbDataSource();
  void setDbDataSource(DbDataSource dbSource);
  void setDbFileNames(bool useFileNames, bool favorJson = false);
  bool dbFavorJson();
  bool dbUseFileNames();
  bool dbCopyPics();
  void setDbCopyPics(bool copy);
  LabelFont dbTitleFont();
  void setDbTitleFont(const LabelFont& font);
  LabelFont dbIndexFont();
  void setDbIndexFont(const LabelFont& font);
  QString appDataFolder();
  void setAppFolderDir(const QString& appDataDir);

  // Disk Collection Browser artwork settings -
  // These settings are kept seperately from above application global settings
  // when dbDataSource is JSON (DbData_subDirJson and DbData_appFolderJson).
  static const std::unique_ptr<DbSettings>& dbSettings();

  bool debugMenuVisible() const;
  void setDebugMenuVisible(bool menuVisible);

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

  void writeRecentBrowserFolders(const QStringList& folders);
//  const int maxRecentBrowserFolders = 10;

  bool mIsFirstTime;

  QString mSessionFileName;
  QString mSessionFilePath;
  QString mMainWindowTitle;
  //
};

Q_DECLARE_METATYPE(RespeqtSettings::ImageSettings)

#endif// RESPEQTSETTINGS_H
