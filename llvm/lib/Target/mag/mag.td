//===----------------------------------------------------------------------===//
// This is the top level entry point for the mag target.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// mag Subtarget features
//===----------------------------------------------------------------------===//

//def FeatureDummy
//    : SubtargetFeature<"dummy", "Dummy", "true",
//                       "Dummy feature">;

//===----------------------------------------------------------------------===//
// Target-dependent interfaces
//===----------------------------------------------------------------------===//

include "magRegisterInfo.td"

//===----------------------------------------------------------------------===//
// Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "magCallingConv.td"

include "magInstrInfo.td"
def magInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// mag processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;
//def : Proc<"dummy-st", []>;

// Will generate magGenAsmWrite.inc included by magInstPrinter.cpp, contents
//  as follows,
// void magInstPrinter::printInstruction(const MCInst *MI, raw_ostream &O) {...}
// const char *magInstPrinter::getRegisterName(unsigned RegNo) {...}
def mag : Target {
  let InstructionSet = magInstrInfo;
}
