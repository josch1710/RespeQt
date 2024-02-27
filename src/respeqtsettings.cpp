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
#include "diskbrowser/diskbrowser.h"
#include "diskbrowser/dbjson.h"
#include <QFileInfo>
#include <memory>
#include <QApplication>
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

void RespeqtSettings::copySettings(QSettings& setFrom, QSettings& setTo)
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

bool RespeqtSettings::isFirstTime() {
  return mIsFirstTime;
}

bool RespeqtSettings::showLogWindow() {
  return mSettings->value("ShowLogWindow", false).toBool();
}

void RespeqtSettings::setShowLogWindow(bool show) {
  mSettings->setValue("ShowLogWindow", show);
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

// mini mode and it's specific window position and width
bool RespeqtSettings::miniMode() {
  return mSettings->value("MiniMode", false).toBool();
}

void RespeqtSettings::setMiniMode(bool miniMode) {
  mSettings->setValue("MiniMode", miniMode);
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

QString RespeqtSettings::mostRecentBrowserFolder() {
  QStringList folders = recentBrowserFolders();

  if (!folders.isEmpty())
    return folders.front();

  return QString();
}

QStringList RespeqtSettings::recentBrowserFolders() {
  QStringList folders;

  int size = mSettings->beginReadArray("RecentBrowserFolders");
  for (int i = 0; i < size; i++) {
    mSettings->setArrayIndex(i);
    QString value = mSettings->value("FileFolder").toString();
    folders.append(value);
  }
  mSettings->endArray();

  return folders;
}

bool RespeqtSettings::isDiskImage(const QString &name)
{
    foreach (const QString &fileType, FileTypes::getDiskImageTypes())
    {
        QString ext = fileType.right(4);
        if (name.endsWith(ext, osCaseSensitivity()))
            return true;
    }
    return false;
}

QStringList RespeqtSettings::buildBrowserFolders()
{
    QStringList folders;

    // build a list of MRU folders for the GUI dropdown list

    foreach (const QString& name, recentBrowserFolders())
    {
        auto fileInf = QFileInfo(name);
        if (fileInf.exists())
        {
            QString path = fileInf.isFile() ? fileInf.path() : name;    // don't want file names in dropdown
            folders += path;
        }
        else if (isDiskImage(name)) // MRU missing. First check if a disk is selected
        {
            QString path = DbUtils::getParentDir(name);
            if (QFileInfo::exists(path))
            {
                folders += path;    // Keep parent folder of bad disk
            }
            else
            {
                qDebug() << "!w" << QString("Disk Collection Browser most recent list updated. '%1' not found.").arg(name);
                delMostRecentBrowserFolder(name);
            }
        }
        else    // Simple case of missing folder
        {
            qDebug() << "!w" << QString("Disk Collection Browser most recent list updated. Folder '%1' not found.").arg(name);
            delMostRecentBrowserFolder(name);
        }
    }
    return folders;
}

void RespeqtSettings::setMostRecentBrowserFolder(const QString& name) {
  if (mostRecentBrowserFolder() == name)
    return;

  auto fileInfo = QFileInfo(name);

  if (!fileInfo.exists())
    return;

  QString path = fileInfo.isFile() ? fileInfo.path() : name;

  QStringList folders = recentBrowserFolders();

  for (int i = 0; i < folders.size(); ++i) {
    QString text = folders.at(i);
    QFileInfo fi = QFileInfo(text);
    QString test = fi.isFile() ? fi.path() : text;
    if (test == path)
      folders.removeAt(i);
  }
  folders.insert(0, name);

  if (folders.count() > 100)    // TBD: warn user?
    folders.removeLast();

  writeRecentBrowserFolders(folders);
}

void RespeqtSettings::delMostRecentBrowserFolder(const QString& name) {
  auto fileInfo = QFileInfo(name);
  QString path = fileInfo.isFile() ? fileInfo.path() : name;
  QStringList folders = recentBrowserFolders();

  for (int i = 0; i < folders.size(); ++i) {
    QString text = folders.at(i);
    QFileInfo fi = QFileInfo(text);
    QString test = fi.isFile() ? fi.path() : text;
    if (test == path)
      folders.removeAt(i);
  }

  writeRecentBrowserFolders(folders);
}

void RespeqtSettings::writeRecentBrowserFolders(const QStringList& folders) {
  int index = 0;

  mSettings->beginWriteArray("RecentBrowserFolders");
  foreach (QString folder, folders) {
    auto fileInfo = QFileInfo(folder);
    if (!fileInfo.exists()) {
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

bool RespeqtSettings::showDiskBrowser() {
  return mSettings->value("/DiskBrowserDlg/show").toBool();
}

void RespeqtSettings::setShowDiskBrowser(bool show /*= true*/) {
  mSettings->setValue("/DiskBrowserDlg/show", show);
}

int RespeqtSettings::diskBrowserHorzSplitPos() {
  return mSettings->value("/DiskBrowserDlg/HorzSplitPos", -1).toInt();
}

int RespeqtSettings::diskBrowserVertSplitPos() {
  return mSettings->value("/DiskBrowserDlg/VertSplitPos", -1).toInt();
}

void RespeqtSettings::setDiskBrowserHorzSplitPos(int pos) {
  mSettings->setValue("/DiskBrowserDlg/HorzSplitPos", pos);
}

void RespeqtSettings::setDiskBrowserVertSplitPos(int pos) {
  mSettings->setValue("/DiskBrowserDlg/VertSplitPos", pos);
}

bool RespeqtSettings::saveMainWinGeometry(QMainWindow* window, bool isMiniMode) {
  if (!window || !saveWindowsPos())
    return false;

  setMiniMode(isMiniMode);
  QString key = isMiniMode ? "MiniWindow" : "MainWindow";
  return saveWidgetGeometry(window, key);
}

bool RespeqtSettings::restoreMainWinGeometry(QMainWindow* window, bool isMiniMode) {
  if (!window || !saveWindowsPos())
    return false;

  QString key = isMiniMode ? "MiniWindow" : "MainWindow";
  QRect rcDef = isMiniMode ? DefaultMiniModeRect : DefaultFullModeRect;
  return restoreWidgetGeometry(window, key, rcDef);
}

bool RespeqtSettings::saveWidgetGeometry(QWidget* widget, const QString& name) {
  if (!widget || !saveWindowsPos())
    return false;

  QString key = name.isEmpty() ? widget->objectName() : name;
  if (key.isEmpty())
    return false;

  mSettings->setValue(key + "/geometry", widget->saveGeometry());
  return true;
}

bool RespeqtSettings::restoreWidgetGeometry(QWidget* widget, const QString& name, const QRect& defRect) {
  if (widget == nullptr)
    return false;

  QString key = name.isEmpty() ? widget->objectName() : name;
  if (key.isEmpty())
    return false;

  key += "/geometry";
  if (mSettings->contains(key)) {
    auto geoBytes = mSettings->value(key).toByteArray();
    widget->restoreGeometry(geoBytes);
  } else if (defRect.isValid()) {
    widget->setGeometry(defRect);
  }

  return true;
}

void RespeqtSettings::setDbDataSource(DbDataSource newDbSource)
{
    auto curDbSource = dbDataSource();
    if (curDbSource == newDbSource)     // sanity check
        return;

    if (!sDbSettings)   // need current artwork data from the source?
    {
        if (curDbSource == DbData_appSettings)
            sDbSettings.reset(new DbIni);
        else
            sDbSettings.reset(new DbJson);      // TBD untested (see above)
    }

    mSettings->setValue("/DiskBrowserDlg/source", newDbSource);

    if (newDbSource == DbData_subDirJson)
    {
        // converting from multi-dir source to seperate JSON files

        const DirMap& dirMap = sDbSettings->getDirMap();
        for (auto itDir = dirMap.begin(); itDir != dirMap.end(); ++itDir)
        {
            const QString dir = itDir.key();
            DbJson* pNew = new DbJson;
            const QString pic = DbUtils::removePrefix(dir, itDir.value().pic);
            pNew->setDataDir(dir);
            pNew->setPicture(pic, itDir.key());

            const ArtMap& artMap = itDir.value().map;
            for (auto itArt = artMap.begin(); itArt != artMap.end(); ++itArt)
            {
                QString disk = itArt.key();
                const FloppyArt& art = itArt.value();
                const QString artPic = DbUtils::removePrefix(dir, art.pic);
                pNew->setPicture(art.pic, dir, disk);
                pNew->setLabel(art.label, dir, disk);
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
                sDbSettings.reset(new DbJson);
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

DbDataSource RespeqtSettings::dbDataSource()
{
    assert(mSettings->group().isEmpty());
    return static_cast<DbDataSource>(mSettings->value("/DiskBrowserDlg/source", DbData_appSettings).toInt());
}

void RespeqtSettings::setDbFileNames(bool useFileNames, bool favorJson)
{
    mSettings->setValue("/DiskBrowserDlg/use_filenames", useFileNames);
    mSettings->setValue("/DiskBrowserDlg/favor_json", favorJson);
}

bool RespeqtSettings::dbFavorJson()
{
    return mSettings->value("/DiskBrowserDlg/json_1st", true).toBool();
}

bool RespeqtSettings::dbUseFileNames()
{
    return mSettings->value("/DiskBrowserDlg/use_filenames", true).toBool();
}

bool RespeqtSettings::dbCopyPics()
{
    return mSettings->value("/DiskBrowserDlg/copy_pics", true).toBool();
}

void RespeqtSettings::setDbCopyPics(bool copy)
{
    mSettings->setValue("/DiskBrowserDlg/copy_pics", copy);
}

QString RespeqtSettings::appDataFolder()
{
    QString folder = mSettings->value("/DiskBrowserDlg/appData_folder", QString()).toString();
    if (folder.isEmpty())
    {
        auto    locType   = QStandardPaths::AppDataLocation;
        QString appFolder = QStandardPaths::writableLocation(locType);
        QDir appDataDir(appFolder);

        if (!appDataDir.exists())
            appDataDir.mkpath(".");

        folder = appDataDir.absolutePath();
    }
    return folder;
}

void RespeqtSettings::setAppFolderDir(const QString& appDataDir)
{
    mSettings->setValue("/DiskBrowserDlg/appData_folder", appDataDir);
}

void RespeqtSettings::setDbTitleFont(const LabelFont& font)
{
    mSettings->beginGroup("/DiskBrowserDlg");
    mSettings->beginGroup("title_font");
    mSettings->setValue("family", font.family());
    mSettings->setValue("size", font.pixelSize() <= 0 ? 10 : font.pixelSize());
    mSettings->setValue("bold", font.bold());
    mSettings->setValue("italic", font.italic());
    mSettings->setValue("color", font.color().name());
    mSettings->setValue("scale", font.scale());
    mSettings->endGroup();
    mSettings->endGroup();
}

LabelFont RespeqtSettings::dbTitleFont()
{
    LabelFont font;
    mSettings->beginGroup("/DiskBrowserDlg");
    mSettings->beginGroup("title_font");
    font.setFamily(mSettings->value("family", Label::DEF_TITLE_FNT).toString());
    font.setPixelSize(mSettings->value("size", 10).toInt());
    font.setBold(mSettings->value("bold", Label::DEF_TITLE_BOLD).toBool());
    font.setItalic(mSettings->value("italic", false).toBool());
    font.setColor(mSettings->value("color", "black").toString());
    font.setScale(mSettings->value("scale", Label::DEF_TITLE_SCALE).toDouble());
    mSettings->endGroup();
    mSettings->endGroup();
    return font;
}

void RespeqtSettings::setDbIndexFont(const LabelFont& font)
{
    mSettings->beginGroup("/DiskBrowserDlg");
    mSettings->beginGroup("index_font");
    mSettings->setValue("family", font.family());
    mSettings->setValue("size", font.pixelSize());
    mSettings->setValue("bold", font.bold());
    mSettings->setValue("italic", font.italic());
    mSettings->setValue("color", font.color().name());
    mSettings->setValue("scale", font.scale());
    mSettings->endGroup();
    mSettings->endGroup();
}

LabelFont RespeqtSettings::dbIndexFont()
{
    LabelFont font;
    mSettings->beginGroup("/DiskBrowserDlg");
    mSettings->beginGroup("index_font");
    font.setFamily(mSettings->value("family", Label::DEF_INDEX_FNT).toString());
    font.setPixelSize(mSettings->value("size", 10).toInt());
    font.setBold(mSettings->value("bold", false).toBool());
    font.setItalic(mSettings->value("italic", false).toBool());
    font.setColor(mSettings->value("color", "black").toString());
    font.setScale(mSettings->value("scale", Label::DEF_INDEX_SCALE).toDouble());
    mSettings->endGroup();
    mSettings->endGroup();
    return font;
}

void RespeqtSettings::setDiskPic(const QString& pic)
{
    mSettings->setValue("/DiskBrowserDlg/disk_pic", pic);
}

QString RespeqtSettings::diskPic()
{
    return mSettings->value("/DiskBrowserDlg/disk_pic").toString();
}

const std::unique_ptr<DbSettings>& RespeqtSettings::dbSettings()
{
    if (!sDbSettings)   // instantiate on demand
    {
        if (instance()->dbDataSource() == DbData_appSettings)
            sDbSettings.reset(new DbIni);
        else
            sDbSettings.reset(new DbJson);
    }
    return sDbSettings;
}
