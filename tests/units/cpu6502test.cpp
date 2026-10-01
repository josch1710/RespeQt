#include "cpu6502test.h"
#include "include/diskimages/cpu6502.h"
#include <QTest>
#include <array>
#include <cstdarg>

namespace Tests {

using DiskImages::CPU6502_FLAG_B;
using DiskImages::CPU6502_FLAG_C;
using DiskImages::CPU6502_FLAG_D;
using DiskImages::CPU6502_FLAG_I;
using DiskImages::CPU6502_FLAG_N;
using DiskImages::CPU6502_FLAG_V;
using DiskImages::CPU6502_FLAG_Z;

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

namespace {
  // write a byte sequence into memory and position the PC on it
  void Load(SimpleCpu6502 &cpu, const unsigned short addr, std::initializer_list<unsigned char> bytes) {
    unsigned short a = addr;
    for (const auto b: bytes) {
      cpu.SetMemory(a++, b);
    }
    cpu.SetPC(addr);
  }

  // execute one instruction at addr and return its cycle count
  int Run(SimpleCpu6502 &cpu, const unsigned short addr, std::initializer_list<unsigned char> bytes) {
    Load(cpu, addr, bytes);
    return cpu.Step();
  }
}// namespace

void Cpu6502Test::testOpcodeTableConsistency() {
  // every opcode of both tables must have a handler and the instruction length must follow the addressing mode
  SimpleCpu6502 cpu02(DiskImages::CPU_6502);
  SimpleCpu6502 cpuC02(DiskImages::CPU_65C02);
  for (int op = 0; op < 256; op++) {
    const auto code = static_cast<unsigned char>(op);
    const int len02 = cpu02.GetOpCodeLength(code);
    const int lenC02 = cpuC02.GetOpCodeLength(code);
    QVERIFY2(len02 >= 1 && len02 <= 3, qPrintable(QString("6502 opcode %1 length %2").arg(op, 2, 16).arg(len02)));
    QVERIFY2(lenC02 >= 1 && lenC02 <= 3, qPrintable(QString("65C02 opcode %1 length %2").arg(op, 2, 16).arg(lenC02)));

    // the PC must advance exactly by the opcode length for every instruction which does not jump
    // (skip control flow opcodes: BRK, JSR, RTI, JMP, RTS, branches and kil)
    const bool isBranch = (op & 0x1F) == 0x10;
    const bool isCtrl = op == 0x00 || op == 0x20 || op == 0x40 || op == 0x4C || op == 0x60 || op == 0x6C || op == 0x7C || op == 0x80;
    if (isBranch || isCtrl) {
      continue;
    }
    for (SimpleCpu6502 *cpu: {&cpu02, &cpuC02}) {
      cpu->SetSP(0xFF);
      cpu->SetSR(0x24);
      cpu->SetX(0);
      cpu->SetY(0);
      // operand bytes 0x50/0x05 keep every effective address in RAM and away from the code at $7000
      const int cycles = Run(*cpu, 0x7000, {code, 0x50, 0x05});
      const int len = cpu->GetOpCodeLength(code);
      const bool isKil = (op & 0x0F) == 0x02 && (op < 0x80 || (op & 0x10) != 0);
      if (cpu == &cpu02 && isKil) {
        // kil halts the CPU
        QCOMPARE(cycles, 0);
        continue;
      }
      QVERIFY2(cycles >= 1 && cycles <= 8, qPrintable(QString("opcode %1 cycles %2").arg(op, 2, 16).arg(cycles)));
      QCOMPARE(static_cast<int>(cpu->GetPC()), 0x7000 + len);
    }
  }
}

void Cpu6502Test::testAluBinaryAndFlags() {
  SimpleCpu6502 cpu;
  cpu.SetSR(0x24);

  // ADC #$50 with A=$50 -> $A0, V=1 N=1 C=0 Z=0 (2 cycles)
  cpu.SetA(0x50);
  QCOMPARE(Run(cpu, 0x1000, {0x69, 0x50}), 2);
  QCOMPARE(cpu.GetA(), 0xA0);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_V);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_C));
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_Z));

  // ADC #$01 with A=$FF, C=0 -> $00, C=1 Z=1
  cpu.SetA(0xFF);
  cpu.SetSR(0x24);
  Run(cpu, 0x1000, {0x69, 0x01});
  QCOMPARE(cpu.GetA(), 0x00);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);

  // SBC #$01 with A=$00, C=1 -> $FF, C=0 (borrow), N=1
  cpu.SetA(0x00);
  cpu.SetSR(0x24 | CPU6502_FLAG_C);
  Run(cpu, 0x1000, {0xE9, 0x01});
  QCOMPARE(cpu.GetA(), 0xFF);
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_C));
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);

  // SBC #$10 with A=$50, C=1 -> $40, C=1
  cpu.SetA(0x50);
  cpu.SetSR(0x24 | CPU6502_FLAG_C);
  Run(cpu, 0x1000, {0xE9, 0x10});
  QCOMPARE(cpu.GetA(), 0x40);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // AND $20 (zero page, 3 cycles)
  cpu.SetMemory(0x0020, 0x0F);
  cpu.SetA(0xF3);
  QCOMPARE(Run(cpu, 0x1000, {0x25, 0x20}), 3);
  QCOMPARE(cpu.GetA(), 0x03);

  // ORA $1234 (absolute, 4 cycles)
  cpu.SetMemory(0x1234, 0xF0);
  cpu.SetA(0x0F);
  QCOMPARE(Run(cpu, 0x1000, {0x0D, 0x34, 0x12}), 4);
  QCOMPARE(cpu.GetA(), 0xFF);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);

  // EOR #$FF -> complement, Z when result zero
  cpu.SetA(0xFF);
  Run(cpu, 0x1000, {0x49, 0xFF});
  QCOMPARE(cpu.GetA(), 0x00);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);

  // CMP #$10 with A=$20 -> C=1 Z=0 N=0; CMP #$20 -> Z=1; CMP #$30 -> C=0 N=1
  cpu.SetA(0x20);
  Run(cpu, 0x1000, {0xC9, 0x10});
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_Z));
  Run(cpu, 0x1000, {0xC9, 0x20});
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);
  Run(cpu, 0x1000, {0xC9, 0x30});
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_C));
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);

  // CPX / CPY immediate
  cpu.SetX(0x05);
  Run(cpu, 0x1000, {0xE0, 0x05});
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);
  cpu.SetY(0x04);
  Run(cpu, 0x1000, {0xC0, 0x05});
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_C));

  // BIT $20: N and V from memory, Z from A & M
  cpu.SetMemory(0x0020, 0xC0);
  cpu.SetA(0x01);
  Run(cpu, 0x1000, {0x24, 0x20});
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_V);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);

  // (Zpg,X) 6 cycles / (Zpg),Y 5 cycles without page cross, 6 cycles with page cross
  cpu.SetMemory(0x0024, 0x00);
  cpu.SetMemory(0x0025, 0x30);
  cpu.SetX(0x04);
  QCOMPARE(Run(cpu, 0x1000, {0x01, 0x20}), 6);
  cpu.SetMemory(0x0020, 0xF0);
  cpu.SetMemory(0x0021, 0x30);
  cpu.SetY(0x05);
  QCOMPARE(Run(cpu, 0x1000, {0x11, 0x20}), 5);
  cpu.SetY(0x20);
  QCOMPARE(Run(cpu, 0x1000, {0x11, 0x20}), 6);
}

void Cpu6502Test::testAdcSbcDecimal() {
  SimpleCpu6502 cpu;

  // 6502: decimal mode does not cost an extra cycle
  // ADC #$05 with A=$05, D=1 -> $10
  cpu.SetSR(0x24 | CPU6502_FLAG_D);
  cpu.SetA(0x05);
  QCOMPARE(Run(cpu, 0x1000, {0x69, 0x05}), 2);
  QCOMPARE(cpu.GetA(), 0x10);
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_C));

  // ADC #$01 with A=$99, D=1 -> $00 with carry
  cpu.SetSR(0x24 | CPU6502_FLAG_D);
  cpu.SetA(0x99);
  Run(cpu, 0x1000, {0x69, 0x01});
  QCOMPARE(cpu.GetA(), 0x00);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // ADC #$19 with A=$19, C=1, D=1 -> $39
  cpu.SetSR(0x24 | CPU6502_FLAG_D | CPU6502_FLAG_C);
  cpu.SetA(0x19);
  Run(cpu, 0x1000, {0x69, 0x19});
  QCOMPARE(cpu.GetA(), 0x39);

  // SBC #$01 with A=$10, C=1, D=1 -> $09
  cpu.SetSR(0x24 | CPU6502_FLAG_D | CPU6502_FLAG_C);
  cpu.SetA(0x10);
  Run(cpu, 0x1000, {0xE9, 0x01});
  QCOMPARE(cpu.GetA(), 0x09);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // SBC #$01 with A=$00, C=1, D=1 -> $99 with borrow
  cpu.SetSR(0x24 | CPU6502_FLAG_D | CPU6502_FLAG_C);
  cpu.SetA(0x00);
  Run(cpu, 0x1000, {0xE9, 0x01});
  QCOMPARE(cpu.GetA(), 0x99);
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_C));

  // 65C02: ADC (Zpg) takes 5 cycles, 6 in decimal mode
  SimpleCpu6502 cpuC(DiskImages::CPU_65C02);
  cpuC.SetMemory(0x0020, 0x00);
  cpuC.SetMemory(0x0021, 0x30);
  cpuC.SetMemory(0x3000, 0x05);
  cpuC.SetSR(0x24);
  cpuC.SetA(0x05);
  QCOMPARE(Run(cpuC, 0x1000, {0x72, 0x20}), 5);
  QCOMPARE(cpuC.GetA(), 0x0A);
  cpuC.SetSR(0x24 | CPU6502_FLAG_D);
  cpuC.SetA(0x05);
  QCOMPARE(Run(cpuC, 0x1000, {0x72, 0x20}), 6);
  QCOMPARE(cpuC.GetA(), 0x10);
}

void Cpu6502Test::testRmwShiftsAndRotates() {
  SimpleCpu6502 cpu;
  cpu.SetSR(0x24);

  // ASL A: $81 -> $02, C=1 (2 cycles)
  cpu.SetA(0x81);
  QCOMPARE(Run(cpu, 0x1000, {0x0A}), 2);
  QCOMPARE(cpu.GetA(), 0x02);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // LSR $20: $01 -> $00, C=1 Z=1 (5 cycles)
  cpu.SetMemory(0x0020, 0x01);
  QCOMPARE(Run(cpu, 0x1000, {0x46, 0x20}), 5);
  QCOMPARE(cpu.GetMemory(0x0020), 0x00);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);

  // ROL $1234 with C=1: $80 -> $01, C=1 (6 cycles)
  cpu.SetSR(0x24 | CPU6502_FLAG_C);
  cpu.SetMemory(0x1234, 0x80);
  QCOMPARE(Run(cpu, 0x1000, {0x2E, 0x34, 0x12}), 6);
  QCOMPARE(cpu.GetMemory(0x1234), 0x01);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // ROR $20,X with C=0: $01 -> $00, C=1 (6 cycles)
  cpu.SetSR(0x24);
  cpu.SetX(0x05);
  cpu.SetMemory(0x0025, 0x01);
  QCOMPARE(Run(cpu, 0x1000, {0x76, 0x20}), 6);
  QCOMPARE(cpu.GetMemory(0x0025), 0x00);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // ROR A with C=1: $00 -> $80, N=1
  cpu.SetSR(0x24 | CPU6502_FLAG_C);
  cpu.SetA(0x00);
  Run(cpu, 0x1000, {0x6A});
  QCOMPARE(cpu.GetA(), 0x80);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);

  // INC $1234,X: 7 cycles regardless of page crossing on the 6502
  cpu.SetX(0xFF);
  cpu.SetMemory(0x1333, 0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0xFE, 0x34, 0x12}), 7);
  QCOMPARE(cpu.GetMemory(0x1333), 0x00);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);

  // DEC $20: $00 -> $FF, N=1
  cpu.SetMemory(0x0020, 0x00);
  QCOMPARE(Run(cpu, 0x1000, {0xC6, 0x20}), 5);
  QCOMPARE(cpu.GetMemory(0x0020), 0xFF);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);

  // INX / DEY / register transfers
  cpu.SetX(0xFF);
  Run(cpu, 0x1000, {0xE8});
  QCOMPARE(cpu.GetX(), 0x00);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);
  cpu.SetY(0x00);
  Run(cpu, 0x1000, {0x88});
  QCOMPARE(cpu.GetY(), 0xFF);
  cpu.SetA(0x42);
  Run(cpu, 0x1000, {0xAA});
  QCOMPARE(cpu.GetX(), 0x42);
  Run(cpu, 0x1000, {0xA8});
  QCOMPARE(cpu.GetY(), 0x42);
  cpu.SetX(0x80);
  Run(cpu, 0x1000, {0x9A});
  QCOMPARE(cpu.GetSP(), 0x80);
  Run(cpu, 0x1000, {0xBA});
  QCOMPARE(cpu.GetX(), 0x80);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);
}

void Cpu6502Test::testLoadStoreAllModes() {
  SimpleCpu6502 cpu;
  cpu.SetSR(0x24);

  // LDA / LDX / LDY immediate set N and Z
  Run(cpu, 0x1000, {0xA9, 0x00});
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);
  Run(cpu, 0x1000, {0xA2, 0x80});
  QCOMPARE(cpu.GetX(), 0x80);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);
  Run(cpu, 0x1000, {0xA0, 0x7F});
  QCOMPARE(cpu.GetY(), 0x7F);
  QVERIFY(!(cpu.GetSR() & (CPU6502_FLAG_N | CPU6502_FLAG_Z)));

  // LDX $1234,Y with page cross (5 cycles), LDY $1234,X without (4 cycles)
  cpu.SetY(0xFF);
  cpu.SetMemory(0x1333, 0x11);
  QCOMPARE(Run(cpu, 0x1000, {0xBE, 0x34, 0x12}), 5);
  QCOMPARE(cpu.GetX(), 0x11);
  cpu.SetX(0x01);
  cpu.SetMemory(0x1235, 0x22);
  QCOMPARE(Run(cpu, 0x1000, {0xBC, 0x34, 0x12}), 4);
  QCOMPARE(cpu.GetY(), 0x22);

  // STA zero page (3) / absolute (4) / absolute,X (5 fixed) / absolute,Y (5) / (Zpg),Y (6)
  cpu.SetA(0xAB);
  cpu.SetX(0x01);
  cpu.SetY(0x02);
  QCOMPARE(Run(cpu, 0x1000, {0x85, 0x40}), 3);
  QCOMPARE(cpu.GetMemory(0x0040), 0xAB);
  QCOMPARE(Run(cpu, 0x1000, {0x8D, 0x00, 0x30}), 4);
  QCOMPARE(cpu.GetMemory(0x3000), 0xAB);
  QCOMPARE(Run(cpu, 0x1000, {0x9D, 0x00, 0x30}), 5);
  QCOMPARE(cpu.GetMemory(0x3001), 0xAB);
  QCOMPARE(Run(cpu, 0x1000, {0x99, 0x00, 0x30}), 5);
  QCOMPARE(cpu.GetMemory(0x3002), 0xAB);
  cpu.SetMemory(0x0020, 0x10);
  cpu.SetMemory(0x0021, 0x30);
  QCOMPARE(Run(cpu, 0x1000, {0x91, 0x20}), 6);
  QCOMPARE(cpu.GetMemory(0x3012), 0xAB);

  // STX zero page,Y (wrap) / STY zero page,X / STX absolute / STY absolute
  cpu.SetX(0x33);
  cpu.SetY(0x44);
  QCOMPARE(Run(cpu, 0x1000, {0x96, 0xF0}), 4);// ($F0 + $44) & $FF = $34
  QCOMPARE(cpu.GetMemory(0x0034), 0x33);
  QCOMPARE(Run(cpu, 0x1000, {0x94, 0x10}), 4);// $10 + $33 = $43
  QCOMPARE(cpu.GetMemory(0x0043), 0x44);
  QCOMPARE(Run(cpu, 0x1000, {0x8E, 0x00, 0x31}), 4);
  QCOMPARE(cpu.GetMemory(0x3100), 0x33);
  QCOMPARE(Run(cpu, 0x1000, {0x8C, 0x01, 0x31}), 4);
  QCOMPARE(cpu.GetMemory(0x3101), 0x44);

  // stack: PHA / PLA / PHP / PLP
  cpu.SetSP(0xFF);
  cpu.SetA(0x5A);
  QCOMPARE(Run(cpu, 0x1000, {0x48}), 3);
  QCOMPARE(cpu.GetMemory(0x01FF), 0x5A);
  cpu.SetA(0x00);
  QCOMPARE(Run(cpu, 0x1000, {0x68}), 4);
  QCOMPARE(cpu.GetA(), 0x5A);
  QCOMPARE(cpu.GetSP(), 0xFF);
  cpu.SetSR(0x24 | CPU6502_FLAG_C);
  Run(cpu, 0x1000, {0x08});
  QCOMPARE(cpu.GetMemory(0x01FF), static_cast<unsigned char>(0x24 | CPU6502_FLAG_C | CPU6502_FLAG_B));
  cpu.SetSR(0x24);
  Run(cpu, 0x1000, {0x28});
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // flag instructions
  Run(cpu, 0x1000, {0x38});
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);
  Run(cpu, 0x1000, {0x18});
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_C));
  Run(cpu, 0x1000, {0xF8});
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_D);
  Run(cpu, 0x1000, {0xD8});
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_D));
  Run(cpu, 0x1000, {0x78});
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_I);
  Run(cpu, 0x1000, {0x58});
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_I));
}

void Cpu6502Test::testBranchCyclesAndJmpIndirect() {
  SimpleCpu6502 cpu;

  // BEQ not taken: 2 cycles
  cpu.SetSR(0x24);
  QCOMPARE(Run(cpu, 0x1000, {0xF0, 0x10}), 2);
  QCOMPARE(cpu.GetPC(), 0x1002);

  // BEQ taken, same page: 3 cycles
  cpu.SetSR(0x24 | CPU6502_FLAG_Z);
  QCOMPARE(Run(cpu, 0x1000, {0xF0, 0x10}), 3);
  QCOMPARE(cpu.GetPC(), 0x1012);

  // BEQ taken, crossing a page: 4 cycles ($10F0 + 2 + $20 = $1112)
  QCOMPARE(Run(cpu, 0x10F0, {0xF0, 0x20}), 4);
  QCOMPARE(cpu.GetPC(), 0x1112);

  // BCS / BCC / BMI / BPL / BVS / BVC
  cpu.SetSR(0x24 | CPU6502_FLAG_C);
  QCOMPARE(Run(cpu, 0x1000, {0xB0, 0x02}), 3);
  QCOMPARE(Run(cpu, 0x1000, {0x90, 0x02}), 2);
  cpu.SetSR(0x24 | CPU6502_FLAG_N);
  QCOMPARE(Run(cpu, 0x1000, {0x30, 0x02}), 3);
  QCOMPARE(Run(cpu, 0x1000, {0x10, 0x02}), 2);
  cpu.SetSR(0x24 | CPU6502_FLAG_V);
  QCOMPARE(Run(cpu, 0x1000, {0x70, 0x02}), 3);
  QCOMPARE(Run(cpu, 0x1000, {0x50, 0x02}), 2);

  // JMP $3000 (3 cycles)
  QCOMPARE(Run(cpu, 0x1000, {0x4C, 0x00, 0x30}), 3);
  QCOMPARE(cpu.GetPC(), 0x3000);

  // JMP ($20FF): 6502 bug reads the high byte from $2000 instead of $2100
  cpu.SetMemory(0x20FF, 0x34);
  cpu.SetMemory(0x2100, 0x12);
  cpu.SetMemory(0x2000, 0x56);
  QCOMPARE(Run(cpu, 0x1000, {0x6C, 0xFF, 0x20}), 5);
  QCOMPARE(cpu.GetPC(), 0x5634);

  // 65C02 fixes the bug and takes 6 cycles
  SimpleCpu6502 cpuC(DiskImages::CPU_65C02);
  cpuC.SetMemory(0x20FF, 0x34);
  cpuC.SetMemory(0x2100, 0x12);
  cpuC.SetMemory(0x2000, 0x56);
  QCOMPARE(Run(cpuC, 0x1000, {0x6C, 0xFF, 0x20}), 6);
  QCOMPARE(cpuC.GetPC(), 0x1234);

  // BRK pushes PC+2 and SR with B set, loads the IRQ vector and sets I (7 cycles)
  cpu.SetSP(0xFF);
  cpu.SetSR(0x24);
  cpu.SetMemory(0xFFFE, 0x00);
  cpu.SetMemory(0xFFFF, 0x40);
  QCOMPARE(Run(cpu, 0x1000, {0x00, 0xEA}), 7);
  QCOMPARE(cpu.GetPC(), 0x4000);
  QCOMPARE(cpu.GetSP(), 0xFC);
  QCOMPARE(cpu.GetMemory(0x01FF), 0x10);
  QCOMPARE(cpu.GetMemory(0x01FE), 0x02);
  QVERIFY(cpu.GetMemory(0x01FD) & CPU6502_FLAG_B);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_I);

  // RTI restores SR and PC (6 cycles)
  Load(cpu, 0x4000, {0x40});
  QCOMPARE(cpu.Step(), 6);
  QCOMPARE(cpu.GetPC(), 0x1002);
  QCOMPARE(cpu.GetSP(), 0xFF);
}

void Cpu6502Test::testUndocumentedOpcodes() {
  SimpleCpu6502 cpu;
  cpu.SetSR(0x24);

  // slo $20: ASL memory then ORA (5 cycles)
  cpu.SetMemory(0x0020, 0x81);
  cpu.SetA(0x01);
  QCOMPARE(Run(cpu, 0x1000, {0x07, 0x20}), 5);
  QCOMPARE(cpu.GetMemory(0x0020), 0x02);
  QCOMPARE(cpu.GetA(), 0x03);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // rla $1234: ROL memory then AND (6 cycles)
  cpu.SetSR(0x24 | CPU6502_FLAG_C);
  cpu.SetMemory(0x1234, 0x02);
  cpu.SetA(0x0F);
  QCOMPARE(Run(cpu, 0x1000, {0x2F, 0x34, 0x12}), 6);
  QCOMPARE(cpu.GetMemory(0x1234), 0x05);
  QCOMPARE(cpu.GetA(), 0x05);

  // sre $20: LSR memory then EOR (5 cycles)
  cpu.SetMemory(0x0020, 0x03);
  cpu.SetA(0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0x47, 0x20}), 5);
  QCOMPARE(cpu.GetMemory(0x0020), 0x01);
  QCOMPARE(cpu.GetA(), 0xFE);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // rra $20: ROR memory then ADC (5 cycles): $02 -> $01, A = $10 + $01 = $11
  cpu.SetSR(0x24);
  cpu.SetMemory(0x0020, 0x02);
  cpu.SetA(0x10);
  QCOMPARE(Run(cpu, 0x1000, {0x67, 0x20}), 5);
  QCOMPARE(cpu.GetMemory(0x0020), 0x01);
  QCOMPARE(cpu.GetA(), 0x11);

  // dcp (Zpg,X): DEC memory then CMP (8 cycles)
  cpu.SetSR(0x24);
  cpu.SetMemory(0x0024, 0x00);
  cpu.SetMemory(0x0025, 0x30);
  cpu.SetMemory(0x3000, 0x11);
  cpu.SetX(0x04);
  cpu.SetA(0x10);
  QCOMPARE(Run(cpu, 0x1000, {0xC3, 0x20}), 8);
  QCOMPARE(cpu.GetMemory(0x3000), 0x10);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // isc $1234,Y: INC memory then SBC (7 cycles): $0F -> $10, A = $20 - $10 = $10
  cpu.SetSR(0x24 | CPU6502_FLAG_C);
  cpu.SetY(0x01);
  cpu.SetMemory(0x1235, 0x0F);
  cpu.SetA(0x20);
  QCOMPARE(Run(cpu, 0x1000, {0xFB, 0x34, 0x12}), 7);
  QCOMPARE(cpu.GetMemory(0x1235), 0x10);
  QCOMPARE(cpu.GetA(), 0x10);

  // lax $20: load A and X (3 cycles)
  cpu.SetMemory(0x0020, 0x7E);
  QCOMPARE(Run(cpu, 0x1000, {0xA7, 0x20}), 3);
  QCOMPARE(cpu.GetA(), 0x7E);
  QCOMPARE(cpu.GetX(), 0x7E);

  // lax (Zpg),Y with page cross (6 cycles)
  cpu.SetMemory(0x0020, 0xF0);
  cpu.SetMemory(0x0021, 0x30);
  cpu.SetMemory(0x3100, 0x33);
  cpu.SetY(0x10);
  QCOMPARE(Run(cpu, 0x1000, {0xB3, 0x20}), 6);
  QCOMPARE(cpu.GetA(), 0x33);

  // aax (sax) $20: store A & X (3 cycles)
  cpu.SetA(0xF0);
  cpu.SetX(0x3C);
  QCOMPARE(Run(cpu, 0x1000, {0x87, 0x20}), 3);
  QCOMPARE(cpu.GetMemory(0x0020), 0x30);

  // aac (anc) #$80 with A=$FF: AND then C = N
  cpu.SetSR(0x24);
  cpu.SetA(0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0x0B, 0x80}), 2);
  QCOMPARE(cpu.GetA(), 0x80);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);

  // asr (alr) #$03 with A=$07: AND -> $03, LSR -> $01, C=1
  cpu.SetSR(0x24);
  cpu.SetA(0x07);
  QCOMPARE(Run(cpu, 0x1000, {0x4B, 0x03}), 2);
  QCOMPARE(cpu.GetA(), 0x01);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // axs (sbx) #$05: X = (A & X) - 5
  cpu.SetA(0xFF);
  cpu.SetX(0x0F);
  Run(cpu, 0x1000, {0xCB, 0x05});
  QCOMPARE(cpu.GetX(), 0x0A);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_C);

  // dop / top: skip operand bytes, the dummy read must not change any register
  cpu.SetA(0x12);
  QCOMPARE(Run(cpu, 0x1000, {0x04, 0x20}), 3);
  QCOMPARE(cpu.GetPC(), 0x1002);
  QCOMPARE(cpu.GetA(), 0x12);
  QCOMPARE(Run(cpu, 0x1000, {0x80, 0x20}), 2);
  QCOMPARE(cpu.GetPC(), 0x1002);
  QCOMPARE(Run(cpu, 0x1000, {0x0C, 0x34, 0x12}), 4);
  QCOMPARE(cpu.GetPC(), 0x1003);
  cpu.SetX(0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0x1C, 0x34, 0x12}), 5);// top Abs,X with page cross
  QCOMPARE(Run(cpu, 0x1000, {0x1A}), 2);// nop

  // kil halts (0 cycles) and consumes only the opcode byte
  QCOMPARE(Run(cpu, 0x1000, {0x02}), 0);
  QCOMPARE(cpu.GetPC(), 0x1001);
}

void Cpu6502Test::test65C02Instructions() {
  SimpleCpu6502 cpu(DiskImages::CPU_65C02);
  cpu.SetSR(0x24);
  cpu.SetSP(0xFF);

  // BRA: always taken, 3 cycles (4 with page cross)
  QCOMPARE(Run(cpu, 0x1000, {0x80, 0x10}), 3);
  QCOMPARE(cpu.GetPC(), 0x1012);
  QCOMPARE(Run(cpu, 0x10F0, {0x80, 0x20}), 4);
  QCOMPARE(cpu.GetPC(), 0x1112);

  // STZ zero page / zero page,X / absolute / absolute,X
  cpu.SetMemory(0x0020, 0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0x64, 0x20}), 3);
  QCOMPARE(cpu.GetMemory(0x0020), 0x00);
  cpu.SetX(0x01);
  cpu.SetMemory(0x0021, 0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0x74, 0x20}), 4);
  QCOMPARE(cpu.GetMemory(0x0021), 0x00);
  cpu.SetMemory(0x3000, 0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0x9C, 0x00, 0x30}), 4);
  QCOMPARE(cpu.GetMemory(0x3000), 0x00);
  cpu.SetMemory(0x3001, 0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0x9E, 0x00, 0x30}), 5);
  QCOMPARE(cpu.GetMemory(0x3001), 0x00);

  // TSB $20: set bits of A in memory, Z from A & M before (5 cycles)
  cpu.SetMemory(0x0020, 0x0F);
  cpu.SetA(0xF0);
  QCOMPARE(Run(cpu, 0x1000, {0x04, 0x20}), 5);
  QCOMPARE(cpu.GetMemory(0x0020), 0xFF);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);

  // TRB $1234: clear bits of A in memory (6 cycles)
  cpu.SetMemory(0x1234, 0xFF);
  cpu.SetA(0x0F);
  QCOMPARE(Run(cpu, 0x1000, {0x1C, 0x34, 0x12}), 6);
  QCOMPARE(cpu.GetMemory(0x1234), 0xF0);
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_Z));

  // INA / DEA
  cpu.SetA(0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0x1A}), 2);
  QCOMPARE(cpu.GetA(), 0x00);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);
  QCOMPARE(Run(cpu, 0x1000, {0x3A}), 2);
  QCOMPARE(cpu.GetA(), 0xFF);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_N);

  // PHX / PLX / PHY / PLY
  cpu.SetX(0x11);
  cpu.SetY(0x22);
  QCOMPARE(Run(cpu, 0x1000, {0xDA}), 3);
  QCOMPARE(Run(cpu, 0x1000, {0x5A}), 3);
  QCOMPARE(cpu.GetSP(), 0xFD);
  cpu.SetX(0);
  cpu.SetY(0);
  QCOMPARE(Run(cpu, 0x1000, {0x7A}), 4);
  QCOMPARE(cpu.GetY(), 0x22);
  QCOMPARE(Run(cpu, 0x1000, {0xFA}), 4);
  QCOMPARE(cpu.GetX(), 0x11);
  QCOMPARE(cpu.GetSP(), 0xFF);

  // BIT #$0F only affects Z
  cpu.SetSR(0x24);
  cpu.SetA(0xF0);
  QCOMPARE(Run(cpu, 0x1000, {0x89, 0x0F}), 2);
  QVERIFY(cpu.GetSR() & CPU6502_FLAG_Z);
  QVERIFY(!(cpu.GetSR() & CPU6502_FLAG_N));

  // LDA (Zpg) / STA (Zpg) (5 cycles each)
  cpu.SetMemory(0x0020, 0x00);
  cpu.SetMemory(0x0021, 0x30);
  cpu.SetMemory(0x3000, 0x5C);
  QCOMPARE(Run(cpu, 0x1000, {0xB2, 0x20}), 5);
  QCOMPARE(cpu.GetA(), 0x5C);
  cpu.SetA(0xC5);
  QCOMPARE(Run(cpu, 0x1000, {0x92, 0x20}), 5);
  QCOMPARE(cpu.GetMemory(0x3000), 0xC5);

  // JMP ($2000,X) (6 cycles)
  cpu.SetX(0x02);
  cpu.SetMemory(0x2002, 0x00);
  cpu.SetMemory(0x2003, 0x50);
  QCOMPARE(Run(cpu, 0x1000, {0x7C, 0x00, 0x20}), 6);
  QCOMPARE(cpu.GetPC(), 0x5000);

  // ASL $1234,X: 6 cycles without, 7 with page cross
  cpu.SetX(0x01);
  QCOMPARE(Run(cpu, 0x1000, {0x1E, 0x34, 0x12}), 6);
  cpu.SetX(0xFF);
  QCOMPARE(Run(cpu, 0x1000, {0x1E, 0x34, 0x12}), 7);

  // undocumented NMOS opcodes are NOPs on the 65C02: 1-byte NOP takes 1 cycle, $5C takes 8 cycles and 3 bytes
  cpu.SetA(0x77);
  QCOMPARE(Run(cpu, 0x1000, {0x03}), 1);
  QCOMPARE(cpu.GetPC(), 0x1001);
  QCOMPARE(cpu.GetA(), 0x77);
  QCOMPARE(Run(cpu, 0x1000, {0x5C, 0x00, 0x00}), 8);
  QCOMPARE(cpu.GetPC(), 0x1003);
  QCOMPARE(Run(cpu, 0x1000, {0x02, 0x00}), 2);
  QCOMPARE(cpu.GetPC(), 0x1002);
}

void Cpu6502Test::testDisassemblyAllModes() {
  SimpleCpu6502 cpu;
  char buffer[128];

  const auto disasm = [&](std::initializer_list<unsigned char> bytes, const unsigned short address = 0x1000) {
    unsigned char code[3] = {0, 0, 0};
    int n = 0;
    for (const auto b: bytes) {
      code[n++] = b;
    }
    const int len = cpu.BuildInstruction(buffer, code, n, address);
    return QString::fromLatin1(buffer).simplified() + QString(" [%1]").arg(len);
  };

  QVERIFY(disasm({0xA9, 0x42}).contains("LDA #$42"));
  QVERIFY(disasm({0xA9, 0x42}).endsWith("[2]"));
  QVERIFY(disasm({0xA5, 0x20}).contains("LDA $0020"));
  QVERIFY(disasm({0xB5, 0x20}).contains("LDA $0020,x"));
  QVERIFY(disasm({0xB6, 0x20}).contains("LDX $0020,y"));
  QVERIFY(disasm({0xAD, 0x34, 0x12}).contains("LDA $1234"));
  QVERIFY(disasm({0xAD, 0x34, 0x12}).endsWith("[3]"));
  QVERIFY(disasm({0xBD, 0x34, 0x12}).contains("LDA $1234,x"));
  QVERIFY(disasm({0xB9, 0x34, 0x12}).contains("LDA $1234,y"));
  QVERIFY(disasm({0xA1, 0x20}).contains("LDA ($0020,x)"));
  QVERIFY(disasm({0xB1, 0x20}).contains("LDA ($0020),y"));
  QVERIFY(disasm({0x6C, 0x34, 0x12}).contains("JMP ($1234)"));
  QVERIFY(disasm({0x0A}).contains("ASL"));
  QVERIFY(disasm({0x0A}).endsWith("[1]"));
  QVERIFY(disasm({0xEA}).contains("NOP"));
  QVERIFY(disasm({0x07, 0x20}).contains("slo $0020"));
  QVERIFY(disasm({0x10, 0x7E}, 0x1000).contains("BPL $1080"));

  // too few bytes for the instruction
  unsigned char partial[1] = {0xAD};
  QCOMPARE(cpu.BuildInstruction(buffer, partial, 1, 0x1000), -1);

  // 65C02 specific addressing modes and mnemonics
  SimpleCpu6502 cpuC(DiskImages::CPU_65C02);
  const auto disasmC = [&](std::initializer_list<unsigned char> bytes) {
    unsigned char code[3] = {0, 0, 0};
    int n = 0;
    for (const auto b: bytes) {
      code[n++] = b;
    }
    cpuC.BuildInstruction(buffer, code, n, 0x1000);
    return QString::fromLatin1(buffer).simplified();
  };
  QVERIFY(disasmC({0xB2, 0x20}).contains("LDA ($0020)"));
  QVERIFY(disasmC({0x7C, 0x34, 0x12}).contains("JMP ($1234,x)"));
  QVERIFY(disasmC({0x64, 0x20}).contains("STZ $0020"));
  QVERIFY(disasmC({0x80, 0x02}).contains("BRA $1004"));
  QVERIFY(disasmC({0x1A}).contains("INA"));
  QVERIFY(disasmC({0x89, 0x0F}).contains("BIT #$0F"));
  QCOMPARE(cpuC.GetOpCodeLength(0x5C), 3);
  QCOMPARE(cpuC.GetOpCodeLength(0x03), 1);

  // BuildTrace uses the same formatting and returns the operand address
  cpuC.SetMemory(0x1000, 0x8D);
  cpuC.SetMemory(0x1001, 0x34);
  cpuC.SetMemory(0x1002, 0x12);
  cpuC.SetPC(0x1000);
  cpuC.SetTrace(true);
  const unsigned short addr = cpuC.BuildTrace(buffer);
  QCOMPARE(addr, 0x1234);
  QVERIFY(QString::fromLatin1(buffer).contains("STA $1234"));
}

} // namespace Tests
