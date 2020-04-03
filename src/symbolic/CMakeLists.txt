# see arithmetic/CMakeLists.txt for comments

configure_file (ibex_ExprOperators.cpp.in ibex_ExprOperators.cpp)
configure_file (ibex_ExprOperators.h.in ibex_ExprOperators.h)

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CmpOp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CmpOp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2DAG.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2DAG.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2Minibex.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2Minibex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCmp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCmp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCopy.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCopy.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCtr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprDiff.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprDiff.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprLinearity.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprLinearity.h
  ${CMAKE_CURRENT_BINARY_DIR}/ibex_ExprOperators.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ibex_ExprOperators.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprPrinter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprPrinter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSimplify.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSimplify.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSize.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSize.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSubNodes.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSubNodes.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprVisitor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InputNodeMap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_InputNodeMap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_NodeMap.h
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
