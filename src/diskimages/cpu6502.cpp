//
// Cpu6502 class
// (c) 1996 Eric BACHER
//
// This class simulates a 6502 CPU.
// All undocumented op-codes are implemented
//

#include "include/diskimages/cpu6502.h"
#include <cstdio>
#include <cstring>

// mask for Xas, Sya and other instructions
constexpr unsigned char UNDOC_MASK = 0xDB;// not sure

namespace DiskImages {
  /**
   * @brief Unified opcode table of the MOS 6502 (including all undocumented opcodes).
   *
   * Every entry is created by one of the constexpr Op* builders declared in Cpu6502 (OpAlu, OpStore, OpRmw,
   * OpBranch, OpImplied, OpNop*, OpCtrl, ...). The builder instantiates the execution template with the very
   * same addressing mode and cycle count that are stored as disassembly metadata, so both can never drift apart.
   */
  const std::array<OpcodeDesc, 256> Cpu6502::s_opTable6502 = {{
    /* 00 */ OpCtrl<&Cpu6502::ExecBrk>("BRK", AddrMode::Implied),
    /* 01 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Ora, 6>("ORA"),
    /* 02 */ OpKil(),
    /* 03 */ OpRmwAlu<AddrMode::IndirectX, &Cpu6502::Slo, 8>("slo"),
    /* 04 */ OpNopRead<AddrMode::ZeroPage, 3>("dop"),
    /* 05 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Ora, 3>("ORA"),
    /* 06 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Asl, 5>("ASL"),
    /* 07 */ OpRmwAlu<AddrMode::ZeroPage, &Cpu6502::Slo, 5>("slo"),
    /* 08 */ OpImplied<&Cpu6502::Php, 3>("PHP"),
    /* 09 */ OpAlu<AddrMode::Immediate, &Cpu6502::Ora, 2>("ORA"),
    /* 0A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Asl, 2>("ASL"),
    /* 0B */ OpAlu<AddrMode::Immediate, &Cpu6502::Aac, 2>("aac", true),
    /* 0C */ OpNopRead<AddrMode::Absolute, 4>("top"),
    /* 0D */ OpAlu<AddrMode::Absolute, &Cpu6502::Ora, 4>("ORA"),
    /* 0E */ OpRmw<AddrMode::Absolute, &Cpu6502::Asl, 6>("ASL"),
    /* 0F */ OpRmwAlu<AddrMode::Absolute, &Cpu6502::Slo, 6>("slo"),

    /* 10 */ OpBranch<CPU6502_FLAG_N, false>("BPL"),
    /* 11 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Ora, 5, true>("ORA"),
    /* 12 */ OpKil(),
    /* 13 */ OpRmwAlu<AddrMode::IndirectY, &Cpu6502::Slo, 8>("slo"),
    /* 14 */ OpNopRead<AddrMode::ZeroPageX, 4>("dop"),
    /* 15 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Ora, 4>("ORA"),
    /* 16 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Asl, 6>("ASL"),
    /* 17 */ OpRmwAlu<AddrMode::ZeroPageX, &Cpu6502::Slo, 6>("slo"),
    /* 18 */ OpImplied<&Cpu6502::Clc, 2>("CLC"),
    /* 19 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Ora, 4, true>("ORA"),
    /* 1A */ OpNop<2>("nop", true),
    /* 1B */ OpRmwAlu<AddrMode::AbsoluteY, &Cpu6502::Slo, 7>("slo"),
    /* 1C */ OpNopRead<AddrMode::AbsoluteX, 4, true>("top"),
    /* 1D */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Ora, 4, true>("ORA"),
    /* 1E */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Asl, 7>("ASL"),
    /* 1F */ OpRmwAlu<AddrMode::AbsoluteX, &Cpu6502::Slo, 7>("slo"),

    /* 20 */ OpCtrl<&Cpu6502::ExecJsr>("JSR", AddrMode::Absolute),
    /* 21 */ OpAlu<AddrMode::IndirectX, &Cpu6502::And, 6>("AND"),
    /* 22 */ OpKil(),
    /* 23 */ OpRmwAlu<AddrMode::IndirectX, &Cpu6502::Rla, 8>("rla"),
    /* 24 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Bit, 3>("BIT"),
    /* 25 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::And, 3>("AND"),
    /* 26 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Rol, 5>("ROL"),
    /* 27 */ OpRmwAlu<AddrMode::ZeroPage, &Cpu6502::Rla, 5>("rla"),
    /* 28 */ OpImplied<&Cpu6502::Plp, 4>("PLP"),
    /* 29 */ OpAlu<AddrMode::Immediate, &Cpu6502::And, 2>("AND"),
    /* 2A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Rol, 2>("ROL"),
    /* 2B */ OpAlu<AddrMode::Immediate, &Cpu6502::Aac, 2>("aac", true),
    /* 2C */ OpAlu<AddrMode::Absolute, &Cpu6502::Bit, 4>("BIT"),
    /* 2D */ OpAlu<AddrMode::Absolute, &Cpu6502::And, 4>("AND"),
    /* 2E */ OpRmw<AddrMode::Absolute, &Cpu6502::Rol, 6>("ROL"),
    /* 2F */ OpRmwAlu<AddrMode::Absolute, &Cpu6502::Rla, 6>("rla"),

    /* 30 */ OpBranch<CPU6502_FLAG_N, true>("BMI"),
    /* 31 */ OpAlu<AddrMode::IndirectY, &Cpu6502::And, 5, true>("AND"),
    /* 32 */ OpKil(),
    /* 33 */ OpRmwAlu<AddrMode::IndirectY, &Cpu6502::Rla, 8>("rla"),
    /* 34 */ OpNopRead<AddrMode::ZeroPageX, 4>("dop"),
    /* 35 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::And, 4>("AND"),
    /* 36 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Rol, 6>("ROL"),
    /* 37 */ OpRmwAlu<AddrMode::ZeroPageX, &Cpu6502::Rla, 6>("rla"),
    /* 38 */ OpImplied<&Cpu6502::Sec, 2>("SEC"),
    /* 39 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::And, 4, true>("AND"),
    /* 3A */ OpNop<2>("nop", true),
    /* 3B */ OpRmwAlu<AddrMode::AbsoluteY, &Cpu6502::Rla, 7>("rla"),
    /* 3C */ OpNopRead<AddrMode::AbsoluteX, 4, true>("top"),
    /* 3D */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::And, 4, true>("AND"),
    /* 3E */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Rol, 7>("ROL"),
    /* 3F */ OpRmwAlu<AddrMode::AbsoluteX, &Cpu6502::Rla, 7>("rla"),

    /* 40 */ OpCtrl<&Cpu6502::ExecRti>("RTI", AddrMode::Implied),
    /* 41 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Eor, 6>("EOR"),
    /* 42 */ OpKil(),
    /* 43 */ OpRmwAlu<AddrMode::IndirectX, &Cpu6502::Sre, 8>("sre"),
    /* 44 */ OpNopRead<AddrMode::ZeroPage, 3>("dop"),
    /* 45 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Eor, 3>("EOR"),
    /* 46 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Lsr, 5>("LSR"),
    /* 47 */ OpRmwAlu<AddrMode::ZeroPage, &Cpu6502::Sre, 5>("sre"),
    /* 48 */ OpImplied<&Cpu6502::Pha, 3>("PHA"),
    /* 49 */ OpAlu<AddrMode::Immediate, &Cpu6502::Eor, 2>("EOR"),
    /* 4A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Lsr, 2>("LSR"),
    /* 4B */ OpAlu<AddrMode::Immediate, &Cpu6502::Asr, 2>("asr", true),
    /* 4C */ OpCtrl<&Cpu6502::ExecJmpAbs>("JMP", AddrMode::Absolute),
    /* 4D */ OpAlu<AddrMode::Absolute, &Cpu6502::Eor, 4>("EOR"),
    /* 4E */ OpRmw<AddrMode::Absolute, &Cpu6502::Lsr, 6>("LSR"),
    /* 4F */ OpRmwAlu<AddrMode::Absolute, &Cpu6502::Sre, 6>("sre"),

    /* 50 */ OpBranch<CPU6502_FLAG_V, false>("BVC"),
    /* 51 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Eor, 5, true>("EOR"),
    /* 52 */ OpKil(),
    /* 53 */ OpRmwAlu<AddrMode::IndirectY, &Cpu6502::Sre, 8>("sre"),
    /* 54 */ OpNopRead<AddrMode::ZeroPageX, 4>("dop"),
    /* 55 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Eor, 4>("EOR"),
    /* 56 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Lsr, 6>("LSR"),
    /* 57 */ OpRmwAlu<AddrMode::ZeroPageX, &Cpu6502::Sre, 6>("sre"),
    /* 58 */ OpImplied<&Cpu6502::Cli, 2>("CLI"),
    /* 59 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Eor, 4, true>("EOR"),
    /* 5A */ OpNop<2>("nop", true),
    /* 5B */ OpRmwAlu<AddrMode::AbsoluteY, &Cpu6502::Sre, 7>("sre"),
    /* 5C */ OpNopRead<AddrMode::AbsoluteX, 4, true>("top"),
    /* 5D */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Eor, 4, true>("EOR"),
    /* 5E */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Lsr, 7>("LSR"),
    /* 5F */ OpRmwAlu<AddrMode::AbsoluteX, &Cpu6502::Sre, 7>("sre"),

    /* 60 */ OpCtrl<&Cpu6502::ExecRts>("RTS", AddrMode::Implied),
    /* 61 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Adc, 6>("ADC"),
    /* 62 */ OpKil(),
    /* 63 */ OpRmwAlu<AddrMode::IndirectX, &Cpu6502::Rra, 8>("rra"),
    /* 64 */ OpNopRead<AddrMode::ZeroPage, 3>("dop"),
    /* 65 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Adc, 3>("ADC"),
    /* 66 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Ror, 5>("ROR"),
    /* 67 */ OpRmwAlu<AddrMode::ZeroPage, &Cpu6502::Rra, 5>("rra"),
    /* 68 */ OpImplied<&Cpu6502::Pla, 4>("PLA"),
    /* 69 */ OpAlu<AddrMode::Immediate, &Cpu6502::Adc, 2>("ADC"),
    /* 6A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Ror, 2>("ROR"),
    /* 6B */ OpAlu<AddrMode::Immediate, &Cpu6502::Arr, 2>("arr", true),
    /* 6C */ OpCtrl<&Cpu6502::ExecJmpIndBug>("JMP", AddrMode::AbsoluteIndirect),
    /* 6D */ OpAlu<AddrMode::Absolute, &Cpu6502::Adc, 4>("ADC"),
    /* 6E */ OpRmw<AddrMode::Absolute, &Cpu6502::Ror, 6>("ROR"),
    /* 6F */ OpRmwAlu<AddrMode::Absolute, &Cpu6502::Rra, 6>("rra"),

    /* 70 */ OpBranch<CPU6502_FLAG_V, true>("BVS"),
    /* 71 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Adc, 5, true>("ADC"),
    /* 72 */ OpKil(),
    /* 73 */ OpRmwAlu<AddrMode::IndirectY, &Cpu6502::Rra, 8>("rra"),
    /* 74 */ OpNopRead<AddrMode::ZeroPageX, 4>("dop"),
    /* 75 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Adc, 4>("ADC"),
    /* 76 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Ror, 6>("ROR"),
    /* 77 */ OpRmwAlu<AddrMode::ZeroPageX, &Cpu6502::Rra, 6>("rra"),
    /* 78 */ OpImplied<&Cpu6502::Sei, 2>("SEI"),
    /* 79 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Adc, 4, true>("ADC"),
    /* 7A */ OpNop<2>("nop", true),
    /* 7B */ OpRmwAlu<AddrMode::AbsoluteY, &Cpu6502::Rra, 7>("rra"),
    /* 7C */ OpNopRead<AddrMode::AbsoluteX, 4, true>("top"),
    /* 7D */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Adc, 4, true>("ADC"),
    /* 7E */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Ror, 7>("ROR"),
    /* 7F */ OpRmwAlu<AddrMode::AbsoluteX, &Cpu6502::Rra, 7>("rra"),

    /* 80 */ OpNopSkip<AddrMode::Immediate, 2>("dop"),
    /* 81 */ OpStore<AddrMode::IndirectX, &Cpu6502::m_A, 6>("STA"),
    /* 82 */ OpNopSkip<AddrMode::Immediate, 2>("dop"),
    /* 83 */ OpStoreSpecial<AddrMode::IndirectX, &Cpu6502::Aax, 6>("aax"),
    /* 84 */ OpStore<AddrMode::ZeroPage, &Cpu6502::m_Y, 3>("STY"),
    /* 85 */ OpStore<AddrMode::ZeroPage, &Cpu6502::m_A, 3>("STA"),
    /* 86 */ OpStore<AddrMode::ZeroPage, &Cpu6502::m_X, 3>("STX"),
    /* 87 */ OpStoreSpecial<AddrMode::ZeroPage, &Cpu6502::Aax, 3>("aax"),
    /* 88 */ OpImplied<&Cpu6502::Dey, 2>("DEY"),
    /* 89 */ OpNopSkip<AddrMode::Immediate, 2>("dop"),
    /* 8A */ OpImplied<&Cpu6502::Txa, 2>("TXA"),
    /* 8B */ OpAlu<AddrMode::Immediate, &Cpu6502::Ane, 2>("xaa", true),
    /* 8C */ OpStore<AddrMode::Absolute, &Cpu6502::m_Y, 4>("STY"),
    /* 8D */ OpStore<AddrMode::Absolute, &Cpu6502::m_A, 4>("STA"),
    /* 8E */ OpStore<AddrMode::Absolute, &Cpu6502::m_X, 4>("STX"),
    /* 8F */ OpStoreSpecial<AddrMode::Absolute, &Cpu6502::Aax, 4>("aax"),

    /* 90 */ OpBranch<CPU6502_FLAG_C, false>("BCC"),
    /* 91 */ OpStore<AddrMode::IndirectY, &Cpu6502::m_A, 6>("STA"),
    /* 92 */ OpKil(),
    /* 93 */ OpStoreSpecial<AddrMode::IndirectY, &Cpu6502::Axa, 6>("axa"),
    /* 94 */ OpStore<AddrMode::ZeroPageX, &Cpu6502::m_Y, 4>("STY"),
    /* 95 */ OpStore<AddrMode::ZeroPageX, &Cpu6502::m_A, 4>("STA"),
    /* 96 */ OpStore<AddrMode::ZeroPageY, &Cpu6502::m_X, 4>("STX"),
    /* 97 */ OpStoreSpecial<AddrMode::ZeroPageY, &Cpu6502::Aax, 4>("aax"),
    /* 98 */ OpImplied<&Cpu6502::Tya, 2>("TYA"),
    /* 99 */ OpStore<AddrMode::AbsoluteY, &Cpu6502::m_A, 5>("STA"),
    /* 9A */ OpImplied<&Cpu6502::Txs, 2>("TXS"),
    /* 9B */ OpStoreSpecial<AddrMode::AbsoluteY, &Cpu6502::Xas, 5>("xas"),
    /* 9C */ OpStoreSpecial<AddrMode::AbsoluteX, &Cpu6502::Sya, 5>("sya"),
    /* 9D */ OpStore<AddrMode::AbsoluteX, &Cpu6502::m_A, 5>("STA"),
    /* 9E */ OpStoreSpecial<AddrMode::AbsoluteY, &Cpu6502::Sxa, 5>("sxa"),
    /* 9F */ OpStoreSpecial<AddrMode::AbsoluteY, &Cpu6502::Axa, 5>("axa"),

    /* A0 */ OpAlu<AddrMode::Immediate, &Cpu6502::Ldy, 2>("LDY"),
    /* A1 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Lda, 6>("LDA"),
    /* A2 */ OpAlu<AddrMode::Immediate, &Cpu6502::Ldx, 2>("LDX"),
    /* A3 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Lax, 6>("lax", true),
    /* A4 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Ldy, 3>("LDY"),
    /* A5 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Lda, 3>("LDA"),
    /* A6 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Ldx, 3>("LDX"),
    /* A7 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Lax, 3>("lax", true),
    /* A8 */ OpImplied<&Cpu6502::Tay, 2>("TAY"),
    /* A9 */ OpAlu<AddrMode::Immediate, &Cpu6502::Lda, 2>("LDA"),
    /* AA */ OpImplied<&Cpu6502::Tax, 2>("TAX"),
    /* AB */ OpAlu<AddrMode::Immediate, &Cpu6502::Atx, 2>("atx", true),
    /* AC */ OpAlu<AddrMode::Absolute, &Cpu6502::Ldy, 4>("LDY"),
    /* AD */ OpAlu<AddrMode::Absolute, &Cpu6502::Lda, 4>("LDA"),
    /* AE */ OpAlu<AddrMode::Absolute, &Cpu6502::Ldx, 4>("LDX"),
    /* AF */ OpAlu<AddrMode::Absolute, &Cpu6502::Lax, 4>("lax", true),

    /* B0 */ OpBranch<CPU6502_FLAG_C, true>("BCS"),
    /* B1 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Lda, 5, true>("LDA"),
    /* B2 */ OpKil(),
    /* B3 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Lax, 5, true>("lax", true),
    /* B4 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Ldy, 4>("LDY"),
    /* B5 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Lda, 4>("LDA"),
    /* B6 */ OpAlu<AddrMode::ZeroPageY, &Cpu6502::Ldx, 4>("LDX"),
    /* B7 */ OpAlu<AddrMode::ZeroPageY, &Cpu6502::Lax, 4>("lax", true),
    /* B8 */ OpImplied<&Cpu6502::Clv, 2>("CLV"),
    /* B9 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Lda, 4, true>("LDA"),
    /* BA */ OpImplied<&Cpu6502::Tsx, 2>("TSX"),
    /* BB */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Lar, 4, true>("lar", true),
    /* BC */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Ldy, 4, true>("LDY"),
    /* BD */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Lda, 4, true>("LDA"),
    /* BE */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Ldx, 4, true>("LDX"),
    /* BF */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Lax, 4, true>("lax", true),

    /* C0 */ OpAlu<AddrMode::Immediate, &Cpu6502::Cpy, 2>("CPY"),
    /* C1 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Cmp, 6>("CMP"),
    /* C2 */ OpNopSkip<AddrMode::Immediate, 2>("dop"),
    /* C3 */ OpRmwAlu<AddrMode::IndirectX, &Cpu6502::Dcp, 8>("dcp"),
    /* C4 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Cpy, 3>("CPY"),
    /* C5 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Cmp, 3>("CMP"),
    /* C6 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Dec, 5>("DEC"),
    /* C7 */ OpRmwAlu<AddrMode::ZeroPage, &Cpu6502::Dcp, 5>("dcp"),
    /* C8 */ OpImplied<&Cpu6502::Iny, 2>("INY"),
    /* C9 */ OpAlu<AddrMode::Immediate, &Cpu6502::Cmp, 2>("CMP"),
    /* CA */ OpImplied<&Cpu6502::Dex, 2>("DEX"),
    /* CB */ OpAlu<AddrMode::Immediate, &Cpu6502::Axs, 2>("axs", true),
    /* CC */ OpAlu<AddrMode::Absolute, &Cpu6502::Cpy, 4>("CPY"),
    /* CD */ OpAlu<AddrMode::Absolute, &Cpu6502::Cmp, 4>("CMP"),
    /* CE */ OpRmw<AddrMode::Absolute, &Cpu6502::Dec, 6>("DEC"),
    /* CF */ OpRmwAlu<AddrMode::Absolute, &Cpu6502::Dcp, 6>("dcp"),

    /* D0 */ OpBranch<CPU6502_FLAG_Z, false>("BNE"),
    /* D1 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Cmp, 5, true>("CMP"),
    /* D2 */ OpKil(),
    /* D3 */ OpRmwAlu<AddrMode::IndirectY, &Cpu6502::Dcp, 8>("dcp"),
    /* D4 */ OpNopRead<AddrMode::ZeroPageX, 4>("dop"),
    /* D5 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Cmp, 4>("CMP"),
    /* D6 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Dec, 6>("DEC"),
    /* D7 */ OpRmwAlu<AddrMode::ZeroPageX, &Cpu6502::Dcp, 6>("dcp"),
    /* D8 */ OpImplied<&Cpu6502::Cld, 2>("CLD"),
    /* D9 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Cmp, 4, true>("CMP"),
    /* DA */ OpNop<2>("nop", true),
    /* DB */ OpRmwAlu<AddrMode::AbsoluteY, &Cpu6502::Dcp, 7>("dcp"),
    /* DC */ OpNopRead<AddrMode::AbsoluteX, 4, true>("top"),
    /* DD */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Cmp, 4, true>("CMP"),
    /* DE */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Dec, 7>("DEC"),
    /* DF */ OpRmwAlu<AddrMode::AbsoluteX, &Cpu6502::Dcp, 7>("dcp"),

    /* E0 */ OpAlu<AddrMode::Immediate, &Cpu6502::Cpx, 2>("CPX"),
    /* E1 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Sbc, 6>("SBC"),
    /* E2 */ OpNopSkip<AddrMode::Immediate, 2>("dop"),
    /* E3 */ OpRmwAlu<AddrMode::IndirectX, &Cpu6502::Isc, 8>("isc"),
    /* E4 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Cpx, 3>("CPX"),
    /* E5 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Sbc, 3>("SBC"),
    /* E6 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Inc, 5>("INC"),
    /* E7 */ OpRmwAlu<AddrMode::ZeroPage, &Cpu6502::Isc, 5>("isc"),
    /* E8 */ OpImplied<&Cpu6502::Inx, 2>("INX"),
    /* E9 */ OpAlu<AddrMode::Immediate, &Cpu6502::Sbc, 2>("SBC"),
    /* EA */ OpNop<2>("NOP"),
    /* EB */ OpAlu<AddrMode::Immediate, &Cpu6502::Sbc, 2>("sbc", true),
    /* EC */ OpAlu<AddrMode::Absolute, &Cpu6502::Cpx, 4>("CPX"),
    /* ED */ OpAlu<AddrMode::Absolute, &Cpu6502::Sbc, 4>("SBC"),
    /* EE */ OpRmw<AddrMode::Absolute, &Cpu6502::Inc, 6>("INC"),
    /* EF */ OpRmwAlu<AddrMode::Absolute, &Cpu6502::Isc, 6>("isc"),

    /* F0 */ OpBranch<CPU6502_FLAG_Z, true>("BEQ"),
    /* F1 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Sbc, 5, true>("SBC"),
    /* F2 */ OpKil(),
    /* F3 */ OpRmwAlu<AddrMode::IndirectY, &Cpu6502::Isc, 8>("isc"),
    /* F4 */ OpNopRead<AddrMode::ZeroPageX, 4>("dop"),
    /* F5 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Sbc, 4>("SBC"),
    /* F6 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Inc, 6>("INC"),
    /* F7 */ OpRmwAlu<AddrMode::ZeroPageX, &Cpu6502::Isc, 6>("isc"),
    /* F8 */ OpImplied<&Cpu6502::Sed, 2>("SED"),
    /* F9 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Sbc, 4, true>("SBC"),
    /* FA */ OpNop<2>("nop", true),
    /* FB */ OpRmwAlu<AddrMode::AbsoluteY, &Cpu6502::Isc, 7>("isc"),
    /* FC */ OpNopRead<AddrMode::AbsoluteX, 4, true>("top"),
    /* FD */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Sbc, 4, true>("SBC"),
    /* FE */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Inc, 7>("INC"),
    /* FF */ OpRmwAlu<AddrMode::AbsoluteX, &Cpu6502::Isc, 7>("isc")
  }};

  /**
   * @brief Unified opcode table of the WDC 65C02.
   *
   * Undocumented NMOS opcodes are NOPs on the 65C02 (1 cycle for 1-byte NOPs). The 65C02 extensions
   * (BRA, STZ, TRB, TSB, INA, DEA, PHX, PHY, PLX, PLY, BIT #Imm/Zpg,X/Abs,X, (Zpg) addressing, JMP (Abs,X))
   * are flagged as "illegal" because they do not exist on the plain 6502.
   */
  const std::array<OpcodeDesc, 256> Cpu6502::s_opTable65C02 = {{
    /* 00 */ OpCtrl<&Cpu6502::ExecBrk>("BRK", AddrMode::Implied),
    /* 01 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Ora, 6>("ORA"),
    /* 02 */ OpNopSkip<AddrMode::Immediate, 2>("NOP"),
    /* 03 */ OpNop1(),
    /* 04 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Tsb, 5>("TSB", true),
    /* 05 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Ora, 3>("ORA"),
    /* 06 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Asl, 5>("ASL"),
    /* 07 */ OpNop1(),
    /* 08 */ OpImplied<&Cpu6502::Php, 3>("PHP"),
    /* 09 */ OpAlu<AddrMode::Immediate, &Cpu6502::Ora, 2>("ORA"),
    /* 0A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Asl, 2>("ASL"),
    /* 0B */ OpNop1(),
    /* 0C */ OpRmw<AddrMode::Absolute, &Cpu6502::Tsb, 6>("TSB", true),
    /* 0D */ OpAlu<AddrMode::Absolute, &Cpu6502::Ora, 4>("ORA"),
    /* 0E */ OpRmw<AddrMode::Absolute, &Cpu6502::Asl, 6>("ASL"),
    /* 0F */ OpNop1(),

    /* 10 */ OpBranch<CPU6502_FLAG_N, false>("BPL"),
    /* 11 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Ora, 5, true>("ORA"),
    /* 12 */ OpAlu<AddrMode::ZeroPageIndirect, &Cpu6502::Ora, 5>("ORA", true),
    /* 13 */ OpNop1(),
    /* 14 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Trb, 5>("TRB", true),
    /* 15 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Ora, 4>("ORA"),
    /* 16 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Asl, 6>("ASL"),
    /* 17 */ OpNop1(),
    /* 18 */ OpImplied<&Cpu6502::Clc, 2>("CLC"),
    /* 19 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Ora, 4, true>("ORA"),
    /* 1A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Inc, 2>("INA", true),
    /* 1B */ OpNop1(),
    /* 1C */ OpRmw<AddrMode::Absolute, &Cpu6502::Trb, 6>("TRB", true),
    /* 1D */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Ora, 4, true>("ORA"),
    /* 1E */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Asl, 6, true>("ASL"),
    /* 1F */ OpNop1(),

    /* 20 */ OpCtrl<&Cpu6502::ExecJsr>("JSR", AddrMode::Absolute),
    /* 21 */ OpAlu<AddrMode::IndirectX, &Cpu6502::And, 6>("AND"),
    /* 22 */ OpNopSkip<AddrMode::Immediate, 2>("NOP"),
    /* 23 */ OpNop1(),
    /* 24 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Bit, 3>("BIT"),
    /* 25 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::And, 3>("AND"),
    /* 26 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Rol, 5>("ROL"),
    /* 27 */ OpNop1(),
    /* 28 */ OpImplied<&Cpu6502::Plp, 4>("PLP"),
    /* 29 */ OpAlu<AddrMode::Immediate, &Cpu6502::And, 2>("AND"),
    /* 2A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Rol, 2>("ROL"),
    /* 2B */ OpNop1(),
    /* 2C */ OpAlu<AddrMode::Absolute, &Cpu6502::Bit, 4>("BIT"),
    /* 2D */ OpAlu<AddrMode::Absolute, &Cpu6502::And, 4>("AND"),
    /* 2E */ OpRmw<AddrMode::Absolute, &Cpu6502::Rol, 6>("ROL"),
    /* 2F */ OpNop1(),

    /* 30 */ OpBranch<CPU6502_FLAG_N, true>("BMI"),
    /* 31 */ OpAlu<AddrMode::IndirectY, &Cpu6502::And, 5, true>("AND"),
    /* 32 */ OpAlu<AddrMode::ZeroPageIndirect, &Cpu6502::And, 5>("AND", true),
    /* 33 */ OpNop1(),
    /* 34 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Bit, 4>("BIT", true),
    /* 35 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::And, 4>("AND"),
    /* 36 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Rol, 6>("ROL"),
    /* 37 */ OpNop1(),
    /* 38 */ OpImplied<&Cpu6502::Sec, 2>("SEC"),
    /* 39 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::And, 4, true>("AND"),
    /* 3A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Dec, 2>("DEA", true),
    /* 3B */ OpNop1(),
    /* 3C */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Bit, 4, true>("BIT", true),
    /* 3D */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::And, 4, true>("AND"),
    /* 3E */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Rol, 6, true>("ROL"),
    /* 3F */ OpNop1(),

    /* 40 */ OpCtrl<&Cpu6502::ExecRti>("RTI", AddrMode::Implied),
    /* 41 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Eor, 6>("EOR"),
    /* 42 */ OpNopSkip<AddrMode::Immediate, 2>("NOP"),
    /* 43 */ OpNop1(),
    /* 44 */ OpNopSkip<AddrMode::Immediate, 3>("NOP"),
    /* 45 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Eor, 3>("EOR"),
    /* 46 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Lsr, 5>("LSR"),
    /* 47 */ OpNop1(),
    /* 48 */ OpImplied<&Cpu6502::Pha, 3>("PHA"),
    /* 49 */ OpAlu<AddrMode::Immediate, &Cpu6502::Eor, 2>("EOR"),
    /* 4A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Lsr, 2>("LSR"),
    /* 4B */ OpNop1(),
    /* 4C */ OpCtrl<&Cpu6502::ExecJmpAbs>("JMP", AddrMode::Absolute),
    /* 4D */ OpAlu<AddrMode::Absolute, &Cpu6502::Eor, 4>("EOR"),
    /* 4E */ OpRmw<AddrMode::Absolute, &Cpu6502::Lsr, 6>("LSR"),
    /* 4F */ OpNop1(),

    /* 50 */ OpBranch<CPU6502_FLAG_V, false>("BVC"),
    /* 51 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Eor, 5, true>("EOR"),
    /* 52 */ OpAlu<AddrMode::ZeroPageIndirect, &Cpu6502::Eor, 5>("EOR", true),
    /* 53 */ OpNop1(),
    /* 54 */ OpNopSkip<AddrMode::Immediate, 4>("NOP"),
    /* 55 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Eor, 4>("EOR"),
    /* 56 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Lsr, 6>("LSR"),
    /* 57 */ OpNop1(),
    /* 58 */ OpImplied<&Cpu6502::Cli, 2>("CLI"),
    /* 59 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Eor, 4, true>("EOR"),
    /* 5A */ OpImplied<&Cpu6502::Phy, 3>("PHY", true),
    /* 5B */ OpNop1(),
    /* 5C */ OpNopSkip<AddrMode::ImmediateWord, 8>("NOP"),
    /* 5D */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Eor, 4, true>("EOR"),
    /* 5E */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Lsr, 6, true>("LSR"),
    /* 5F */ OpNop1(),

    /* 60 */ OpCtrl<&Cpu6502::ExecRts>("RTS", AddrMode::Implied),
    /* 61 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Adc, 6>("ADC"),
    /* 62 */ OpNopSkip<AddrMode::Immediate, 2>("NOP"),
    /* 63 */ OpNop1(),
    /* 64 */ OpStz<AddrMode::ZeroPage, 3>(),
    /* 65 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Adc, 3>("ADC"),
    /* 66 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Ror, 5>("ROR"),
    /* 67 */ OpNop1(),
    /* 68 */ OpImplied<&Cpu6502::Pla, 4>("PLA"),
    /* 69 */ OpAlu<AddrMode::Immediate, &Cpu6502::Adc, 2>("ADC"),
    /* 6A */ OpRmw<AddrMode::Accumulator, &Cpu6502::Ror, 2>("ROR"),
    /* 6B */ OpNop1(),
    /* 6C */ OpCtrl<&Cpu6502::ExecJmpInd>("JMP", AddrMode::AbsoluteIndirect),
    /* 6D */ OpAlu<AddrMode::Absolute, &Cpu6502::Adc, 4>("ADC"),
    /* 6E */ OpRmw<AddrMode::Absolute, &Cpu6502::Ror, 6>("ROR"),
    /* 6F */ OpNop1(),

    /* 70 */ OpBranch<CPU6502_FLAG_V, true>("BVS"),
    /* 71 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Adc, 5, true>("ADC"),
    /* 72 */ OpAlu<AddrMode::ZeroPageIndirect, &Cpu6502::Adc, 5, false, true>("ADC", true),
    /* 73 */ OpNop1(),
    /* 74 */ OpStz<AddrMode::ZeroPageX, 4>(),
    /* 75 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Adc, 4>("ADC"),
    /* 76 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Ror, 6>("ROR"),
    /* 77 */ OpNop1(),
    /* 78 */ OpImplied<&Cpu6502::Sei, 2>("SEI"),
    /* 79 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Adc, 4, true>("ADC"),
    /* 7A */ OpImplied<&Cpu6502::Ply, 4>("PLY", true),
    /* 7B */ OpNop1(),
    /* 7C */ OpCtrl<&Cpu6502::ExecJmpAbsXInd>("JMP", AddrMode::AbsoluteXIndirect, true),
    /* 7D */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Adc, 4, true>("ADC"),
    /* 7E */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Ror, 6, true>("ROR"),
    /* 7F */ OpNop1(),

    /* 80 */ OpCtrl<&Cpu6502::ExecBra>("BRA", AddrMode::Relative, true),
    /* 81 */ OpStore<AddrMode::IndirectX, &Cpu6502::m_A, 6>("STA"),
    /* 82 */ OpNopSkip<AddrMode::Immediate, 2>("NOP"),
    /* 83 */ OpNop1(),
    /* 84 */ OpStore<AddrMode::ZeroPage, &Cpu6502::m_Y, 3>("STY"),
    /* 85 */ OpStore<AddrMode::ZeroPage, &Cpu6502::m_A, 3>("STA"),
    /* 86 */ OpStore<AddrMode::ZeroPage, &Cpu6502::m_X, 3>("STX"),
    /* 87 */ OpNop1(),
    /* 88 */ OpImplied<&Cpu6502::Dey, 2>("DEY"),
    /* 89 */ OpAlu<AddrMode::Immediate, &Cpu6502::BitImm, 2>("BIT", true),
    /* 8A */ OpImplied<&Cpu6502::Txa, 2>("TXA"),
    /* 8B */ OpNop1(),
    /* 8C */ OpStore<AddrMode::Absolute, &Cpu6502::m_Y, 4>("STY"),
    /* 8D */ OpStore<AddrMode::Absolute, &Cpu6502::m_A, 4>("STA"),
    /* 8E */ OpStore<AddrMode::Absolute, &Cpu6502::m_X, 4>("STX"),
    /* 8F */ OpNop1(),

    /* 90 */ OpBranch<CPU6502_FLAG_C, false>("BCC"),
    /* 91 */ OpStore<AddrMode::IndirectY, &Cpu6502::m_A, 6>("STA"),
    /* 92 */ OpStore<AddrMode::ZeroPageIndirect, &Cpu6502::m_A, 5>("STA", true),
    /* 93 */ OpNop1(),
    /* 94 */ OpStore<AddrMode::ZeroPageX, &Cpu6502::m_Y, 4>("STY"),
    /* 95 */ OpStore<AddrMode::ZeroPageX, &Cpu6502::m_A, 4>("STA"),
    /* 96 */ OpStore<AddrMode::ZeroPageY, &Cpu6502::m_X, 4>("STX"),
    /* 97 */ OpNop1(),
    /* 98 */ OpImplied<&Cpu6502::Tya, 2>("TYA"),
    /* 99 */ OpStore<AddrMode::AbsoluteY, &Cpu6502::m_A, 5>("STA"),
    /* 9A */ OpImplied<&Cpu6502::Txs, 2>("TXS"),
    /* 9B */ OpNop1(),
    /* 9C */ OpStz<AddrMode::Absolute, 4>(),
    /* 9D */ OpStore<AddrMode::AbsoluteX, &Cpu6502::m_A, 5>("STA"),
    /* 9E */ OpStz<AddrMode::AbsoluteX, 5>(),
    /* 9F */ OpNop1(),

    /* A0 */ OpAlu<AddrMode::Immediate, &Cpu6502::Ldy, 2>("LDY"),
    /* A1 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Lda, 6>("LDA"),
    /* A2 */ OpAlu<AddrMode::Immediate, &Cpu6502::Ldx, 2>("LDX"),
    /* A3 */ OpNop1(),
    /* A4 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Ldy, 3>("LDY"),
    /* A5 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Lda, 3>("LDA"),
    /* A6 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Ldx, 3>("LDX"),
    /* A7 */ OpNop1(),
    /* A8 */ OpImplied<&Cpu6502::Tay, 2>("TAY"),
    /* A9 */ OpAlu<AddrMode::Immediate, &Cpu6502::Lda, 2>("LDA"),
    /* AA */ OpImplied<&Cpu6502::Tax, 2>("TAX"),
    /* AB */ OpNop1(),
    /* AC */ OpAlu<AddrMode::Absolute, &Cpu6502::Ldy, 4>("LDY"),
    /* AD */ OpAlu<AddrMode::Absolute, &Cpu6502::Lda, 4>("LDA"),
    /* AE */ OpAlu<AddrMode::Absolute, &Cpu6502::Ldx, 4>("LDX"),
    /* AF */ OpNop1(),

    /* B0 */ OpBranch<CPU6502_FLAG_C, true>("BCS"),
    /* B1 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Lda, 5, true>("LDA"),
    /* B2 */ OpAlu<AddrMode::ZeroPageIndirect, &Cpu6502::Lda, 5>("LDA", true),
    /* B3 */ OpNop1(),
    /* B4 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Ldy, 4>("LDY"),
    /* B5 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Lda, 4>("LDA"),
    /* B6 */ OpAlu<AddrMode::ZeroPageY, &Cpu6502::Ldx, 4>("LDX"),
    /* B7 */ OpNop1(),
    /* B8 */ OpImplied<&Cpu6502::Clv, 2>("CLV"),
    /* B9 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Lda, 4, true>("LDA"),
    /* BA */ OpImplied<&Cpu6502::Tsx, 2>("TSX"),
    /* BB */ OpNop1(),
    /* BC */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Ldy, 4, true>("LDY"),
    /* BD */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Lda, 4, true>("LDA"),
    /* BE */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Ldx, 4, true>("LDX"),
    /* BF */ OpNop1(),

    /* C0 */ OpAlu<AddrMode::Immediate, &Cpu6502::Cpy, 2>("CPY"),
    /* C1 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Cmp, 6>("CMP"),
    /* C2 */ OpNopSkip<AddrMode::Immediate, 2>("NOP"),
    /* C3 */ OpNop1(),
    /* C4 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Cpy, 3>("CPY"),
    /* C5 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Cmp, 3>("CMP"),
    /* C6 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Dec, 5>("DEC"),
    /* C7 */ OpNop1(),
    /* C8 */ OpImplied<&Cpu6502::Iny, 2>("INY"),
    /* C9 */ OpAlu<AddrMode::Immediate, &Cpu6502::Cmp, 2>("CMP"),
    /* CA */ OpImplied<&Cpu6502::Dex, 2>("DEX"),
    /* CB */ OpNop1(),
    /* CC */ OpAlu<AddrMode::Absolute, &Cpu6502::Cpy, 4>("CPY"),
    /* CD */ OpAlu<AddrMode::Absolute, &Cpu6502::Cmp, 4>("CMP"),
    /* CE */ OpRmw<AddrMode::Absolute, &Cpu6502::Dec, 6>("DEC"),
    /* CF */ OpNop1(),

    /* D0 */ OpBranch<CPU6502_FLAG_Z, false>("BNE"),
    /* D1 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Cmp, 5, true>("CMP"),
    /* D2 */ OpAlu<AddrMode::ZeroPageIndirect, &Cpu6502::Cmp, 5>("CMP", true),
    /* D3 */ OpNop1(),
    /* D4 */ OpNopSkip<AddrMode::Immediate, 4>("NOP"),
    /* D5 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Cmp, 4>("CMP"),
    /* D6 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Dec, 6>("DEC"),
    /* D7 */ OpNop1(),
    /* D8 */ OpImplied<&Cpu6502::Cld, 2>("CLD"),
    /* D9 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Cmp, 4, true>("CMP"),
    /* DA */ OpImplied<&Cpu6502::Phx, 3>("PHX", true),
    /* DB */ OpNop1(),
    /* DC */ OpNopSkip<AddrMode::ImmediateWord, 4>("NOP"),
    /* DD */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Cmp, 4, true>("CMP"),
    /* DE */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Dec, 7>("DEC"),
    /* DF */ OpNop1(),

    /* E0 */ OpAlu<AddrMode::Immediate, &Cpu6502::Cpx, 2>("CPX"),
    /* E1 */ OpAlu<AddrMode::IndirectX, &Cpu6502::Sbc, 6>("SBC"),
    /* E2 */ OpNopSkip<AddrMode::Immediate, 2>("NOP"),
    /* E3 */ OpNop1(),
    /* E4 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Cpx, 3>("CPX"),
    /* E5 */ OpAlu<AddrMode::ZeroPage, &Cpu6502::Sbc, 3>("SBC"),
    /* E6 */ OpRmw<AddrMode::ZeroPage, &Cpu6502::Inc, 5>("INC"),
    /* E7 */ OpNop1(),
    /* E8 */ OpImplied<&Cpu6502::Inx, 2>("INX"),
    /* E9 */ OpAlu<AddrMode::Immediate, &Cpu6502::Sbc, 2>("SBC"),
    /* EA */ OpNop<2>("NOP"),
    /* EB */ OpNop1(),
    /* EC */ OpAlu<AddrMode::Absolute, &Cpu6502::Cpx, 4>("CPX"),
    /* ED */ OpAlu<AddrMode::Absolute, &Cpu6502::Sbc, 4>("SBC"),
    /* EE */ OpRmw<AddrMode::Absolute, &Cpu6502::Inc, 6>("INC"),
    /* EF */ OpNop1(),

    /* F0 */ OpBranch<CPU6502_FLAG_Z, true>("BEQ"),
    /* F1 */ OpAlu<AddrMode::IndirectY, &Cpu6502::Sbc, 5, true>("SBC"),
    /* F2 */ OpAlu<AddrMode::ZeroPageIndirect, &Cpu6502::Sbc, 5, false, true>("SBC", true),
    /* F3 */ OpNop1(),
    /* F4 */ OpNopSkip<AddrMode::Immediate, 4>("NOP"),
    /* F5 */ OpAlu<AddrMode::ZeroPageX, &Cpu6502::Sbc, 4>("SBC"),
    /* F6 */ OpRmw<AddrMode::ZeroPageX, &Cpu6502::Inc, 6>("INC"),
    /* F7 */ OpNop1(),
    /* F8 */ OpImplied<&Cpu6502::Sed, 2>("SED"),
    /* F9 */ OpAlu<AddrMode::AbsoluteY, &Cpu6502::Sbc, 4, true>("SBC"),
    /* FA */ OpImplied<&Cpu6502::Plx, 4>("PLX", true),
    /* FB */ OpNop1(),
    /* FC */ OpNopSkip<AddrMode::ImmediateWord, 4>("NOP"),
    /* FD */ OpAlu<AddrMode::AbsoluteX, &Cpu6502::Sbc, 4, true>("SBC"),
    /* FE */ OpRmw<AddrMode::AbsoluteX, &Cpu6502::Inc, 7>("INC"),
    /* FF */ OpNop1()
  }};

  Cpu6502::Cpu6502(const CPU_ENUM cpuType) {
    m_cpuType = cpuType;
    m_activeOpTable = (cpuType == CPU_65C02) ? s_opTable65C02.data() : s_opTable6502.data();
    m_PC = 0;
    m_SR = CPU6502_FLAG_U;
    m_SP = 0xFF;
    m_A = m_X = m_Y = 0;
    m_traceOn = false;
    m_instructionsSkipped = 0;

    // Compute the BCD lookup table
    for (unsigned int t = 0; t < 256; t++) {
      m_BCDTable[0][t] = static_cast<unsigned char>((t >> 4) * 10 + (t & 0x0F));
      m_BCDTable[1][t] = static_cast<unsigned char>(((t % 100 / 10) << 4) | t % 10);
      m_BCDTable[2][t] = 0;
      if ((t & 0x0F) >= 0x0A) {
        m_BCDTable[2][t] = 1;
      }
      if (t >= 0xA0) {
        m_BCDTable[2][t] = 1;
      }
    }
  }

  int Cpu6502::Branch(const unsigned char val) {
    const unsigned short OldPC = m_PC;
    m_PC = static_cast<unsigned short>(m_PC + static_cast<signed char>(val));
    if ((OldPC ^ m_PC) & 0xFF00) {
      return 4;// Different page
    }
    return 3;// Same page
  }

  void Cpu6502::Aac(const unsigned char val) {
    And(val);
    SetFlagC(m_SR & CPU6502_FLAG_N);
  }

  void Cpu6502::Aax(const unsigned short addr) {
    WriteByte(addr, m_A & m_X);
  }

  void Cpu6502::Adc(const unsigned char val) {
    const unsigned char oldA = m_A;
    const auto sum = static_cast<unsigned short>(val + m_A + (m_SR & CPU6502_FLAG_C));
    const auto lowSum {static_cast<unsigned char>(sum)};
    SetFlagN(lowSum);
    SetFlagV((lowSum ^ oldA) & 0x80 && (lowSum ^ val) & 0x80);

    if (m_SR & CPU6502_FLAG_D) {

      // BCD mode
      if (m_BCDTable[2][m_A] || m_BCDTable[2][val]) {
        // bad BCD values
        unsigned char lowA = m_A & 0x0F;
        unsigned char lowVal = val & 0x0F;
        unsigned short highA = static_cast<unsigned short>(m_A) & 0xF0;
        unsigned short highVal = static_cast<unsigned short>(val) & 0xF0;

        // fix low part of register A and val
        if (lowA >= 0x0A) {
          lowA = static_cast<unsigned char>(lowA + 0x06);
        } else {

          // val is fixed only if register A is a good BCD value
          if (lowVal >= 0x0A) {
            lowVal = static_cast<unsigned char>(lowVal + 0x06);
          }
        }

        // make the sum of low part
        auto sumLow = static_cast<unsigned short>(lowA + lowVal + (m_SR & CPU6502_FLAG_C));

        // fix BCD value if both parts are valid BCD values
        if (sumLow >= 0x0A && lowA < 0x0A && lowVal < 0x0A) {
          sumLow = static_cast<unsigned short>(sumLow + 0x06);
        }

        // set carry for low part
        const unsigned short lowCarry = sumLow & 0xF0 ? 0x10 : 0;
        sumLow &= 0x0F;

        // fix high part of register A and val
        if (highA >= 0xA0) {
          highA = static_cast<unsigned short>(highA + 0x60);
        } else {

          // val is fixed only if register A is a good BCD value
          if (highVal >= 0xA0) {
            highVal = static_cast<unsigned short>(highVal + 0x60);
          }
        }

        // make the sum of high part
        auto sumHigh = static_cast<unsigned short>(highA + highVal + lowCarry);

        // fix BCD value if both parts are valid BCD values
        if (sumHigh >= 0xA0 && highA < 0xA0 && highVal < 0xA0) {
          sumHigh = static_cast<unsigned short>(sumHigh + 0x60);
        }

        // make the sum and set carry flag.
        const auto sumBCD = static_cast<unsigned short>(sumHigh + sumLow);
        SetFlagC(static_cast<unsigned char>(sumBCD >> 8));

        // fix A for overflow setting.
        m_A = static_cast<unsigned char>(sumBCD);

      } else {

        // good BCD values
        const auto sumBCD = m_BCDTable[0][m_A] + m_BCDTable[0][val] + (m_SR & CPU6502_FLAG_C);
        SetFlagC(sumBCD > 99);
        m_A = m_BCDTable[1][sumBCD & 0xFF];
      }
    } else {

      // binary mode
      SetFlagC(static_cast<unsigned char>(sum >> 8));
      m_A = static_cast<unsigned char>(sum);
    }
    SetFlagZ(m_A);
  }

  void Cpu6502::And(const unsigned char val) {
    m_A &= val;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Ane(const unsigned char val) {
    // m_A = (((m_A & val) | 0xEE) & (m_A | val)) & m_X;
    m_A &= m_X & val;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Arr(const unsigned char val) {
    const unsigned char tmpA = m_A & val;
    const unsigned char highA = (tmpA >> 4) & 0x0F;

    m_A = Ror(tmpA);

    // Set overflow and carry flag
    SetFlagV((tmpA ^ m_A) & CPU6502_FLAG_V);
    SetFlagC(highA + (highA & 1) > 5);

    // BCD fixup if we are in decimal mode
    if (m_SR & CPU6502_FLAG_D) {
      // BCD fixup for low part.
      if (const unsigned char lowA = tmpA & 0x0F; lowA + (lowA & 1) > 5) {
        m_A = static_cast<unsigned char>((m_A & 0xF0) | ((m_A + 0x06) & 0x0F));
      }

      // BCD fixup for high part.
      if (m_SR & CPU6502_FLAG_C) {
        m_A = static_cast<unsigned char>(m_A + 0x60);
      }
    }
  }

  unsigned char Cpu6502::Asl(unsigned char val) {
    SetFlagC(val & 0x80);
    val = static_cast<unsigned char>(val << 1);
    SetFlagN(val);
    SetFlagZ(val);
    return val;
  }

  void Cpu6502::Asr(const unsigned char val) {
    And(val);
    m_A = Lsr(m_A);
  }

  void Cpu6502::Atx(const unsigned char val) {
    m_A = m_X = m_A & val;// not sure
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Axa(const unsigned short addr) {
    WriteByte(addr, m_A & m_X & UNDOC_MASK);// not sure
  }

  void Cpu6502::Axs(const unsigned char val) {
    const auto reg = static_cast<unsigned short>((m_A & m_X) - val);

    m_X = static_cast<unsigned char>(reg);
    SetFlagC(reg < 0x100);
    SetFlagN(m_X);
    SetFlagZ(m_X);
  }

  void Cpu6502::Bit(const unsigned char val) {
    SetFlagN(val);
    SetFlagV(val & CPU6502_FLAG_V);
    SetFlagZ(val & m_A);
  }

  void Cpu6502::BitImm(const unsigned char val) {
    SetFlagZ(val & m_A);
  }

  void Cpu6502::Brk() {
    SetFlagB(1);
    PushWord(m_PC);
    PushByte(m_SR);
    SetFlagI(1);
    m_PC = ReadWord(CPU6502_VEC_IRQ);
  }

  void Cpu6502::Clc() {
    SetFlagC(0);
  }

  void Cpu6502::Cld() {
    SetFlagD(0);
  }

  void Cpu6502::Cli() {
    SetFlagI(0);
  }

  void Cpu6502::Clv() {
    SetFlagV(0);
  }

  void Cpu6502::Cmp(const unsigned char val) {
    const auto cmp = static_cast<unsigned short>(m_A - val);

    SetFlagC(cmp < 0x100);
    SetFlagN(static_cast<unsigned char>(cmp));
    SetFlagZ(static_cast<unsigned char>(cmp & 0xFF));
  }

  void Cpu6502::Cpx(const unsigned char val) {
    const auto  cmp = static_cast<unsigned short>(m_X - val);

    SetFlagC(cmp < 0x100);
    SetFlagN(static_cast<unsigned char>(cmp));
    SetFlagZ(static_cast<unsigned char>(cmp & 0xFF));
  }

  void Cpu6502::Cpy(const unsigned char val) {
    const auto cmp = static_cast<unsigned short>(m_Y - val);

    SetFlagC(cmp < 0x100);
    SetFlagN(static_cast<unsigned char>(cmp));
    SetFlagZ(static_cast<unsigned char>(cmp & 0xFF));
  }

  void Cpu6502::Dcp(const unsigned short addr, const unsigned char val) {
    const unsigned char reg = Dec(val);

    WriteByte(addr, reg);
    Cmp(reg);
  }

  unsigned char Cpu6502::Dec(unsigned char val) {
    val--;
    SetFlagN(val);
    SetFlagZ(val);
    return val;
  }

  void Cpu6502::Dex() {
    m_X--;
    SetFlagN(m_X);
    SetFlagZ(m_X);
  }

  void Cpu6502::Dey() {
    m_Y--;
    SetFlagN(m_Y);
    SetFlagZ(m_Y);
  }

  void Cpu6502::Eor(const unsigned char val) {
    m_A ^= val;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  unsigned char Cpu6502::Inc(unsigned char val) {
    val++;
    SetFlagN(val);
    SetFlagZ(val);
    return val;
  }

  void Cpu6502::Inx() {
    m_X++;
    SetFlagN(m_X);
    SetFlagZ(m_X);
  }

  void Cpu6502::Iny() {
    m_Y++;
    SetFlagN(m_Y);
    SetFlagZ(m_Y);
  }

  void Cpu6502::Isc(const unsigned short addr, const unsigned char val) {
    const unsigned char reg = Inc(val);

    WriteByte(addr, reg);
    Sbc(reg);
  }

  void Cpu6502::Jsr(const unsigned short addr) {
    m_PC--;// This really is a 6502 bug
    PushWord(m_PC);
    m_PC = addr;
  }

  void Cpu6502::Lar(const unsigned char val) {
    m_A = m_X = m_SP = m_SP & val;
    SetFlagN(m_A);
  }

  void Cpu6502::Lax(const unsigned char val) {
    m_A = m_X = val;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Lda(const unsigned char val) {
    m_A = val;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Ldx(const unsigned char val) {
    m_X = val;
    SetFlagN(m_X);
    SetFlagZ(m_X);
  }

  void Cpu6502::Ldy(const unsigned char val) {
    m_Y = val;
    SetFlagN(m_Y);
    SetFlagZ(m_Y);
  }

  unsigned char Cpu6502::Lsr(unsigned char val) {
    SetFlagC(val & 0x01);
    val = static_cast<unsigned char>(val >> 1);
    SetFlagN(val);
    SetFlagZ(val);
    return val;
  }

  void Cpu6502::Ora(const unsigned char val) {
    m_A |= val;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Pha() {
    PushByte(m_A);
  }

  void Cpu6502::Php() {
    PushByte(m_SR | CPU6502_FLAG_B);// This really is another 6502 bug
  }

  void Cpu6502::Phx() {
    PushByte(m_X);
  }

  void Cpu6502::Phy() {
    PushByte(m_Y);
  }

  void Cpu6502::Pla() {
    m_A = PopByte();
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Plp() {
    m_SR = PopByte();
    SetFlagB(0);
  }

  void Cpu6502::Plx() {
    m_X = PopByte();
    SetFlagN(m_X);
    SetFlagZ(m_X);
  }

  void Cpu6502::Ply() {
    m_Y = PopByte();
    SetFlagN(m_Y);
    SetFlagZ(m_Y);
  }

  void Cpu6502::Rla(const unsigned short addr, const unsigned char val) {
    const unsigned char reg = Rol(val);

    WriteByte(addr, reg);
    And(reg);
  }

  unsigned char Cpu6502::Rol(unsigned char val) {
    if (val & 0x80) {
      val = static_cast<unsigned char>(val << 1);
      if (m_SR & CPU6502_FLAG_C) {
        val |= 0x01;
      }
      SetFlagC(1);
    } else {
      val = static_cast<unsigned char>(val << 1);
      if (m_SR & CPU6502_FLAG_C) {
        val |= 0x01;
      }
      SetFlagC(0);
    }
    SetFlagN(val);
    SetFlagZ(val);
    return val;
  }

  unsigned char Cpu6502::Ror(unsigned char val) {
    if (m_SR & CPU6502_FLAG_C) {
      SetFlagC(val & 0x01);
      val = static_cast<unsigned char>(val >> 1);
      val |= 0x80;
    } else {
      SetFlagC(val & 0x01);
      val = static_cast<unsigned char>(val >> 1);
    }
    SetFlagN(val);
    SetFlagZ(val);
    return val;
  }

  void Cpu6502::Rra(const unsigned short addr, const unsigned char val) {
    const unsigned char reg = Ror(val);

    WriteByte(addr, reg);
    Adc(reg);
  }

  void Cpu6502::Rti() {
    m_SR = PopByte();
    m_PC = PopWord();
  }

  void Cpu6502::Rts() {
    m_PC = PopWord();
    m_PC++;
  }

  void Cpu6502::Sbc(const unsigned char val) {
    const unsigned char oldA = m_A;

    const auto dif = static_cast<unsigned short>(m_A - val - (m_SR & CPU6502_FLAG_C ? 0 : 1));
    const auto lowDif = static_cast<unsigned char>(dif);
    SetFlagN(lowDif);
    SetFlagV((lowDif ^ oldA) & 0x80 && (oldA ^ val) & 0x80);

    if (m_SR & CPU6502_FLAG_D) {

      // BCD mode
      if (m_BCDTable[2][m_A] || m_BCDTable[2][val]) {

        // bad BCD values
        const unsigned char lowA = m_A & 0x0F;
        const unsigned char lowVal = val & 0x0F;
        const unsigned short highA = static_cast<unsigned short>(m_A) & 0xF0;
        const unsigned short highVal = static_cast<unsigned short>(val) & 0xF0;

        // make the dif of low part
        auto difLow = static_cast<unsigned short>(lowA - lowVal - (m_SR & CPU6502_FLAG_C ? 0 : 1));

        // fix BCD value
        if (difLow & 0x10) {
          difLow = static_cast<unsigned short>(difLow - 0x06);
        }

        // set carry for low part
        const unsigned short lowCarry = difLow & 0xF0 ? 0x10 : 0;
        difLow &= 0x0F;

        // make the dif of high part
        auto  difHigh = static_cast<unsigned short>(highA - highVal - lowCarry);

        // fix BCD value if both parts are valid BCD values
        if (difHigh & 0x100) {
          difHigh = static_cast<unsigned short>(difHigh - 0x60);
        }

        // make the dif and set carry flag.
        const unsigned short difBCD = difHigh | difLow;
        SetFlagC(difBCD >> 8 == 0);

        // fix A for overflow setting.
        m_A = static_cast<unsigned char>(difBCD);
        SetFlagZ(difBCD != 0);
      } else {

        // good BCD values
        auto difBCD = m_BCDTable[0][m_A] - m_BCDTable[0][val] - (m_SR & CPU6502_FLAG_C ? 0 : 1);
        if (difBCD < 0)
          difBCD += 100;
        SetFlagC(m_A >= val + (m_SR & CPU6502_FLAG_C ? 0 : 1));
        m_A = m_BCDTable[1][difBCD & 0xFF];
        SetFlagZ(m_A);
      }
    } else {

      // binary mode
      SetFlagC(dif >> 8 == 0);
      m_A = static_cast<unsigned char>(dif);
      SetFlagZ(m_A);
    }
  }

  void Cpu6502::Sec() {
    SetFlagC(1);
  }

  void Cpu6502::Sed() {
    SetFlagD(1);
  }

  void Cpu6502::Sei() {
    SetFlagI(1);
  }

  void Cpu6502::Slo(const unsigned short addr, const unsigned char val) {
    const unsigned char reg = Asl(val);

    WriteByte(addr, reg);
    Ora(reg);
  }

  void Cpu6502::Sre(const unsigned short addr, const unsigned char val) {
    const unsigned char reg = Lsr(val);

    WriteByte(addr, reg);
    Eor(reg);
  }

  void Cpu6502::Sxa(const unsigned short addr) {
    WriteByte(addr, m_X & UNDOC_MASK);// not sure
  }

  void Cpu6502::Sya(const unsigned short addr) {
    WriteByte(addr, m_Y & UNDOC_MASK);// not sure
  }

  void Cpu6502::Tax() {
    m_X = m_A;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Tay() {
    m_Y = m_A;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  unsigned char Cpu6502::Trb(const unsigned char val) {
    SetFlagZ(val & m_A);
    return (m_A ^ 0xFF) & val;
  }

  unsigned char Cpu6502::Tsb(const unsigned char val) {
    SetFlagZ(val & m_A);
    return m_A | val;
  }

  void Cpu6502::Tsx() {
    m_X = m_SP;
    SetFlagN(m_X);
    SetFlagZ(m_X);
  }

  void Cpu6502::Txa() {
    m_A = m_X;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Txs() {
    m_SP = m_X;
  }

  void Cpu6502::Tya() {
    m_A = m_Y;
    SetFlagN(m_A);
    SetFlagZ(m_A);
  }

  void Cpu6502::Xas(const unsigned short addr) {
    m_SP = m_A & m_X;
    WriteByte(addr, m_SP & UNDOC_MASK);// not sure
  }

  int Cpu6502::Step() {
    char buf[256];

    BuildTrace(buf);
    if (buf[0]) {
      Trace(0, true, "%s", buf);
    }

    // table driven O(1) dispatch: the handler fetches its operands, executes and returns the cycle count
    const unsigned char opCode = ReadByte(m_PC++);
    return (this->*m_activeOpTable[opCode].handler)();
  }

  int Cpu6502::GetOpCodeLength(const unsigned char opCode) const
  {
    return OpcodeLength(m_activeOpTable[opCode].mode);
  }


  const char *Cpu6502::GetAddressLabel(const unsigned short addr) const {
    static char buffer[6];
    snprintf(buffer, 6, "$%04X", static_cast<int>(addr) & 0xFFFF);
    return buffer;
  }

  const char *Cpu6502::GetAddressOrLabel(const unsigned short addr) const {
    if (const char *label = GetAddressLabel(addr); label != nullptr) {
      return label;
    }
    return Cpu6502::GetAddressLabel(addr);
  }

  const char *Cpu6502::GetAddressLabelAllBanks(const unsigned short addr) const {
    // default implementation when no bank system is available
    return GetAddressOrLabel(addr);
  }

  const char *Cpu6502::GetAddressOrLabelAllBanks(const unsigned short addr) const {
    if (const char *label = GetAddressLabelAllBanks(addr); label != nullptr) {
      return label;
    }
    return Cpu6502::GetAddressLabelAllBanks(addr);
  }

  // format the operand of an instruction according to its addressing mode and return the operand address (0xFFFF if none)
  unsigned short Cpu6502::FormatOperand(char *&p, const AddrMode mode, const unsigned char *opCodes, const unsigned short address, const bool allBanks) const {
    const auto labelOf = [this, allBanks](const unsigned short a) { return allBanks ? GetAddressOrLabelAllBanks(a) : GetAddressOrLabel(a); };
    const auto append = [&p](const char *s) {
      strcpy(p, s);
      p += strlen(p);
    };
    const unsigned short zp = static_cast<unsigned short>(opCodes[1]) & 0x00FF;
    const unsigned short abs = static_cast<unsigned short>((opCodes[1] & 0x00FF) | ((opCodes[2] << 8) & 0xFF00));
    unsigned short addr = 0xFFFF;
    switch (mode) {
      case AddrMode::Immediate:
        snprintf(p, 7, "#$%02X ", static_cast<int>(opCodes[1]) & 0xFF);
        p += strlen(p);
        break;
      case AddrMode::ImmediateWord:
        snprintf(p, 9, "#$%04X ", static_cast<int>(abs) & 0xFFFF);
        p += strlen(p);
        break;
      case AddrMode::ZeroPage:
        addr = zp;
        append(labelOf(addr));
        break;
      case AddrMode::ZeroPageX:
        addr = zp;
        append(labelOf(addr));
        append(",x");
        break;
      case AddrMode::ZeroPageY:
        addr = zp;
        append(labelOf(addr));
        append(",y");
        break;
      case AddrMode::IndirectX:
        addr = zp;
        append("(");
        append(labelOf(addr));
        append(",x)");
        break;
      case AddrMode::IndirectY:
        addr = zp;
        append("(");
        append(labelOf(addr));
        append("),y");
        break;
      case AddrMode::ZeroPageIndirect:
        addr = zp;
        append("(");
        append(labelOf(addr));
        append(")");
        break;
      case AddrMode::Relative:
        append(labelOf(static_cast<unsigned short>(address + 2 + static_cast<signed char>(opCodes[1]))));
        break;
      case AddrMode::Absolute:
        addr = abs;
        append(labelOf(addr));
        break;
      case AddrMode::AbsoluteX:
        addr = abs;
        append(labelOf(addr));
        append(",x");
        break;
      case AddrMode::AbsoluteY:
        addr = abs;
        append(labelOf(addr));
        append(",y");
        break;
      case AddrMode::AbsoluteIndirect:
        addr = abs;
        append("(");
        append(labelOf(addr));
        append(")");
        break;
      case AddrMode::AbsoluteXIndirect:
        addr = abs;
        append("(");
        append(labelOf(addr));
        append(",x)");
        break;
      default:
        break;
    }
    return addr;
  }

  // format "<opcode bytes> <label>: <mnemonic> <operand>" and return the operand address (0xFFFF if none)
  unsigned short Cpu6502::FormatInstruction(char *&p, const unsigned char *opCodes, const int lenOpCode, const unsigned short address, const bool allBanks) const {
    for (int i = 0; i < lenOpCode; i++) {
      snprintf(p, 5, "%02X ", static_cast<int>(opCodes[i]) & 0xFF);
      p += 3;
    }
    for (int i = lenOpCode; i < 3; i++) {
      strcpy(p, "-- ");
      p += 3;
    }
    const char *label = GetAddressLabel(address);
    size_t lenLabel = 0;
    if (label != nullptr) {
      strcpy(p, label);
      lenLabel = strlen(label);
      p += lenLabel;
      *p++ = ':';
      lenLabel++;
    }
    for (auto i = lenLabel; i < 17; i++) {
      *p++ = ' ';
    }
    const OpcodeDesc &desc = m_activeOpTable[opCodes[0]];
    strcpy(p, desc.szName);
    p += 3;
    *p++ = ' ';
    return FormatOperand(p, desc.mode, opCodes, address, allBanks);
  }

  unsigned short Cpu6502::BuildTrace(char *buffer) {
    char *p = buffer;
    *p = 0;
    if (!m_traceOn) {
      m_instructionsSkipped = 0;
      return 0xFFFF;
    }
    if (IsAddressSkipped(m_PC)) {
      m_instructionsSkipped++;
      return 0xFFFF;
    }
    const unsigned char opCode = ReadByte(m_PC);
    const int lenOpCode = GetOpCodeLength(opCode);
    snprintf(p, 28, "A=%02X X=%02X Y=%02X P=%02X SP=%02X  ", static_cast<int>(m_A) & 0xFF, static_cast<int>(m_X) & 0xFF, static_cast<int>(m_Y) & 0xFF, static_cast<int>(m_SR) & 0xFF, static_cast<int>(m_SP) & 0xFF);
    p += strlen(p);
    snprintf(p, 6, "%04X:", static_cast<int>(m_PC) & 0xFFFF);
    p += strlen(p);
    unsigned char opCodes[3] = {opCode, 0, 0};
    for (int i = 1; i < lenOpCode; i++) {
      opCodes[i] = ReadByte(static_cast<unsigned short>(m_PC + i));
    }
    const unsigned short addr = FormatInstruction(p, opCodes, lenOpCode, m_PC, false);
    *p = 0;
    if (m_instructionsSkipped) {
      snprintf(p, 30, " ; %d instructions skipped", m_instructionsSkipped);
    }
    // The call to ReadByte is commented because it would be a second access for display purpose only and
    // this access may trigger something if address is a register (Port B for example)
    /*
	if (addr != 0xFFFF) {
		unsigned char val = ReadByte(addr);
        snprintf(p, 15, " ; $%04X=$%02X", ((int)addr) & 0xFFFF, ((int)val) & 0xFF);
	}
    */
    m_instructionsSkipped = 0;
    return addr;
  }

  int Cpu6502::BuildInstruction(char *buffer, unsigned char *data, const int lenData, const unsigned short address) {
    char *p = buffer;
    *p = 0;
    if (lenData < 1) {
      return -1;
    }
    const unsigned char opCode = data[0];
    const int lenOpCode = GetOpCodeLength(opCode);
    if (lenData < lenOpCode) {
      return -1;
    }
    snprintf(p, 8, "$%04X: ", static_cast<int>(address) & 0xFFFF);
    p += strlen(p);
    unsigned char opCodes[3] = {opCode, 0, 0};
    for (int i = 1; i < lenOpCode; i++) {
      opCodes[i] = data[i];
    }
    FormatInstruction(p, opCodes, lenOpCode, address, true);
    *p = 0;
    return lenOpCode;
  }

  int Cpu6502::Reset() {
    SetFlagB(0);
    SetFlagI(1);
    SetFlagD(0);
    m_PC = ReadWord(CPU6502_VEC_RESET);
    return 7;
  }

  int Cpu6502::Nmi() {
    PushWord(m_PC);
    PushByte(m_SR);
    SetFlagI(1);
    m_PC = ReadWord(CPU6502_VEC_NMI);
    return 7;
  }

  int Cpu6502::Irq() {
    int nClockCount;

    if (m_SR & CPU6502_FLAG_I) {
      nClockCount = 0;
    } else {
      SetFlagB(0);
      PushWord(m_PC);
      PushByte(m_SR);
      SetFlagI(1);
      m_PC = ReadWord(CPU6502_VEC_IRQ);
      nClockCount = 7;
    }
    return nClockCount;
  }

}