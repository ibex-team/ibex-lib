# see arithmetic/CMakeLists.txt for comments

set (IBEX_SUBDIR_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtrGenerator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_MainGenerator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Expr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprGenerator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprPrinter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_NumConstraint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Source.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Scope.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SyntaxError.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp
  PARENT_SCOPE)

set (IBEX_SUBDIR_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CtrGenerator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_MainGenerator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Expr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprGenerator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprPrinter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprVisitor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_NumConstraint.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Source.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Scope.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SyntaxError.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_UnknownFileException.h
  ${CMAKE_CURRENT_SOURCE_DIR}/parser.h
  PARENT_SCOPE)

set (IBEX_SUBDIR_INCDIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  PARENT_SCOPE)
