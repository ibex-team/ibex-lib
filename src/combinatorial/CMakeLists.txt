# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_QInter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_QInter.h
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
