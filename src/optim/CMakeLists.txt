# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpMultipliers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpMultipliers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpOptimData.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpOptimData.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_DefaultOptimizerConfig.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_DefaultOptimizerConfig.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_DefaultOptimizer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LineSearch.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LineSearch.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Optimizer04Config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Optimizer04Config.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_OptimizerConfig.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Optimizer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Optimizer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_OptimMemory.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_UnconstrainedLocalSearch.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_UnconstrainedLocalSearch.h
)

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
