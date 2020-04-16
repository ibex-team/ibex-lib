# This file is written to be used via 'add_subdirectory' not 'include'
# Following advice from https://cmake.org/cmake/help/v3.0/command/file.html, the
# command FILE (GLOB ...) should not be used to set these variables.
# Paths to files should be absolute.

target_sources (ibex PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderCertify.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderCertify.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderDefault.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderDefault.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderDuality.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderDuality.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderFwdBwd.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderFwdBwd.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinder.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderInHC4.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderInHC4.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderProbing.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderProbing.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderXTaylor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ibex_LoupFinderXTaylor.h
)

target_include_directories (ibex PUBLIC
                              "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>")
