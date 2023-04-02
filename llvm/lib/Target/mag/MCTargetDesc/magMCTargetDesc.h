#ifndef LLVM_LIB_TARGET_mag_MCTARGETDESC_magMCTARGETDESC_H
#define LLVM_LIB_TARGET_mag_MCTARGETDESC_magMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class Target;
class Triple;

extern Target ThemagTarget;

} // End llvm namespace

// Defines symbolic names for mag registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "magGenRegisterInfo.inc"

// Defines symbolic names for the mag instructions.
#define GET_INSTRINFO_ENUM
#include "magGenInstrInfo.inc"

#endif
