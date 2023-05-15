#include "magInfo.h"

#define GET_REGINFO_ENUM
#include "magGenRegisterInfo.inc"

namespace llvm {
namespace magABI {
MCRegister getBPReg() { return mag::X9; }
MCRegister getSCSPReg() { return mag::X18; }
} // namespace magABI
} // namespace llvm
