#ifndef LLVM_LIB_TARGET_mag_MCTARGETDESC_magINFO_H
#define LLVM_LIB_TARGET_mag_MCTARGETDESC_magINFO_H

#include "llvm/MC/MCInstrDesc.h"

namespace llvm {

namespace magCC {
enum CondCode {
  EQ,
  NE,
  LE,
  GT,
  LEU,
  GTU,
  INVALID,
};

CondCode getOppositeBranchCondition(CondCode);

enum BRCondCode {
  BREQ = 0x0,
};
} // end namespace magCC

namespace magOp {
enum OperandType : unsigned {
  OPERAND_MAGM16 = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_UIMM16,
};
} // namespace magOp

} // end namespace llvm

#endif
