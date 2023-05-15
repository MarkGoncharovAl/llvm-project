#ifndef __LLVM_LIB_TARGET_MAG_SIM_H__
#define __LLVM_LIB_TARGET_MAG_SIM_H__

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


// namespace mag {
// enum {
// };
// } // namespace mag

} // namespace llvm

#endif // __LLVM_LIB_TARGET_MAG_SIM_H__
