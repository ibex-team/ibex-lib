# This file is written to be used via 'add_subdirectory' not 'include'
# Following advice from https://cmake.org/cmake/help/v3.0/command/file.html, the
# command FILE (GLOB ...) should not be used to set these variables.
# Paths to files should be absolute.

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Dim.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Dim.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Domain.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_DoubleIndex.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_DoubleIndex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InnerArith.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InnerArith.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Interval.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Interval.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_IntervalMatrix.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_IntervalMatrix.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_IntervalVector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_IntervalVector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InvalidIntervalVectorOp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearArith.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Matrix.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Matrix.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetMembership.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_TemplateDomain.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_TemplateMatrix.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_TemplateVector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Vector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Vector.h
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
