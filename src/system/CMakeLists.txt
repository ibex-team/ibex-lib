# see arithmetic/CMakeLists.txt for comments

set (IBEX_SUBDIR_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExtendedSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncActivation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncKhunTucker.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FritzJohnCond.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_NormalizedSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_System.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SystemFactory.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SystemMerge.cpp
  PARENT_SCOPE)

set (IBEX_SUBDIR_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExtendedSystem.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncActivation.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncKhunTucker.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FritzJohnCond.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_NormalizedSystem.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SystemFactory.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_System.h
  PARENT_SCOPE)

set (IBEX_SUBDIR_INCDIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  PARENT_SCOPE)
