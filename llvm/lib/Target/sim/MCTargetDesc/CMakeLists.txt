add_llvm_component_library(LLVMsimDesc
  simInstPrinter.cpp
  simMCAsmInfo.cpp
  simMCTargetDesc.cpp
  simInfo.cpp

  LINK_COMPONENTS
  MC
  simInfo
  Support

  ADD_TO_COMPONENT
  sim
  )
