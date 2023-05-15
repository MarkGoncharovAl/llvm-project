add_llvm_component_library(LLVMmagInfo
  magTargetInfo.cpp

  LINK_COMPONENTS
  Support

  ADD_TO_COMPONENT
  mag
  )
