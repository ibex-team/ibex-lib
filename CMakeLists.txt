cmake_minimum_required (VERSION 3.0.2)
# CMake doc can be found at https://cmake.org/cmake/help/v3.0/

message (WARNING "CMake support is still experimental, use waf to build Ibex")

project (IBEX VERSION 2.8.6 LANGUAGES CXX)
set (IBEX_DESCRIPTION "A C++ library for interval-based algorithm design")
set (IBEX_URL "http://www.ibex-lib.org/")

################################################################################
# Add cmake.utils to the list of CMAKE_MODULE_PATH
################################################################################
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake.utils")

################################################################################
# Options for install directory
################################################################################
set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
set (CMAKE_INSTALL_INCLUDEDIR_3RD ${CMAKE_INSTALL_INCLUDEDIR}/ibex/3rd)
set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")
set (CMAKE_INSTALL_LIBDIR_3RD ${CMAKE_INSTALL_LIBDIR}/ibex/3rd)
set (CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
set (CMAKE_INSTALL_PKGCONFIG "share/pkgconfig" CACHE PATH "pkg files (share/pkgconfig)")

################################################################################
# Print information (to ease debugging)
################################################################################
message (STATUS "Running on system ${CMAKE_HOST_SYSTEM} with processor ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message (STATUS "Using CMake ${CMAKE_VERSION}")
message (STATUS "Configuring Ibex ${IBEX_VERSION}")
message (STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

################################################################################
# Options for shared or static library
################################################################################
option (BUILD_SHARED_LIBS "Set to ON to build Ibex as a shared library" OFF)
if (BUILD_SHARED_LIBS)
  message (STATUS "Building libibex as shared library")
else ()
  message (STATUS "Building libibex as static library")
endif ()

################################################################################
# Check that the compiler supports c++11
################################################################################
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG ("-std=c++11" COMPILER_SUPPORTS_CXX11)
if (COMPILER_SUPPORTS_CXX11)
  add_compile_options ("-std=c++11")
else ()
  message (FATAL_ERROR "Ibex needs a compiler with C++11 support")
endif ()

################################################################################
# Set flags and build type (release or debug)
################################################################################
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message (STATUS "Setting build type to 'Release' as none was specified.")
  set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
  set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif ()

add_compile_options ("$<$<CONFIG:Release>:-O3;-DNDEBUG>"
                     "$<$<CONFIG:Debug>:-O0;-g;-pg;-Wall;-DDEBUG>")

if (WIN32)
  # We need this for strdup under Windows (see issue #287)
  add_definitions(-U__STRICT_ANSI__)
endif ()

################################################################################
# Configure the libraries for interval arithmetic and linear programming
################################################################################
add_subdirectory (interval_lib_wrapper)
add_subdirectory (lp_lib_wrapper)

################################################################################
# Go to src subdirectory that builds the libibex target
################################################################################
add_subdirectory (src)

################################################################################
# Generate ibex.pc
################################################################################
include (PkgConfigFile)

################################################################################
# Tests
################################################################################
include(CTest)
if (BUILD_TESTING)
  add_custom_target (check
                      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure $(ARGS)
                      DEPENDS ibex COMMENT "Running the tests")
  add_subdirectory (tests EXCLUDE_FROM_ALL)
endif ()

################################################################################
# TODO benchs
################################################################################
#add_subdirectory (benchs)

################################################################################
# archives and packages
################################################################################
set (CPACK_GENERATOR "TGZ" "ZIP" "DEB")
string (TOLOWER "${CMAKE_PROJECT_NAME}" CPACK_PACKAGE_NAME)
set (CPACK_PACKAGE_VENDOR "IbexTeam")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${IBEX_DESCRIPTION})
set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set (CPACK_PACKAGE_CONTACT "Maintainer <mail@url.com>") # TODO which email ?

set (CPACK_DEBIAN_PACKAGE_HOMEPAGE ${IBEX_URL})
set (CPACK_DEB_COMPONENT_INSTALL ON)

Include (CPack)
