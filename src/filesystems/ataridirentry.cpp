
#include "filesystems/ataridirentry.h"

namespace Filesystems {
  /* AtariDirEntry */
  AtariDirEntry::AtariDirEntry()
      : no(-1), firstSector(-1), dir(-1), size(0) {}


  QString AtariDirEntry::name() const {
    QString s = baseName();
    QString e = suffix();
    if (!e.isEmpty()) {
      s.append(".");
      s.append(e);
    }
    return s;
  }

  QString AtariDirEntry::niceName() const {
    QString n = name();
    if (n == n.toUpper()) {
      return n.toLower();
    } else {
      return n;
    }
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

  void AtariDirEntry::makeFromAtariDosEntry(const QByteArray &entry, int aNo, int aDir, bool dd) {
    // Translate the attributes
    attributes = Attributes();

    internalData = entry;

    auto f = (quint8) entry.at(0);

    if (f & 0x10) {
      attributes |= Directory;
    }
    if (f & 0x20) {
      attributes |= Locked;
    }
    if ((f & 0x01) && !(f & 0x40)) {
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
        size = ((quint8) entry.at(1) + (quint8) entry.at(2) * 256) * 253;
      } else {
        size = ((quint8) entry.at(1) + (quint8) entry.at(2) * 256) * 125;
      }
    }

    // Translate the first sector
    firstSector = (quint8) entry.at(3) + (quint8) entry.at(4) * 256;

    // Put an invalid date
    dateTime = QDateTime();

    dir = aDir;
    no = aNo;
  }

  void AtariDirEntry::makeFromSpartaDosEntry(const QByteArray &entry, int aNo, int aDir) {
    // Translate the attributes
    attributes = Attributes();

    internalData = entry;

    auto f = (quint8) entry.at(0);

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
      size = (quint8) entry.at(3) + (quint8) entry.at(4) * 256 + (quint8) entry.at(5) * 65536;
    }

    // Translate the first sector
    firstSector = (quint8) entry.at(1) + (quint8) entry.at(2) * 256;

    // Translate the date/time
    int year = (quint8) entry.at(19) + 1900;
    if (year < 1980) {
      year += 100;
    }
    QDate date(year, (quint8) entry.at(18), (quint8) entry.at(17));
    QTime time((quint8) entry.at(20), (quint8) entry.at(21), (quint8) entry.at(22));
    dateTime = QDateTime(date, time);

    dir = aDir;
    no = aNo;
  }

}// namespace Filesystems
