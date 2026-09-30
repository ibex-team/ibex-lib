# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BscHijackGuard.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BscHijackGuard.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Json.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LSmearVariants.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LSmearVariants.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Json.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_MLModel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_MLModel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_MLOptimizerConfig.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_MLOptimizerConfig.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_MLNodeServer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_MLNodeServer.h
)

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
