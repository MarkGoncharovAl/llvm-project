//===-- magTargetStreamer.cpp - mag Target Streamer Methods -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides mag specific target streamer methods.
//
//===----------------------------------------------------------------------===//

#include "magTargetStreamer.h"
#include "magInfo.h"
#include "magMCTargetDesc.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/magAttributes.h"
#include "llvm/Support/magISAInfo.h"

using namespace llvm;

magTargetStreamer::magTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}

void magTargetStreamer::finish() { finishAttributeSection(); }

void magTargetStreamer::emitDirectiveOptionPush() {}
void magTargetStreamer::emitDirectiveOptionPop() {}
void magTargetStreamer::emitDirectiveOptionPIC() {}
void magTargetStreamer::emitDirectiveOptionNoPIC() {}
void magTargetStreamer::emitDirectiveOptionRVC() {}
void magTargetStreamer::emitDirectiveOptionNoRVC() {}
void magTargetStreamer::emitDirectiveOptionRelax() {}
void magTargetStreamer::emitDirectiveOptionNoRelax() {}
void magTargetStreamer::emitAttribute(unsigned Attribute, unsigned Value) {}
void magTargetStreamer::finishAttributeSection() {}
void magTargetStreamer::emitTextAttribute(unsigned Attribute,
                                            StringRef String) {}
void magTargetStreamer::emitIntTextAttribute(unsigned Attribute,
                                               unsigned IntValue,
                                               StringRef StringValue) {}

void magTargetStreamer::emitTargetAttributes(const MCSubtargetInfo &STI) {
//   if (STI.hasFeature(mag::FeatureRV32E))
//     emitAttribute(magAttrs::STACK_ALIGN, magAttrs::ALIGN_4);
//   else
    emitAttribute(magAttrs::STACK_ALIGN, magAttrs::ALIGN_16);

  unsigned XLen = 32;
  std::vector<std::string> FeatureVector;
  magFeatures::toFeatureVector(FeatureVector, STI.getFeatureBits());

  auto ParseResult = llvm::magISAInfo::parseFeatures(XLen, FeatureVector);
  if (!ParseResult) {
    /* Assume any error about features should handled earlier.  */
    consumeError(ParseResult.takeError());
    llvm_unreachable("Parsing feature error when emitTargetAttributes?");
  } else {
    auto &ISAInfo = *ParseResult;
    emitTextAttribute(magAttrs::ARCH, ISAInfo->toString());
  }
}

// This part is for ascii assembly output
magTargetAsmStreamer::magTargetAsmStreamer(MCStreamer &S,
                                               formatted_raw_ostream &OS)
    : magTargetStreamer(S), OS(OS) {}

void magTargetAsmStreamer::emitDirectiveOptionPush() {
  OS << "\t.option\tpush\n";
}

void magTargetAsmStreamer::emitDirectiveOptionPop() {
  OS << "\t.option\tpop\n";
}

void magTargetAsmStreamer::emitDirectiveOptionPIC() {
  OS << "\t.option\tpic\n";
}

void magTargetAsmStreamer::emitDirectiveOptionNoPIC() {
  OS << "\t.option\tnopic\n";
}

void magTargetAsmStreamer::emitDirectiveOptionRVC() {
  OS << "\t.option\trvc\n";
}

void magTargetAsmStreamer::emitDirectiveOptionNoRVC() {
  OS << "\t.option\tnorvc\n";
}

void magTargetAsmStreamer::emitDirectiveOptionRelax() {
  OS << "\t.option\trelax\n";
}

void magTargetAsmStreamer::emitDirectiveOptionNoRelax() {
  OS << "\t.option\tnorelax\n";
}

void magTargetAsmStreamer::emitAttribute(unsigned Attribute, unsigned Value) {
  OS << "\t.attribute\t" << Attribute << ", " << Twine(Value) << "\n";
}

void magTargetAsmStreamer::emitTextAttribute(unsigned Attribute,
                                               StringRef String) {
  OS << "\t.attribute\t" << Attribute << ", \"" << String << "\"\n";
}

void magTargetAsmStreamer::emitIntTextAttribute(unsigned Attribute,
                                                  unsigned IntValue,
                                                  StringRef StringValue) {}

void magTargetAsmStreamer::finishAttributeSection() {}
