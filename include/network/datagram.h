#ifndef RESPEQT_DATAGRAM_H
#define RESPEQT_DATAGRAM_H

#include <QByteArray>

namespace Network
{
    class Datagram: public QByteArray
    {
    public:
        [[nodiscard]] auto getU16At(int index) const -> quint16;
        auto setU16At(quint16 value, int index) -> void;
        [[nodiscard]] auto getU32At(int index) const -> quint32;
        auto setU32At(quint32 value, int index) -> void;
        [[nodiscard]] auto getStringAt(int index) const -> QString;
        auto setStringAt(const QString &value, int index) -> void;
        [[nodiscard]] auto getRawBytes(quint16 length, int index) const -> QByteArray;
        auto setRawBytes(const QByteArray& data, int index) -> void;
        [[nodiscard]] auto createAnswer() const -> Datagram;
        [[nodiscard]] auto getSessionID() const -> quint16 { return getU16At(0); }

    private:
        auto copyBytes(QByteArray &dst, int from, int to, int length) const -> void;
    };
}


#endif  //RESPEQT_DATAGRAM_H
