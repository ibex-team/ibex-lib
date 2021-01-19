# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BoolInterval.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Pdc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcAnd.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcAnd.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcCleared.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcCleared.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcDiameterLT.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcDiameterLT.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcFirstOrder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcFirstOrder.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcFwdBwd.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcFwdBwd.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcHansenFeasibility.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcHansenFeasibility.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcImageSubset.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcImageSubset.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcNo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcNo.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcNot.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcNot.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcOr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcOr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcYes.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_PdcYes.h
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
