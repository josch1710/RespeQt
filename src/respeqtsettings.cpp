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
#include "sio/serialport.h"
#include "sio/sioworker.h"
#include "diskbrowser/diskbrowser.h"
#include "ui/uiscale.h"
#include <QFileInfo>
#include <memory>
#include <QStandardPaths>

std::unique_ptr<RespeqtSettings> RespeqtSettings::sInstance;

std::unique_ptr<DbSettings> RespeqtSettings::sDbSettings;

RespeqtSettings::RespeqtSettings() {
  mSettings = new QSettings();  // uses QApplication's info to determine setting to use

  mIsFirstTime = mSettings->value("FirstTime", true).toBool();
  mSettings->setValue("FirstTime", false);
}

RespeqtSettings::~RespeqtSettings()
{
    sDbSettings.reset();

    delete mSettings;
}

// Get session file name from Mainwindow //
void RespeqtSettings::setSessionFile(const QString &g_sessionFile, const QString &g_sessionFilePath) {
  mSessionFileName = g_sessionFile;
  mSessionFilePath = g_sessionFilePath;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void RespeqtSettings::copySettings(QSettings& setFrom, QSettings& setTo) // NOLINT(*-no-recursion)
{
    foreach(const QString& key, setFrom.childKeys())
    {
        auto value = setFrom.value(key);
        setTo.setValue(key,value);
    }

    foreach(const QString& group, setFrom.childGroups())
    {
        setFrom.beginGroup(group);
        setTo.beginGroup(group);
        copySettings(setFrom, setTo);
        setTo.endGroup();
        setFrom.endGroup();
    }
}

// Save all session related settings, so that a session could be fully restored later //
void RespeqtSettings::saveSessionToFile(const QString &fileName)
{
    QSettings s(fileName, QSettings::IniFormat);

    copySettings(*mSettings, s);
}

// Get all session related settings, so that a session could be fully restored //
void RespeqtSettings::loadSessionFromFile(const QString &fileName)
{
    QSettings s(fileName, QSettings::IniFormat);

    copySettings(s, *mSettings);
}

// Get MainWindow title from MainWindow  //
void RespeqtSettings::setMainWindowTitle(const QString &g_mainWindowTitle) {
  mMainWindowTitle = g_mainWindowTitle;
}

bool RespeqtSettings::isFirstTime() const {
  return mIsFirstTime;
}

bool RespeqtSettings::showLogWindow() const {
  return mSettings->value("ShowLogWindow", false).toBool();
}

void RespeqtSettings::setShowLogWindow(const bool show) const
{
  mSettings->setValue("ShowLogWindow", show);
}

QString RespeqtSettings::serialPortName() const {
  return mSettings->value("SerialPortName").toString();
}

void RespeqtSettings::setSerialPortName(const QString &name) const
{
  mSettings->setValue("SerialPortName", name);
}

int RespeqtSettings::serialPortMaximumSpeed() const {
  return mSettings->value("MaximumSerialPortSpeed").toInt();
}

void RespeqtSettings::setSerialPortMaximumSpeed(const int speed) const
{
  mSettings->setValue("MaximumSerialPortSpeed", speed);
}

bool RespeqtSettings::serialPortUsePokeyDivisors() const {
  return mSettings->value("SerialPortUsePokeyDivisors").toBool();
}

void RespeqtSettings::setSerialPortUsePokeyDivisors(const bool use) const
{
  mSettings->setValue("SerialPortUsePokeyDivisors", use);
}

int RespeqtSettings::serialPortPokeyDivisor() const {
  return mSettings->value("SerialPortPokeyDivisor").toInt();
}

void RespeqtSettings::setSerialPortPokeyDivisor(const int divisor) const
{
  mSettings->setValue("SerialPortPokeyDivisor", divisor);
}

int RespeqtSettings::serialPortHandshakingMethod() const {
  return mSettings->value("HandshakingMethod").toInt();
}

void RespeqtSettings::setSerialPortHandshakingMethod(const int method) const
{
  mSettings->setValue("HandshakingMethod", method);
}

bool RespeqtSettings::serialPortTriggerOnFallingEdge() const {
  return mSettings->value("FallingEdge").toBool();
}

void RespeqtSettings::setSerialPortTriggerOnFallingEdge(const bool use) const
{
  mSettings->setValue("FallingEdge", use);
}

bool RespeqtSettings::serialPortDTRControlEnable() const {
  return mSettings->value("DTRControlEnable").toBool();
}

void RespeqtSettings::setSerialPortDTRControlEnable(const bool use) const
{
  mSettings->setValue("DTRControlEnable", use);
}

int RespeqtSettings::serialPortWriteDelay() const {
  return mSettings->value("WriteDelay").toInt();
}

void RespeqtSettings::setSerialPortWriteDelay(const int delay) const
{
  mSettings->setValue("WriteDelay", delay);
}

unsigned int RespeqtSettings::serialPortCompErrDelay() const {
  return mSettings->value("CompErrDelay").toUInt();
}

void RespeqtSettings::setSerialPortCompErrDelay(const unsigned int delay) const
{
  mSettings->setValue("CompErrDelay", delay);
}

QString RespeqtSettings::atariSioDriverName() const {
  return mSettings->value("AtariSioDriverName").toString();
}

void RespeqtSettings::setAtariSioDriverName(const QString &name) const
{
  mSettings->setValue("AtariSioDriverName", name);
}

int RespeqtSettings::atariSioHandshakingMethod() const {
  return mSettings->value("AtariSioHandshakingMethod").toInt();
}

void RespeqtSettings::setAtariSioHandshakingMethod(const int method) const
{
  mSettings->setValue("AtariSioHandshakingMethod", method);
}

SIO::SerialBackend RespeqtSettings::backend() const {
  return static_cast<SIO::SerialBackend>(mSettings->value("Backend").toInt());
}

void RespeqtSettings::setBackend(SIO::SerialBackend backend) const
{
  mSettings->setValue("Backend", static_cast<int>(backend));
}

QString RespeqtSettings::lastRclDir() const {
  return mSettings->value("LastRclDir").toString();
}

void RespeqtSettings::setRclDir(const QString &dir) const
{
  mSettings->setValue("LastRclDir", dir);
}

bool RespeqtSettings::useHighSpeedExeLoader() const {
  return mSettings->value("UseHighSpeedExeLoader").toBool();
}

void RespeqtSettings::setUseHighSpeedExeLoader(const bool use) const
{
  mSettings->setValue("UseHighSpeedExeLoader", use);
}

bool RespeqtSettings::printerEmulation() const {
  return mSettings->value("PrinterEmulation").toBool();
}

void RespeqtSettings::setPrinterEmulation(const bool status) const
{
  mSettings->setValue("PrinterEmulation", status);
}

bool RespeqtSettings::useCustomCasBaud() const {
  return mSettings->value("UseCustomCasBaud").toBool();
}

void RespeqtSettings::setUseCustomCasBaud(const bool use) const
{
  mSettings->setValue("UseCustomCasBaud", use);
}

int RespeqtSettings::customCasBaud() const {
  return mSettings->value("CustomCasBaud").toInt();
}

void RespeqtSettings::setCustomCasBaud(const int baud) const
{
  mSettings->setValue("CustomCasBaud", baud);
}

RespeqtSettings::ImageSettings RespeqtSettings::getImageSettingsFromName(const QString &fileName) const {
  mSettings->beginReadArray("MountedImageSettings");
  for (int i = 0; i < SIO::DISK_COUNT; i++) {
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

RespeqtSettings::ImageSettings RespeqtSettings::mountedImageSetting(const int no) const
{
  ImageSettings is;
  is.fileName = mSettings->value(QString("MountedImageSettings/%1/FileName").arg(no)).toString();
  is.isWriteProtected = mSettings->value(QString("MountedImageSettings/%1/IsWriteProtected").arg(no)).toBool();
  return is;
}

RespeqtSettings::ImageSettings RespeqtSettings::recentImageSetting(const int no) const
{
  ImageSettings is;
  is.fileName = mSettings->value(QString("RecentImageSettings/%1/FileName").arg(no)).toString();
  is.isWriteProtected = mSettings->value(QString("RecentImageSettings/%1/IsWriteProtected").arg(no)).toBool();
  return is;
}

void RespeqtSettings::setMountedImageProtection(const int no, const bool prot) const
{
  mSettings->setValue(QString("MountedImageSettings/%1/IsWriteProtected").arg(no), prot);
}

void RespeqtSettings::setMountedImageSetting(const int no, const QString &fileName, const bool prot) const
{
  mSettings->setValue(QString("MountedImageSettings/%1/FileName").arg(no), fileName);
  mSettings->setValue(QString("MountedImageSettings/%1/IsWriteProtected").arg(no), prot);
}

void RespeqtSettings::mountImage(const int no, const QString &fileName, const bool isWriteProtected) const
{
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
      auto [_fileName, _isWriteProtected] = mSettings->value(QString("RecentImageSettings/%1").arg(j + 1)).value<ImageSettings>();
      mSettings->setValue("FileName", _fileName);
      mSettings->setValue("IsWriteProtected", _isWriteProtected);
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

void RespeqtSettings::unmountImage(const int no) const
{
  auto [fileName, isWriteProtected] = mSettings->value(QString("MountedImageSettings/%1").arg(no)).value<ImageSettings>();

  for (int i = NUM_RECENT_FILES - 1; i > 0; i--) {
    auto [_fileName, _isWriteProtected] = mSettings->value(QString("RecentImageSettings/%1").arg(no)).value<ImageSettings>();
    mSettings->setValue(QString("RecentImageSettings/%1/FileName").arg(no), _fileName);
    mSettings->setValue(QString("RecentImageSettings/%1/IsWriteProtected").arg(no), _isWriteProtected);
  }

  mSettings->setValue("RecentImageSettings/0/FileName", fileName);
  mSettings->setValue("RecentImageSettings/0/IsWriteProtected", isWriteProtected);

  setMountedImageSetting(no, "", false);
}

void RespeqtSettings::swapImages(const int no1, const int no2) const
{
  auto [fileName1, isWriteProtected1] = mountedImageSetting(no1);
  auto [fileName2, isWriteProtected2] = mountedImageSetting(no2);
  setMountedImageSetting(no1, fileName2, isWriteProtected2);
  setMountedImageSetting(no2, fileName1, isWriteProtected1);
}

// Save drive visibility status //
bool RespeqtSettings::saveDiskVis() const {
  return mSettings->value("SaveDiskVisibility", true).toBool();
}

void RespeqtSettings::setSaveDiskVis(const bool saveDvis) const
{
  mSettings->setValue("SaveDiskVisibility", saveDvis);
}

// Drive visibility status //
bool RespeqtSettings::D9DOVisible() const {
  return mSettings->value("D9DOVisible", true).toBool();
}

void RespeqtSettings::setD9DOVisible(const bool dVis) const
{
  if (saveDiskVis()) {
    mSettings->setValue("D9DOVisible", dVis);
  }
}

// Shade Mode Enable //
bool RespeqtSettings::enableShade() const {
  return mSettings->value("EnableShadeByDefault", false).toBool();
}

void RespeqtSettings::setEnableShade(const bool shade) const
{
  mSettings->setValue("EnableShadeByDefault", shade);
}

// Explorer Window always on top
bool RespeqtSettings::explorerOnTop() const {
  return mSettings->value("ExplorerOnTop").toBool();
}

void RespeqtSettings::setExplorerOnTop(const bool expOnTop) const
{
  mSettings->setValue("ExplorerOnTop", expOnTop);
}

// Save/return last main window position/size option //
bool RespeqtSettings::saveWindowsPos() const {
  return mSettings->value("SaveWindowsPosSize", true).toBool();
}

void RespeqtSettings::setSaveWindowsPos(const bool saveMwp) const
{
  mSettings->setValue("SaveWindowsPosSize", saveMwp);
}

// mini mode and it's specific window position and width
bool RespeqtSettings::miniMode() const {
  return mSettings->value("MiniMode", false).toBool();
}

void RespeqtSettings::setMiniMode(const bool miniMode) const
{
  mSettings->setValue("MiniMode", miniMode);
}

QString RespeqtSettings::lastDiskImageDir() const {
  return mSettings->value("LastDiskImageDir").toString();
}

void RespeqtSettings::setLastDiskImageDir(const QString &dir) const
{
  mSettings->setValue("LastDiskImageDir", dir);
}

QString RespeqtSettings::lastFolderImageDir() const {
  return mSettings->value("LastFolderImageDir").toString();
}

void RespeqtSettings::setLastFolderImageDir(const QString &dir) const
{
  mSettings->setValue("LastFolderImageDir", dir);
}

QString RespeqtSettings::lastSessionDir() const {
  return mSettings->value("LastSessionDir").toString();
}

void RespeqtSettings::setLastSessionDir(const QString &dir) const
{
  mSettings->setValue("LastSessionDir", dir);
}

QString RespeqtSettings::lastExeDir() const {
  return mSettings->value("LastExeDir").toString();
}

void RespeqtSettings::setLastExeDir(const QString &dir) const
{
  mSettings->setValue("LastExeDir", dir);
}

QString RespeqtSettings::lastExtractDir() const {
  return mSettings->value("LastExtractDir").toString();
}

void RespeqtSettings::setLastExtractDir(const QString &dir) const
{
  mSettings->setValue("LastExtractDir", dir);
}

QString RespeqtSettings::lastPrinterTextDir() const {
  return mSettings->value("LastPrinterTextDir").toString();
}

void RespeqtSettings::setLastPrinterTextDir(const QString &dir) const
{
  mSettings->setValue("LastPrinterTextDir", dir);
}

QString RespeqtSettings::lastCasDir() const {
  return mSettings->value("LastCasDir").toString();
}

void RespeqtSettings::setLastCasDir(const QString &dir) const
{
  mSettings->setValue("LastCasDir", dir);
}

QString RespeqtSettings::i18nLanguage() const {
  return mSettings->value("I18nLanguage").toString();
}

void RespeqtSettings::setI18nLanguage(const QString &lang) const
{
  mSettings->setValue("I18nLanguage", lang);
}

bool RespeqtSettings::minimizeToTray() const {
  return mSettings->value("MinimizeToTray").toBool();
}

void RespeqtSettings::setMinimizeToTray(const bool tray) const
{
  mSettings->setValue("MinimizeToTray", tray);
}

bool RespeqtSettings::filterUnderscore() const {
  return mSettings->value("FilterUnderscore").toBool();
}

void RespeqtSettings::setfilterUnderscore(const bool filter) const
{
  mSettings->setValue("FilterUnderscore", filter);
}

bool RespeqtSettings::limitFileEntries() const {
  return mSettings->value("LimitFileEntries").toBool();
}

void RespeqtSettings::setlimitFileEntries(const bool limit) const
{
  mSettings->setValue("LimitFileEntries", limit);
}

bool RespeqtSettings::capitalLettersInPCLINK() const {
  return mSettings->value("CapitalLettersInPCLINK").toBool();
}

void RespeqtSettings::setCapitalLettersInPCLINK(const bool caps) const
{
  mSettings->setValue("CapitalLettersInPCLINK", caps);
}

bool RespeqtSettings::isURLSubmitEnabled() const {
  return mSettings->value("URLSubmit").toBool();
}

void RespeqtSettings::setURLSubmit(const bool enabled) const
{
  mSettings->setValue("URLSubmit", enabled);
}

bool RespeqtSettings::isSpyMode() const {
  return mSettings->value("SpyMode").toBool();
}

void RespeqtSettings::setSpyMode(const bool enabled) const
{
  mSettings->setValue("SpyMode", enabled);
}

bool RespeqtSettings::isCommandName() const {
  return mSettings->value("CommandName").toBool();
}

void RespeqtSettings::setCommandName(const bool enabled) const
{
  mSettings->setValue("CommandName", enabled);
}

bool RespeqtSettings::isTrackLayout() const {
  return mSettings->value("TrackLayout").toBool();
}

void RespeqtSettings::setTrackLayout(const bool enabled) const
{
  mSettings->setValue("TrackLayout", enabled);
}

void RespeqtSettings::setPrinterName(const int no, const QString &printerName) const
{
  mSettings->setValue(QString("ConnectedPrinterSettings/%1/PrinterName").arg(no), printerName);
}

QString RespeqtSettings::printerName(const int no) const {
  return mSettings->value(QString("ConnectedPrinterSettings/%1/PrinterName").arg(no)).toString();
}

QString RespeqtSettings::atariFixedFontFamily() const {
  return mSettings->value("AtariFixedFontFamily").toString();
}

void RespeqtSettings::setAtariFixedFontFamily(const QString& fontFamily) const
{
  mSettings->setValue("AtariFixedFontFamily", fontFamily);
}

bool RespeqtSettings::isPrinterSpyMode() const {
  return mSettings->value("PrinterSpyMode").toBool();
}

void RespeqtSettings::setPrinterSpyMode(const bool enabled) const
{
  mSettings->setValue("PrinterSpyMode", enabled);
}

bool RespeqtSettings::displayGraphicsInstructions() const {
  return mSettings->value("DisplayGraphicsInstructions").toBool();
}

void RespeqtSettings::setDisplayGraphicsInstructions(const bool enabled) const
{
  mSettings->setValue("DisplayGraphicsInstructions", enabled);
}

bool RespeqtSettings::clearOnStatus() const {
  return mSettings->value("ClearOnStatus").toBool();
}

void RespeqtSettings::setClearOnStatus(const bool enabled) const
{
  mSettings->setValue("ClearOnStatus", enabled);
}

bool RespeqtSettings::displayTransmission() const {
  return mSettings->value("DisplayTransmission").toBool();
}

void RespeqtSettings::setDisplayTransmission(const bool displayTransmission) const
{
  mSettings->setValue("DisplayTransmission", displayTransmission);
}

bool RespeqtSettings::displayDriveHead() const {
  return mSettings->value("DisplayDriveHead").toBool();
}

void RespeqtSettings::setDisplayDriveHead(const bool displayDriveHead) const
{
  mSettings->setValue("DisplayDriveHead", displayDriveHead);
}

bool RespeqtSettings::displayFdcCommands() const {
  return mSettings->value("displayFdcCommands").toBool();
}

void RespeqtSettings::setDisplayFdcCommands(const bool displayFdcCommands) const
{
  mSettings->setValue("DisplayFdcCommands", displayFdcCommands);
}

bool RespeqtSettings::displayIndexPulse() const {
  return mSettings->value("DisplayIndexPulse").toBool();
}

void RespeqtSettings::setDisplayIndexPulse(const bool displayIndexPulse) const
{
  mSettings->setValue("DisplayIndexPulse", displayIndexPulse);
}

bool RespeqtSettings::displayMotorOnOff() const {
  return mSettings->value("DisplayMotorOnOff").toBool();
}

void RespeqtSettings::setDisplayMotorOnOff(const bool displayMotorOnOff) const
{
  mSettings->setValue("DisplayMotorOnOff", displayMotorOnOff);
}

bool RespeqtSettings::displayIDAddressMarks() const {
  return mSettings->value("DisplayIDAddressMarks").toBool();
}

void RespeqtSettings::setDisplayIDAddressMarks(const bool displayIDAddressMarks) const
{
  mSettings->setValue("DisplayIDAddressMarks", displayIDAddressMarks);
}

bool RespeqtSettings::displayTrackInformation() const {
  return mSettings->value("DisplayTrackInformation").toBool();
}

void RespeqtSettings::setDisplayTrackInformation(const bool displayTrackInformation) const
{
  mSettings->setValue("DisplayTrackInformation", displayTrackInformation);
}

bool RespeqtSettings::disassembleUploadedCode() const {
  return mSettings->value("DisassembleUploadedCode").toBool();
}

void RespeqtSettings::setDisassembleUploadedCode(const bool disassembleUploadedCode) const
{
  mSettings->setValue("DisassembleUploadedCode", disassembleUploadedCode);
}

bool RespeqtSettings::translatorAutomaticDetection() const {
  return mSettings->value("TranslatorAutomaticDetection").toBool();
}

void RespeqtSettings::setTranslatorAutomaticDetection(const bool translatorAutomaticDetection) const
{
  mSettings->setValue("TranslatorAutomaticDetection", translatorAutomaticDetection);
}

bool RespeqtSettings::sioAutoReconnect() const {
  return mSettings->value("SioAutoReconnect").toBool();
}

void RespeqtSettings::setSioAutoReconnect(const bool sioAutoReconnect) const
{
  mSettings->setValue("SioAutoReconnect", sioAutoReconnect);
}

bool RespeqtSettings::hideChipMode() const {
  return mSettings->value("HideChipMode").toBool();
}

void RespeqtSettings::setHideChipMode(const bool hidden) const
{
  mSettings->setValue("HideChipMode", hidden);
}

bool RespeqtSettings::hideHappyMode() const {
  return mSettings->value("HideHappyMode").toBool();
}

void RespeqtSettings::setHideHappyMode(const bool hidden) const
{
  mSettings->setValue("HideHappyMode", hidden);
}

bool RespeqtSettings::hideNextImage() const {
  return mSettings->value("HideNextImage").toBool();
}

void RespeqtSettings::setHideNextImage(const bool hidden) const
{
  mSettings->setValue("HideNextImage", hidden);
}

bool RespeqtSettings::hideOSBMode() const {
  return mSettings->value("HideOSBMode").toBool();
}

void RespeqtSettings::setHideOSBMode(const bool hidden) const
{
  mSettings->setValue("HideOSBMode", hidden);
}

bool RespeqtSettings::hideToolDisk() const {
  return mSettings->value("HideToolDisk").toBool();
}

void RespeqtSettings::setHideToolDisk(const bool hidden) const
{
  mSettings->setValue("HideToolDisk", hidden);
}

QString RespeqtSettings::translatorDiskImagePath() const {
  return mSettings->value("TranslatorDiskImagePath").toString();
}

void RespeqtSettings::setTranslatorDiskImagePath(const QString &diskImage) const
{
  mSettings->setValue("TranslatorDiskImagePath", diskImage);
}

QString RespeqtSettings::toolDiskImagePath() const {
  return mSettings->value("ToolDiskImagePath").toString();
}

void RespeqtSettings::setToolDiskImagePath(const QString &diskImage) const
{
  mSettings->setValue("ToolDiskImagePath", diskImage);
}

bool RespeqtSettings::activateChipModeWithTool() const {
  return mSettings->value("ActivateChipModeWithTool").toBool();
}

void RespeqtSettings::setActivateChipModeWithTool(const bool activate) const
{
  mSettings->setValue("ActivateChipModeWithTool", activate);
}

bool RespeqtSettings::activateHappyModeWithTool() const {
  return mSettings->value("ActivateHappyModeWithTool").toBool();
}

void RespeqtSettings::setActivateHappyModeWithTool(const bool activate) const
{
  mSettings->setValue("ActivateHappyModeWithTool", activate);
}

bool RespeqtSettings::displayCpuInstructions() const {
  return mSettings->value("DisplayCpuInstructions").toBool();
}

void RespeqtSettings::setDisplayCpuInstructions(const bool displayCpuInstructions) const
{
  mSettings->setValue("DisplayCpuInstructions", displayCpuInstructions);
}

QString RespeqtSettings::traceFilename() const {
  return mSettings->value("TraceFilename").toString();
}

void RespeqtSettings::setTraceFilename(const QString &filename) const
{
  mSettings->setValue("TraceFilename", filename);
}

#ifdef Q_OS_MAC
void RespeqtSettings::setNativeMenu(const bool nativeMenu) const
{
  mSettings->setValue("NativeMenu", nativeMenu);
}

bool RespeqtSettings::nativeMenu() const {
  return mSettings->value("NativeMenu").toBool();
}
#endif

void RespeqtSettings::setRawPrinterName(const QString &name) const
{
  mSettings->setValue("RawPrinterName", name);
}

QString RespeqtSettings::rawPrinterName() const
{
    return mSettings->value("RawPrinterName").toString();
}

bool RespeqtSettings::debugMenuVisible() const {
  return mSettings->value("DebugMenuVisible").toBool();
}

void RespeqtSettings::setDebugMenuVisible(const bool menuVisible) const
{
    mSettings->setValue("DebugMenuVisible", menuVisible);
}

auto RespeqtSettings::isTnfsEnabled() const -> bool
{
    return mSettings->value("IsTnfsEnabled", false).toBool();
}

auto RespeqtSettings::setTnfsEnabled(const bool enable) const -> void
{
    mSettings->setValue("IsTnfsEnabled", enable);
}

QString RespeqtSettings::mostRecentBrowserFolder() const
{
  if (QStringList folders = recentBrowserFolders(); !folders.isEmpty())
    return folders.front();

  return {};
}

QStringList RespeqtSettings::recentBrowserFolders() const
{
  QStringList folders;

  const int size = mSettings->beginReadArray("RecentBrowserFolders");
  for (int i = 0; i < size; i++) {
    mSettings->setArrayIndex(i);
    QString value = mSettings->value("FileFolder").toString();
    folders.append(value);
  }
  mSettings->endArray();

  return folders;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
bool RespeqtSettings::isDiskImage(const QString &name) // NOLINT(*-convert-member-functions-to-static)
{
    foreach (const QString &fileType, FileTypes::getDiskImageTypes())
    {
      if (QString ext = fileType.right(4); name.endsWith(ext, osCaseSensitivity()))
            return true;
    }
    return false;
}

QStringList RespeqtSettings::buildBrowserFolders()
{
  QStringList folders;

  // build a list of MRU folders for the GUI dropdown list

  foreach(const QString& name, recentBrowserFolders())
  {
    if (auto fileInfo = QFileInfo(name); fileInfo.exists())
    {
      const QString path = fileInfo.isFile() ? fileInfo.path() : name; // don't want file names in dropdown
      folders += path;
    }
    else if (isDiskImage(name)) // MRU missing. First check if a disk is selected
    {
      if (QString path = DbUtils::getParentDir(name); QFileInfo::exists(path))
      {
        folders += path; // Keep parent folder of bad disk
      }
      else
      {
        qDebug() << "!w" << QString("Disk Collection Browser most recent list updated. '%1' not found.").arg(name);
        delMostRecentBrowserFolder(name);
      }
    }
    else // Simple case of missing folder
    {
      qDebug() << "!w" << QString("Disk Collection Browser most recent list updated. Folder '%1' not found.").
        arg(name);
      delMostRecentBrowserFolder(name);
    }
  }
  return folders;
}

void RespeqtSettings::setMostRecentBrowserFolder(const QString& name) const
{
  if (mostRecentBrowserFolder() == name)
    return;

  const auto fileInfo = QFileInfo(name);

  if (!fileInfo.exists())
    return;

  const QString path = fileInfo.isFile() ? fileInfo.path() : name;

  QStringList folders = recentBrowserFolders();

  for (int i = 0; i < folders.size(); ++i) {
    QString text = folders.at(i);
    auto fi = QFileInfo(text);
    if (QString test = fi.isFile() ? fi.path() : text; test == path)
      folders.removeAt(i);
  }
  folders.insert(0, name);

  if (folders.count() > 100)    // TBD: warn user?
    folders.removeLast();

  writeRecentBrowserFolders(folders);
}

void RespeqtSettings::delMostRecentBrowserFolder(const QString& name) const
{
  const auto fileInfo = QFileInfo(name);
  const QString path = fileInfo.isFile() ? fileInfo.path() : name;
  QStringList folders = recentBrowserFolders();

  for (int i = 0; i < folders.size(); ++i) {
    QString text = folders.at(i);
    auto fi = QFileInfo(text);
    if (QString test = fi.isFile() ? fi.path() : text; test == path)
      folders.removeAt(i);
  }

  writeRecentBrowserFolders(folders);
}

void RespeqtSettings::writeRecentBrowserFolders(const QStringList& folders) const
{
  int index = 0;

  mSettings->beginWriteArray("RecentBrowserFolders");
  foreach (QString folder, folders) {
    if (auto fileInfo = QFileInfo(folder); !fileInfo.exists()) {
      if (fileInfo.isFile())
        folder = fileInfo.path();
      else
        continue;

      if (!QFileInfo::exists(folder))
        continue;
    }
    mSettings->setArrayIndex(index++);
    mSettings->setValue("FileFolder", folder);
  }
  mSettings->endArray();
}

bool RespeqtSettings::showDiskBrowser() const
{
  return mSettings->value("/DiskBrowserDlg/show").toBool();
}

void RespeqtSettings::setShowDiskBrowser(const bool show /*= true*/) const
{
  mSettings->setValue("/DiskBrowserDlg/show", show);
}

int RespeqtSettings::diskBrowserHorzSplitPos() const
{
  return mSettings->value("/DiskBrowserDlg/HorzSplitPos", -1).toInt();
}

int RespeqtSettings::diskBrowserVertSplitPos() const
{
  return mSettings->value("/DiskBrowserDlg/VertSplitPos", -1).toInt();
}

void RespeqtSettings::setDiskBrowserHorzSplitPos(const int pos) const
{
  mSettings->setValue("/DiskBrowserDlg/HorzSplitPos", pos);
}

void RespeqtSettings::setDiskBrowserVertSplitPos(const int pos) const
{
  mSettings->setValue("/DiskBrowserDlg/VertSplitPos", pos);
}

void RespeqtSettings::setOptionsDlgSplitPos(const int pos) const
{
  mSettings->setValue("/OptionsDialog/SplitterPos", pos);
}

int RespeqtSettings::optionsDialogSplitPos() const {
  return mSettings->value("/OptionsDialog/SplitterPos",-1).toInt();
}

bool RespeqtSettings::saveMainWinGeometry(const QMainWindow* window, const bool isMiniMode) const
{
  if (!window || !saveWindowsPos())
    return false;

  setMiniMode(isMiniMode);
  const QString key = isMiniMode ? "MiniWindow" : "MainWindow";
  return saveWidgetGeometry(window, key);
}

bool RespeqtSettings::restoreMainWinGeometry(QMainWindow* window, const bool isMiniMode) const
{
  if (!window || !saveWindowsPos())
    return false;

  const QString key = isMiniMode ? "MiniWindow" : "MainWindow";
  const QRect rcDef = isMiniMode ? DefaultMiniModeRect : DefaultFullModeRect;
  return restoreWidgetGeometry(window, key, rcDef);
}

bool RespeqtSettings::saveWidgetGeometry(const QWidget* widget, const QString& name) const
{
  if (!widget || !saveWindowsPos())
    return false;

  const QString key = name.isEmpty() ? widget->objectName() : name;
  if (key.isEmpty())
    return false;

  mSettings->setValue(key + "/geometry", widget->saveGeometry());
  return true;
}

bool RespeqtSettings::restoreWidgetGeometry(QWidget* widget, const QString& name, const QRect& defRect) const
{
  if (widget == nullptr)
    return false;

  QString key = name.isEmpty() ? widget->objectName() : name;
  if (key.isEmpty())
    return false;

  key += "/geometry";
  if (mSettings->contains(key)) {
    const auto geoBytes = mSettings->value(key).toByteArray();
    widget->restoreGeometry(geoBytes);
  } else if (defRect.isValid()) {
    widget->setGeometry(defRect);
  }

  return true;
}

bool RespeqtSettings::windowPosSaved(const QWidget* widget, const QString& name) const
{
    if (widget == nullptr)
        return false;

    QString key = name.isEmpty() ? widget->objectName() : name;
    if (key.isEmpty())
        return false;

    key += "/geometry";

    return mSettings->contains(key);
}

void RespeqtSettings::setDbDataSource(const DbDataSource newDbSource)
{
    const auto curDbSource = dbDataSource();
    if (curDbSource == newDbSource)     // sanity check
        return;

    if (!sDbSettings)   // need current artwork data from the source?
    {
        if (curDbSource == DbData_appSettings)
            sDbSettings = std::make_unique<DbIni>();
        else
            sDbSettings = std::make_unique<DbJson>();      // TBD untested (see above)
    }

    mSettings->setValue("/DiskBrowserDlg/source", newDbSource);

    if (newDbSource == DbData_subDirJson)
    {
        // converting from multi-dir source to seperate JSON files

        const DirMap& dirMap = sDbSettings->getDirMap();
        for (auto itDir = dirMap.begin(); itDir != dirMap.end(); ++itDir)
        {
            const QString& dir = itDir.key();
            const auto pNew = new DbJson;
            const QString pic = DbUtils::removePrefix(dir, itDir.value().pic);
            pNew->setDataDir(dir);
            pNew->setPicture(pic, itDir.key());

            const ArtMap& artMap = itDir.value().map;
            for (auto itArt = artMap.begin(); itArt != artMap.end(); ++itArt)
            {
                const QString& disk = itArt.key();
                const auto& [picturePath, label] = itArt.value();
                //const QString artPic = DbUtils::removePrefix(dir, art.pic);
                pNew->setPicture(picturePath, dir, disk);
                pNew->setLabel(label, dir, disk);
            }
            //pNew->save(); delete does this next
            delete pNew;
        }
    }
    else //if (!sDbSettings->isEmpty()) TBD where does this optimization go?
    {
        // converting to a multi-dir source (app settings or single JSON)

        if (curDbSource == DbData_subDirJson)
        {
            // converting from seperate subdirs/JSON (to multi-dir source)

            DbSettings* pNew;
            if (newDbSource == DbData_appSettings)
                pNew = new DbIni;
            else
                pNew = new DbJson;

            foreach (const QString& dir, buildBrowserFolders())
            {
                sDbSettings = std::make_unique<DbJson>();
                sDbSettings->setDataDir(dir);
                pNew->merge(*sDbSettings);
            }
            if (newDbSource == DbData_appFolderJson)
                pNew->setDataDir(appDataFolder());

            //pNew->save();    not needed (re: next line)
            delete pNew;    // really not a fan of this save on delete design (TBD refactor)
        }
        else
        {
            // converting to/from app settings and single JSON

            DbSettings* pNew = nullptr;
            if (newDbSource == DbData_appSettings)
                pNew = new DbIni;
            else
                pNew = new DbJson;

            pNew->clone(*sDbSettings);
            pNew->save();
            sDbSettings.reset(pNew);
        }
    }
    // sDbSettings.reset(); // TBD needed?
    // Dev Note: The above line asserts (DbJson::save()) when converting single to multi JSON.
    // That's because properties such as the DB source are changed out from under the
    // original DbJson object. When the save method runs, the code is checking
    // to make sure there's one and only one node as it writes it out.
}

DbDataSource RespeqtSettings::dbDataSource() const
{
    //assert(mSettings->group().isEmpty());
    return static_cast<DbDataSource>(mSettings->value("/DiskBrowserDlg/source", DbData_appSettings).toInt());
}

void RespeqtSettings::setDbFileNames(const bool useFileNames, const bool favorJson) const
{
    mSettings->setValue("/DiskBrowserDlg/use_filenames", useFileNames);
    mSettings->setValue("/DiskBrowserDlg/favor_json", favorJson);
}

bool RespeqtSettings::dbFavorJson() const
{
    return mSettings->value("/DiskBrowserDlg/json_1st", true).toBool();
}

bool RespeqtSettings::dbUseFileNames() const
{
    return mSettings->value("/DiskBrowserDlg/use_filenames", true).toBool();
}

bool RespeqtSettings::dbCopyPics() const
{
    return mSettings->value("/DiskBrowserDlg/copy_pics", true).toBool();
}

void RespeqtSettings::setDbCopyPics(const bool copy) const
{
    mSettings->setValue("/DiskBrowserDlg/copy_pics", copy);
}

QString RespeqtSettings::appDataFolder() const
{
    QString folder = mSettings->value("/DiskBrowserDlg/appData_folder", QString()).toString();
    if (folder.isEmpty())
    {
        constexpr auto    locType   = QStandardPaths::AppDataLocation;
        const QString appFolder = QStandardPaths::writableLocation(locType);
        const QDir appDataDir(appFolder);

        if (!appDataDir.exists())
            if (!appDataDir.mkpath("."))
              return {};

        folder = appDataDir.absolutePath();
    }
    return folder;
}

void RespeqtSettings::setAppFolderDir(const QString& appDataDir) const
{
    mSettings->setValue("/DiskBrowserDlg/appData_folder", appDataDir);
}

// The two disk label fonts are stored by size in points, and the stored value
// has to be one Qt accepts on the way back in. It used to be a pixel size taken
// straight from the font, which is -1 whenever the font was built with a point
// size instead -- OptionsDialog builds exactly such a font, so "size" ended up
// as -1 in the settings and setPixelSize(-1) then failed on every read. Note
// that the number is a starting value only: PicPreview::scaleFonts() replaces
// it with a size derived from the label rectangle and the user's scale factor
// before anything is painted.
static int labelFontPointSize(const int points) {
    return points > 0 ? points : UiScale::defaultPointSize();
}

static int labelFontPointSize(const QFont& font) {
    return labelFontPointSize(font.pointSize());
}

void RespeqtSettings::setDbTitleFont(const LabelFont& font) const
{
    mSettings->beginGroup("/DiskBrowserDlg");
    mSettings->beginGroup("title_font");
    mSettings->setValue("family", font.family());
    mSettings->setValue("size", labelFontPointSize(font));
    mSettings->setValue("bold", font.bold());
    mSettings->setValue("italic", font.italic());
    mSettings->setValue("color", font.color().name());
    mSettings->setValue("scale", font.scale());
    mSettings->endGroup();
    mSettings->endGroup();
}

LabelFont RespeqtSettings::dbTitleFont() const
{
    LabelFont font;
    mSettings->beginGroup("/DiskBrowserDlg");
    mSettings->beginGroup("title_font");
    font.setFamily(mSettings->value("family", Label::DEF_TITLE_FNT).toString());
    font.setPointSize(labelFontPointSize(mSettings->value("size", 0).toInt()));
    font.setBold(mSettings->value("bold", Label::DEF_TITLE_BOLD).toBool());
    font.setItalic(mSettings->value("italic", false).toBool());
    font.setColor(mSettings->value("color", "black").toString());
    font.setScale(mSettings->value("scale", Label::DEF_TITLE_SCALE).toDouble());
    mSettings->endGroup();
    mSettings->endGroup();
    return font;
}

void RespeqtSettings::setDbIndexFont(const LabelFont& font) const
{
    mSettings->beginGroup("/DiskBrowserDlg");
    mSettings->beginGroup("index_font");
    mSettings->setValue("family", font.family());
    mSettings->setValue("size", labelFontPointSize(font));
    mSettings->setValue("bold", font.bold());
    mSettings->setValue("italic", font.italic());
    mSettings->setValue("color", font.color().name());
    mSettings->setValue("scale", font.scale());
    mSettings->endGroup();
    mSettings->endGroup();
}

LabelFont RespeqtSettings::dbIndexFont() const
{
    LabelFont font;
    mSettings->beginGroup("/DiskBrowserDlg");
    mSettings->beginGroup("index_font");
    font.setFamily(mSettings->value("family", Label::DEF_INDEX_FNT).toString());
    font.setPointSize(labelFontPointSize(mSettings->value("size", 0).toInt()));
    font.setBold(mSettings->value("bold", false).toBool());
    font.setItalic(mSettings->value("italic", false).toBool());
    font.setColor(mSettings->value("color", "black").toString());
    font.setScale(mSettings->value("scale", Label::DEF_INDEX_SCALE).toDouble());
    mSettings->endGroup();
    mSettings->endGroup();
    return font;
}

void RespeqtSettings::setDiskPic(const QString& pic) const
{
    mSettings->setValue("/DiskBrowserDlg/disk_pic", pic);
}

QString RespeqtSettings::diskPic() const
{
    return mSettings->value("/DiskBrowserDlg/disk_pic").toString();
}

const std::unique_ptr<DbSettings>& RespeqtSettings::dbSettings()
{
    if (!sDbSettings)   // instantiate on demand
    {
        if (instance()->dbDataSource() == DbData_appSettings)
            sDbSettings = std::make_unique<DbIni>();
        else
            sDbSettings = std::make_unique<DbJson>();
    }
    return sDbSettings;
}
