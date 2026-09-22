/*
 * mainwindow.h
 *
 * Copyright 2015 Joseph Zatarski
 * Copyright 2016 TheMontezuma
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

// ReSharper disable CppUnusedIncludeDirective
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTextEdit>
#include <QTranslator>

#include "ui/aboutdialog.h"
#include "ui/createimagedialog.h"
#include "ui/diskeditdialog.h"
#include "ui/docdisplaywindow.h"
#include "ui/drivewidget.h"
#include "ui/logdisplaydialog.h"
#include "ui/optionsdialog.h"
#include "printers/outputwindow.h"
#include "ui/printerwidget.h"
#include "siorecorder.h"
#include "sioworker.h"
#include "diskbrowser/diskbrowserdlg.h"
#include "network/tnfsudp.h"
#include "network/tnfstcp.h"

#define MAXBLOCK 500

namespace Ui {
  class MainWindow;
}

namespace UI {

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;
  QString g_sessionFile;
  QString g_sessionFilePath;
  QString g_mainWindowTitle;
  void doLogMessage(int type, const QString &msg);
  bool checkChangeDbSource(DbDataSource dbSourceNew);
  static void logMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
  static MainWindow *instance() { return sInstance; }
  void setupDebugItems() const;

  bool eventFilter(QObject *obj, QEvent *event) override;

private:
  static MainWindow *sInstance;
  int untitledName;
  ::Ui::MainWindow *ui;
  SioWorkerPtr sio;
  bool shownFirstTime;
  std::vector<PrinterWidget *> printerWidgets;
  std::vector<DriveWidget *> diskWidgets;
  // InfoWidget* infoWidget;
  //SioRecorder *mRecorder{nullptr};
  QString mTestfile;

  QLabel *speedLabel, *onOffLabel, *prtOnOffLabel, *netLabel, *clearMessagesLabel, *limitEntriesLabel;//

  DocDisplayWindow *docDisplayWindow;//
  QTranslator respeqt_translator, respeqt_qt_translator;
  QSystemTrayIcon trayIcon;
  Qt::WindowFlags oldWindowFlags;
  Qt::WindowStates oldWindowStates;
  QString lastMessage;
  int lastMessageRepeat{0};
  DiskBrowserDlg* diskBrowserDlg;

  bool isClosing{false};

  LogDisplayDialog *logWindow_;

  QList<QAction *> recentFilesActions_;

  QRect savedGeometry;          // for mini mode toggle
  QPoint savedPosition;         // for mini frameless move

  bool isD9DOVisible = true;    // column 2 disks visible
  bool isMiniMode    = false;   // mini mode disk 1 only
  bool isShadeMode   = false;   // mini shade mode main win

  Network::TnfsUdp tnfsudp;
  Network::TnfsTcp tnfstcp;

  void setSession();//
  void updateRecentFileActions();
  [[nodiscard]] char containingDiskSlot(const QPoint &point) const;
  void mountFile(char no, const QString &fileName, bool prot);
  void mountDiskImage(char no);
  void mountFolderImage(char no);
  bool ejectImage(char no, bool ask = true);
  void loadNextSide(char no);
  void toggleHappy(char no, bool open) const;
  void toggleChip(char no, bool open) const;
  void toggleOSB(char no, bool open) const;
  void toggleToolDisk(char no, bool open) const;
  void toggleWriteProtection(char no, bool protectionEnabled) const;
  void updateHighSpeed() const;

  void openEditor(char no) const;
  void saveDisk(char no);
  void saveDiskAs(char no);
  void revertDisk(char no);
  QMessageBox::StandardButton saveImageWhenClosing(char no, QMessageBox::StandardButton previousAnswer, int number);
  void loadTranslators();
  void autoSaveDisk(char no);//
  void setUpPrinterEmulationWidgets(bool enabled) const;

  void createDeviceWidgets();
  //SimpleDiskImage *installDiskImage(char no);
  void changeFonts();
  void connectUISignal();
  [[nodiscard]] DiskImages::SimpleDiskImage *installDiskImage() const;

  void restoreLayout();

protected:
  void showEvent(QShowEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragLeaveEvent(QDragLeaveEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void closeEvent(QCloseEvent *event) override;
  void hideEvent(QHideEvent *event) override;
#if (QT_VERSION_MAJOR < 6)
  void enterEvent(QEvent *) override;
#else
  void enterEvent(QEnterEvent *) override;
#endif
  void leaveEvent(QEvent *) override;

signals:
  [[maybe_unused]] void logMessage(int type, const QString &msg);
  void newSlot(char slot);
  void fileMounted(bool mounted);
  void sendLogText(QString logText);
  void sendLogTextChange(QString logTextChange);
  void fontChanged(const QFont &font);
  void startTnfs();
  void stopTnfs();
  void startBootExe(const QString &fileName);

public slots:
  char firstEmptyDiskSlot(char startFrom = 0, bool createOne = true);   //
  void mountFileWithDefaultProtection(char no, const QString &fileName);//
  void autoCommit(char no, bool st) const;                                    //
  void happy(char no, bool st) const;                                         //
  void chip(char no, bool st) const;                                          //
  void bootExeTriggered(const QString &fileName);
  void toggleSnapshotCapture(bool toggle) const;
  void replaySnapshot();
  void bootExe(const QString &fileName);

  //void closeTextPrinterWindow(const Printers::TextPrinterWindow *window);

private slots:
  void openRecent();//

  void cassettePlaybackTriggered(); //
  void selectBootExeTriggered();    //
  void saveSessionTriggered();      //
  void openSessionTriggered();      //
  void newImageTriggered();         //
  void ejectAllTriggered();         //
  void showOptionsTriggered();      //
  void startEmulationTriggered() const;   //
  void printerEmulationTriggered() const; //
  void hideShowTriggered();         //
  void quitApplicationTriggered();  //
  void showAboutTriggered();        //
  void showDocumentationTriggered() const;//
  void toggleLimitEntriesTriggered() const;

  // Device widget events
  void mountDiskTriggered(char no);                        //
  void mountFolderTriggered(char no);                      //
  void ejectTriggered(char no);                            //
  void nextSideTriggered(char no);                         //
  void happyToggled(char no, bool open) const;                //
  void chipToggled(char no, bool open) const;                    //
  void OSBToggled(char no, bool open) const;                     //
  void toolDiskTriggered(char no, bool open) const;              //
  void protectTriggered(char no, bool writeProtectEnabled) const;//
  void editDiskTriggered(char no) const;                         //
  void saveTriggered(char no);                             //
  void autoSaveTriggered(char no);                         //
  void saveAsTriggered(char no);                           // MIA
  void revertTriggered(char no);                           // MIA

  void bootOptionTriggered(char no);//
  void toggleMiniModeTriggered();//
  void toggleShadeTriggered();   //
  void showLogWindowTriggered(); //
  void diskBrowserTriggered();   // what are all these trailing comment markers for?
  void showHideDrives() const;                       //
  void sioFinished() const;                          //
  void sioStarted() const;                           //
  void sioStatusChanged(const QString &status) const;//
  //void textPrinterWindowClosed();
  void docDisplayWindowClosed() const;                   //
  void deviceStatusChanged(unsigned char deviceNo);//
  void uiMessage(int t, QString message);
  // TODO Check on Windows and Linux
  void trayIconActivated(QSystemTrayIcon::ActivationReason reason);//
  //void keepBootExeOpen();                                          // Signal AutoBootDialog::keepOpen MIA
  void logChanged(QString text);

  // TNFS
  void sessionConnected() const;
  void allSessionsDisconnected() const;
};

} // namespace UI

#endif// MAINWINDOW_H
