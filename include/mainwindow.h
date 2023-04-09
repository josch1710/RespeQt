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
  ~MainWindow();
  QString g_sessionFile;
  QString g_sessionFilePath;
  QString g_mainWindowTitle;
  void doLogMessage(int type, const QString &msg);
  static void logMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
  static MainWindow *instance() {
    return sInstance;
  }

private:
  static MainWindow *sInstance;
  int untitledName;
  Ui::MainWindow *ui;
  SioWorkerPtr sio;
  bool shownFirstTime;
  PrinterWidget *printerWidgets[PRINTER_COUNT];//
  DriveWidget *diskWidgets[DISK_COUNT];        //
  // InfoWidget* infoWidget;
  SioRecorder *mRecorder{nullptr};
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
  int containingDiskSlot(const QPoint &point);
  void bootExe(const QString &fileName);
  void mountFile(int no, const QString &fileName, bool prot);
  void mountDiskImage(int no);
  void mountFolderImage(int no);
  bool ejectImage(int no, bool ask = true);
  void loadNextSide(int no);
  void toggleHappy(int no, bool enabled);
  void toggleChip(int no, bool open);
  void toggleOSB(int no, bool open);
  void toggleToolDisk(int no, bool open);
  void toggleWriteProtection(int no, bool protectionEnabled);
  void updateHighSpeed();

  void openEditor(int no);
  void saveDisk(int no);
  void saveDiskAs(int no);
  void revertDisk(int no);
  QMessageBox::StandardButton saveImageWhenClosing(int no, QMessageBox::StandardButton previousAnswer, int number);
  void loadTranslators();
  void autoSaveDisk(int no);//
  void setUpPrinterEmulationWidgets(bool enabled);

  void createDeviceWidgets();
  SimpleDiskImage *installDiskImage(int no);
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
  void logMessage(int type, const QString &msg);
  void newSlot(int slot);
  void fileMounted(bool mounted);
  void sendLogText(QString logText);            //
  void sendLogTextChange(QString logTextChange);//
  void setFont(const QFont &font);              //

public slots:
  void show();
  int firstEmptyDiskSlot(int startFrom = 0, bool createOne = true);    //
  void mountFileWithDefaultProtection(int no, const QString &fileName);//
  void autoCommit(int no, bool st);                                    //
  void happy(int no, bool st);                                         //
  void chip(int no, bool st);                                          //
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
  void mountDiskTriggered(int deviceId);                        //
  void mountFolderTriggered(int deviceId);                      //
  void ejectTriggered(int deviceId);                            //
  void nextSideTriggered(int deviceId);                         //
  void happyToggled(int deviceId, bool enabled);                //
  void chipToggled(int deviceId, bool open);                    //
  void OSBToggled(int deviceId, bool open);                     //
  void toolDiskTriggered(int deviceId, bool open);              //
  void protectTriggered(int deviceId, bool writeProtectEnabled);//
  void editDiskTriggered(int deviceId);                         //
  void saveTriggered(int deviceId);                             //
  void autoSaveTriggered(int deviceId);                         //
  void saveAsTriggered(int deviceId);                           // MIA
  void revertTriggered(int deviceId);                           // MIA


  void bootOptionTriggered();    //
  void toggleMiniModeTriggered();//
  void toggleShadeTriggered();   //
  void showLogWindowTriggered(); //

  void showHideDrives();                //
  void sioFinished();                   //
  void sioStarted();                    //
  void sioStatusChanged(QString status);//
  void textPrinterWindowClosed();
  void docDisplayWindowClosed();         //
  void deviceStatusChanged(int deviceNo);//
  void uiMessage(int t, const QString message);
  // TODO Check on Windows and Linux
  void trayIconActivated(QSystemTrayIcon::ActivationReason reason);//
  void keepBootExeOpen();                                          // Signal AutoBootDialog::keepOpen MIA
  void saveWindowGeometry();
  void saveMiniWindowGeometry();
  void logChanged(QString text);
};

#endif// MAINWINDOW_H
