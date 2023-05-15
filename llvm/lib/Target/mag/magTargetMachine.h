#ifndef __LLVM_LIB_TARGET_MAG_SIMTARGETMACHINE_H__
#define __LLVM_LIB_TARGET_MAG_SIMTARGETMACHINE_H__

#include "magInstrInfo.h"
#include "magSubtarget.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class magTargetMachine : public LLVMTargetMachine {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  magSubtarget Subtarget;
  // mutable StringMap<std::unique_ptr<magSubtarget>> SubtargetMap;

public:
  magTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                    StringRef FS, const TargetOptions &Options,
                    Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                    CodeGenOpt::Level OL, bool JIT);
  ~magTargetMachine() override;

  const magSubtarget *getSubtargetImpl() const { return &Subtarget; }
  const magSubtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

#if 0
  bool
  addPassesToEmitFile(PassManagerBase &, raw_pwrite_stream &,
                      raw_pwrite_stream *, CodeGenFileType,
                      bool /*DisableVerify*/ = true,
                      MachineModuleInfoWrapperPass *MMIWP = nullptr) override {
    return false;
  }
#endif
  // TargetTransformInfo getTargetTransformInfo(const Function &F) override;
};
} // end namespace llvm

#endif // __LLVM_LIB_TARGET_MAG_SIMTARGETMACHINE_H__
