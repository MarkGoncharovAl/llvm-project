#include "simMCTargetDesc.h"
#include "TargetInfo/simTargetInfo.h"
#include "simInfo.h"
#include "simInstPrinter.h"
#include "simMCAsmInfo.h"
#include "simTargetStreamer.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "simGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "simGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "simGenSubtargetInfo.inc"

static MCInstrInfo *createsimMCInstrInfo() {
  auto *X = new MCInstrInfo();
  InitsimMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createsimMCRegisterInfo(const Triple &TT) {
  auto *X = new MCRegisterInfo();
  InitsimMCRegisterInfo(X, sim::X1);
  return X;
}

static MCSubtargetInfo *createsimMCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
  return createsimMCSubtargetInfoImpl(TT, CPU, /*TuneCPU=*/CPU, FS);
}

static MCAsmInfo *createsimMCAsmInfo(const MCRegisterInfo &MRI,
                                      const Triple &TT,
                                      const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new simMCAsmInfo(TT);
  MCRegister SP = MRI.getDwarfRegNum(sim::X2, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);
  return MAI;
}

static MCInstPrinter *createsimMCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new simInstPrinter(MAI, MII, MRI);
}

simTargetStreamer::simTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}
simTargetStreamer::~simTargetStreamer() = default;

static MCTargetStreamer *createTargetAsmStreamer(MCStreamer &S,
                                                 formatted_raw_ostream &OS,
                                                 MCInstPrinter *InstPrint,
                                                 bool isVerboseAsm) {
  return new simTargetStreamer(S);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializesimTargetMC() {
  // Register the MC asm info.
  Target &ThesimTarget = getThesimTarget();
  RegisterMCAsmInfoFn X(ThesimTarget, createsimMCAsmInfo);

  // Register the MC instruction info.
  TargetRegistry::RegisterMCInstrInfo(ThesimTarget, createsimMCInstrInfo);

  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(ThesimTarget, createsimMCRegisterInfo);

  // Register the MC subtarget info.
  TargetRegistry::RegisterMCSubtargetInfo(ThesimTarget,
                                          createsimMCSubtargetInfo);

  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(ThesimTarget, createsimMCInstPrinter);

  TargetRegistry::RegisterAsmTargetStreamer(ThesimTarget,
                                            createTargetAsmStreamer);
}
