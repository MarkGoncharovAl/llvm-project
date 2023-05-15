//===-- magELFObjectWriter.cpp - mag ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/magFixupKinds.h"
#include "MCTargetDesc/magMCExpr.h"
#include "MCTargetDesc/magMCTargetDesc.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class magELFObjectWriter : public MCELFObjectTargetWriter {
public:
  magELFObjectWriter(uint8_t OSABI, bool Is64Bit);

  ~magELFObjectWriter() override;

  // Return true if the given relocation must be with a symbol rather than
  // section plus offset.
  bool needsRelocateWithSymbol(const MCSymbol &Sym,
                               unsigned Type) const override {
    // TODO: this is very conservative, update once RISC-V psABI requirements
    //       are clarified.
    return true;
  }

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
};
}

magELFObjectWriter::magELFObjectWriter(uint8_t OSABI, bool Is64Bit)
    : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_mag,
                              /*HasRelocationAddend*/ true) {}

magELFObjectWriter::~magELFObjectWriter() {}

unsigned magELFObjectWriter::getRelocType(MCContext &Ctx,
                                            const MCValue &Target,
                                            const MCFixup &Fixup,
                                            bool IsPCRel) const {
  const MCExpr *Expr = Fixup.getValue();
  // Determine the type of the relocation
  unsigned Kind = Fixup.getTargetKind();
  if (Kind >= FirstLiteralRelocationKind)
    return Kind - FirstLiteralRelocationKind;
  if (IsPCRel) {
    switch (Kind) {
    default:
      Ctx.reportError(Fixup.getLoc(), "Unsupported relocation type");
      return ELF::R_mag_NONE;
    case FK_Data_4:
    case FK_PCRel_4:
      return ELF::R_mag_32_PCREL;
    case mag::fixup_mag_pcrel_hi20:
      return ELF::R_mag_PCREL_HI20;
    case mag::fixup_mag_pcrel_lo12_i:
      return ELF::R_mag_PCREL_LO12_I;
    case mag::fixup_mag_pcrel_lo12_s:
      return ELF::R_mag_PCREL_LO12_S;
    case mag::fixup_mag_got_hi20:
      return ELF::R_mag_GOT_HI20;
    case mag::fixup_mag_tls_got_hi20:
      return ELF::R_mag_TLS_GOT_HI20;
    case mag::fixup_mag_tls_gd_hi20:
      return ELF::R_mag_TLS_GD_HI20;
    case mag::fixup_mag_jal:
      return ELF::R_mag_JAL;
    case mag::fixup_mag_branch:
      return ELF::R_mag_BRANCH;
    case mag::fixup_mag_rvc_jump:
      return ELF::R_mag_RVC_JUMP;
    case mag::fixup_mag_rvc_branch:
      return ELF::R_mag_RVC_BRANCH;
    case mag::fixup_mag_call:
      return ELF::R_mag_CALL;
    case mag::fixup_mag_call_plt:
      return ELF::R_mag_CALL_PLT;
    case mag::fixup_mag_add_8:
      return ELF::R_mag_ADD8;
    case mag::fixup_mag_sub_8:
      return ELF::R_mag_SUB8;
    case mag::fixup_mag_add_16:
      return ELF::R_mag_ADD16;
    case mag::fixup_mag_sub_16:
      return ELF::R_mag_SUB16;
    case mag::fixup_mag_add_32:
      return ELF::R_mag_ADD32;
    case mag::fixup_mag_sub_32:
      return ELF::R_mag_SUB32;
    case mag::fixup_mag_add_64:
      return ELF::R_mag_ADD64;
    case mag::fixup_mag_sub_64:
      return ELF::R_mag_SUB64;
    }
  }

  switch (Kind) {
  default:
    Ctx.reportError(Fixup.getLoc(), "Unsupported relocation type");
    return ELF::R_mag_NONE;
  case FK_Data_1:
    Ctx.reportError(Fixup.getLoc(), "1-byte data relocations not supported");
    return ELF::R_mag_NONE;
  case FK_Data_2:
    Ctx.reportError(Fixup.getLoc(), "2-byte data relocations not supported");
    return ELF::R_mag_NONE;
  case FK_Data_4:
    if (Expr->getKind() == MCExpr::Target &&
        cast<magMCExpr>(Expr)->getKind() == magMCExpr::VK_mag_32_PCREL)
      return ELF::R_mag_32_PCREL;
    return ELF::R_mag_32;
  case FK_Data_8:
    return ELF::R_mag_64;
  case mag::fixup_mag_hi20:
    return ELF::R_mag_HI20;
  case mag::fixup_mag_lo12_i:
    return ELF::R_mag_LO12_I;
  case mag::fixup_mag_lo12_s:
    return ELF::R_mag_LO12_S;
  case mag::fixup_mag_tprel_hi20:
    return ELF::R_mag_TPREL_HI20;
  case mag::fixup_mag_tprel_lo12_i:
    return ELF::R_mag_TPREL_LO12_I;
  case mag::fixup_mag_tprel_lo12_s:
    return ELF::R_mag_TPREL_LO12_S;
  case mag::fixup_mag_tprel_add:
    return ELF::R_mag_TPREL_ADD;
  case mag::fixup_mag_relax:
    return ELF::R_mag_RELAX;
  case mag::fixup_mag_align:
    return ELF::R_mag_ALIGN;
  case mag::fixup_mag_set_6b:
    return ELF::R_mag_SET6;
  case mag::fixup_mag_sub_6b:
    return ELF::R_mag_SUB6;
  case mag::fixup_mag_add_8:
    return ELF::R_mag_ADD8;
  case mag::fixup_mag_set_8:
    return ELF::R_mag_SET8;
  case mag::fixup_mag_sub_8:
    return ELF::R_mag_SUB8;
  case mag::fixup_mag_set_16:
    return ELF::R_mag_SET16;
  case mag::fixup_mag_add_16:
    return ELF::R_mag_ADD16;
  case mag::fixup_mag_sub_16:
    return ELF::R_mag_SUB16;
  case mag::fixup_mag_set_32:
    return ELF::R_mag_SET32;
  case mag::fixup_mag_add_32:
    return ELF::R_mag_ADD32;
  case mag::fixup_mag_sub_32:
    return ELF::R_mag_SUB32;
  case mag::fixup_mag_add_64:
    return ELF::R_mag_ADD64;
  case mag::fixup_mag_sub_64:
    return ELF::R_mag_SUB64;
  }
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createmagELFObjectWriter(uint8_t OSABI, bool Is64Bit) {
  return std::make_unique<magELFObjectWriter>(OSABI, Is64Bit);
}
