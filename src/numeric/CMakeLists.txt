# see arithmetic/CMakeLists.txt for comments

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LPSolver.cpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/ibex_LPSolver.cpp
)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LPSolver.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/ibex_LPSolver.h
)

list (APPEND IBEX_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/.ibex_LPSolver.cpp.in.swp
  ${CMAKE_CURRENT_SOURCE_DIR}/.ibex_LPSolver.h.in.swp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Certificate.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Certificate.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Kernel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Kernel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LPException.h
  ${CMAKE_CURRENT_BINARY_DIR}/ibex_LPSolver.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/ibex_LPSolver.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Linear.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Linear.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearException.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Linearizer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Linearizer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerCombo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerCombo.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerDuality.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerDuality.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerFixed.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerFixed.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerXTaylor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerXTaylor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Newton.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Newton.h
  )

list (APPEND IBEX_INCDIRS ${CMAKE_CURRENT_SOURCE_DIR}
                          ${CMAKE_CURRENT_BINARY_DIR})

# Propagate new values of list to parent scope
set (IBEX_SRC ${IBEX_SRC} PARENT_SCOPE)
set (IBEX_INCDIRS ${IBEX_INCDIRS} PARENT_SCOPE)
