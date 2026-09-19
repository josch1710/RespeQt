
#include "filesystems/ataridirentry.h"

namespace Filesystems {
  /* AtariDirEntry */
  AtariDirEntry::AtariDirEntry()
      : firstSector(UINT16_MAX), no(UINT16_MAX), dir(UINT16_MAX), size(0) {}


  QString AtariDirEntry::name() const {
    QString s = baseName();
    if (const QString e = suffix(); !e.isEmpty()) {
      s.append(".");
      s.append(e);
    }
    return s;
  }

  QString AtariDirEntry::niceName() const {
    QString n = name();
    if (n == n.toUpper()) {
      return n.toLower();
    }
    return n;
  }

  QString AtariDirEntry::baseName() const {
    return QString::fromLatin1(atariName.left(8).constData()).trimmed();
  }

  QString AtariDirEntry::suffix() const {
    return QString::fromLatin1(atariName.right(3).constData()).trimmed();
  }

  QString AtariDirEntry::attributeNames() const {
    QString result;
    if (attributes & Locked) {
      result += "Locked";
    }
    if (attributes & Hidden) {
      if (!result.isEmpty()) {
        result += ", ";
      }
      result += "Hidden";
    }
    if (attributes & Archived) {
      if (!result.isEmpty()) {
        result += ", ";
      }
      result += "Archived";
    }
    if (attributes & Directory) {
      if (!result.isEmpty()) {
        result += ", ";
      }
      result += "Directory";
    }
    if (attributes & Dos10) {
      if (!result.isEmpty()) {
        result += ", ";
      }
      result += "Dos 1.0 file";
    }
    if (attributes & Dos25) {
      if (!result.isEmpty()) {
        result += ", ";
      }
      result += "Dos 2.5 file";
    }
    if (attributes & MyDos) {
      if (!result.isEmpty()) {
        result += ", ";
      }
      result += "MyDos file";
    }
    return result;
  }

  void AtariDirEntry::makeFromAtariDosEntry(const QByteArray &entry, const quint16 aNo, const quint16 aDir, const bool dd) {
    // Translate the attributes
    attributes = Attributes();

    internalData = entry;

    const auto f {static_cast<quint8>(entry.at(0))};

    if (f & 0x10) {
      attributes |= Directory;
    }
    if (f & 0x20) {
      attributes |= Locked;
    }
    if (f & 0x01 && !(f & 0x40)) {
      attributes |= Dos25;
    }
    if (f & 0x04) {
      attributes |= MyDos;
    }
    if (!(f & 0x02) && !(f & 0x10)) {
      attributes |= Dos10;
    }

    // Translate the name
    atariName = entry.mid(5, 11);

    // Translate the size in blocks
    if (attributes & Directory) {
      size = -1;
    } else {
      if (dd) {
        size = (static_cast<quint8>(entry.at(1)) + static_cast<quint8>(entry.at(2)) * 256) * 253;
      } else {
        size = (static_cast<quint8>(entry.at(1)) + static_cast<quint8>(entry.at(2)) * 256) * 125;
      }
    }

    // Translate the first sector
    firstSector = static_cast<quint8>(entry.at(3)) + static_cast<quint8>(entry.at(4)) * 256;

    // Put an invalid date
    dateTime = QDateTime();

    dir = aDir;
    no = aNo;
  }

  void AtariDirEntry::makeFromSpartaDosEntry(const QByteArray &entry, const int aNo, const int aDir) {
    // Translate the attributes
    attributes = Attributes();

    internalData = entry;

    const auto f = static_cast<quint8>(entry.at(0));

    if (f & 0x01) {
      attributes |= Locked;
    }
    if (f & 0x02) {
      attributes |= Hidden;
    }
    if (f & 0x04) {
      attributes |= Archived;
    }
    if (f & 0x20) {
      attributes |= Directory;
    }

    // Translate the name
    atariName = entry.mid(6, 11);

    // Translate the size
    if (attributes & Directory) {
      size = -1;
    } else {
      size = static_cast<quint8>(entry.at(3)) + static_cast<quint8>(entry.at(4)) * 256 + static_cast<quint8>(entry.at(5)) * 65536;
    }

    // Translate the first sector
    firstSector = static_cast<quint8>(entry.at(1)) + static_cast<quint8>(entry.at(2)) * 256;

    // Translate the date/time
    int year = static_cast<quint8>(entry.at(19)) + 1900;
    if (year < 1980) {
      year += 100;
    }
    const QDate date(year, static_cast<quint8>(entry.at(18)), static_cast<quint8>(entry.at(17)));
    const QTime time(static_cast<quint8>(entry.at(20)), static_cast<quint8>(entry.at(21)), static_cast<quint8>(entry.at(22)));
    dateTime = QDateTime(date, time);

    dir = static_cast<quint16>(aDir);
    no = aNo;
  }

}// namespace Filesystems
