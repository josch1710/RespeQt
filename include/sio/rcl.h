/*
 * rcl.h
 *
 * Copyright 2015, 2017 Joseph Zatarski
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef RCL_H
#define RCL_H

#include "sio/sioworker.h"

namespace SIO {

// RespeQt Time Server //
class RCl : public SioDevice {
  Q_OBJECT

public:
  QString g_fileFilter;
  explicit RCl(const SioWorkerPtr& worker) : SioDevice(worker) {}
  void handleCommand(quint8 command, quint8 aux1, quint8 aux2) override;
  QString toAtariFileName(const QString& dosFileName);
  QString toDosFileName(const QString& atariFileName);

public slots:
  void gotNewSlot(int slot);     //
  void fileMounted(bool mounted);//

signals:
  void findNewSlot(int startFrom, bool createOne);
  void mountFile(int no, QString fileName);
  void toggleAutoCommit(int no, bool st);
  void toggleHappy(int no, bool st);
  void toggleChip(int no, bool st);
  void bootExe(QString fileName);

private:
  QString imageFileName;
  static char rclSlotNo;
  QMutex mutex;
};

} // namespace SIO

#endif// RCL_H
