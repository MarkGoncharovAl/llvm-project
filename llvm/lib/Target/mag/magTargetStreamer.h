#ifndef LLVM_LIB_TARGET_mag_magTARGETSTREAMER_H
#define LLVM_LIB_TARGET_mag_magTARGETSTREAMER_H

#include "llvm/MC/MCStreamer.h"

namespace llvm {

class magTargetStreamer : public MCTargetStreamer {
public:
  magTargetStreamer(MCStreamer &S);
  ~magTargetStreamer() override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_mag_magTARGETSTREAMER_H