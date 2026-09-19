//
// disassembly1050 class
// (c) 2018 Eric BACHER
//

#include "include/diskimages/disassembly1050.h"

namespace DiskImages {
  namespace
  {
    struct labels {
      unsigned short address;
      const char *label;
    };
  }

  static constexpr labels labels1050[] = {
          {0x0400, "FCOMMAND"},
          {0x0401, "FTRACK"},
          {0x0402, "FSECTOR"},
          {0x0403, "FDATA"},
          {0x0280, "DRA"},
          {0x0281, "DDRA"},
          {0x0282, "DRB"},
          {0x0283, "DDRB"},
          {0x0284, "EDGECTRL"},
          {0x0294, "RTIMNOIT"},
          {0x0295, "RINTFLAG"},
          {0x0296, "WR64NO"},
          {0x0297, "WR1024NO"},
          {0x029C, "RTIMIT"},
          {0x029E, "WR64IT"},
          {0x029F, "WR1024IT"},
  };

  disassembly1050::disassembly1050()
      : Cpu6502(CPU_6502) {
  }

  const char *disassembly1050::GetAddressLabel(const unsigned short addr) const {
    for (const auto & [address, label] : labels1050) {
      if (address == addr) {
        return label;
      }
    }
    return nullptr;
  }
}