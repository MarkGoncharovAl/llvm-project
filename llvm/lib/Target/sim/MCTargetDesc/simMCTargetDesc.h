#ifndef __LLVM_LIB_TARGET_SIM_MCTARGETDESC_SIMMCTARGETDESC_H__
#define __LLVM_LIB_TARGET_SIM_MCTARGETDESC_SIMMCTARGETDESC_H__

#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class Target;
class Triple;

extern Target ThesimTarget;

} // End llvm namespace

// Defines symbolic names for sim registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "simGenRegisterInfo.inc"

// Defines symbolic names for the sim instructions.
#define GET_INSTRINFO_ENUM
#include "simGenInstrInfo.inc"

#endif // __LLVM_LIB_TARGET_SIM_MCTARGETDESC_SIMMCTARGETDESC_H__
