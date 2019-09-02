# see arithmetic/CMakeLists.txt for comments

list (APPEND IBEX_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Bisection.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BisectionPoint.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Bsc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Bsc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LargestFirst.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LargestFirst.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_NoBisectableVariableException.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_RoundRobin.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_RoundRobin.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SmearFunction.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SmearFunction.h
  )

list (APPEND IBEX_INCDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Propagate new values of list to parent scope
set (IBEX_SRC ${IBEX_SRC} PARENT_SCOPE)
set (IBEX_INCDIRS ${IBEX_INCDIRS} PARENT_SCOPE)
