//===----------------------------------------------------------------------===//
//
// Implements the info about mag target spec.
//
//===----------------------------------------------------------------------===//

#include "magTargetMachine.h"
#include "mag.h"
//#include "magTargetTransformInfo.h"
#include "TargetInfo/magTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetOptions.h"

#define DEBUG_TYPE "mag"

using namespace llvm;

static Reloc::Model getRelocModel(Optional<Reloc::Model> RM) {
  return RM.getValueOr(Reloc::Static);
}

/// magTargetMachine ctor - Create an ILP32 Architecture model
magTargetMachine::magTargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     Optional<Reloc::Model> RM,
                                     Optional<CodeModel::Model> CM,
                                     CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T,
                        "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-"
                        "f32:32:32-i64:32-f64:32-a:0:32-n32",
                        TT, CPU, FS, Options, getRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  initAsmInfo();
}

magTargetMachine::~magTargetMachine() = default;

namespace {

/// mag Code Generator Pass Configuration Options.
class magPassConfig : public TargetPassConfig {
public:
  magPassConfig(magTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  magTargetMachine &getmagTargetMachine() const {
    return getTM<magTargetMachine>();
  }

  bool addInstSelector() override;
  // void addPreEmitPass() override;
  // void addPreRegAlloc() override;
};

} // end anonymous namespace

TargetPassConfig *magTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new magPassConfig(*this, PM);
}

bool magPassConfig::addInstSelector() {
  addPass(createmagISelDag(getmagTargetMachine(), getOptLevel()));
  return false;
}

// void magPassConfig::addPreEmitPass() { llvm_unreachable(""); }

// void magPassConfig::addPreRegAlloc() { llvm_unreachable(""); }

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializemagTarget() {
  RegisterTargetMachine<magTargetMachine> X(getThemagTarget());
}

#if 0
TargetTransformInfo
magTargetMachine::getTargetTransformInfo(const Function &F) {
  return TargetTransformInfo(magTTIImpl(this, F));
}
#endif