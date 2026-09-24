#ifndef SDXPROTOCOLS_H
#define SDXPROTOCOLS_H

#include "sio/sioworker.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef ushort
#define ushort unsigned short
#endif

#ifndef ulong
#define ulong unsigned long
#endif

namespace SIO {

#pragma pack(push, 1)
struct DIRENTRY {
  quint8 status;
  quint8 map_l, map_h;
  quint8 len_l, len_m, len_h;
  char fname[11];
  quint8 stamp[6];
};
#pragma pack(pop)

class SDXProtocol : public SioDevice {
  Q_OBJECT

public:
  static constexpr quint32 SDX_MAXLEN = 16777215UL;

  /* SDX required attribute mask */
  static constexpr quint8 RA_PROTECT     = 0x01;
  static constexpr quint8 RA_HIDDEN      = 0x02;
  static constexpr quint8 RA_ARCHIVED    = 0x04;
  static constexpr quint8 RA_SUBDIR      = 0x08;
  static constexpr quint8 RA_NO_PROTECT  = 0x10;
  static constexpr quint8 RA_NO_HIDDEN   = 0x20;
  static constexpr quint8 RA_NO_ARCHIVED = 0x40;
  static constexpr quint8 RA_NO_SUBDIR   = 0x80;

  /* SDX set attribute mask */
  static constexpr quint8 SA_PROTECT   = 0x01;
  static constexpr quint8 SA_UNPROTECT = 0x10;
  static constexpr quint8 SA_SUBDIR    = 0x08;   /* illegal mode */
  static constexpr quint8 SA_UNSUBDIR  = 0x80;   /* illegal mode */

  explicit SDXProtocol(const SioWorkerPtr& worker);
  ~SDXProtocol() override = default;
};

} // namespace SIO

#endif// SDXPROTOCOLS_H
