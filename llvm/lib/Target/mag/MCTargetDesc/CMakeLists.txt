add_llvm_component_library(LLVMmagDesc
  magInstPrinter.cpp
  magMCAsmInfo.cpp
  magMCTargetDesc.cpp
  magInfo.cpp
  magAsmBackend.cpp
  magMCCodeEmitter.cpp
  magMCExpr.cpp
  magELFObjectWriter.cpp
  magTargetStreamer.cpp
  magELFStreamer.cpp
  magMCObjectFIleInfo.cpp

  LINK_COMPONENTS
  MC
  magInfo
  Support

  ADD_TO_COMPONENT
  mag
  )
