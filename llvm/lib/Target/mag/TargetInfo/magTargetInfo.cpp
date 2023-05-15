#include "mag.h"
#include "llvm/IR/Module.h"
#include "TargetInfo/magTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getThemagTarget() {
  static Target ThemagTarget;
  return ThemagTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializemagTargetInfo() {
  RegisterTarget<Triple::mag, false> X(getThemagTarget(), "mag", "mag (32-bit magulator arch)", "sim");
}
