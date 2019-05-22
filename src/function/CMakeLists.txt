# see arithmetic/CMakeLists.txt for comments

set (IBEX_SUBDIR_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CompiledFunction.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Eval.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Fnc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncProj.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Function.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FunctionBuild.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Gradient.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_HC4Revise.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InHC4Revise.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_NumConstraint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_VarSet.cpp
  PARENT_SCOPE)

set (IBEX_SUBDIR_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BwdAlgorithm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CompiledFunction.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Eval.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprData.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprDomain.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Fnc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncProj.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Function.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FwdAlgorithm.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Gradient.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_HC4Revise.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InHC4Revise.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_NumConstraint.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_VarSet.h
  PARENT_SCOPE)

set (IBEX_SUBDIR_INCDIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  PARENT_SCOPE)
