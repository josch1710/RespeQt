/*
 * miscutils.h
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#ifndef MISCUTILS_H
#define MISCUTILS_H

#include "zlib.h"
#include <QFile>
#include <QString>

void deltree(const QString &name);
quint8 sioChecksum(const QByteArray &data, uint size);

class GzFile : public QFile {
  Q_OBJECT

public:
  GzFile(const QString &path);

  bool open(OpenMode mode) override;
  void close() override;
  bool seek(qint64 pos) override;
  bool isSequential() const override;
  bool atEnd() const override;

protected:
  gzFile mHandle;
  qint64 readData(char *data, qint64 maxSize) override;
  qint64 writeData(const char *data, qint64 maxSize) override;
};

inline Qt::CaseSensitivity osCaseSensitivity() {
#if defined Q_OS_LINUX
  return Qt::CaseSensitive;
#else
  return Qt::CaseInsensitive;
#endif
}

class FileTypes : public QObject
{
    Q_OBJECT

public:
    enum class FileType : quint8
    {
        Unknown,
        Dir,
        Atr,
        AtrGz,
        Xfd,
        XfdGz,
        Pro,
        ProGz,
        Atx,
        AtxGz,
        Cas,
        CasGz,
        Xex,
        XexGz
    };
    static FileType getFileType(const QString &fileName);
    __attribute__((unused)) static QString getFileTypeName(FileType type);
    static QStringList toStringList(const QList<QByteArray>& list);

    static bool isArchive(FileType type);
    static const QStringList &getDiskImageTypes()
    {
        static QStringList list{
            "*.xfd", "*.atr", "*.pro", "*.atx"
#if defined Q_OS_LINUX
            ,
            "*.XFD", "*.ATR", "*.PRO", "*.ATX"  // Linux is case sensitive
#endif
        };
        return list;
    }
};
#endif// MISCUTILS_H
