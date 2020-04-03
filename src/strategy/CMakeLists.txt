# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BoxEvent.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BoxProperties.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BoxProperties.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Bxp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpActiveCtr.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpActiveCtr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpActiveCtrs.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpActiveCtrs.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpLinearRelaxArgMin.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpLinearRelaxArgMin.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpSystemCache.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_BxpSystemCache.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Cell.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Cell.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellBuffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellBuffer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellList.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellList.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellStack.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_CellStack.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Paver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Paver.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetImage.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetImage.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SubPaving.h
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
