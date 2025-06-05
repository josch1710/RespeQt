#ifndef ATARIDIRENTRY_H
#define ATARIDIRENTRY_H

#include <QDateTime>

namespace Filesystems {

  class AtariDirEntry {
  public:
    AtariDirEntry();
    enum Attribute {
      Locked = 1,
      Hidden = 2,
      Archived = 4,
      Directory = 8,
      Dos10 = 16,
      Dos25 = 32,
      MyDos = 64
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

    quint16 firstSector;
    QByteArray atariName;
    Attributes attributes;
    int no;
    quint16 dir;
    int size;
    QDateTime dateTime;

    QString name() const;
    QString niceName() const;
    QString baseName() const;
    QString suffix() const;
    QString attributeNames() const;

    QByteArray internalData;

    void makeFromAtariDosEntry(const QByteArray &entry, int aNo, int aDir, bool dd = false);
    void makeFromSpartaDosEntry(const QByteArray &entry, int aNo, int aDir);

    inline bool isValid() { return no != -1; }
  };

  Q_DECLARE_OPERATORS_FOR_FLAGS(AtariDirEntry::Attributes)


}// namespace Filesystems

#endif
