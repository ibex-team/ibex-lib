cmake_minimum_required (VERSION 3.5.1)

project (FILIB VERSION 3.0.2.2 LANGUAGES CXX)

message (STATUS "Running on system ${CMAKE_HOST_SYSTEM} with processor ${CMAKE_HOST_SYSTEM_PROCESSOR}")
if (CMAKE_CROSSCOMPILING)
  message (STATUS "Targeting system ${CMAKE_SYSTEM} with processor ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

# =========== Fixme! See issue #561# ====================================================================
if (CMAKE_SYSTEM MATCHES "Darwin" AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
	message (FATAL_ERROR "Ibex + Filib is not supported yet on MacOS arm64: try to use Gaol instead!")
endif ()
# =======================================================================================================

# options and command-line arguments
option (BUILD_SHARED_LIBS "Set to ON to build shared libraries" OFF)
set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")

# Values for config.h.in
set (VERSION 3.0.2)

# Looking for headers (for config.h.in)
option (STDC_HEADERS "Set to OFF if you do not have standard headers" ON)
include (CheckIncludeFileCXX)
foreach (name "dlfcn" "stdint" "inttypes" "stdlib" "strings" "string" "memory"
              "sys/stat" "sys/types" "unistd")
  string (TOUPPER "${name}" uppername)
  string (REPLACE "/" "_" uppername "${uppername}")
  set (header "${name}.h")
  set (var "HAVE_${uppername}_H")
  check_include_file_cxx ("${header}" "${var}")
endforeach ()

# config.h.in -> config.h
configure_file (config.h.in config.h)

# Checking for -frounding-math and -ffloat-store
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag ("-frounding-math" COMPILER_SUPPORTS_FROUNDING_MATH)
check_cxx_compiler_flag ("-ffloat-store" COMPILER_SUPPORTS_FFLOAT_STORE)

# Setting HAVE_SSE and HAVE_X87
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
	set(HAVE_SSE ON)
elseif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "i.86")
	set(HAVE_X87 ON)
endif ()

# rounding_control_config.hpp.in -> rounding_control_config.hpp
set (RC_CONFIG "rounding_control/rounding_control_config.hpp")
configure_file (${RC_CONFIG}.in ${RC_CONFIG})

# We DO NOT follow advice from https://cmake.org/cmake/help/v3.0/command/file.html,
# and we use the command FILE (GLOB ...) to find all headers and sources files
# because we assume that no file will be added in this directory.
# If one wants to use this CMakeLists.txt file to develop Filib, one can expand
# theses variables by hand as done in ibex/src/*/CMakeLists.txt files.
file (GLOB_RECURSE SRCS *.cpp)
file (GLOB_RECURSE HEADERS *.icc *.hpp)
list (APPEND HEADERS "${CMAKE_CURRENT_BINARY_DIR}/${RC_CONFIG}")

if (WIN32 AND BUILD_SHARED_LIBS)
  # Filib library is called prim
  add_library (prim STATIC ${SRCS} ${HEADERS})
  set_target_properties(prim PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
  set_target_properties(prim PROPERTIES LINK_FLAGS "/DEF:prim.def")
else()
  # Filib library is called prim
  add_library (prim ${SRCS} ${HEADERS})
endif()

set_target_properties (prim PROPERTIES VERSION 0.0.0 SOVERSION 0)

# Need source and binary dir (for rounding_control_config.hpp) in include dirs
set (incdirs "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
target_include_directories (prim PUBLIC "$<BUILD_INTERFACE:${incdirs}>")
target_include_directories (prim PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# Add compile flags
if (COMPILER_SUPPORTS_FROUNDING_MATH)
  target_compile_options (prim PUBLIC "-frounding-math")
endif ()
if (COMPILER_SUPPORTS_FFLOAT_STORE AND NOT HAVE_SSE)
  target_compile_options (prim PUBLIC "-ffloat-store")
endif ()

# install headers
install(DIRECTORY ieee fp_traits interval rounding_control COMPONENT devtools
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.icc")
install (FILES "${CMAKE_CURRENT_BINARY_DIR}/${RC_CONFIG}"
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/rounding_control/
          COMPONENT devtools)

# install library (and add it to ibexExport)
install(TARGETS prim EXPORT ibexExport DESTINATION ${CMAKE_INSTALL_LIBDIR}
                                                            COMPONENT devtools)
