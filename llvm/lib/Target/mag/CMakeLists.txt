add_llvm_component_group(mag)

set(LLVM_TARGET_DEFINITIONS mag.td)

# Generate magGenRegisterInfo.inc and magGenInstrInfo.inc which included by
#  your hand code C++ files.
# magGenRegisterInfo.inc came from magRegisterInfo.td, magGenInstrInfo.inc
#  came from magInstrInfo.td.
tablegen(LLVM magGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM magGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM magGenCallingConv.inc -gen-callingconv)
tablegen(LLVM magGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM magGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM magGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM magGenDAGISel.inc -gen-dag-isel)

#tablegen(LLVM magGenDisassemblerTables.inc -gen-disassembler)

# magCommonTableGen must be defined
add_public_tablegen_target(magCommonTableGen)

# magCodeGen should match with LLVMBuild.txt magCodeGen
add_llvm_target(magCodeGen
  magAsmPrinter.cpp
  magFrameLowering.cpp
  magInstrInfo.cpp
  magISelDagToDag.cpp
  magISelLowering.cpp
  magMachineFunctionInfo.cpp
  magMCInstLower.cpp
  magRegisterInfo.cpp
  magSubtarget.cpp
  magTargetMachine.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  MC
  magDesc
  magInfo
  SelectionDAG
  Support
  Target
  GlobalISel

  ADD_TO_COMPONENT
  mag
)

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
