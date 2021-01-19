# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellBeamSearch.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellBeamSearch.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellBuffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellBuffer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellBufferOptim.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellCostFunc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellCostFunc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Cell.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellDoubleHeap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Cell.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellHeap.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellHeap.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellList.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellList.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellStack.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellStack.h
)

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
