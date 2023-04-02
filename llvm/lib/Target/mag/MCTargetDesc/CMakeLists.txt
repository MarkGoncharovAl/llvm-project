add_llvm_component_library(LLVMmagDesc
  magInstPrinter.cpp
  magMCAsmInfo.cpp
  magMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  magInfo
  Support

  ADD_TO_COMPONENT
  mag
  )
