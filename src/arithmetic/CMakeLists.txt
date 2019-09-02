# This files is written to be used via 'add_subdirectory' not 'include'
# It should append to the following variables:
#   - IBEX_SRC for source files (.cpp, .h, ...)
#   - IBEX_INCDIRS for include directories
# The changed variables must be reset with PARENT_SCOPE to be available in the
# parent CMakeLists.txt
# Following advice from https://cmake.org/cmake/help/v3.0/command/file.html, the
# command FILE (GLOB ...) should not be used to set these variables.
# Paths to files should be absolute.


list (APPEND IBEX_SRC
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

list (APPEND IBEX_INCDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Propagate new values of list to parent scope
set (IBEX_SRC ${IBEX_SRC} PARENT_SCOPE)
set (IBEX_INCDIRS ${IBEX_INCDIRS} PARENT_SCOPE)
