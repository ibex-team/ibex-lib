# see arithmetic/CMakeLists.txt for comments

set (OPERATORS_LIST atanhc atanhccc crossproduct sinc trace)

foreach (op ${OPERATORS_LIST})
  set (hdr "ibex_${op}.h")
  set (cpp "ibex_${op}.cpp")

  # Handles includes and IBEX_SRC
  string (CONCAT OPERATORS_INCLUDES ${OPERATORS_INCLUDES} "#include \"${hdr}\"\n")
  list (APPEND IBEX_OPS_HDR ${CMAKE_CURRENT_SOURCE_DIR}/${hdr})
  list (APPEND IBEX_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${cpp})
  
  # Parse header
  file (STRINGS ${hdr} line REGEX "^class [BU]i?naryOperator<" LIMIT_COUNT 1)
  if (NOT line)
    message (FATAL_ERROR "Error while parsing ${hdr}: no class line")
  endif ()
  string(REGEX MATCH " (.*)Operator<([^,]*),([^,]*),([^,]*),?(.*)>" _ ${line})

  if (CMAKE_MATCH_1 STREQUAL "Unary")
    set (_t "${CMAKE_MATCH_2},${CMAKE_MATCH_3},${CMAKE_MATCH_4}")
    string (CONCAT OPERATORS_MACRO_UNARY "${OPERATORS_MACRO_UNARY}"
                                              "ADD_UNARY_OPERATOR(${_t});\n")
    set (inline_pre "inline ${CMAKE_MATCH_4} ")
    set (inline_post "(const ${CMAKE_MATCH_3}& x) { return UnaryOperator<${_t}>::fwd(x); }\n")
  else () # Binary
    set (_t "${CMAKE_MATCH_2},${CMAKE_MATCH_3},${CMAKE_MATCH_4},${CMAKE_MATCH_5}")
    string (CONCAT OPERATORS_MACRO_BINARY "${OPERATORS_MACRO_BINARY}"
                                              "ADD_BINARY_OPERATOR(${_t});\n")
    set (inline_pre "inline ${CMAKE_MATCH_5} ")
    set (inline_post "(const ${CMAKE_MATCH_3}& x1, const ${CMAKE_MATCH_4}& x2) { return BinaryOperator<${_t}>::fwd(x1,x2); }\n")
  endif ()
  set (OP_VARNAME ${CMAKE_MATCH_2})

  # Parse source file
  file (STRINGS ${cpp} line REGEX "^extern const char .*;$" LIMIT_COUNT 1)
  if (NOT line)
    message (FATAL_ERROR "Error while parsing ${cpp}: no const char line")
  endif ()
  string(REGEX MATCH " const char ${OP_VARNAME}\\[\\] = \"(.*)\";$" _ ${line})
  string (CONCAT OPERATORS_FCT_DEF "${OPERATORS_FCT_DEF}"
                                "${inline_pre}${CMAKE_MATCH_1}${inline_post}")
endforeach()

list (APPEND IBEX_INCDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Propagate new values to parent scope
set (IBEX_SRC ${IBEX_SRC} PARENT_SCOPE)
set (IBEX_OPS_HDR ${IBEX_OPS_HDR} PARENT_SCOPE)
set (IBEX_INCDIRS ${IBEX_INCDIRS} PARENT_SCOPE)
set (OPERATORS_INCLUDES "${OPERATORS_INCLUDES}" PARENT_SCOPE)
set (OPERATORS_MACRO_UNARY "${OPERATORS_MACRO_UNARY}" PARENT_SCOPE)
set (OPERATORS_MACRO_BINARY "${OPERATORS_MACRO_BINARY}" PARENT_SCOPE)
set (OPERATORS_FCT_DEF "${OPERATORS_FCT_DEF}" PARENT_SCOPE)
