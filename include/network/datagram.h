#ifndef RESPEQT_DATAGRAM_H
#define RESPEQT_DATAGRAM_H

#include <QByteArray>

namespace Network
{
    class Datagram: public QByteArray
    {
    public:
        auto getU16At(const int index) const -> quint16;
        auto setU16At(quint16 value, const int index) -> void;
        auto getU32At(const int index) const -> quint32;
        auto setU32At(quint32 value, const int index) -> void;
        auto getStringAt(int index) const -> QString;
        auto setStringAt(const QString &value, int index) -> void;
        auto getRawBytes(const quint16 length, const int index) const -> QByteArray;
        auto setRawBytes(const QByteArray& data, int index) -> void;
        auto createAnswer() const -> Datagram;
        auto getSessionID() const -> quint16 { return getU16At(0); }

    private:
        auto copyBytes(QByteArray &dst, int from, int to, int length) const -> void;
    };
}


#endif  //RESPEQT_DATAGRAM_H
