
#include "network/datagram.h"

#include <QString>

namespace Network {
    auto Datagram::getU16At(const int index) const -> quint16 {
        return (at(index + 1) << 8) | at(index);
    }
    
    auto Datagram::setU16At(quint16 value, const int index) -> void {
        char bytesValue[2] {
            static_cast<char>(value & 0xFF),
            static_cast<char>((value >> 8) & 0xFF)
        };
        replace(index, 2, bytesValue, 2);
    }

    auto Datagram::getU32At(const int index) const -> quint32 {
        return (at(index + 3) << 24) | (at(index + 2) << 16)
            | (at(index + 1) << 8) | at(index);
    }
    
    auto Datagram::setU32At(quint32 value, const int index) -> void {
        char bytesValue[4] {
            static_cast<char>(value & 0xFF),
            static_cast<char>((value >> 8) & 0xFF),
            static_cast<char>((value >> 16) & 0xFF),
            static_cast<char>((value >> 24) & 0xFF)
        };
        replace(index, 4, bytesValue, 4);
    }
    
    auto Datagram::getStringAt(int index) const -> QString {
        QString string{""};
    
        while(at(index) != 0) { // TODO Algorithm?
            string.append(at(index));
            index++;
        }
        return string;
    }
    
    auto Datagram::setStringAt(const QString &value, int index) -> void {
        insert(index, value);
        (*this)[index + value.length()] = 0;
    }

    auto Datagram::copyBytes(QByteArray &dst, int from, int to, int length) const -> void {
        auto i = 0;
    
        while(i < length && from + i < this->length()) { // TODO foreach?
            dst[to + i] = (*this)[from + i];
            i++;
        }
    }

    auto Datagram::createAnswer() const -> Datagram
    {
        Datagram answer;
        copyBytes(answer, 0, 0, 4);
        answer[4] = 0;
        return answer;
    }

}