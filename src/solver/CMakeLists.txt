# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_DefaultSolver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_DefaultSolver.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Solver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Solver.h
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
