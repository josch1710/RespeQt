#ifndef RAWOUTPUT_H
#define RAWOUTPUT_H

#include "nativeoutput.h"
#include <QObject>

#if defined(Q_OS_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(Q_OS_MAC) || defined(Q_OS_LINUX)
#include <cups/cups.h>
#endif

class QComboBox;

namespace Printers {
  class RawOutput {
  public:
    RawOutput();
    virtual ~RawOutput();

    virtual bool beginOutput();
    virtual bool endOutput();
    virtual void updateBoundingBox();
    virtual void newPage(bool linefeed = false);
    virtual bool setupOutput() { return true; }
    bool sendBuffer(const QByteArray &b, unsigned int len);

    static QString typeName() {
      return QObject::tr("Raw output");
    }

    static void setupRawPrinters(QComboBox *list);

  protected:
    QString rawPrinterName;
#if defined(Q_OS_WIN)
    HANDLE mJob;
#elif defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    int mJobId;
    cups_dest_t *mDest;
    cups_dinfo_t *mInfo;
    http_t *mHttp;
#endif
  };
}// namespace Printers
#endif// RAWOUTPUT_H
