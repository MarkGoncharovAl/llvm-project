//===-- magMCCodeEmitter.cpp - Convert mag code to machine code -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the magMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/magInfo.h"
#include "MCTargetDesc/magFixupKinds.h"
#include "MCTargetDesc/magMCExpr.h"
#include "MCTargetDesc/magMCTargetDesc.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");
STATISTIC(MCNumFixups, "Number of MC fixups created");

namespace {
class magMCCodeEmitter : public MCCodeEmitter {
  magMCCodeEmitter(const magMCCodeEmitter &) = delete;
  void operator=(const magMCCodeEmitter &) = delete;
  MCContext &Ctx;
  MCInstrInfo const &MCII;

public:
  magMCCodeEmitter(MCContext &ctx, MCInstrInfo const &MCII)
      : Ctx(ctx), MCII(MCII) {}

  ~magMCCodeEmitter() override {}

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  void expandFunctionCall(const MCInst &MI, raw_ostream &OS,
                          SmallVectorImpl<MCFixup> &Fixups,
                          const MCSubtargetInfo &STI) const;

  void expandAddTPRel(const MCInst &MI, raw_ostream &OS,
                      SmallVectorImpl<MCFixup> &Fixups,
                      const MCSubtargetInfo &STI) const;

  /// TableGen'erated function for getting the binary encoding for an
  /// instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// Return binary encoding of operand. If the machine operand requires
  /// relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getImmOpValueAsr1(const MCInst &MI, unsigned OpNo,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getImmOpValue(const MCInst &MI, unsigned OpNo,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const;

  unsigned getVMaskReg(const MCInst &MI, unsigned OpNo,
                       SmallVectorImpl<MCFixup> &Fixups,
                       const MCSubtargetInfo &STI) const;

private:
  FeatureBitset computeAvailableFeatures(const FeatureBitset &FB) const;
  void
  verifyInstructionPredicates(const MCInst &MI,
                              const FeatureBitset &AvailableFeatures) const;
};
} // end anonymous namespace

MCCodeEmitter *llvm::createmagMCCodeEmitter(const MCInstrInfo &MCII,
                                              const MCRegisterInfo &MRI,
                                              MCContext &Ctx) {
  return new magMCCodeEmitter(Ctx, MCII);
}

// Expand PseudoCALL(Reg), PseudoTAIL and PseudoJump to AUIPC and JALR with
// relocation types. We expand those pseudo-instructions while encoding them,
// meaning AUIPC and JALR won't go through mag MC to MC compressed
// instruction transformation. This is acceptable because AUIPC has no 16-bit
// form and C_JALR has no immediate operand field.  We let linker relaxation
// deal with it. When linker relaxation is enabled, AUIPC and JALR have a
// chance to relax to JAL.
// If the C extension is enabled, JAL has a chance relax to C_JAL.
void magMCCodeEmitter::expandFunctionCall(const MCInst &MI, raw_ostream &OS,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  MCInst TmpInst;
  MCOperand Func;
  MCRegister Ra;
//   if (MI.getOpcode() == mag::PseudoTAIL) {
//     Func = MI.getOperand(0);
//     Ra = mag::X6;
//   } else if (MI.getOpcode() == mag::PseudoCALLReg) {
//     Func = MI.getOperand(1);
//     Ra = MI.getOperand(0).getReg();
//   } else
  if (MI.getOpcode() == mag::PseudoCALL) {
    Func = MI.getOperand(0);
    Ra = mag::X1;
//   } else if (MI.getOpcode() == mag::PseudoJump) {
//     Func = MI.getOperand(1);
//     Ra = MI.getOperand(0).getReg();
//   }
  }
  uint32_t Binary;

  assert(Func.isExpr() && "Expected expression");

  const MCExpr *CallExpr = Func.getExpr();

  // Emit AUIPC Ra, Func with R_mag_CALL relocation type.
  TmpInst = MCInstBuilder(mag::AUIPC)
                .addReg(Ra)
                .addOperand(MCOperand::createExpr(CallExpr));
  Binary = getBinaryCodeForInstr(TmpInst, Fixups, STI);
  support::endian::write(OS, Binary, support::little);

//   if (MI.getOpcode() == mag::PseudoTAIL ||
//       MI.getOpcode() == mag::PseudoJump)
//     // Emit JALR X0, Ra, 0
//     TmpInst = MCInstBuilder(mag::JALR).addReg(mag::X0).addReg(Ra).addImm(0);
//   else
//     // Emit JALR Ra, Ra, 0
    TmpInst = MCInstBuilder(mag::JALR).addReg(Ra).addReg(Ra).addImm(0);
  Binary = getBinaryCodeForInstr(TmpInst, Fixups, STI);
  support::endian::write(OS, Binary, support::little);
}

// Expand PseudoAddTPRel to a magple ADD with the correct relocation.
void magMCCodeEmitter::expandAddTPRel(const MCInst &MI, raw_ostream &OS,
                                        SmallVectorImpl<MCFixup> &Fixups,
                                        const MCSubtargetInfo &STI) const {
  MCOperand DestReg = MI.getOperand(0);
  MCOperand SrcReg = MI.getOperand(1);
  MCOperand TPReg = MI.getOperand(2);
  assert(TPReg.isReg() && TPReg.getReg() == mag::X4 &&
         "Expected thread pointer as second input to TP-relative add");

  MCOperand SrcSymbol = MI.getOperand(3);
  assert(SrcSymbol.isExpr() &&
         "Expected expression as third input to TP-relative add");

  const magMCExpr *Expr = dyn_cast<magMCExpr>(SrcSymbol.getExpr());
  assert(Expr && Expr->getKind() == magMCExpr::VK_mag_TPREL_ADD &&
         "Expected tprel_add relocation on TP-relative symbol");

  // Emit the correct tprel_add relocation for the symbol.
  Fixups.push_back(MCFixup::create(
      0, Expr, MCFixupKind(mag::fixup_mag_tprel_add), MI.getLoc()));

  // Emit fixup_mag_relax for tprel_add where the relax feature is enabled.
//   if (STI.getFeatureBits()[mag::FeatureRelax]) {
//     const MCConstantExpr *Dummy = MCConstantExpr::create(0, Ctx);
//     Fixups.push_back(MCFixup::create(
//         0, Dummy, MCFixupKind(mag::fixup_mag_relax), MI.getLoc()));
//   }

  // Emit a normal ADD instruction with the given operands.
  MCInst TmpInst = MCInstBuilder(mag::ADD)
                       .addOperand(DestReg)
                       .addOperand(SrcReg)
                       .addOperand(TPReg);
  uint32_t Binary = getBinaryCodeForInstr(TmpInst, Fixups, STI);
  support::endian::write(OS, Binary, support::little);
}

void magMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  verifyInstructionPredicates(MI,
                              computeAvailableFeatures(STI.getFeatureBits()));

  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  // Get byte count of instruction.
  unsigned Size = Desc.getSize();

  // magInstrInfo::getInstSizeInBytes expects that the total size of the
  // expanded instructions for each pseudo is correct in the Size field of the
  // tablegen definition for the pseudo.
  if (MI.getOpcode() == mag::PseudoCALL) {
    expandFunctionCall(MI, OS, Fixups, STI);
    MCNumEmitted += 2;
    return;
  }

//   if (MI.getOpcode() == mag::PseudoAddTPRel) {
//     expandAddTPRel(MI, OS, Fixups, STI);
//     MCNumEmitted += 1;
//     return;
//   }

  switch (Size) {
  default:
    llvm_unreachable("Unhandled encodeInstruction length!");
  case 2: {
    uint16_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
    support::endian::write<uint16_t>(OS, Bits, support::little);
    break;
  }
  case 4: {
    uint32_t Bits = getBinaryCodeForInstr(MI, Fixups, STI);
    support::endian::write(OS, Bits, support::little);
    break;
  }
  }

  ++MCNumEmitted; // Keep track of the # of mi's emitted.
}

unsigned
magMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      const MCSubtargetInfo &STI) const {

  if (MO.isReg())
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());

  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());

  llvm_unreachable("Unhandled expression!");
  return 0;
}

unsigned
magMCCodeEmitter::getImmOpValueAsr1(const MCInst &MI, unsigned OpNo,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);

  if (MO.isImm()) {
    unsigned Res = MO.getImm();
    assert((Res & 1) == 0 && "LSB is non-zero");
    return Res >> 1;
  }

  return getImmOpValue(MI, OpNo, Fixups, STI);
}

unsigned magMCCodeEmitter::getImmOpValue(const MCInst &MI, unsigned OpNo,
                                           SmallVectorImpl<MCFixup> &Fixups,
                                           const MCSubtargetInfo &STI) const {
  bool EnableRelax = false; // STI.getFeatureBits()[mag::FeatureRelax];
  const MCOperand &MO = MI.getOperand(OpNo);

  MCInstrDesc const &Desc = MCII.get(MI.getOpcode());
  unsigned MIFrm = magII::getFormat(Desc.TSFlags);

  // If the destination is an immediate, there is nothing to do.
  if (MO.isImm())
    return MO.getImm();

  assert(MO.isExpr() &&
         "getImmOpValue expects only expressions or immediates");
  const MCExpr *Expr = MO.getExpr();
  MCExpr::ExprKind Kind = Expr->getKind();
  mag::Fixups FixupKind = mag::fixup_mag_invalid;
  bool RelaxCandidate = false;
  if (Kind == MCExpr::Target) {
    const magMCExpr *RVExpr = cast<magMCExpr>(Expr);

    switch (RVExpr->getKind()) {
    case magMCExpr::VK_mag_None:
    case magMCExpr::VK_mag_Invalid:
    case magMCExpr::VK_mag_32_PCREL:
      llvm_unreachable("Unhandled fixup kind!");
    case magMCExpr::VK_mag_TPREL_ADD:
      // tprel_add is only used to indicate that a relocation should be emitted
      // for an add instruction used in TP-relative addressing. It should not be
      // expanded as if representing an actual instruction operand and so to
      // encounter it here is an error.
      llvm_unreachable(
          "VK_mag_TPREL_ADD should not represent an instruction operand");
    case magMCExpr::VK_mag_LO:
      if (MIFrm == magII::InstFormatI)
        FixupKind = mag::fixup_mag_lo12_i;
      else if (MIFrm == magII::InstFormatS)
        FixupKind = mag::fixup_mag_lo12_s;
      else
        llvm_unreachable("VK_mag_LO used with unexpected instruction format");
      RelaxCandidate = true;
      break;
    case magMCExpr::VK_mag_HI:
      FixupKind = mag::fixup_mag_hi20;
      RelaxCandidate = true;
      break;
    case magMCExpr::VK_mag_PCREL_LO:
      if (MIFrm == magII::InstFormatI)
        FixupKind = mag::fixup_mag_pcrel_lo12_i;
      else if (MIFrm == magII::InstFormatS)
        FixupKind = mag::fixup_mag_pcrel_lo12_s;
      else
        llvm_unreachable(
            "VK_mag_PCREL_LO used with unexpected instruction format");
      RelaxCandidate = true;
      break;
    case magMCExpr::VK_mag_PCREL_HI:
      FixupKind = mag::fixup_mag_pcrel_hi20;
      RelaxCandidate = true;
      break;
    case magMCExpr::VK_mag_GOT_HI:
      FixupKind = mag::fixup_mag_got_hi20;
      break;
    case magMCExpr::VK_mag_TPREL_LO:
      if (MIFrm == magII::InstFormatI)
        FixupKind = mag::fixup_mag_tprel_lo12_i;
      else if (MIFrm == magII::InstFormatS)
        FixupKind = mag::fixup_mag_tprel_lo12_s;
      else
        llvm_unreachable(
            "VK_mag_TPREL_LO used with unexpected instruction format");
      RelaxCandidate = true;
      break;
    case magMCExpr::VK_mag_TPREL_HI:
      FixupKind = mag::fixup_mag_tprel_hi20;
      RelaxCandidate = true;
      break;
    case magMCExpr::VK_mag_TLS_GOT_HI:
      FixupKind = mag::fixup_mag_tls_got_hi20;
      break;
    case magMCExpr::VK_mag_TLS_GD_HI:
      FixupKind = mag::fixup_mag_tls_gd_hi20;
      break;
    case magMCExpr::VK_mag_CALL:
      FixupKind = mag::fixup_mag_call;
      RelaxCandidate = true;
      break;
    case magMCExpr::VK_mag_CALL_PLT:
      FixupKind = mag::fixup_mag_call_plt;
      RelaxCandidate = true;
      break;
    }
  } else if (Kind == MCExpr::SymbolRef &&
             cast<MCSymbolRefExpr>(Expr)->getKind() == MCSymbolRefExpr::VK_None) {
    if (MIFrm == magII::InstFormatJ) {
      FixupKind = mag::fixup_mag_jal;
    } else if (MIFrm == magII::InstFormatB) {
      FixupKind = mag::fixup_mag_branch;
    } else {
      llvm_unreachable("Unhandled fixup");
    } 
    // } else if (MIFrm == magII::InstFormatCJ) {
    //   FixupKind = mag::fixup_mag_rvc_jump;
    // } else if (MIFrm == magII::InstFormatCB) {
    //   FixupKind = mag::fixup_mag_rvc_branch;
    // }
  }

  assert(FixupKind != mag::fixup_mag_invalid && "Unhandled expression!");

  Fixups.push_back(
      MCFixup::create(0, Expr, MCFixupKind(FixupKind), MI.getLoc()));
  ++MCNumFixups;

  // Ensure an R_mag_RELAX relocation will be emitted if linker relaxation is
  // enabled and the current fixup will result in a relocation that may be
  // relaxed.
  if (EnableRelax && RelaxCandidate) {
    const MCConstantExpr *Dummy = MCConstantExpr::create(0, Ctx);
    Fixups.push_back(
    MCFixup::create(0, Dummy, MCFixupKind(mag::fixup_mag_relax),
                    MI.getLoc()));
    ++MCNumFixups;
  }

  return 0;
}

unsigned magMCCodeEmitter::getVMaskReg(const MCInst &MI, unsigned OpNo,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
    llvm_unreachable("Andrey && Dmitryy && Kirill && Vasilii were here");
}

#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "magGenMCCodeEmitter.inc"
