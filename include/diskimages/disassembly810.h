//
// disassembly810 class
// (c) 2018 Eric BACHER
//

#ifndef DISASSEMBLY810_H
#define DISASSEMBLY810_H

#include "diskimages/cpu6502.h"

namespace DiskImages {

  class disassembly810 : public Cpu6502 {
  public:
    disassembly810();
    ~disassembly810() override = default;

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
#endif// DISASSEMBLY810_H
