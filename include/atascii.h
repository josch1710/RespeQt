#ifndef ATASCII_H
#define ATASCII_H

#include <QChar>
#include <map>

class Atascii {
public:
  Atascii();
  QChar operator()(const unsigned char atascii) const;
  unsigned char operator()(const QChar &unicode) const;

protected:
  static std::map<unsigned char, QChar> mapping;
};

#endif// ATASCII_H
