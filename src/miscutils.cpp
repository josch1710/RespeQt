/*
 * miscutils.cpp
 *
 * This file is copyrighted by either Fatih Aygun, Ray Ataergin, or both.
 * However, the years for these copyrights are unfortunately unknown. If you
 * know the specific year(s) please let the current maintainer know.
 */

#include "miscutils.h"

#include <QDebug>
#include <QDir>
#include <QFile>

void deltree(const QString &name) {
  QFileInfo info(name);

  if (info.isDir()) {
    QDir dir(name);
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    foreach (QFileInfo file, list) {
      deltree(file.absoluteFilePath());
    }
    QString n = dir.dirName();
    dir.cdUp();
    dir.rmdir(n);
  } else {
    QFile::remove(name);
  }
}

quint8 sioChecksum(const QByteArray &data, uint size) {
  //    qDebug() << "!d" << tr("DBG -- Serial Port sioChecksum...");

  uint i;
  uint sum = 0;

  for (i = 0; i < size; i++) {
    sum += (quint8) data.at(i);
    if (sum > 255) {
      sum -= 255;
    }
  }

  return sum;
}



/* FileTypes */

FileType getFileType(const QString &fileName) {
  QByteArray header;
  FileType result = FileType::Unknown;

  /* Check if it is a folder */

  if (QFileInfo(fileName).isDir()) {
    return FileType::Dir;
  }

  /* Read the file header */
  if (QFile::exists(fileName)) {
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
      header = file.read(4);
    }
    while (header.count() < 4) {
      header.append('\x0');
    }

    /* Check if the file is gzipped */

    bool gz = false;

    if ((quint8) header.at(0) == 0x1f && (quint8) header.at(1) == 0x8b) {
      /* The file is gzipped, read the real header */
      gz = true;
      GzFile file(fileName);
      if (file.open(QFile::ReadOnly)) {
        header = file.read(4);
      } else {
        header = QByteArray(4, 0);
      }
      while (header.count() < 4) {
        header.append('\x0');
      }
    }

    quint8 b0 = header.at(0);
    quint8 b1 = header.at(1);
    quint8 b2 = header.at(2);
    quint8 b3 = header.at(3);

    /* Determine the file type */

    if (b0 == 'A' && b1 == 'T' && b2 == '8' && b3 == 'X') {
      result = FileType::Atx;
    } else if (b0 == 'F' && b1 == 'U' && b2 == 'J' && b3 == 'I') {
      result = FileType::Cas;
    } else if (b0 == 0x96 && b1 == 0x02) {
      result = FileType::Atr;
    } else if (b0 == 0xFF && b1 == 0xFF) {
      result = FileType::Xex;
    } else if (b0 == 'D' && b1 == 'I') {
      result = FileType::Pro;
    } else if (fileName.endsWith(".XFD", Qt::CaseInsensitive) || fileName.endsWith(".XFZ", Qt::CaseInsensitive) || fileName.endsWith(".XFD.GZ", Qt::CaseInsensitive)) {
      result = FileType::Xfd;
    }

    if (result != FileType::Unknown && gz) {
      result = static_cast<FileType>(static_cast<quint8>(result) + 1); // TODO BAD HACK
    }
  }
  else {
    // Not a file, we guess the type by extension
    QFileInfo file(fileName);
    auto suffix = file.completeSuffix().toLower();
    qDebug() << "!e" << suffix;
    if(suffix.compare("atr"))
        return FileType::Atr;
    else if (suffix.compare("atr.gz"))
        return FileType::AtrGz;
    else if (suffix.compare("atx"))
        return FileType::Atx;
    else if (suffix.compare("atx.gz"))
        return FileType::AtxGz;
    else if (suffix.compare("pro"))
        return FileType::Pro;
    else if (suffix.compare("pro.gz"))
        return FileType::ProGz;
    else if (suffix.compare("xfd"))
        return FileType::Xfd;
    else if (suffix.compare("xfz") || suffix.compare("xfd.gz"))
        return FileType::XfdGz;
  }
  return result;
}

bool isArchive(FileType type) {
  return type == FileType::AtxGz ||
    type == FileType::AtrGz ||
    type == FileType::ProGz ||
    type == FileType::XfdGz;
}

QString getFileTypeName(FileType type) {
  switch (type) {
    case FileType::Atr:
      return QObject::tr("ATR disk image");
    case FileType::AtrGz:
      return QObject::tr("gzipped ATR disk image");
    case FileType::Xfd:
      return QObject::tr("XFD disk image");
    case FileType::XfdGz:
      return QObject::tr("gzipped XFD disk image");
    case FileType::Pro:
      return QObject::tr("PRO disk image");
    case FileType::ProGz:
      return QObject::tr("gzipped PRO disk image");
    case FileType::Atx:
      return QObject::tr("VAPI (ATX) disk image");
    case FileType::AtxGz:
      return QObject::tr("gzipped VAPI (ATX) disk image");
    case FileType::Cas:
      return QObject::tr("CAS cassette image");
    case FileType::CasGz:
      return QObject::tr("gzipped CAS cassette image");
    case FileType::Xex:
      return QObject::tr("Atari executable");
    case FileType::XexGz:
      return QObject::tr("gzipped Atari executable");
    default:
      return QObject::tr("unknown file type");
  }
}

/* GzFile */

GzFile::GzFile(const QString &path)
    : QFile(path) {
  mHandle = 0;
}

bool GzFile::open(OpenMode mode) {
  if (QFile::open(mode)) {
    if ((mode & ReadOnly) == ReadOnly) {
      mHandle = gzdopen(handle(), "rb");
    } else if ((mode & ReadWrite) == ReadWrite) {
      mHandle = gzdopen(handle(), "wb+");
    } else if ((mode & WriteOnly) == WriteOnly) {
      mHandle = gzdopen(handle(), "wb");
    }

    if (mHandle == nullptr) {
      setErrorString(tr("gzdopen() failed."));
      return false;
    }
  } else {
    return false;
  }
  return true;
}

void GzFile::close() {
  if (mHandle) {
    gzclose(mHandle);
  }
  QFile::close();
  mHandle = nullptr;
}

bool GzFile::isSequential() const {
  return true;
}

bool GzFile::seek(qint64 pos) {
  bool result = gzseek(mHandle, pos, SEEK_SET) != -1;
  if (!result) {
    setErrorString(tr("gzseek() failed."));
  }
  return result;
}

qint64 GzFile::readData(char *data, qint64 maxSize) {
  if (mHandle == nullptr) {
    return 0;
  }

  return gzread(mHandle, data, maxSize);
}

qint64 GzFile::writeData(const char *data, qint64 maxSize) {
  if (mHandle == nullptr) {
    return 0;
  }

  return gzwrite(mHandle, data, maxSize);
}

bool GzFile::atEnd() const {
  return gzeof(mHandle);
}
