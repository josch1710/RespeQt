//
// disassembly810 class
// (c) 2018 Eric BACHER
//

// ReSharper disable CppCStyleCast
#include "include/diskimages/disassembly810.h"

namespace DiskImages {
  namespace
  {
    struct labels {
      unsigned short address;
      const char *label;
    };
  }

  static constexpr labels labels810[] = {
          {0x0000, "FCOMMAND"},
          {0x0001, "FTRACK"},
          {0x0002, "FSECTOR"},
          {0x0003, "FDATA"},
          {0x0380, "DRA"},
          {0x0381, "DDRA"},
          {0x0382, "DRB"},
          {0x0383, "DDRB"},
          {0x0384, "EDGECTRL"},
          {0x0394, "RTIMNOIT"},
          {0x0395, "RINTFLAG"},
          {0x0396, "WR64NO"},
          {0x0397, "WR1024NO"},
          {0x039C, "RTIMIT"},
          {0x039E, "WR64IT"},
          {0x039F, "WR1024IT"},
  };

  disassembly810::disassembly810()
      : Cpu6502(CPU_6502) {}

  const char *disassembly810::GetAddressLabel(const unsigned short addr) const {
    for (auto & [address, label] : labels810) {
      if (address == addr) {
        return label;
      }
    }
    return nullptr;
  }

}