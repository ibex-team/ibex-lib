# This files is written to be used via 'add_subdirectory' not 'include'
# It should set the following variables (with PARENT_SCOPE):
#   - IBEX_SUBDIR_SOURCES for source files  
#   - IBEX_SUBDIR_HEADERS for header files
#   - IBEX_SUBDIR_INCDIRS for include directories
# Variables should be set with PARENT_SCOPE in order for the parent script to
# use them.
# Following advice from
# https://cmake.org/cmake/help/v2.8.12/cmake.html#command:file, the command
# FILE (GLOB ...) should not be used to set these variables.
# Paths to files should be absolute.


set (IBEX_SUBDIR_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Dim.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_DoubleIndex.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InnerArith.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Interval.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_IntervalMatrix.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_IntervalVector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Matrix.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetMembership.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Vector.cpp
  PARENT_SCOPE)

set (IBEX_SUBDIR_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Dim.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Domain.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_DoubleIndex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InnerArith.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Interval.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_IntervalMatrix.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_IntervalVector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InvalidIntervalVectorOp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearArith.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Matrix.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_TemplateDomain.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_TemplateMatrix.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_TemplateVector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Vector.h
  PARENT_SCOPE)

set (IBEX_SUBDIR_INCDIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  PARENT_SCOPE)
