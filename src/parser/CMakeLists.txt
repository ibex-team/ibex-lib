# see arithmetic/CMakeLists.txt for comments

list (APPEND IBEX_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_CtrGenerator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_CtrGenerator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_SysGenerator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_SysGenerator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Expr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Expr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprGenerator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprGenerator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprPrinter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprPrinter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_ExprVisitor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_NumConstraint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_NumConstraint.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Source.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Source.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Scope.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Scope.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Struct.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_P_Struct.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SyntaxError.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SyntaxError.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_UnknownFileException.h
  ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l
  ${CMAKE_CURRENT_SOURCE_DIR}/parser.yc
  )

list (APPEND IBEX_INCDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Propagate new values of list to parent scope
set (IBEX_SRC ${IBEX_SRC} PARENT_SCOPE)
set (IBEX_INCDIRS ${IBEX_INCDIRS} PARENT_SCOPE)
