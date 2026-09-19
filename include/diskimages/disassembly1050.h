//
// disassembly1050 class
// (c) 2018 Eric BACHER
//

#ifndef DISASSEMBLY1050_H
#define DISASSEMBLY1050_H

#include "diskimages/cpu6502.h"

namespace DiskImages {

  class disassembly1050 : public Cpu6502 {
  public:
    disassembly1050();
    ~disassembly1050() override = default;

    // read/write a byte in memory (unused)
    unsigned char ReadByte(unsigned short) override { return 0; }
    void WriteByte(unsigned short, unsigned char) override {}

    // enable/disable traces (unused)
    void Trace(int, bool, const char *, ...) override {}
    [[maybe_unused]] virtual void Dump(char *, int) {}
    bool IsAddressSkipped(unsigned short) override { return false; }

    const char *GetAddressLabel(unsigned short addr) const override;
  };
}
#endif// DISASSEMBLY1050_H
