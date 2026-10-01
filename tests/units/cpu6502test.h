#ifndef CPU6502TEST_H
#define CPU6502TEST_H

#include <QObject>

namespace Tests {

class Cpu6502Test : public QObject {
  Q_OBJECT

public:
  Cpu6502Test() = default;
  ~Cpu6502Test() override = default;

private slots:
  void testStackOperations();
  void testBranchesAndLoops();
  void testZeroPageAddressing();
  void testIndirectAddressing();
  void testJsrRts();
  void testCrossPageCalculations();
  void testDisassemblyRelativeJump();
  void testOpcodeTableConsistency();
  void testAluBinaryAndFlags();
  void testAdcSbcDecimal();
  void testRmwShiftsAndRotates();
  void testLoadStoreAllModes();
  void testBranchCyclesAndJmpIndirect();
  void testUndocumentedOpcodes();
  void test65C02Instructions();
  void testDisassemblyAllModes();
};

} // namespace Tests

#endif // CPU6502TEST_H
