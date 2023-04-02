#ifndef LLVM_LIB_TARGET_mag_mag_H
#define LLVM_LIB_TARGET_mag_mag_H

#include "MCTargetDesc/magMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class magTargetMachine;
class FunctionPass;
class magSubtarget;
class AsmPrinter;
class InstructionSelector;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineOperand;
class PassRegistry;

bool lowermagMachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                    AsmPrinter &AP);
bool LowermagMachineOperandToMCOperand(const MachineOperand &MO,
                                         MCOperand &MCOp, const AsmPrinter &AP);

FunctionPass *createmagISelDag(magTargetMachine &TM,
                                CodeGenOpt::Level OptLevel);


namespace mag {
enum {
  GP = mag::R0,
  RA = mag::R1,
  SP = mag::R2,
  FP = mag::R3,
  BP = mag::R4,
};
} // namespace mag

} // namespace llvm

#endif
