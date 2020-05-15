# see arithmetic/CMakeLists.txt for comments

################################################################################
# Deal with files in src/operators and set OPERATORS_* variables
################################################################################
set (OPERATORS_LIST atanhc atanhccc crossproduct sinc trace)

foreach (op ${OPERATORS_LIST})
  set (hdr "ibex_${op}.h")
  set (hdrfile "${CMAKE_CURRENT_SOURCE_DIR}/../operators/ibex_${op}.h")
  #set (cpp "ibex_${op}.cpp")
  set (cppfile "${CMAKE_CURRENT_SOURCE_DIR}/../operators/ibex_${op}.cpp")

  # Handles includes
  string (APPEND OPERATORS_INCLUDES "#include \"${hdr}\"\n")
  target_sources (ibex PRIVATE ${hdrfile} ${cppfile})

  # Parse header
  file (STRINGS ${hdrfile} line REGEX "^class [BU]i?naryOperator<" LIMIT_COUNT 1)
  if (NOT line)
    message (FATAL_ERROR "Error while parsing ${hdr}: no class line")
  endif ()
  string(REGEX MATCH " (.*)Operator<([^,]*),([^,]*),([^,]*),?(.*)>" _ ${line})

  if (CMAKE_MATCH_1 STREQUAL "Unary")
    set (_t "${CMAKE_MATCH_2},${CMAKE_MATCH_3},${CMAKE_MATCH_4}")
    string (APPEND OPERATORS_MACRO_UNARY "ADD_UNARY_OPERATOR(${_t});\n")
    set (inline_pre "inline ${CMAKE_MATCH_4} ")
    set (inline_post "(const ${CMAKE_MATCH_3}& x) { return UnaryOperator<${_t}>::fwd(x); }\n")
  else () # Binary
    set (_t "${CMAKE_MATCH_2},${CMAKE_MATCH_3},${CMAKE_MATCH_4},${CMAKE_MATCH_5}")
    string (APPEND OPERATORS_MACRO_BINARY "ADD_BINARY_OPERATOR(${_t});\n")
    set (inline_pre "inline ${CMAKE_MATCH_5} ")
    set (inline_post "(const ${CMAKE_MATCH_3}& x1, const ${CMAKE_MATCH_4}& x2) { return BinaryOperator<${_t}>::fwd(x1,x2); }\n")
  endif ()
  set (OP_VARNAME ${CMAKE_MATCH_2})

  # Parse source file
  file (STRINGS ${cppfile} line REGEX "^extern const char .*;$" LIMIT_COUNT 1)
  if (NOT line)
    message (FATAL_ERROR "Error while parsing ${cpp}: no const char line")
  endif ()
  string (REGEX MATCH " const char ${OP_VARNAME}\\[\\] = \"(.*)\";$" _ ${line})
  string (APPEND OPERATORS_FCT_DEF "${inline_pre}${CMAKE_MATCH_1}${inline_post}")
endforeach()

################################################################################
# configure files with OPERATORS_* variables
################################################################################
configure_file (ibex_ExprOperators.cpp.in ibex_ExprOperators.cpp)
configure_file (ibex_ExprOperators.h.in ibex_ExprOperators.h)

################################################################################
target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CmpOp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CmpOp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2DAG.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2DAG.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2Minibex.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2Minibex.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2Polynom.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Expr2Polynom.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCmp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCmp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCopy.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCopy.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprCtr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprMonomial.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprMonomial.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprFuncDomain.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprDiff.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprDiff.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprLinearity.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprLinearity.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprOccCounter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprOccCounter.h
  ${CMAKE_CURRENT_BINARY_DIR}/ibex_ExprOperators.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ibex_ExprOperators.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprPolynomial.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprPolynomial.h	
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprPrinter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprPrinter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSimplify.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSimplify.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSimplify2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExprSimplify2.h
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
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
                  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../operators>")
