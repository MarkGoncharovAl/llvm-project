#include "magMCTargetDesc.h"
#include "TargetInfo/magTargetInfo.h"
#include "magInfo.h"
#include "magInstPrinter.h"
#include "magMCAsmInfo.h"
#include "magTargetStreamer.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "magGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "magGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "magGenRegisterInfo.inc"

static MCInstrInfo *createmagMCInstrInfo() {
  auto *X = new MCInstrInfo();
  InitmagMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createmagMCRegisterInfo(const Triple &TT) {
  auto *X = new MCRegisterInfo();
  InitmagMCRegisterInfo(X, mag::R1);
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
  MCRegister SP = MRI.getDwarfRegNum(mag::R2, true);
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

magTargetStreamer::magTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}
magTargetStreamer::~magTargetStreamer() = default;

static MCTargetStreamer *createTargetAsmStreamer(MCStreamer &S,
                                                 formatted_raw_ostream &OS,
                                                 MCInstPrinter *InstPrint,
                                                 bool isVerboseAsm) {
  return new magTargetStreamer(S);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializemagTargetMC() {
  // Register the MC asm info.
  Target &ThemagTarget = getThemagTarget();
  RegisterMCAsmInfoFn X(ThemagTarget, createmagMCAsmInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(ThemagTarget, createmagMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(ThemagTarget, createmagMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(ThemagTarget,
                                          createmagMCSubtargetInfo);

  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(ThemagTarget, createmagMCInstPrinter);

  TargetRegistry::RegisterAsmTargetStreamer(ThemagTarget,
                                            createTargetAsmStreamer);
}
