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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QFrame>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QTextEdit>
#include <QTranslator>
#include <QtDebug>
#include <memory>

#include "aboutdialog.h"
#include "createimagedialog.h"
#include "diskeditdialog.h"
#include "docdisplaywindow.h"
#include "drivewidget.h"
#include "infowidget.h"
#include "logdisplaydialog.h"
#include "network.h"
#include "optionsdialog.h"
#include "printers/outputwindow.h"
#include "printers/textprinterwindow.h"
#include "printerwidget.h"
#include "serialport.h"
#include "siorecorder.h"
#include "sioworker.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow() override;
  QString g_sessionFile;
  QString g_sessionFilePath;
  QString g_mainWindowTitle;
  void doLogMessage(int type, const QString &msg);
  static void logMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
  static MainWindow *instance() { return sInstance; }
  void showEvent(QShowEvent *event) override;

private:
  static MainWindow *sInstance;
  int untitledName;
  Ui::MainWindow *ui;
  SioWorkerPtr sio;
  bool shownFirstTime;
  std::vector<PrinterWidget *> printerWidgets{PRINTER_COUNT};
  std::vector<DriveWidget *> diskWidgets{DISK_COUNT};
  // InfoWidget* infoWidget;
  //SioRecorder *mRecorder{nullptr};
  QString mTestfile{};

  QLabel *speedLabel, *onOffLabel, *prtOnOffLabel, *netLabel, *clearMessagesLabel, *limitEntriesLabel;//

  DocDisplayWindow *docDisplayWindow;//
  QTranslator respeqt_translator, respeqt_qt_translator;
  QSystemTrayIcon trayIcon;
  Qt::WindowFlags oldWindowFlags;
  Qt::WindowStates oldWindowStates;
  QString lastMessage;
  int lastMessageRepeat;

  bool isClosing;

  LogDisplayDialog *logWindow_;

  QList<QAction *> recentFilesActions_;

  void setSession();//
  void updateRecentFileActions();
  char containingDiskSlot(const QPoint &point);
  void bootExe(const QString &fileName);
  void mountFile(char no, const QString &fileName, bool prot);
  void mountDiskImage(char no);
  void mountFolderImage(char no);
  bool ejectImage(char no, bool ask = true);
  void loadNextSide(char no);
  void toggleHappy(char no, bool enabled);
  void toggleChip(char no, bool open);
  void toggleOSB(char no, bool open);
  void toggleToolDisk(char no, bool open);
  void toggleWriteProtection(char no, bool protectionEnabled);
  void updateHighSpeed();

  void openEditor(char no);
  void saveDisk(char no);
  void saveDiskAs(char no);
  void revertDisk(char no);
  QMessageBox::StandardButton saveImageWhenClosing(char no, QMessageBox::StandardButton previousAnswer, int number);
  void loadTranslators();
  void autoSaveDisk(char no);//
  void setUpPrinterEmulationWidgets(bool enabled);

  void createDeviceWidgets();
  //SimpleDiskImage *installDiskImage(char no);
  void changeFonts();
  void connectUISignal();
  SimpleDiskImage *installDiskImage();

  void setupDebugItems();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragLeaveEvent(QDragLeaveEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void closeEvent(QCloseEvent *event) override;
  void hideEvent(QHideEvent *event) override;
  void enterEvent(QEvent *) override;
  void leaveEvent(QEvent *) override;
  bool eventFilter(QObject *obj, QEvent *event) override;

signals:
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotImplementedFunctions"
#pragma ide diagnostic ignored "NotUsedFunctions"
  void logMessage(int type, const QString &msg);
#pragma clang diagnostic pop
  void newSlot(char slot);
  void fileMounted(bool mounted);
  void sendLogText(QString logText);
  void sendLogTextChange(QString logTextChange);
  void fontChanged(const QFont &font);

public slots:
  char firstEmptyDiskSlot(char startFrom = 0, bool createOne = true);   //
  void mountFileWithDefaultProtection(char no, const QString &fileName);//
  void autoCommit(char no, bool st);                                    //
  void happy(char no, bool st);                                         //
  void chip(char no, bool st);                                          //
  void bootExeTriggered(const QString &fileName);
  void toggleSnapshotCapture(bool toggle);
  void replaySnapshot();
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
  void startEmulationTriggered();   //
  void printerEmulationTriggered(); //
  void hideShowTriggered();         //
  void quitApplicationTriggered();  //
  void showAboutTriggered();        //
  void showDocumentationTriggered();//
  void toggleLimitEntriesTriggered();

  // Device widget events
  void mountDiskTriggered(char no);                        //
  void mountFolderTriggered(char no);                      //
  void ejectTriggered(char no);                            //
  void nextSideTriggered(char no);                         //
  void happyToggled(char no, bool enabled);                //
  void chipToggled(char no, bool open);                    //
  void OSBToggled(char no, bool open);                     //
  void toolDiskTriggered(char no, bool open);              //
  void protectTriggered(char no, bool writeProtectEnabled);//
  void editDiskTriggered(char no);                         //
  void saveTriggered(char no);                             //
  void autoSaveTriggered(char no);                         //
  void saveAsTriggered(char no);                           // MIA
  void revertTriggered(char no);                           // MIA


  void bootOptionTriggered();    //
  void toggleMiniModeTriggered();//
  void toggleShadeTriggered();   //
  void showLogWindowTriggered(); //

  void showHideDrives();                       //
  void sioFinished();                          //
  void sioStarted();                           //
  void sioStatusChanged(const QString &status);//
  //void textPrinterWindowClosed();
  void docDisplayWindowClosed();                   //
  void deviceStatusChanged(unsigned char deviceNo);//
  void uiMessage(int t, QString message);
  // TODO Check on Windows and Linux
  void trayIconActivated(QSystemTrayIcon::ActivationReason reason);//
  //void keepBootExeOpen();                                          // Signal AutoBootDialog::keepOpen MIA
  void saveWindowGeometry();
  void saveMiniWindowGeometry();
  void logChanged(QString text);
};

#endif// MAINWINDOW_H
