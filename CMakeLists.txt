cmake_minimum_required (VERSION 2.8.12)
# Cmake doc can be found at https://cmake.org/cmake/help/v2.8.12/cmake.html

message (WARNING "CMake support is still experimental, use waf to build Ibex")

project (IBEX CXX)
set(IBEX_VERSION_MAJOR 2)
set(IBEX_VERSION_MINOR 8)
set(IBEX_VERSION_PATCHLEVEL 0)
set(IBEX_VERSION "${IBEX_VERSION_MAJOR}.${IBEX_VERSION_MINOR}.${IBEX_VERSION_PATCHLEVEL}")


################################################################################
# Print information (to ease debugging)
################################################################################
message (STATUS "Running on system ${CMAKE_HOST_SYSTEM} with processor ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message (STATUS "Using CMake ${CMAKE_VERSION}")
message (STATUS "Configuring Ibex ${IBEX_VERSION}")
message (STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
# TODO handle different type of build

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

################################################################################
# Configure the library for interval arithmetic
################################################################################
file (GLOB INTERVAL_PLUGIN_DIRS "plugins/interval_lib_*")
set (INTERVAL_LIB_LIST "")
foreach (plugin_absolute_path ${INTERVAL_PLUGIN_DIRS})
  get_filename_component (plugin_name ${plugin_absolute_path} NAME)
  string (SUBSTRING ${plugin_name} 13 -1 itvlib)
  list (APPEND INTERVAL_LIB_LIST ${itvlib})
endforeach ()
list (SORT INTERVAL_LIB_LIST)

set(INTERVAL_LIB "gaol" CACHE STRING "Library used for interval arithmetic")
set_property(CACHE INTERVAL_LIB PROPERTY STRINGS ${INTERVAL_LIB_LIST})

list(FIND INTERVAL_LIB_LIST ${INTERVAL_LIB} is_valid)
if (is_valid LESS 0)
  message (FATAL_ERROR "Unsupported library for interval arithmetic: ${INTERVAL_LIB}")
else ()
  message (STATUS "Library for interval arithmetic: ${INTERVAL_LIB}")
endif ()

set (ITV_LIB_SUBDIR plugins/interval_lib_${INTERVAL_LIB})
set (INTERVAL_LIB_WRAPPER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${ITV_LIB_SUBDIR})
set (INTERVAL_LIB_WRAPPER_SRC ${INTERVAL_LIB_WRAPPER_DIR}/ibex_IntervalLibWrapper.cpp)
add_subdirectory (${ITV_LIB_SUBDIR})

################################################################################
# Configure the library for linear programming
################################################################################
set(LP_LIB "none" CACHE STRING "Library used for linear programming")
set (IBEX_LP_LIB_INCLUDES "#define __IBEX_NO_LP_SOLVER__")
set (IBEX_LP_LIB_EXTRA_ATTRIBUTES "")
set (LP_LIB_SUBDIR plugins/lp_lib_${LP_LIB})
set (LP_LIB_WRAPPER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${LP_LIB_SUBDIR})

################################################################################
# Go to src subdirectory that builds the libibex target
################################################################################
add_subdirectory (src)

################################################################################
# Tests
################################################################################
include(CTest)
if (BUILD_TESTING)
  add_custom_target (check COMMAND ${CMAKE_CTEST_COMMAND} -V $(ARGS)
                           DEPENDS ibex COMMENT "Running the tests")
  add_subdirectory (tests EXCLUDE_FROM_ALL)
endif ()

################################################################################
# TODO examples and benchs
################################################################################
#add_subdirectory (examples)
#add_subdirectory (benchs)

################################################################################
# TODO archives and packages
################################################################################
#set (IBEX_DIST_ARCHIVE_NAME "ibex-${IBEX_VERSION}")
#Include (CPack)
