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

    [[nodiscard]] QString name() const;
    [[nodiscard]] QString niceName() const;
    [[nodiscard]] QString baseName() const;
    [[nodiscard]] QString suffix() const;
    [[nodiscard]] QString attributeNames() const;

    QByteArray internalData;

    void makeFromAtariDosEntry(const QByteArray &entry, quint16 aNo, quint16 aDir, bool dd = false);
    void makeFromSpartaDosEntry(const QByteArray &entry, int aNo, int aDir);

    [[nodiscard]] bool isValid() const { return no != -1; }
  };

  Q_DECLARE_OPERATORS_FOR_FLAGS(AtariDirEntry::Attributes)


}// namespace Filesystems

#endif
