#include "magMCTargetDesc.h"
#include "TargetInfo/magTargetInfo.h"
#include "magInfo.h"
#include "magInstPrinter.h"
#include "magELFStreamer.h"
#include "magMCObjectFIleInfo.h"
#include "magMCAsmInfo.h"
#include "magTargetStreamer.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "magGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "magGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "magGenSubtargetInfo.inc"

static MCInstrInfo *createmagMCInstrInfo() {
  auto *X = new MCInstrInfo();
  InitmagMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createmagMCRegisterInfo(const Triple &TT) {
  auto *X = new MCRegisterInfo();
  InitmagMCRegisterInfo(X, mag::X1);
  return X;
}

static MCSubtargetInfo *createmagMCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
  return createmagMCSubtargetInfoImpl(TT, CPU, /*TuneCPU=*/CPU, FS);
}

static MCAsmInfo *createmagMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new magMCAsmInfo(TT);
  MCRegister SP = MRI.getDwarfRegNum(mag::X2, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);
  return MAI;
}

static MCInstPrinter *createmagMCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new magInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *createmagTargetAsmStreamer(MCStreamer &S,
                                                 formatted_raw_ostream &OS,
                                                 MCInstPrinter *InstPrint,
                                                 bool isVerboseAsm) {
  return new magTargetStreamer(S);
}

static MCObjectFileInfo *
createmagMCObjectFileInfo(MCContext &Ctx, bool PIC,
                            bool LargeCodeModel = false) {
  MCObjectFileInfo *MOFI = new magMCObjectFileInfo();
  MOFI->initMCObjectFileInfo(Ctx, PIC, LargeCodeModel);
  return MOFI;
}

static MCTargetStreamer *
createmagObjectTargetStreamer(MCStreamer &S, const MCSubtargetInfo &STI) {
  const Triple &TT = STI.getTargetTriple();
  if (TT.isOSBinFormatELF())
    return new magTargetELFStreamer(S, STI);
  return nullptr;
}

class magMCInstrAnalysis : public MCInstrAnalysis {
public:
  explicit magMCInstrAnalysis(const MCInstrInfo *Info)
      : MCInstrAnalysis(Info) {}

  bool evaluateBranch(const MCInst &Inst, uint64_t Addr, uint64_t Size,
                      uint64_t &Target) const override {
    if (isConditionalBranch(Inst)) {
      int64_t Imm;
      if (Size == 2)
        Imm = Inst.getOperand(1).getImm();
      else
        Imm = Inst.getOperand(2).getImm();
      Target = Addr + Imm;
      return true;
    }

    if (Inst.getOpcode() == mag::JAL) {
      Target = Addr + Inst.getOperand(1).getImm();
      return true;
    }

    return false;
  }
};

static MCInstrAnalysis *createmagInstrAnalysis(const MCInstrInfo *Info) {
  return new magMCInstrAnalysis(Info);
}

static MCTargetStreamer *createmagNullTargetStreamer(MCStreamer &S) {
  return new magTargetStreamer(S);
}

namespace {
MCStreamer *createmagELFStreamer(const Triple &T, MCContext &Context,
                                   std::unique_ptr<MCAsmBackend> &&MAB,
                                   std::unique_ptr<MCObjectWriter> &&MOW,
                                   std::unique_ptr<MCCodeEmitter> &&MCE,
                                   bool RelaxAll) {
  return createmagELFStreamer(Context, std::move(MAB), std::move(MOW),
                                std::move(MCE), RelaxAll);
}
} // end anonymous namespace

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializemagTargetMC() {
  // Register the MC asm info.
  Target &ThemagTarget = getThemagTarget();
  RegisterMCAsmInfoFn X(ThemagTarget, createmagMCAsmInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCObjectFileInfo(ThemagTarget, createmagMCObjectFileInfo);
  TargetRegistry::RegisterMCInstrInfo(ThemagTarget, createmagMCInstrInfo);
  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(ThemagTarget, createmagMCRegisterInfo);

  TargetRegistry::RegisterMCAsmBackend(ThemagTarget, createmagAsmBackend);
  TargetRegistry::RegisterMCCodeEmitter(ThemagTarget, createmagMCCodeEmitter);
  TargetRegistry::RegisterMCInstPrinter(ThemagTarget, createmagMCInstPrinter);
  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(ThemagTarget,
                                          createmagMCSubtargetInfo);
  TargetRegistry::RegisterELFStreamer(ThemagTarget, createmagELFStreamer);
  TargetRegistry::RegisterObjectTargetStreamer(
        ThemagTarget, createmagObjectTargetStreamer);
  TargetRegistry::RegisterMCInstrAnalysis(ThemagTarget, createmagInstrAnalysis);
  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(ThemagTarget, createmagMCInstPrinter);

  TargetRegistry::RegisterAsmTargetStreamer(ThemagTarget,
                                            createmagTargetAsmStreamer);

  TargetRegistry::RegisterNullTargetStreamer(ThemagTarget,
                                               createmagNullTargetStreamer);                   
}
