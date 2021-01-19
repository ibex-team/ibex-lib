# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Sep.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Sep.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepBoundaryCtc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepBoundaryCtc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepCtcPair.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepCtcPair.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepFwdBwd.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepFwdBwd.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepInter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepInter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepInverse.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepInverse.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepNot.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepNot.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepQInter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepQInter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepUnion.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SepUnion.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Set.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Set.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetBisect.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetBisect.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetConnectedComponents.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetInterval.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetInterval.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetLeaf.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetLeaf.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetNode.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetNode.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetValueNode.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetValueNode.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_SetVisitor.h
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
