//
// Cpu6502 class
// (c) 1996 Eric BACHER
//
// This class simulates a 6502 CPU.
// All undocumented op-codes are implemented
//

#ifndef CPU6502_HPP
#define CPU6502_HPP 1

// get Low/High byte of a word.
#define LO_BYTE(val) ((unsigned char) ((val) &0xff))
#define HI_BYTE(val) ((unsigned char) (((val) >> 8) & 0xff))
#define MAKE_WORD(l, h) ((unsigned short) ((l) | ((h) << 8)))

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

  using MODE_ENUM = enum MODE_ENUM {
    // 6502
    MODE_IMMEDIATE,
    MODE_ABSOLUTE,
    MODE_ZERO_PAGE,
    MODE_ACCUMULATOR,
    MODE_IMPLIED,
    MODE_INDEXED_INDIRECT,
    MODE_INDIRECT_INDEXED,
    MODE_ZERO_PAGE_X,
    MODE_ZERO_PAGE_Y,
    MODE_ABSOLUTE_X,
    MODE_ABSOLUTE_Y,
    MODE_RELATIVE,
    MODE_INDIRECT,
    // 65C02
    MODE_ZERO_PAGE_INDIRECT,
    MODE_ZERO_PAGE_RELATIVE,
    MODE_IMMEDIATE_WORD// for 3-byte NOP
  };

  using OPCODE = struct OPCODE {
    const char szName[4];
    bool bIllegal;
    MODE_ENUM wMode;
  };

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
      if (val) m_SR |= CPU6502_FLAG_B;
      else
        m_SR &= ~CPU6502_FLAG_B;
    }

    void SetFlagI(const unsigned char val) {
      if (val) m_SR |= CPU6502_FLAG_I;
      else
        m_SR &= ~CPU6502_FLAG_I;
    }

    void SetFlagZ(const unsigned char val) {
      if (val == 0) m_SR |= CPU6502_FLAG_Z;
      else
        m_SR &= ~CPU6502_FLAG_Z;
    }

    void SetFlagN(const unsigned char val) {
      if (val & CPU6502_FLAG_N) m_SR |= CPU6502_FLAG_N;
      else
        m_SR &= ~CPU6502_FLAG_N;
    }

    void SetFlagC(const unsigned char val) {
      if (val) m_SR |= CPU6502_FLAG_C;
      else
        m_SR &= ~CPU6502_FLAG_C;
    }

    void SetFlagV(const unsigned char val) {
      if (val) m_SR |= CPU6502_FLAG_V;
      else
        m_SR &= ~CPU6502_FLAG_V;
    }

    void SetFlagD(const unsigned char val) {
      if (val) m_SR |= CPU6502_FLAG_D;
      else
        m_SR &= ~CPU6502_FLAG_D;
    }

    // read/write a word in memory
    unsigned short ReadWordBug(const unsigned short addr) { return MAKE_WORD(ReadByte(addr), ReadByte((addr & 0xFF00) | (addr + 1 & 0x00FF))); }
    unsigned short ReadWord(const unsigned short addr) { return MAKE_WORD(ReadByte(addr), ReadByte(addr + 1)); }
    void WriteWord(const unsigned short addr, const unsigned short val) {
      WriteByte(addr, LO_BYTE(val));
      WriteByte(addr + 1, HI_BYTE(val));
    }

    // push/pop a value on the stack.
    void PushByte(const unsigned char val) {
      WriteByte(0x100 + m_SP, val);
      m_SP--;
    }

    void PushWord(const unsigned short val) {
      PushByte(HI_BYTE(val));
      PushByte(LO_BYTE(val));
    }

    unsigned char PopByte() {
      m_SP++;
      return ReadByte(0x100 + m_SP);
    }

    unsigned short PopWord() {
      const unsigned char uLow = PopByte();
      return static_cast<unsigned short>(uLow | uLow << 8);
    }

    // execute one instruction and returns the number of cycles.
    [[maybe_unused]]  virtual int Step();

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

    // get address depending on addressing mode
    unsigned short FetchZPage(const unsigned short addr) { return ReadByte(addr); }
    unsigned short FetchAbsolute(const unsigned short addr) { return ReadWord(addr); }
    unsigned short FetchAbsoluteX(const unsigned short addr) { return ReadWord(addr) + m_X; }
    unsigned short FetchAbsoluteY(const unsigned short addr) { return ReadWord(addr) + m_Y; }
    unsigned short FetchZPageX(const unsigned short addr) { return static_cast<unsigned short>(ReadByte(addr) + m_X); }
    unsigned short FetchZPageY(const unsigned short addr) { return static_cast<unsigned short>(ReadByte(addr) + m_Y); }
    unsigned short FetchXIndirect(const unsigned short addr) { return ReadWord(static_cast<unsigned char>(ReadByte(addr) + m_X)); }
    unsigned short FetchIndirectY(const unsigned short addr) { return ReadWord(ReadByte(addr)) + m_Y; }
    unsigned short FetchIndirectBug(const unsigned short addr) { return ReadWordBug(ReadWord(addr)); }
    unsigned short FetchIndirect(const unsigned short addr) { return ReadWord(ReadWord(addr)); }
    unsigned short FetchZPageIndirect(const unsigned short addr) { return ReadWord( ReadByte(addr)); }
    unsigned short FetchAbsoluteXIndirect(const unsigned short addr) { return ReadWord(ReadWord(addr) + m_X); }

    // get value depending on addressing mode
    unsigned char ReadImm(const unsigned short addr) { return ReadByte(addr); }
    unsigned char ReadZPage(const unsigned short addr) { return ReadByte(FetchZPage(addr)); }
    unsigned char ReadAbsolute(const unsigned short addr) { return ReadByte(FetchAbsolute(addr)); }
    unsigned char ReadAbsoluteX(const unsigned short addr) { return ReadByte(FetchAbsoluteX(addr)); }
    unsigned char ReadAbsoluteY(const unsigned short addr) { return ReadByte(FetchAbsoluteY(addr)); }
    unsigned char ReadZPageX(const unsigned short addr) { return ReadByte(FetchZPageX(addr)); }
    unsigned char ReadZPageY(const unsigned short addr) { return ReadByte(FetchZPageY(addr)); }
    unsigned char ReadXIndirect(const unsigned short addr) { return ReadByte(FetchXIndirect(addr)); }
    unsigned char ReadIndirectY(const unsigned short addr) { return ReadByte(FetchIndirectY(addr)); }
    unsigned char ReadZPageIndirect(const unsigned short addr) { return ReadByte(FetchZPageIndirect(addr)); }

    // modify m_PC with relative branch
    inline int Branch(unsigned char val);

    // opcode functions
    inline void Aac(unsigned char val);
    inline void Aax(unsigned short addr);
    inline void Adc(unsigned char val);
    inline void And(unsigned char val);
    inline void Ane(unsigned char val);
    inline void Arr(unsigned char val);
    inline unsigned char Asl(unsigned char val);
    inline void Asr(unsigned char val);
    inline void Atx(unsigned char val);
    inline void Axa(unsigned short addr);
    inline void Axs(unsigned char val);
    inline int Bcc(unsigned char val);
    inline int Bcs(unsigned char val);
    inline int Beq(unsigned char val);
    inline void Bit(unsigned char val);
    inline void BitImm(unsigned char val);
    inline int Bmi(unsigned char val);
    inline int Bne(unsigned char val);
    inline int Bpl(unsigned char val);
    inline int Bvc(unsigned char val);
    inline int Bvs(unsigned char val);
    inline void Brk();
    inline void Clc();
    inline void Cld();
    inline void Cli();
    inline void Clv();
    inline void Cmp(unsigned char val);
    inline void Cpx(unsigned char val);
    inline void Cpy(unsigned char val);
    inline void Dcp(unsigned short addr, unsigned char val);
    inline unsigned char Dec(unsigned char val);
    inline void Dea();
    inline void Dex();
    inline void Dey();
    inline void Eor(unsigned char val);
    inline unsigned char Inc(unsigned char val);
    inline void Ina();
    inline void Inx();
    inline void Iny();
    inline void Isc(unsigned short addr, unsigned char val);
    inline void Jmp(unsigned short addr);
    inline void Jsr(unsigned short addr);
    inline void Lar(unsigned char val);
    inline void Lax(unsigned char val);
    inline void Lda(unsigned char val);
    inline void Ldx(unsigned char val);
    inline void Ldy(unsigned char val);
    inline unsigned char Lsr(unsigned char val);
    inline void Ora(unsigned char val);
    inline void Pha();
    inline void Php();
    inline void Phx();
    inline void Phy();
    inline void Pla();
    inline void Plp();
    inline void Plx();
    inline void Ply();
    inline void Rla(unsigned short addr, unsigned char val);
    inline unsigned char Rol(unsigned char val);
    inline unsigned char Ror(unsigned char val);
    inline void Rra(unsigned short addr, unsigned char val);
    inline void Rti();
    inline void Rts();
    inline void Sbc(unsigned char val);
    inline void Sec();
    inline void Sed();
    inline void Sei();
    inline void Slo(unsigned short addr, unsigned char val);
    inline void Sta(unsigned short addr);
    inline void Stx(unsigned short addr);
    inline void Sty(unsigned short addr);
    inline void Stz(unsigned short addr);
    inline void Sre(unsigned short addr, unsigned char val);
    inline void Sxa(unsigned short addr);
    inline void Sya(unsigned short addr);
    inline void Tax();
    inline void Tay();
    inline unsigned char Trb(unsigned char val);
    inline unsigned char Tsb(unsigned char val);
    inline void Tsx();
    inline void Txa();
    inline void Txs();
    inline void Tya();
    inline void Xas(unsigned short addr);

    // find the length of an op-code
    [[nodiscard]] inline int GetOpCodeLength(unsigned char opCode) const;

    // get label for a given address
    inline const char *GetAddressOrLabel(unsigned short addr) const;
    inline const char *GetAddressOrLabelAllBanks(unsigned short addr) const;
  };
}
#endif
