# see arithmetic/CMakeLists.txt for comments

set (IBEX_SUBDIR_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Bsc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LargestFirst.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_RoundRobin.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SmearFunction.cpp
  PARENT_SCOPE)

set (IBEX_SUBDIR_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Bisection.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BisectionPoint.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Bsc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LargestFirst.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_NoBisectableVariableException.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_RoundRobin.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SmearFunction.h
  PARENT_SCOPE)

set (IBEX_SUBDIR_INCDIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  PARENT_SCOPE)
