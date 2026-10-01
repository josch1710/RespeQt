//
// Cpu6502 class
// (c) 1996 Eric BACHER
//
// This class simulates a 6502 CPU.
// All undocumented op-codes are implemented
//

#ifndef CPU6502_HPP
#define CPU6502_HPP 1

#include <array>

// get Low/High byte of a word.
constexpr unsigned char LO_BYTE(const unsigned short val) { return static_cast<unsigned char>(val & 0xff); }
constexpr unsigned char HI_BYTE(const unsigned short val) { return static_cast<unsigned char> ((val >> 8) & 0xff); }
constexpr unsigned short MAKE_WORD(const unsigned char l, const unsigned char h) { return static_cast<unsigned short> (l | (h << 8)); }

namespace DiskImages {
  // vectors for 6502
  constexpr unsigned short CPU6502_VEC_NMI = 0xFFFA;
  constexpr unsigned short CPU6502_VEC_RESET = 0xFFFC;
  constexpr unsigned short CPU6502_VEC_IRQ = 0xFFFE;

  // flags for status register
  constexpr unsigned char CPU6502_FLAG_C = 0x01;
  constexpr unsigned char CPU6502_FLAG_Z = 0x02;
  constexpr unsigned char CPU6502_FLAG_I = 0x04;
  constexpr unsigned char CPU6502_FLAG_D = 0x08;
  constexpr unsigned char CPU6502_FLAG_B = 0x10;
  constexpr unsigned char CPU6502_FLAG_U = 0x20;
  constexpr unsigned char CPU6502_FLAG_V = 0x40;
  constexpr unsigned char CPU6502_FLAG_N = 0x80;

  enum class AddrMode {
    Immediate,
    ZeroPage,
    ZeroPageX,
    ZeroPageY,
    Absolute,
    AbsoluteX,
    AbsoluteY,
    IndirectX,
    IndirectY,
    ZeroPageIndirect,
    AbsoluteIndirect,
    AbsoluteXIndirect,
    Accumulator,
    Implied,
    Relative,
    ImmediateWord
  };

  // number of bytes of an instruction depending on its addressing mode
  constexpr int OpcodeLength(const AddrMode mode) {
    switch (mode) {
      case AddrMode::Immediate:
      case AddrMode::ZeroPage:
      case AddrMode::ZeroPageX:
      case AddrMode::ZeroPageY:
      case AddrMode::IndirectX:
      case AddrMode::IndirectY:
      case AddrMode::ZeroPageIndirect:
      case AddrMode::Relative:
        return 2;
      case AddrMode::Absolute:
      case AddrMode::AbsoluteX:
      case AddrMode::AbsoluteY:
      case AddrMode::AbsoluteIndirect:
      case AddrMode::AbsoluteXIndirect:
      case AddrMode::ImmediateWord:
        return 3;
      default:
        return 1;
    }
  }

  // helper for static_assert in discarded if-constexpr branches
  template<AddrMode>
  inline constexpr bool DependentFalse = false;

  class Cpu6502;
  using OpHandler = int (Cpu6502::*)();

  // unified opcode descriptor: disassembly metadata and execution handler in one place
  struct OpcodeDesc {
    const char *szName;// 3 character mnemonic (string literal)
    bool bIllegal;
    AddrMode mode;
    OpHandler handler;
  };

  // mnemonic type accepted by the opcode table builders: enforces exactly 3 characters at compile time
  using Mnemonic = const char (&)[4];

  using CPU_ENUM = enum CPU_ENUM {
    CPU_6502,
    CPU_65C02
  };

  // This class implements 6502 CPU emulation.
  // It must be derived because ReadByte and WriteByte are pure virtual functions.
  class Cpu6502 {

  public:
    // constructors and destructor
    explicit Cpu6502(CPU_ENUM cpuType);
    virtual ~Cpu6502() = default;

    // return register value
    [[nodiscard]] unsigned short GetPC() const { return m_PC; }
    [[nodiscard]] unsigned char GetSR() const { return m_SR; }
    [[nodiscard]] unsigned char GetSP() const { return m_SP; }
    [[nodiscard]] unsigned char GetA() const { return m_A; }
    [[nodiscard]] unsigned char GetX() const { return m_X; }
    [[nodiscard]] unsigned char GetY() const { return m_Y; }

    // set register value
    void SetPC(const unsigned short PC) { m_PC = PC; }
    void SetSR(const unsigned char SR) { m_SR = SR; }
    void SetSP(const unsigned char SP) { m_SP = SP; }
    void SetA(const unsigned char A) { m_A = A; }
    void SetX(const unsigned char X) { m_X = X; }
    void SetY(const unsigned char Y) { m_Y = Y; }

    // modify a flag in status register
    void SetFlagB(const unsigned char val) {
      if (val) 
        m_SR |= CPU6502_FLAG_B;
      else
        m_SR &= static_cast<unsigned char>(~CPU6502_FLAG_B);
    }

    void SetFlagI(const unsigned char val) {
      if (val) 
        m_SR |= CPU6502_FLAG_I;
      else
        m_SR &= static_cast<unsigned char>(~CPU6502_FLAG_I);
    }

    void SetFlagZ(const unsigned char val) {
      if (val == 0) 
        m_SR |= CPU6502_FLAG_Z;
      else
        m_SR &= static_cast<unsigned char>(~CPU6502_FLAG_Z);
    }

    void SetFlagN(const unsigned char val) {
      if (val & CPU6502_FLAG_N) 
        m_SR |= CPU6502_FLAG_N;
      else
        m_SR &= static_cast<unsigned char>(~CPU6502_FLAG_N);
    }

    void SetFlagC(const unsigned char val) {
      if (val)
        m_SR |= CPU6502_FLAG_C;
      else
        m_SR &= static_cast<unsigned char>(~CPU6502_FLAG_C);
    }

    void SetFlagV(const unsigned char val) {
      if (val)
        m_SR |= CPU6502_FLAG_V;
      else
        m_SR &= static_cast<unsigned char>(~CPU6502_FLAG_V);
    }

    void SetFlagD(const unsigned char val) {
      if (val)
        m_SR |= CPU6502_FLAG_D;
      else
        m_SR &= static_cast<unsigned char>(~CPU6502_FLAG_D);
    }

    // read/write a word in memory
    unsigned short ReadWordBug(const unsigned short addr) { return MAKE_WORD(ReadByte(addr), ReadByte(static_cast<unsigned short>((addr & 0xFF00) | ((addr + 1) & 0x00FF)))); }
    unsigned short ReadWord(const unsigned short addr) { return MAKE_WORD(ReadByte(addr), ReadByte(static_cast<unsigned short>(addr + 1))); }
    void WriteWord(const unsigned short addr, const unsigned short val) {
      WriteByte(addr, LO_BYTE(val));
      WriteByte(static_cast<unsigned short>(addr + 1), HI_BYTE(val));
    }

    // push/pop a value on the stack.
    void PushByte(const unsigned char val) {
      WriteByte(static_cast<unsigned short>(0x100 + m_SP), val);
      m_SP--;
    }

    void PushWord(const unsigned short val) {
      PushByte(HI_BYTE(val));
      PushByte(LO_BYTE(val));
    }

    unsigned char PopByte() {
      m_SP++;
      return ReadByte(static_cast<unsigned short>(0x100 + m_SP));
    }

    unsigned short PopWord() {
      const unsigned char uLow = PopByte();
      const unsigned char uHigh = PopByte();
      return MAKE_WORD(uLow, uHigh);
    }

    // execute one instruction and returns the number of cycles.
    [[maybe_unused]]  virtual int Step();

    // find the length of an op-code (1 to 3 bytes) for the active CPU type
    [[nodiscard]] int GetOpCodeLength(unsigned char opCode) const;

    // build a trace of the next instruction. Buffer should be at least 128 bytes
    virtual unsigned short BuildTrace(char *buffer);
    virtual int BuildInstruction(char *buffer, unsigned char *data, int lenData, unsigned short address);
    virtual const char *GetAddressLabel(unsigned short addr) const;
    virtual const char *GetAddressLabelAllBanks(unsigned short addr) const;
    virtual void Trace(int module, bool debug, const char *msg, ...) = 0;
    [[maybe_unused]] virtual bool HasTrace() { return m_traceOn; }
    [[maybe_unused]] virtual void SetTrace(const bool traceOn) {
      m_traceOn = traceOn;
      m_instructionsSkipped = 0;
    }

    // trigger an interrupt line and returns the number of cycles.
    int Reset();
    [[maybe_unused]] int Nmi();
    [[maybe_unused]] int Irq();

    // read/write a byte in memory
    virtual unsigned char ReadByte(unsigned short addr) = 0;
    virtual void WriteByte(unsigned short addr, unsigned char val) = 0;
    virtual bool IsAddressSkipped(unsigned short addr) = 0;

  private:
    // cpu type
    CPU_ENUM m_cpuType;

    // registers
    unsigned short m_PC;
    unsigned char m_SR;
    unsigned char m_SP;
    unsigned char m_A;
    unsigned char m_X;
    unsigned char m_Y;

    // BCD loopkup table
    unsigned char m_BCDTable[3][256]{};

    // disassemble instructions
    bool m_traceOn;
    int m_instructionsSkipped;

    // opcode handler function pointer type
    using OpHandler = DiskImages::OpHandler;
    using OpcodeDesc = DiskImages::OpcodeDesc;

    // active dispatch table pointer
    const OpcodeDesc *m_activeOpTable{nullptr};

    // static opcode dispatch tables for 6502 and 65C02
    static const std::array<OpcodeDesc, 256> s_opTable6502;
    static const std::array<OpcodeDesc, 256> s_opTable65C02;

    // addressing helper templates: fetch the operand bytes following the opcode, advance the PC
    // and return the effective address of the operand. pageCrossed is set for the indexed modes
    // which may cost an extra cycle (Abs,X / Abs,Y / (Zpg),Y).
    template<AddrMode Mode>
    inline unsigned short FetchAddr(bool &pageCrossed) {
      if constexpr (Mode == AddrMode::Immediate) {
        return m_PC++;
      } else if constexpr (Mode == AddrMode::ZeroPage) {
        return ReadByte(m_PC++);
      } else if constexpr (Mode == AddrMode::ZeroPageX) {
        return static_cast<unsigned short>((ReadByte(m_PC++) + m_X) & 0xFF);
      } else if constexpr (Mode == AddrMode::ZeroPageY) {
        return static_cast<unsigned short>((ReadByte(m_PC++) + m_Y) & 0xFF);
      } else if constexpr (Mode == AddrMode::Absolute) {
        const unsigned short addr = ReadWord(m_PC);
        m_PC = static_cast<unsigned short>(m_PC + 2);
        return addr;
      } else if constexpr (Mode == AddrMode::AbsoluteX) {
        const unsigned short base = ReadWord(m_PC);
        m_PC = static_cast<unsigned short>(m_PC + 2);
        const unsigned short addr = static_cast<unsigned short>(base + m_X);
        pageCrossed = ((base ^ addr) & 0xFF00) != 0;
        return addr;
      } else if constexpr (Mode == AddrMode::AbsoluteY) {
        const unsigned short base = ReadWord(m_PC);
        m_PC = static_cast<unsigned short>(m_PC + 2);
        const unsigned short addr = static_cast<unsigned short>(base + m_Y);
        pageCrossed = ((base ^ addr) & 0xFF00) != 0;
        return addr;
      } else if constexpr (Mode == AddrMode::IndirectX) {
        const unsigned short ptr = static_cast<unsigned short>((ReadByte(m_PC++) + m_X) & 0xFF);
        return ReadWordBug(ptr);
      } else if constexpr (Mode == AddrMode::IndirectY) {
        const unsigned short base = ReadWordBug(ReadByte(m_PC++));
        const unsigned short addr = static_cast<unsigned short>(base + m_Y);
        pageCrossed = ((base ^ addr) & 0xFF00) != 0;
        return addr;
      } else if constexpr (Mode == AddrMode::ZeroPageIndirect) {
        return ReadWordBug(ReadByte(m_PC++));
      } else if constexpr (Mode == AddrMode::AbsoluteIndirect) {
        const unsigned short ptr = ReadWord(m_PC);
        m_PC = static_cast<unsigned short>(m_PC + 2);
        return ReadWordBug(ptr);
      } else if constexpr (Mode == AddrMode::AbsoluteXIndirect) {
        const unsigned short base = ReadWord(m_PC);
        m_PC = static_cast<unsigned short>(m_PC + 2);
        return ReadWord(static_cast<unsigned short>(base + m_X));
      } else if constexpr (Mode == AddrMode::Relative) {
        const auto offset = static_cast<signed char>(ReadByte(m_PC++));
        return static_cast<unsigned short>(m_PC + offset);
      } else {
        static_assert(DependentFalse<Mode>, "addressing mode has no operand address");
        return 0;
      }
    }

    template<AddrMode Mode>
    inline unsigned short FetchAddr() {
      bool dummy = false;
      return FetchAddr<Mode>(dummy);
    }

    // ALU execution template (LDA, LDX, LDY, ORA, AND, EOR, ADC, SBC, CMP, CPX, CPY, BIT, BitImm, etc.)
    // CheckCross adds one cycle on page crossing, DecimalPenalty adds one cycle in decimal mode (65C02 ADC/SBC)
    template<AddrMode Mode, void (Cpu6502::*Op)(unsigned char), int BaseCycles, bool CheckCross = false, bool DecimalPenalty = false>
    int ExecAlu() {
      bool pageCrossed = false;
      const unsigned char val = ReadByte(FetchAddr<Mode>(pageCrossed));
      const bool decimal = DecimalPenalty && (m_SR & CPU6502_FLAG_D) != 0;
      (this->*Op)(val);
      return BaseCycles + ((CheckCross && pageCrossed) ? 1 : 0) + (decimal ? 1 : 0);
    }

    // Store execution template (STA, STX, STY)
    template<AddrMode Mode, unsigned char Cpu6502::*Reg, int BaseCycles>
    int ExecStore() {
      const unsigned short addr = FetchAddr<Mode>();
      WriteByte(addr, this->*Reg);
      return BaseCycles;
    }

    // Store zero template (STZ)
    template<AddrMode Mode, int BaseCycles>
    int ExecStz() {
      const unsigned short addr = FetchAddr<Mode>();
      WriteByte(addr, 0);
      return BaseCycles;
    }

    // Special Store template (Aax/Sax, Axa, Sxa, Sya, Xas)
    template<AddrMode Mode, void (Cpu6502::*Op)(unsigned short), int BaseCycles>
    int ExecStoreSpecial() {
      const unsigned short addr = FetchAddr<Mode>();
      (this->*Op)(addr);
      return BaseCycles;
    }

    // Read-Modify-Write template (ASL, LSR, ROL, ROR, INC, DEC, TRB, TSB)
    // CheckCross models the 65C02 shift/rotate Abs,X timing (6 cycles + 1 on page cross)
    template<AddrMode Mode, unsigned char (Cpu6502::*Op)(unsigned char), int BaseCycles, bool CheckCross = false>
    int ExecRmw() {
      bool pageCrossed = false;
      if constexpr (Mode == AddrMode::Accumulator) {
        m_A = (this->*Op)(m_A);
      } else {
        const unsigned short addr = FetchAddr<Mode>(pageCrossed);
        const unsigned char val = ReadByte(addr);
        WriteByte(addr, (this->*Op)(val));
      }
      return BaseCycles + ((CheckCross && pageCrossed) ? 1 : 0);
    }

    // Undocumented RMW + ALU template (SLO, SRE, RLA, RRA, ISC, DCP)
    template<AddrMode Mode, void (Cpu6502::*Op)(unsigned short, unsigned char), int BaseCycles>
    int ExecRmwAlu() {
      const unsigned short addr = FetchAddr<Mode>();
      const unsigned char val = ReadByte(addr);
      (this->*Op)(addr, val);
      return BaseCycles;
    }

    // Branch template: 2 cycles if not taken, 3 if taken, 4 if taken across a page boundary
    template<unsigned char FlagMask, bool Condition>
    int ExecBranch() {
      const unsigned char offset = ReadByte(m_PC++);
      const bool flagSet = (m_SR & FlagMask) != 0;
      if (flagSet == Condition) {
        return Branch(offset);
      }
      return 2;
    }

    // Unconditional Branch (BRA for 65C02)
    int ExecBra() {
      const unsigned char offset = ReadByte(m_PC++);
      return Branch(offset);
    }

    // Implied register/flag template
    template<void (Cpu6502::*Op)(), int Cycles>
    int ExecImplied() {
      (this->*Op)();
      return Cycles;
    }

    // NOP templates
    template<int Cycles>
    int ExecNop() {
      return Cycles;
    }

    // NOP skipping its operand bytes without any memory access (65C02 NOPs, dop #Imm)
    template<AddrMode Mode, int Cycles>
    int ExecNopSkip() {
      m_PC = static_cast<unsigned short>(m_PC + OpcodeLength(Mode) - 1);
      return Cycles;
    }

    // NOP performing a dummy read of its operand (undocumented dop/top)
    template<AddrMode Mode, int BaseCycles, bool CheckCross = false>
    int ExecNopRead() {
      bool pageCrossed = false;
      const unsigned short addr = FetchAddr<Mode>(pageCrossed);
      ReadByte(addr);
      return BaseCycles + ((CheckCross && pageCrossed) ? 1 : 0);
    }

    // Control flow handlers
    int ExecJsr() {
      const unsigned short addr = FetchAddr<AddrMode::Absolute>();
      Jsr(addr);
      return 6;
    }

    int ExecJmpAbs() {
      m_PC = FetchAddr<AddrMode::Absolute>();
      return 3;
    }

    // JMP (Abs) on the 6502: the pointer does not cross a page boundary (ReadWordBug)
    int ExecJmpIndBug() {
      m_PC = FetchAddr<AddrMode::AbsoluteIndirect>();
      return 5;
    }

    // JMP (Abs) on the 65C02: the page boundary bug is fixed
    int ExecJmpInd() {
      m_PC = ReadWord(ReadWord(m_PC));
      return 6;
    }

    int ExecJmpAbsXInd() {
      m_PC = FetchAddr<AddrMode::AbsoluteXIndirect>();
      return 6;
    }

    int ExecRts() {
      Rts();
      return 6;
    }

    int ExecRti() {
      Rti();
      return 6;
    }

    int ExecBrk() {
      m_PC++;
      Brk();
      return 7;
    }

    int ExecKil() {
      return 0;
    }

    // Declarative opcode table builders. Each one returns an OpcodeDesc whose handler is the matching
    // Exec* template instantiated with the very same addressing mode and cycle count that are stored as
    // disassembly metadata, so metadata and execution can never drift apart. The builders are constexpr,
    // hence the opcode tables are initialized at compile time.

    // read operand and apply an ALU operation (LDA, ORA, ADC, CMP, BIT, lax, ...)
    template<AddrMode Mode, void (Cpu6502::*Op)(unsigned char), int Cycles, bool CheckCross = false, bool DecimalPenalty = false>
    static constexpr OpcodeDesc OpAlu(Mnemonic name, const bool illegal = false) {
      return {name, illegal, Mode, &Cpu6502::ExecAlu<Mode, Op, Cycles, CheckCross, DecimalPenalty>};
    }

    // store a register (STA, STX, STY)
    template<AddrMode Mode, unsigned char Cpu6502::*Reg, int Cycles>
    static constexpr OpcodeDesc OpStore(Mnemonic name, const bool illegal = false) {
      return {name, illegal, Mode, &Cpu6502::ExecStore<Mode, Reg, Cycles>};
    }

    // store zero (65C02 STZ)
    template<AddrMode Mode, int Cycles>
    static constexpr OpcodeDesc OpStz() {
      return {"STZ", true, Mode, &Cpu6502::ExecStz<Mode, Cycles>};
    }

    // undocumented stores (aax, axa, sxa, sya, xas)
    template<AddrMode Mode, void (Cpu6502::*Op)(unsigned short), int Cycles>
    static constexpr OpcodeDesc OpStoreSpecial(Mnemonic name) {
      return {name, true, Mode, &Cpu6502::ExecStoreSpecial<Mode, Op, Cycles>};
    }

    // read-modify-write (ASL, LSR, ROL, ROR, INC, DEC, TRB, TSB), optional page cross penalty (65C02)
    template<AddrMode Mode, unsigned char (Cpu6502::*Op)(unsigned char), int Cycles, bool CheckCross = false>
    static constexpr OpcodeDesc OpRmw(Mnemonic name, const bool illegal = false) {
      return {name, illegal, Mode, &Cpu6502::ExecRmw<Mode, Op, Cycles, CheckCross>};
    }

    // undocumented RMW + ALU combos (slo, rla, sre, rra, dcp, isc)
    template<AddrMode Mode, void (Cpu6502::*Op)(unsigned short, unsigned char), int Cycles>
    static constexpr OpcodeDesc OpRmwAlu(Mnemonic name) {
      return {name, true, Mode, &Cpu6502::ExecRmwAlu<Mode, Op, Cycles>};
    }

    // conditional branch on a status flag
    template<unsigned char FlagMask, bool Condition>
    static constexpr OpcodeDesc OpBranch(Mnemonic name) {
      return {name, false, AddrMode::Relative, &Cpu6502::ExecBranch<FlagMask, Condition>};
    }

    // implied register/flag/stack operation
    template<void (Cpu6502::*Op)(), int Cycles>
    static constexpr OpcodeDesc OpImplied(Mnemonic name, const bool illegal = false) {
      return {name, illegal, AddrMode::Implied, &Cpu6502::ExecImplied<Op, Cycles>};
    }

    // 1-byte NOP
    template<int Cycles>
    static constexpr OpcodeDesc OpNop(Mnemonic name, const bool illegal = false) {
      return {name, illegal, AddrMode::Implied, &Cpu6502::ExecNop<Cycles>};
    }

    // NOP skipping its operand bytes without memory access (65C02 NOPs, dop #Imm)
    template<AddrMode Mode, int Cycles>
    static constexpr OpcodeDesc OpNopSkip(Mnemonic name) {
      return {name, true, Mode, &Cpu6502::ExecNopSkip<Mode, Cycles>};
    }

    // undocumented NOP with a dummy operand read (dop/top)
    template<AddrMode Mode, int Cycles, bool CheckCross = false>
    static constexpr OpcodeDesc OpNopRead(Mnemonic name) {
      return {name, true, Mode, &Cpu6502::ExecNopRead<Mode, Cycles, CheckCross>};
    }

    // control flow with a dedicated handler (BRK, JSR, JMP, RTS, RTI, BRA, kil)
    template<OpHandler Handler>
    static constexpr OpcodeDesc OpCtrl(Mnemonic name, const AddrMode mode, const bool illegal = false) {
      return {name, illegal, mode, Handler};
    }

    // shorthands: 1 cycle NOP replacing every undocumented NMOS opcode on the 65C02, and the NMOS halt
    static constexpr OpcodeDesc OpNop1() { return OpNop<1>("NOP", true); }
    static constexpr OpcodeDesc OpKil() { return OpCtrl<&Cpu6502::ExecKil>("kil", AddrMode::Implied, true); }

    // modify m_PC with relative branch and return the number of cycles (3, or 4 when crossing a page)
    int Branch(unsigned char val);

    // opcode functions
    void Aac(unsigned char val);
    void Aax(unsigned short addr);
    void Adc(unsigned char val);
    void And(unsigned char val);
    void Ane(unsigned char val);
    void Arr(unsigned char val);
    unsigned char Asl(unsigned char val);
    void Asr(unsigned char val);
    void Atx(unsigned char val);
    void Axa(unsigned short addr);
    void Axs(unsigned char val);
    void Bit(unsigned char val);
    void BitImm(unsigned char val);
    void Brk();
    void Clc();
    void Cld();
    void Cli();
    void Clv();
    void Cmp(unsigned char val);
    void Cpx(unsigned char val);
    void Cpy(unsigned char val);
    void Dcp(unsigned short addr, unsigned char val);
    unsigned char Dec(unsigned char val);
    void Dex();
    void Dey();
    void Eor(unsigned char val);
    unsigned char Inc(unsigned char val);
    void Inx();
    void Iny();
    void Isc(unsigned short addr, unsigned char val);
    void Jsr(unsigned short addr);
    void Lar(unsigned char val);
    void Lax(unsigned char val);
    void Lda(unsigned char val);
    void Ldx(unsigned char val);
    void Ldy(unsigned char val);
    unsigned char Lsr(unsigned char val);
    void Ora(unsigned char val);
    void Pha();
    void Php();
    void Phx();
    void Phy();
    void Pla();
    void Plp();
    void Plx();
    void Ply();
    void Rla(unsigned short addr, unsigned char val);
    unsigned char Rol(unsigned char val);
    unsigned char Ror(unsigned char val);
    void Rra(unsigned short addr, unsigned char val);
    void Rti();
    void Rts();
    void Sbc(unsigned char val);
    void Sec();
    void Sed();
    void Sei();
    void Slo(unsigned short addr, unsigned char val);
    void Sre(unsigned short addr, unsigned char val);
    void Sxa(unsigned short addr);
    void Sya(unsigned short addr);
    void Tax();
    void Tay();
    unsigned char Trb(unsigned char val);
    unsigned char Tsb(unsigned char val);
    void Tsx();
    void Txa();
    void Txs();
    void Tya();
    void Xas(unsigned short addr);

    // disassembly helpers shared by BuildTrace and BuildInstruction
    unsigned short FormatInstruction(char *&p, const unsigned char *opCodes, int lenOpCode, unsigned short address, bool allBanks) const;
    unsigned short FormatOperand(char *&p, AddrMode mode, const unsigned char *opCodes, unsigned short address, bool allBanks) const;

    // get label for a given address
    [[nodiscard]] const char *GetAddressOrLabel(unsigned short addr) const;
    [[nodiscard]] const char *GetAddressOrLabelAllBanks(unsigned short addr) const;
  };
}
#endif
