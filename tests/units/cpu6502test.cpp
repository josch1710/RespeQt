#include "cpu6502test.h"
#include "include/diskimages/cpu6502.h"
#include <QTest>
#include <array>
#include <cstdarg>

namespace Tests {

class SimpleCpu6502 : public DiskImages::Cpu6502 {
public:
  explicit SimpleCpu6502(DiskImages::CPU_ENUM cpuType = DiskImages::CPU_6502)
      : Cpu6502(cpuType) {
    m_memory.fill(0);
  }

  unsigned char ReadByte(unsigned short addr) override {
    return m_memory[addr];
  }

  void WriteByte(unsigned short addr, unsigned char val) override {
    m_memory[addr] = val;
  }

  bool IsAddressSkipped(unsigned short /*addr*/) override {
    return false;
  }

  void Trace(int /*module*/, bool /*debug*/, const char */*msg*/, ...) override {
  }

  void SetMemory(unsigned short addr, unsigned char val) {
    m_memory[addr] = val;
  }

  unsigned char GetMemory(unsigned short addr) const {
    return m_memory[addr];
  }

private:
  std::array<unsigned char, 65536> m_memory{};
};

void Cpu6502Test::testStackOperations() {
  SimpleCpu6502 cpu;
  cpu.SetSP(0xFF);

  // Push byte
  cpu.PushByte(0x42);
  QCOMPARE(cpu.GetSP(), 0xFE);
  QCOMPARE(cpu.GetMemory(0x01FF), 0x42);

  // Pop byte
  unsigned char val = cpu.PopByte();
  QCOMPARE(val, 0x42);
  QCOMPARE(cpu.GetSP(), 0xFF);

  // Push word: high byte first, low byte second
  cpu.PushWord(0x1234);
  QCOMPARE(cpu.GetSP(), 0xFD);
  QCOMPARE(cpu.GetMemory(0x01FF), 0x12); // High byte
  QCOMPARE(cpu.GetMemory(0x01FE), 0x34); // Low byte

  // Pop word
  unsigned short wordVal = cpu.PopWord();
  QCOMPARE(wordVal, 0x1234);
  QCOMPARE(cpu.GetSP(), 0xFF);
}

void Cpu6502Test::testBranchesAndLoops() {
  SimpleCpu6502 cpu;

  // Forward branch: BNE +4 (relative +4 from after branch offset)
  // Code at 0x1000:
  // 1000: A9 01 (LDA #$01) -> Z=0
  // 1002: D0 04 (BNE +$04) -> jumps to 1004 + 4 = 1008
  cpu.SetMemory(0x1000, 0xA9);
  cpu.SetMemory(0x1001, 0x01);
  cpu.SetMemory(0x1002, 0xD0);
  cpu.SetMemory(0x1003, 0x04);
  cpu.SetPC(0x1000);

  cpu.Step(); // LDA
  QCOMPARE(cpu.GetA(), 0x01);
  QCOMPARE(cpu.GetPC(), 0x1002);

  cpu.Step(); // BNE
  QCOMPARE(cpu.GetPC(), 0x1008);

  // Backward branch: loop
  // Code at 0x2000:
  // 2000: A2 03 (LDX #$03)
  // 2002: CA    (DEX)
  // 2003: D0 FD (BNE -3) -> jumps to 2005 - 3 = 2002
  cpu.SetMemory(0x2000, 0xA2);
  cpu.SetMemory(0x2001, 0x03);
  cpu.SetMemory(0x2002, 0xCA);
  cpu.SetMemory(0x2003, 0xD0);
  cpu.SetMemory(0x2004, 0xFD); // -3 in two's complement is 0xFD
  cpu.SetPC(0x2000);

  cpu.Step(); // LDX #3
  QCOMPARE(cpu.GetX(), 3);

  // Iteration 1: X becomes 2, branch taken
  cpu.Step(); // DEX
  QCOMPARE(cpu.GetX(), 2);
  cpu.Step(); // BNE
  QCOMPARE(cpu.GetPC(), 0x2002);

  // Iteration 2: X becomes 1, branch taken
  cpu.Step(); // DEX
  QCOMPARE(cpu.GetX(), 1);
  cpu.Step(); // BNE
  QCOMPARE(cpu.GetPC(), 0x2002);

  // Iteration 3: X becomes 0, branch not taken
  cpu.Step(); // DEX
  QCOMPARE(cpu.GetX(), 0);
  cpu.Step(); // BNE
  QCOMPARE(cpu.GetPC(), 0x2005);
}

void Cpu6502Test::testZeroPageAddressing() {
  SimpleCpu6502 cpu;

  // Zero-page indexed with wraparound (FetchZPageX)
  // Base ZP addr = $80, X = $90 -> Address should wrap to ($80 + $90) & 0xFF = $0010 (not $0110)
  cpu.SetMemory(0x0010, 0x77);
  cpu.SetMemory(0x0110, 0x88);

  // LDA $80,X (opcode $B5)
  cpu.SetMemory(0x1000, 0xB5);
  cpu.SetMemory(0x1001, 0x80);
  cpu.SetPC(0x1000);
  cpu.SetX(0x90);

  cpu.Step();
  QCOMPARE(cpu.GetA(), 0x77);

  // Zero-page indexed with Y wraparound (FetchZPageY)
  // LDX $E0,Y (opcode $B6)
  // Base ZP addr = $E0, Y = $30 -> Address should wrap to ($E0 + $30) & 0xFF = $0010
  cpu.SetMemory(0x1002, 0xB6);
  cpu.SetMemory(0x1003, 0xE0);
  cpu.SetPC(0x1002);
  cpu.SetY(0x30);

  cpu.Step();
  QCOMPARE(cpu.GetX(), 0x77);
}

void Cpu6502Test::testIndirectAddressing() {
  SimpleCpu6502 cpu;

  // Indexed Indirect: (Indirect, X) e.g. LDA ($20, X) (opcode $A1)
  // X = 4 -> pointer in ZP is at $24 ($0024).
  // At $0024-$0025: $00, $30 -> pointer to $3000.
  // At $3000: value $55.
  cpu.SetMemory(0x0024, 0x00);
  cpu.SetMemory(0x0025, 0x30);
  cpu.SetMemory(0x3000, 0x55);

  cpu.SetMemory(0x1000, 0xA1);
  cpu.SetMemory(0x1001, 0x20);
  cpu.SetPC(0x1000);
  cpu.SetX(0x04);

  cpu.Step();
  QCOMPARE(cpu.GetA(), 0x55);

  // Indirect Indexed: (Indirect), Y e.g. LDA ($20), Y (opcode $B1)
  // Pointer at $0020-$0021: $00, $40 -> base $4000.
  // Y = 5 -> target $4005.
  // At $4005: value $99.
  cpu.SetMemory(0x0020, 0x00);
  cpu.SetMemory(0x0021, 0x40);
  cpu.SetMemory(0x4005, 0x99);

  cpu.SetMemory(0x1002, 0xB1);
  cpu.SetMemory(0x1003, 0x20);
  cpu.SetPC(0x1002);
  cpu.SetY(0x05);

  cpu.Step();
  QCOMPARE(cpu.GetA(), 0x99);

  // Store Indexed Indirect: STA ($20, X) (opcode $81)
  // X = 4 -> pointer at $0024 ($3000). A = $AA.
  cpu.SetA(0xAA);
  cpu.SetMemory(0x1004, 0x81);
  cpu.SetMemory(0x1005, 0x20);
  cpu.SetPC(0x1004);
  cpu.SetX(0x04);

  cpu.Step();
  QCOMPARE(cpu.GetMemory(0x3000), 0xAA);
}

void Cpu6502Test::testJsrRts() {
  SimpleCpu6502 cpu;
  cpu.SetSP(0xFF);

  // Main code at 0x1000:
  // 1000: 20 00 20 (JSR $2000)
  // 1003: A9 42    (LDA #$42)
  cpu.SetMemory(0x1000, 0x20);
  cpu.SetMemory(0x1001, 0x00);
  cpu.SetMemory(0x1002, 0x20);
  cpu.SetMemory(0x1003, 0xA9);
  cpu.SetMemory(0x1004, 0x42);

  // Subroutine at 0x2000:
  // 2000: E8       (INX)
  // 2001: 60       (RTS)
  cpu.SetMemory(0x2000, 0xE8);
  cpu.SetMemory(0x2001, 0x60);

  cpu.SetPC(0x1000);
  cpu.SetX(0x05);

  cpu.Step(); // JSR $2000
  QCOMPARE(cpu.GetPC(), 0x2000);
  QCOMPARE(cpu.GetSP(), 0xFD); // Pushed 2 bytes

  cpu.Step(); // INX
  QCOMPARE(cpu.GetX(), 0x06);
  QCOMPARE(cpu.GetPC(), 0x2001);

  cpu.Step(); // RTS
  QCOMPARE(cpu.GetPC(), 0x1003); // Returned exactly to next instruction
  QCOMPARE(cpu.GetSP(), 0xFF);   // Stack pointer restored

  cpu.Step(); // LDA #$42
  QCOMPARE(cpu.GetA(), 0x42);
}

void Cpu6502Test::testCrossPageCalculations() {
  SimpleCpu6502 cpu;

  // LDA $0280, X with X=$10 -> target $0290 (same page) -> 4 cycles
  cpu.SetMemory(0x1000, 0xBD);
  cpu.SetMemory(0x1001, 0x80);
  cpu.SetMemory(0x1002, 0x02);
  cpu.SetPC(0x1000);
  cpu.SetX(0x10);
  int cycles = cpu.Step();
  QCOMPARE(cycles, 4);

  // LDA $0280, X with X=$90 -> target $0310 (different page: $0200 -> $0300) -> 5 cycles
  cpu.SetMemory(0x1003, 0xBD);
  cpu.SetMemory(0x1004, 0x80);
  cpu.SetMemory(0x1005, 0x02);
  cpu.SetPC(0x1003);
  cpu.SetX(0x90);
  cycles = cpu.Step();
  QCOMPARE(cycles, 5);
}

void Cpu6502Test::testDisassemblyRelativeJump() {
  SimpleCpu6502 cpu;
  char buffer[128];

  // BNE -2 (0xFD) at address $1000:
  // Instruction bytes: D0 FD
  // Destination address: 0x1000 + 2 + (-3) = 0x0FFF.
  // Wait, offset 0xFE (-2) at 0x1000 -> 0x1000 + 2 + (-2) = 0x1000
  unsigned char code[2] = {0xD0, 0xFE};
  cpu.BuildInstruction(buffer, code, 2, 0x1000);

  // Buffer should contain "BNE $1000"
  QVERIFY(strstr(buffer, "$1000") != nullptr);
}

} // namespace Tests
