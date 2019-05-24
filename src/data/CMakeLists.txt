# see arithmetic/CMakeLists.txt for comments

list (APPEND IBEX_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Cov.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Cov.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovIBUList.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovIBUList.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovIUList.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovIUList.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovList.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovList.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovManifold.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovManifold.h
  )

list (APPEND IBEX_INCDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Propagate new values of list to parent scope
set (IBEX_SRC ${IBEX_SRC} PARENT_SCOPE)
set (IBEX_INCDIRS ${IBEX_INCDIRS} PARENT_SCOPE)
