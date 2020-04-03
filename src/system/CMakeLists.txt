# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExtendedSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_ExtendedSystem.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncActiveCtrs.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncActiveCtrs.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncKuhnTucker.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_FncKuhnTucker.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_KuhnTuckerSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_KuhnTuckerSystem.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_NormalizedSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_NormalizedSystem.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_System.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_System.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SystemFactory.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SystemFactory.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SystemMerge.cpp
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
