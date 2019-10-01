cmake_minimum_required (VERSION 3.0.2)

message (WARNING "CMake support is still experimental, use waf to build Ibex")

project (IBEX_ROBOTICS VERSION 14.42 LANGUAGES CXX)
set (IBEX_ROBOTICS_DESCRIPTION "Ensta-robotics plugin of Ibex")

################################################################################
# Options for install directory
################################################################################
set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")

################################################################################
# Check that the compiler supports c++11
################################################################################
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(FATAL_ERROR "Ibex needs a compiler with C++11 support")
endif()

if (WIN32)
  # We need this for strdup under Windows (see issue #287)
  add_definitions(-U__STRICT_ANSI__)
endif ()

################################################################################
# Looking for Ibex
################################################################################
#find_package (IBEX 2.8.3 REQUIRED)
include (FindPkgConfig)
pkg_search_module (IBEX REQUIRED ibex)
message (STATUS "Found Ibex version ${IBEX_VERSION}")

################################################################################
# Compile sources
################################################################################
add_subdirectory (src)
