# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
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
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovOptimData.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovOptimData.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovSolverData.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CovSolverData.h
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
