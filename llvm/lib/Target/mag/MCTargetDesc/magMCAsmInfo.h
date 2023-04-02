#ifndef LLVM_LIB_TARGET_mag_MCTARGETDESC_magMCASMINFO_H
#define LLVM_LIB_TARGET_mag_MCTARGETDESC_magMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class magMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit magMCAsmInfo(const Triple &TT);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_mag_MCTARGETDESC_magMCASMINFO_H
