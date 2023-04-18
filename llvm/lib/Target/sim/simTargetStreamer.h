#ifndef __LLVM_LIB_TARGET_SIM_SIMTARGETSTREAMER_H__
#define __LLVM_LIB_TARGET_SIM_SIMTARGETSTREAMER_H__

#include "llvm/MC/MCStreamer.h"

namespace llvm {

class simTargetStreamer : public MCTargetStreamer {
public:
  simTargetStreamer(MCStreamer &S);
  ~simTargetStreamer() override;
};

} // end namespace llvm

#endif // __LLVM_LIB_TARGET_SIM_SIMTARGETSTREAMER_H__
