# see arithmetic/CMakeLists.txt for comments

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Certificate.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Certificate.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Kernel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Kernel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LPException.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LPSolver.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LPSolver.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Linear.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Linear.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearException.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Linearizer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Linearizer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerCompo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerCompo.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerDuality.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerDuality.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerFixed.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerFixed.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerXTaylor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LinearizerXTaylor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Newton.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_Newton.h
  )

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
