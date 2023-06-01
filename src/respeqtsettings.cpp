/*
 * respeqtsettings.cpp
 *
 * Copyright 2015 Joseph Zatarski
 * Copyright 2016, 2017 TheMontezuma
 * Copyright 2017 blind
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "respeqtsettings.h"
#include "serialport.h"
//#include "mainwindow.h"

#include <memory>
#include <QApplication>

std::unique_ptr<RespeqtSettings> RespeqtSettings::sInstance;

RespeqtSettings::RespeqtSettings() {
  mSettings = new QSettings();//uses QApplication's info to determine setting to use

  mIsFirstTime = mSettings->value("FirstTime", true).toBool();
  mSettings->setValue("FirstTime", false);
}

RespeqtSettings::~RespeqtSettings() {
  delete mSettings;
}

// Get session file name from Mainwindow //
void RespeqtSettings::setSessionFile(const QString &g_sessionFile, const QString &g_sessionFilePath) {
  mSessionFileName = g_sessionFile;
  mSessionFilePath = g_sessionFilePath;
}

// Save all session related settings, so that a session could be fully restored later //
void RespeqtSettings::saveSessionToFile(const QString &fileName) {
  QSettings s(fileName, QSettings::IniFormat);

  s.beginGroup("RespeQt");
  s.setValue("Backend", static_cast<int>(backend()));
  s.setValue("AtariSioDriverName", atariSioDriverName());
  s.setValue("AtariSioHandshakingMethod", atariSioHandshakingMethod());
  s.setValue("SerialPortName", serialPortName());
  s.setValue("HandshakingMethod", serialPortHandshakingMethod());
  s.setValue("FallingEdge", serialPortTriggerOnFallingEdge());
  s.setValue("DTRControlEnable", serialPortDTRControlEnable());
  s.setValue("WriteDelay", serialPortWriteDelay());
  s.setValue("CompErrDelay", serialPortCompErrDelay());
  s.setValue("MaximumSerialPortSpeed", serialPortMaximumSpeed());
  s.setValue("SerialPortUsePokeyDivisors", serialPortUsePokeyDivisors());
  s.setValue("SerialPortPokeyDivisor", serialPortPokeyDivisor());
  s.setValue("UseHighSpeedExeLoader", useHighSpeedExeLoader());
  s.setValue("PrinterEmulation", printerEmulation());
  s.setValue("CustomCasBaud", customCasBaud());
  s.setValue("UseCustomCasBaud", useCustomCasBaud());
  s.setValue("I18nLanguage", i18nLanguage());
  s.setValue("SaveWindowsPosSize", saveWindowsPos());
  s.setValue("SaveDiskVisibility", saveDiskVis());
  s.setValue("D9DOVisible", D9DOVisible());
  s.setValue("MiniMode", miniMode());
  s.setValue("MiniX", lastMiniHorizontalPos());
  s.setValue("MiniY", lastMiniVerticalPos());
  s.setValue("MiniW", lastMiniWidth());
  s.setValue("MainX", lastHorizontalPos());
  s.setValue("MainY", lastVerticalPos());
  s.setValue("MainW", lastWidth());
  s.setValue("MainH", lastHeight());
  s.setValue("PrtX", lastPrtHorizontalPos());
  s.setValue("PrtY", lastPrtVerticalPos());
  s.setValue("PrtW", lastPrtWidth());
  s.setValue("PrtH", lastPrtHeight());
  s.setValue("FilterUnderscore", filterUnderscore());
  s.setValue("LimitFileEntries", limitFileEntries());
  s.setValue("CapitalLettersInPCLINK", capitalLettersInPCLINK());
  s.setValue("URLSubmit", isURLSubmitEnabled());
  s.setValue("SpyMode", isSpyMode());
  s.setValue("CommandName", isCommandName());
  s.setValue("TrackLayout", isTrackLayout());
  s.setValue("UseLargeFont", useLargeFont());
  s.setValue("ExplorerOnTop", explorerOnTop());
  s.setValue("EnableShadeByDefault", enableShade());
  s.setValue("PrinterSpyMode", isPrinterSpyMode());
  s.setValue("DisplayGraphicsInstructions", displayGraphicsInstructions());
  s.setValue("ClearOnStatus", clearOnStatus());
  s.setValue("DisplayTransmission", displayTransmission());
  s.setValue("DisplayFdcCommands", displayFdcCommands());
  s.setValue("DisplayIndexPulse", displayIndexPulse());
  s.setValue("DisplayMotorOnOff", displayMotorOnOff());
  s.setValue("DisplayIDAddressMarks", displayIDAddressMarks());
  s.setValue("DisplayTrackInformation", displayTrackInformation());
  s.setValue("DisassembleUploadedCode", disassembleUploadedCode());
  s.setValue("TranslatorAutomaticDetection", translatorAutomaticDetection());
  s.setValue("TranslatorDiskImagePath", translatorDiskImagePath());
  s.setValue("SioAutoReconnect", sioAutoReconnect());
  s.setValue("HideChipMode", hideChipMode());
  s.setValue("HideHappyMode", hideHappyMode());
  s.setValue("HideNextImage", hideNextImage());
  s.setValue("HideOSBMode", hideOSBMode());
  s.setValue("HideToolDisk", hideToolDisk());
  s.setValue("ToolDiskImagePath", toolDiskImagePath());
  s.setValue("ActivateChipModeWithTool", activateChipModeWithTool());
  s.setValue("ActivateHappyModeWithTool", activateHappyModeWithTool());
  s.setValue("DisplayCpuInstructions", displayCpuInstructions());
  s.setValue("TraceFilename", traceFilename());
  s.setValue("RawPrinterName", rawPrinterName());
  s.setValue("LastRclDir", lastRclDir());
  s.setValue("DebugMenuVisible", debugMenuVisible());
#ifdef Q_OS_MAC
  s.setValue("NativeMenu", nativeMenu());
#endif
  s.endGroup();
  //
  s.beginWriteArray("MountedImageSettings");
  for (auto i = 0; i < 15; i++) {//
    auto is = mountedImageSetting(i);
    s.setArrayIndex(i);
    s.setValue("FileName", is.fileName);
    s.setValue("IsWriteProtected", is.isWriteProtected);
  }
  s.endArray();

  s.beginWriteArray("ConnectedPrinterSettings");
  for (auto i = 0; i < PRINTER_COUNT; i++) {
    s.setArrayIndex(i);
    s.setValue("PrinterName", printerName(i));
  }
  s.endArray();
}

// Get all session related settings, so that a session could be fully restored //
void RespeqtSettings::loadSessionFromFile(const QString &fileName) {
  QSettings s(fileName, QSettings::IniFormat);
  s.beginGroup("RespeQt");
  // danc: TBD - remove hard-coded 'magic' numbers from this code block
  setBackend(static_cast<SerialBackend>(s.value("Backend", 0).toInt()));
  setAtariSioDriverName(s.value("AtariSioDriverName", AtariSioBackend::defaultPortName()).toString());
  setAtariSioHandshakingMethod(s.value("AtariSioHandshakingMethod", 0).toInt());
  setSerialPortName(s.value("SerialPortName", StandardSerialPortBackend::defaultPortName()).toString());
  setSerialPortHandshakingMethod(s.value("HandshakingMethod", 0).toInt());
  setSerialPortTriggerOnFallingEdge(s.value("FallingEdge", false).toBool());
  setRclDir(mSettings->value("LastRclDir", "").toString());
  setSerialPortDTRControlEnable(s.value("DTRControlEnable", false).toBool());
  setSerialPortWriteDelay(s.value("WriteDelay", 1).toInt());
  setSerialPortCompErrDelay(s.value("CompErrDelay", 1).toInt());
  setSerialPortMaximumSpeed(s.value("MaximumSerialPortSpeed", 2).toInt());
  setSerialPortUsePokeyDivisors(s.value("SerialPortUsePokeyDivisors", false).toBool());
  setSerialPortPokeyDivisor(s.value("SerialPortPokeyDivisor", 6).toInt());
  setUseHighSpeedExeLoader(s.value("UseHighSpeedExeLoader", false).toBool());
  setPrinterEmulation(s.value("PrinterEmulation", true).toBool());
  setCustomCasBaud(s.value("CustomCasBaud", 875).toInt());
  setUseCustomCasBaud(s.value("UseCustomCasBaud", false).toBool());
  setI18nLanguage(s.value("I18nLanguage").toString());
  setSaveWindowsPos(s.value("SaveWindowsPosSize", true).toBool());
  setSaveDiskVis(s.value("SaveDiskVisibility", true).toBool());
  setMiniMode(s.value("MiniMode", false).toBool());
  setD9DOVisible(s.value("D9DOVisible", true).toBool());
  setLastHorizontalPos(s.value("MainX", 20).toInt());
  setLastVerticalPos(s.value("MainY", 40).toInt());
  setLastWidth(s.value("MainW", DefaultFullModeSize.width()).toInt());
  setLastHeight(s.value("MainH", DefaultFullModeSize.height()).toInt());
  //if (mMainW < 688 && mdVis) mMainW = 688;
  //if (mMainH < 426 && mdVis) mMainH = 426;
  setLastMiniHorizontalPos(s.value("MiniX", DefaultMiniModePos.x()).toInt());
  setLastMiniVerticalPos(s.value("MiniY", DefaultMiniModePos.y()).toInt());
  setLastMiniWidth(s.value("MiniW", DefaultMiniModeSize.width()).toInt());
  setLastPrtHorizontalPos(s.value("PrtX", 20).toInt());
  setLastPrtVerticalPos(s.value("PrtY", 40).toInt());
  setLastPrtWidth(s.value("PrtW", 600).toInt());
  setLastPrtHeight(s.value("PrtH", 486).toInt());
  setfilterUnderscore(s.value("FilterUnderscore", true).toBool());
  setlimitFileEntries(s.value("LimitFileEntries", true).toBool());
  setCapitalLettersInPCLINK(s.value("CapitalLettersInPCLINK", false).toBool());
  setURLSubmit(s.value("URLSubmit", false).toBool());
  setSpyMode(s.value("SpyMode", false).toBool());
  setCommandName(s.value("CommandName", false).toBool());
  setTrackLayout(s.value("TrackLayout", false).toBool());
  setUseLargeFont(s.value("UseLargeFont", false).toBool());
  setExplorerOnTop(s.value("ExplorerOnTop", false).toBool());
  setEnableShade(s.value("EnableShadeByDefault", true).toBool());
  setDisplayTransmission(s.value("DisplayTransmission", false).toBool());
  setDisplayDriveHead(s.value("DisplayDriveHead", false).toBool());
  setDisplayFdcCommands(s.value("DisplayFdcCommands", false).toBool());
  setDisplayIndexPulse(s.value("DisplayIndexPulse", false).toBool());
  setDisplayMotorOnOff(s.value("DisplayMotorOnOff", false).toBool());
  setDisplayIDAddressMarks(s.value("DisplayIDAddressMarks", false).toBool());
  setDisplayTrackInformation(s.value("DisplayTrackInformation", false).toBool());
  setDisassembleUploadedCode(s.value("DisassembleUploadedCode", false).toBool());
  setTranslatorAutomaticDetection(s.value("TranslatorAutomaticDetection", false).toBool());
  setTranslatorDiskImagePath(s.value("TranslatorDiskImagePath", false).toString());
  setSioAutoReconnect(s.value("SioAutoReconnect", false).toBool());
  setHideChipMode(s.value("HideChipMode", false).toBool());
  setHideHappyMode(s.value("HideHappyMode", false).toBool());
  setHideNextImage(s.value("HideNextImage", false).toBool());
  setHideOSBMode(s.value("HideOSBMode", false).toBool());
  setHideToolDisk(s.value("HideToolDisk", false).toBool());
  setToolDiskImagePath(s.value("ToolDiskImagePath", false).toString());
  setActivateChipModeWithTool(s.value("ActivateChipModeWithTool", false).toBool());
  setActivateHappyModeWithTool(s.value("ActivateHappyModeWithTool", false).toBool());
  setDisplayCpuInstructions(s.value("DisplayCpuInstructions", false).toBool());
  setTraceFilename(s.value("TraceFilename", "").toString());
  setRawPrinterName(s.value("RawPrinterName", "").toString());
  setPrinterSpyMode(s.value("PrinterSpyMode", false).toBool());
  setDisplayGraphicsInstructions(s.value("DisplayGraphicsInstructions", true).toBool());
  setClearOnStatus(s.value("ClearOnStatus", false).toBool());
  setDebugMenuVisible(s.value("DebugMenuVisible", false).toBool());

#ifdef Q_OS_MAC
  setNativeMenu(s.value("NativeMenu", false).toBool());
#endif
  s.endGroup();
  //
  s.beginReadArray("MountedImageSettings");
  for (auto i = 0; i < 15; i++) {//
    s.setArrayIndex(i);
    setMountedImageSetting(i, s.value("FileName", "").toString(), s.value("IsWriteProtected", false).toBool());
  }
  s.endArray();

  s.beginReadArray("ConnectedPrinterSettings");
  for (auto i = 0; i < PRINTER_COUNT; i++) {
    s.setArrayIndex(i);
    setPrinterName(i, s.value("PrinterName", "").toString());
  }
}

// Get MainWindow title from MainWindow  //
void RespeqtSettings::setMainWindowTitle(const QString &g_mainWindowTitle) {
  mMainWindowTitle = g_mainWindowTitle;
}

bool RespeqtSettings::isFirstTime() {
  return mIsFirstTime;
}

void RespeqtSettings::saveGeometry(const QRect &geometry, bool isMiniMode) {
  if (saveWindowsPos()) {
    setMiniMode(isMiniMode);
    if (isMiniMode) {
      setLastMiniHorizontalPos(geometry.x());
      setLastMiniVerticalPos(geometry.y());
      setLastMiniWidth(geometry.width());
    } else {
      setLastHorizontalPos(geometry.x());
      setLastVerticalPos(geometry.y());
      setLastWidth(geometry.width());
      setLastHeight(geometry.height());
    }
  }
}

QString RespeqtSettings::serialPortName() {
  return mSettings->value("SerialPortName").toString();
}

void RespeqtSettings::setSerialPortName(const QString &name) {
  mSettings->setValue("SerialPortName", name);
}

int RespeqtSettings::serialPortMaximumSpeed() {
  return mSettings->value("MaximumSerialPortSpeed").toInt();
}

void RespeqtSettings::setSerialPortMaximumSpeed(int speed) {
  mSettings->setValue("MaximumSerialPortSpeed", speed);
}

bool RespeqtSettings::serialPortUsePokeyDivisors() {
  return mSettings->value("SerialPortUsePokeyDivisors").toBool();
}

void RespeqtSettings::setSerialPortUsePokeyDivisors(bool use) {
  mSettings->setValue("SerialPortUsePokeyDivisors", use);
}

int RespeqtSettings::serialPortPokeyDivisor() {
  return mSettings->value("").toInt();
}

void RespeqtSettings::setSerialPortPokeyDivisor(int divisor) {
  mSettings->setValue("SerialPortPokeyDivisor", divisor);
}

int RespeqtSettings::serialPortHandshakingMethod() {
  return mSettings->value("HandshakingMethod").toInt();
}

void RespeqtSettings::setSerialPortHandshakingMethod(int method) {
  mSettings->setValue("HandshakingMethod", method);
}

bool RespeqtSettings::serialPortTriggerOnFallingEdge() {
  return mSettings->value("FallingEdge").toBool();
}

void RespeqtSettings::setSerialPortTriggerOnFallingEdge(bool use) {
  mSettings->setValue("FallingEdge", use);
}

bool RespeqtSettings::serialPortDTRControlEnable() {
  return mSettings->value("DTRControlEnable").toBool();
}

void RespeqtSettings::setSerialPortDTRControlEnable(bool use) {
  mSettings->setValue("DTRControlEnable", use);
}

int RespeqtSettings::serialPortWriteDelay() {
  return mSettings->value("WriteDelay").toInt();
}

void RespeqtSettings::setSerialPortWriteDelay(int delay) {
  mSettings->setValue("WriteDelay", delay);
}

int RespeqtSettings::serialPortCompErrDelay() {
  return mSettings->value("CompErrDelay").toInt();
}

void RespeqtSettings::setSerialPortCompErrDelay(int delay) {
  mSettings->setValue("CompErrDelay", delay);
}

QString RespeqtSettings::atariSioDriverName() {
  return mSettings->value("AtariSioDriverName").toString();
}

void RespeqtSettings::setAtariSioDriverName(const QString &name) {
  mSettings->setValue("AtariSioDriverName", name);
}

int RespeqtSettings::atariSioHandshakingMethod() {
  return mSettings->value("AtariSioHandshakingMethod").toInt();
}

void RespeqtSettings::setAtariSioHandshakingMethod(int method) {
  mSettings->setValue("AtariSioHandshakingMethod", method);
}

SerialBackend RespeqtSettings::backend() {
  return static_cast<SerialBackend>(mSettings->value("Backend").toInt());
}

void RespeqtSettings::setBackend(SerialBackend backend) {
  mSettings->setValue("Backend", static_cast<int>(backend));
}

QString RespeqtSettings::lastRclDir() {
  return mSettings->value("LastRclDir").toString();
}

void RespeqtSettings::setRclDir(const QString &dir) {
  mSettings->setValue("LastRclDir", dir);
}

bool RespeqtSettings::useHighSpeedExeLoader() {
  return mSettings->value("UseHighSpeedExeLoader").toBool();
}

void RespeqtSettings::setUseHighSpeedExeLoader(bool use) {
  mSettings->setValue("UseHighSpeedExeLoader", use);
}

bool RespeqtSettings::printerEmulation() {
  return mSettings->value("PrinterEmulation").toBool();
}

void RespeqtSettings::setPrinterEmulation(bool status) {
  mSettings->setValue("PrinterEmulation", status);
}

bool RespeqtSettings::useCustomCasBaud() {
  return mSettings->value("UseCustomCasBaud").toBool();
}

void RespeqtSettings::setUseCustomCasBaud(bool use) {
  mSettings->setValue("UseCustomCasBaud", use);
}

int RespeqtSettings::customCasBaud() {
  return mSettings->value("CustomCasBaud").toInt();
}

void RespeqtSettings::setCustomCasBaud(int baud) {
  mSettings->setValue("CustomCasBaud", baud);
}

RespeqtSettings::ImageSettings RespeqtSettings::getImageSettingsFromName(const QString &fileName) {
  mSettings->beginReadArray("MountedImageSettings");
  for (int i = 0; i < DISK_COUNT; i++) {
    mSettings->setArrayIndex(i);
    if (mSettings->value("fileName").toString() == fileName) {
      ImageSettings image;
      image.fileName = mSettings->value("fileName").toString();
      image.isWriteProtected = mSettings->value("isWriteProtected").toBool();
      mSettings->endArray();
      return image;
    }
  }
  mSettings->endArray();

  // We didn't found anything in the list of mounted images
  mSettings->beginReadArray("RecentImageSettings");
  for (int i = 0; i < NUM_RECENT_FILES; i++) {
    mSettings->setArrayIndex(i);
    if (mSettings->value("fileName").toString() == fileName) {
      ImageSettings image;
      image.fileName = mSettings->value("fileName").toString();
      image.isWriteProtected = mSettings->value("isWriteProtected").toBool();
      mSettings->endArray();
      return image;
    }
  }
  mSettings->endArray();

  return ImageSettings{};
}

RespeqtSettings::ImageSettings RespeqtSettings::mountedImageSetting(int no) {
  RespeqtSettings::ImageSettings is;
  is.fileName = mSettings->value(QString("MountedImageSettings/%1/FileName").arg(no)).toString();
  is.isWriteProtected = mSettings->value(QString("MountedImageSettings/%1/IsWriteProtected").arg(no)).toBool();
  return is;
}

RespeqtSettings::ImageSettings RespeqtSettings::recentImageSetting(int no) {
  ImageSettings is;
  is.fileName = mSettings->value(QString("RecentImageSettings/%1/FileName").arg(no)).toString();
  is.isWriteProtected = mSettings->value(QString("RecentImageSettings/%1/IsWriteProtected").arg(no)).toBool();
  return is;
}

void RespeqtSettings::setMountedImageProtection(int no, bool prot) {
  mSettings->setValue(QString("MountedImageSettings/%1/IsWriteProtected").arg(no), prot);
}

void RespeqtSettings::setMountedImageSetting(int no, const QString &fileName, bool prot) {
  mSettings->setValue(QString("MountedImageSettings/%1/FileName").arg(no), fileName);
  mSettings->setValue(QString("MountedImageSettings/%1/IsWriteProtected").arg(no), prot);
}

void RespeqtSettings::mountImage(int no, const QString &fileName, bool isWriteProtected) {
  if (fileName.isEmpty()) {
    return;
  }

  int i;
  bool found = false;
  mSettings->beginReadArray("RecentImageSettings");
  for (i = 0; i < NUM_RECENT_FILES; i++) {
    mSettings->setArrayIndex(i);
    if (mSettings->value("FileName").toString() == fileName) {
      found = true;
      break;
    }
  }
  mSettings->endArray();

  if (found) {
    mSettings->beginWriteArray("RecentImageSettings");
    for (int j = i; j < NUM_RECENT_FILES - 1; j++) {
      mSettings->setArrayIndex(j);
      auto image = mSettings->value(QString("RecentImageSettings/%1").arg(j + 1)).value<ImageSettings>();
      mSettings->setValue("FileName", image.fileName);
      mSettings->setValue("IsWriteProtected", image.isWriteProtected);
    }
    mSettings->endArray();
    i = NUM_RECENT_FILES - 1;
  }
  else {
    mSettings->beginReadArray("RecentImageSettings");
    for(quint8 j = 0; j < NUM_RECENT_FILES; j++) {
      mSettings->setArrayIndex(i);
      if (!mSettings->value("FileName").toString().isEmpty())
        continue;

      i = j;
    }
    mSettings->endArray();
  }

  mSettings->setValue(QString("RecentImageSettings/%1/FileName").arg(i), fileName);
  mSettings->setValue(QString("RecentImageSettings/%1/IsWriteProtected").arg(i), isWriteProtected);

  setMountedImageSetting(no, fileName, isWriteProtected);
}

void RespeqtSettings::unmountImage(int no) {
  auto is = mSettings->value(QString("MountedImageSettings/%1").arg(no)).value<ImageSettings>();

  for (int i = (NUM_RECENT_FILES - 1); i > 0; i--) {
    auto image = mSettings->value(QString("RecentImageSettings/%1").arg(no)).value<ImageSettings>();
    mSettings->setValue(QString("RecentImageSettings/%1/FileName").arg(no), image.fileName);
    mSettings->setValue(QString("RecentImageSettings/%1/IsWriteProtected").arg(no), image.isWriteProtected);
  }

  mSettings->setValue("RecentImageSettings/0/FileName", is.fileName);
  mSettings->setValue("RecentImageSettings/0/IsWriteProtected", is.isWriteProtected);

  setMountedImageSetting(no, "", false);
}

void RespeqtSettings::swapImages(int no1, int no2) {
  ImageSettings is1 = mountedImageSetting(no1);
  ImageSettings is2 = mountedImageSetting(no2);
  setMountedImageSetting(no1, is2.fileName, is2.isWriteProtected);
  setMountedImageSetting(no2, is1.fileName, is1.isWriteProtected);
}

// Save drive visibility status //
bool RespeqtSettings::saveDiskVis() {
  return mSettings->value("SaveDiskVisibility", true).toBool();
}

void RespeqtSettings::setSaveDiskVis(bool saveDvis) {
  mSettings->setValue("SaveDiskVisibility", saveDvis);
}

// Drive visibility status //
bool RespeqtSettings::D9DOVisible() {
  return mSettings->value("D9DOVisible", true).toBool();
}

void RespeqtSettings::setD9DOVisible(bool dVis) {
  if (saveDiskVis()) {
    mSettings->setValue("D9DOVisible", dVis);
  }
}

// Shade Mode Enable //
bool RespeqtSettings::enableShade() {
  return mSettings->value("EnableShadeByDefault", false).toBool();
}

void RespeqtSettings::setEnableShade(bool shade) {
  mSettings->setValue("EnableShadeByDefault", shade);
}

// Use Large Font //
bool RespeqtSettings::useLargeFont() {
  return mSettings->value("UseLargeFont", false).toBool();
}

void RespeqtSettings::setUseLargeFont(bool largeFont) {
  mSettings->setValue("UseLargeFont", largeFont);
}

// Explorer Window always on top
bool RespeqtSettings::explorerOnTop() {
  return mSettings->value("ExplorerOnTop").toBool();
}

void RespeqtSettings::setExplorerOnTop(bool expOnTop) {
  mSettings->setValue("ExplorerOnTop", expOnTop);
}

// Save/return last main window position/size option //
bool RespeqtSettings::saveWindowsPos() {
  return mSettings->value("SaveWindowsPosSize", true).toBool();
}

void RespeqtSettings::setSaveWindowsPos(bool saveMwp) {
  mSettings->setValue("SaveWindowsPosSize", saveMwp);
}
// Last main window position/size (No Session File) //

int RespeqtSettings::lastHorizontalPos() {
  return mSettings->value("MainX", DefaultFullModePos.x()).toInt();
}

void RespeqtSettings::setLastHorizontalPos(int lastHpos) {
  mSettings->setValue("MainX", lastHpos);
}

int RespeqtSettings::lastVerticalPos() {
  return mSettings->value("MainY", DefaultFullModePos.y()).toInt();
}

void RespeqtSettings::setLastVerticalPos(int lastVpos) {
  mSettings->setValue("MainY", lastVpos);
}
int RespeqtSettings::lastWidth() {
  return mSettings->value("MainW", DefaultFullModeSize.width()).toInt();
}

void RespeqtSettings::setLastWidth(int lastW) {
  mSettings->setValue("MainW", lastW);
}

int RespeqtSettings::lastHeight() {
  return mSettings->value("MainH", DefaultFullModeSize.height()).toInt();
}

void RespeqtSettings::setLastHeight(int lastH) {
  mSettings->setValue("MainH", lastH);
}

// mini mode and it's specific window position and width
bool RespeqtSettings::miniMode() {
  return mSettings->value("MiniMode", false).toBool();
}

void RespeqtSettings::setMiniMode(bool miniMode) {
  mSettings->setValue("MiniMode", miniMode);
}

int RespeqtSettings::lastMiniHorizontalPos() {
  return mSettings->value("MiniX", DefaultMiniModePos.x()).toInt();
}

void RespeqtSettings::setLastMiniHorizontalPos(int lastMHpos) {
  mSettings->setValue("MiniX", lastMHpos);
}

int RespeqtSettings::lastMiniVerticalPos() {
  return mSettings->value("MiniY", DefaultMiniModePos.y()).toInt();
}

void RespeqtSettings::setLastMiniVerticalPos(int lastMVpos) {
  mSettings->setValue("MiniY", lastMVpos);
}

int RespeqtSettings::lastMiniWidth() {
  return mSettings->value("MiniW", DefaultMiniModeSize.width()).toInt();
}

void RespeqtSettings::setLastMiniWidth(int lastWidth) {
  mSettings->setValue("MiniW", lastWidth);
}

// Last print window position/size (No Session File) //
int RespeqtSettings::lastPrtHorizontalPos() {
  return mSettings->value("PrtX").toInt();
}

void RespeqtSettings::setLastPrtHorizontalPos(int lastPrtHpos) {
  mSettings->setValue("PrtX", lastPrtHpos);
}

int RespeqtSettings::lastPrtVerticalPos() {
  return mSettings->value("PrtY").toInt();
}

void RespeqtSettings::setLastPrtVerticalPos(int lastPrtVpos) {
  mSettings->setValue("PrtY", lastPrtVpos);
}

int RespeqtSettings::lastPrtWidth() {
  return mSettings->value("PrtW").toInt();
}

void RespeqtSettings::setLastPrtWidth(int lastPrtW) {
  mSettings->setValue("PrtW", lastPrtW);
}

int RespeqtSettings::lastPrtHeight() {
  return mSettings->value("PrtH").toInt();
}

void RespeqtSettings::setLastPrtHeight(int lastPrtH) {
  mSettings->setValue("PrtH", lastPrtH);
}

QString RespeqtSettings::lastDiskImageDir() {
  return mSettings->value("LastDiskImageDir").toString();
}

void RespeqtSettings::setLastDiskImageDir(const QString &dir) {
  mSettings->setValue("LastDiskImageDir", dir);
}

QString RespeqtSettings::lastFolderImageDir() {
  return mSettings->value("LastFolderImageDir").toString();
}

void RespeqtSettings::setLastFolderImageDir(const QString &dir) {
  mSettings->setValue("LastFolderImageDir", dir);
}

QString RespeqtSettings::lastSessionDir() {
  return mSettings->value("LastSessionDir").toString();
}

void RespeqtSettings::setLastSessionDir(const QString &dir) {
  mSettings->setValue("LastSessionDir", dir);
}

QString RespeqtSettings::lastExeDir() {
  return mSettings->value("LastExeDir").toString();
}

void RespeqtSettings::setLastExeDir(const QString &dir) {
  mSettings->setValue("LastExeDir", dir);
}

QString RespeqtSettings::lastExtractDir() {
  return mSettings->value("LastExtractDir").toString();
}

void RespeqtSettings::setLastExtractDir(const QString &dir) {
  mSettings->setValue("LastExtractDir", dir);
}

QString RespeqtSettings::lastPrinterTextDir() {
  return mSettings->value("LastPrinterTextDir").toString();
}

void RespeqtSettings::setLastPrinterTextDir(const QString &dir) {
  mSettings->setValue("LastPrinterTextDir", dir);
}

QString RespeqtSettings::lastCasDir() {
  return mSettings->value("LastCasDir").toString();
}

void RespeqtSettings::setLastCasDir(const QString &dir) {
  mSettings->setValue("LastCasDir", dir);
}

QString RespeqtSettings::i18nLanguage() {
  return mSettings->value("I18nLanguage").toString();
}

void RespeqtSettings::setI18nLanguage(const QString &lang) {
  mSettings->setValue("I18nLanguage", lang);
}

bool RespeqtSettings::minimizeToTray() {
  return mSettings->value("MinimizeToTray").toBool();
}

void RespeqtSettings::setMinimizeToTray(bool tray) {
  mSettings->setValue("MinimizeToTray", tray);
}

bool RespeqtSettings::filterUnderscore() {
  return mSettings->value("FilterUnderscore").toBool();
}

void RespeqtSettings::setfilterUnderscore(bool filter) {
  mSettings->setValue("FilterUnderscore", filter);
}

bool RespeqtSettings::limitFileEntries() {
  return mSettings->value("LimitFileEntries").toBool();
}

void RespeqtSettings::setlimitFileEntries(bool limit) {
  mSettings->setValue("LimitFileEntries", limit);
}

bool RespeqtSettings::capitalLettersInPCLINK() {
  return mSettings->value("CapitalLettersInPCLINK").toBool();
}

void RespeqtSettings::setCapitalLettersInPCLINK(bool caps) {
  mSettings->setValue("CapitalLettersInPCLINK", caps);
}

bool RespeqtSettings::isURLSubmitEnabled() {
  return mSettings->value("URLSubmit").toBool();
}

void RespeqtSettings::setURLSubmit(bool enabled) {
  mSettings->setValue("URLSubmit", enabled);
}

bool RespeqtSettings::isSpyMode() {
  return mSettings->value("SpyMode").toBool();
}

void RespeqtSettings::setSpyMode(bool enabled) {
  mSettings->setValue("SpyMode", enabled);
}

bool RespeqtSettings::isCommandName() {
  return mSettings->value("CommandName").toBool();
}

void RespeqtSettings::setCommandName(bool enabled) {
  mSettings->setValue("CommandName", enabled);
}

bool RespeqtSettings::isTrackLayout() {
  return mSettings->value("TrackLayout").toBool();
}

void RespeqtSettings::setTrackLayout(bool enabled) {
  mSettings->setValue("TrackLayout", enabled);
}

void RespeqtSettings::setPrinterName(int no, const QString &printerName) {
  mSettings->setValue(QString("ConnectedPrinterSettings/%1/PrinterName").arg(no), printerName);
}

QString RespeqtSettings::printerName(int no) const {
  return mSettings->value(QString("ConnectedPrinterSettings/%1/PrinterName").arg(no)).toString();
}

QString RespeqtSettings::atariFixedFontFamily() {
  return mSettings->value("AtariFixedFontFamily").toString();
}

void RespeqtSettings::setAtariFixedFontFamily(QString fontFamily) {
  mSettings->setValue("AtariFixedFontFamily", fontFamily);
}

bool RespeqtSettings::isPrinterSpyMode() {
  return mSettings->value("PrinterSpyMode").toBool();
}

void RespeqtSettings::setPrinterSpyMode(bool enabled) {
  mSettings->setValue("PrinterSpyMode", enabled);
}

bool RespeqtSettings::displayGraphicsInstructions() {
  return mSettings->value("DisplayGraphicsInstructions").toBool();
}

void RespeqtSettings::setDisplayGraphicsInstructions(bool enabled) {
  mSettings->setValue("DisplayGraphicsInstructions", enabled);
}

bool RespeqtSettings::clearOnStatus() {
  return mSettings->value("ClearOnStatus").toBool();
}

void RespeqtSettings::setClearOnStatus(bool enabled) {
  mSettings->setValue("ClearOnStatus", enabled);
}

bool RespeqtSettings::displayTransmission() {
  return mSettings->value("DisplayTransmission").toBool();
}

void RespeqtSettings::setDisplayTransmission(bool displayTransmission) {
  mSettings->setValue("DisplayTransmission", displayTransmission);
}

bool RespeqtSettings::displayDriveHead() {
  return mSettings->value("DisplayDriveHead").toBool();
}

void RespeqtSettings::setDisplayDriveHead(bool displayDriveHead) {
  mSettings->setValue("DisplayDriveHead", displayDriveHead);
}

bool RespeqtSettings::displayFdcCommands() {
  return mSettings->value("displayFdcCommands").toBool();
}

void RespeqtSettings::setDisplayFdcCommands(bool displayFdcCommands) {
  mSettings->setValue("DisplayFdcCommands", displayFdcCommands);
}

bool RespeqtSettings::displayIndexPulse() {
  return mSettings->value("DisplayIndexPulse").toBool();
}

void RespeqtSettings::setDisplayIndexPulse(bool displayIndexPulse) {
  mSettings->setValue("DisplayIndexPulse", displayIndexPulse);
}

bool RespeqtSettings::displayMotorOnOff() {
  return mSettings->value("DisplayMotorOnOff").toBool();
}

void RespeqtSettings::setDisplayMotorOnOff(bool displayMotorOnOff) {
  mSettings->setValue("DisplayMotorOnOff", displayMotorOnOff);
}

bool RespeqtSettings::displayIDAddressMarks() {
  return mSettings->value("DisplayIDAddressMarks").toBool();
}

void RespeqtSettings::setDisplayIDAddressMarks(bool displayIDAddressMarks) {
  mSettings->setValue("DisplayIDAddressMarks", displayIDAddressMarks);
}

bool RespeqtSettings::displayTrackInformation() {
  return mSettings->value("DisplayTrackInformation").toBool();
}

void RespeqtSettings::setDisplayTrackInformation(bool displayTrackInformation) {
  mSettings->setValue("DisplayTrackInformation", displayTrackInformation);
}

bool RespeqtSettings::disassembleUploadedCode() {
  return mSettings->value("DisassembleUploadedCode").toBool();
}

void RespeqtSettings::setDisassembleUploadedCode(bool disassembleUploadedCode) {
  mSettings->setValue("DisassembleUploadedCode", disassembleUploadedCode);
}

bool RespeqtSettings::translatorAutomaticDetection() {
  return mSettings->value("TranslatorAutomaticDetection").toBool();
}

void RespeqtSettings::setTranslatorAutomaticDetection(bool translatorAutomaticDetection) {
  mSettings->setValue("TranslatorAutomaticDetection", translatorAutomaticDetection);
}

bool RespeqtSettings::sioAutoReconnect() {
  return mSettings->value("SioAutoReconnect").toBool();
}

void RespeqtSettings::setSioAutoReconnect(bool sioAutoReconnect) {
  mSettings->setValue("SioAutoReconnect", sioAutoReconnect);
}

bool RespeqtSettings::hideChipMode() {
  return mSettings->value("HideChipMode").toBool();
}

void RespeqtSettings::setHideChipMode(bool hidden) {
  mSettings->setValue("HideChipMode", hidden);
}

bool RespeqtSettings::hideHappyMode() {
  return mSettings->value("HideHappyMode").toBool();
}

void RespeqtSettings::setHideHappyMode(bool hidden) {
  mSettings->setValue("HideHappyMode", hidden);
}

bool RespeqtSettings::hideNextImage() {
  return mSettings->value("HideNextImage").toBool();
}

void RespeqtSettings::setHideNextImage(bool hidden) {
  mSettings->setValue("HideNextImage", hidden);
}

bool RespeqtSettings::hideOSBMode() {
  return mSettings->value("HideOSBMode").toBool();
}

void RespeqtSettings::setHideOSBMode(bool hidden) {
  mSettings->setValue("HideOSBMode", hidden);
}

bool RespeqtSettings::hideToolDisk() {
  return mSettings->value("HideToolDisk").toBool();
}

void RespeqtSettings::setHideToolDisk(bool hidden) {
  mSettings->setValue("HideToolDisk", hidden);
}

QString RespeqtSettings::translatorDiskImagePath() {
  return mSettings->value("TranslatorDiskImagePath").toString();
}

void RespeqtSettings::setTranslatorDiskImagePath(const QString &diskImage) {
  mSettings->setValue("TranslatorDiskImagePath", diskImage);
}

QString RespeqtSettings::toolDiskImagePath() {
  return mSettings->value("ToolDiskImagePath").toString();
}

void RespeqtSettings::setToolDiskImagePath(const QString &diskImage) {
  mSettings->setValue("ToolDiskImagePath", diskImage);
}

bool RespeqtSettings::activateChipModeWithTool() {
  return mSettings->value("ActivateChipModeWithTool").toBool();
}

void RespeqtSettings::setActivateChipModeWithTool(bool activate) {
  mSettings->setValue("ActivateChipModeWithTool", activate);
}

bool RespeqtSettings::activateHappyModeWithTool() {
  return mSettings->value("ActivateHappyModeWithTool").toBool();
}

void RespeqtSettings::setActivateHappyModeWithTool(bool activate) {
  mSettings->setValue("ActivateHappyModeWithTool", activate);
}

bool RespeqtSettings::displayCpuInstructions() {
  return mSettings->value("DisplayCpuInstructions").toBool();
}

void RespeqtSettings::setDisplayCpuInstructions(bool displayCpuInstructions) {
  mSettings->setValue("DisplayCpuInstructions", displayCpuInstructions);
}

QString RespeqtSettings::traceFilename() {
  return mSettings->value("TraceFilename").toString();
}

void RespeqtSettings::setTraceFilename(const QString &filename) {
  mSettings->setValue("TraceFilename", filename);
}

#ifdef Q_OS_MAC
void RespeqtSettings::setNativeMenu(bool nativeMenu) {
  mSettings->setValue("NativeMenu", nativeMenu);
}

bool RespeqtSettings::nativeMenu() {
  return mSettings->value("NativeMenu").toBool();
}
#endif

void RespeqtSettings::setRawPrinterName(const QString &name) {
  mSettings->setValue("RawPrinterName", name);
}

QString RespeqtSettings::rawPrinterName() const {
  return mSettings->value("RawPrinterName").toString();
}

bool RespeqtSettings::debugMenuVisible() const {
  return mSettings->value("DebugMenuVisible").toBool();
}

void RespeqtSettings::setDebugMenuVisible(bool menuVisible) {
  mSettings->setValue("DebugMenuVisible", menuVisible);
}
