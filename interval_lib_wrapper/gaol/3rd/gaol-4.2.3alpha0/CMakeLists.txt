cmake_minimum_required (VERSION 3.5.1)

set (GAOL_MAJOR_VERSION 4)
set (GAOL_MINOR_VERSION 2)
set (GAOL_MICRO_VERSION 3)
set (GAOL_VERSION "${GAOL_MAJOR_VERSION}.${GAOL_MINOR_VERSION}.${GAOL_MICRO_VERSION}")
project (GAOL VERSION ${GAOL_VERSION} LANGUAGES C CXX)

set (PACKAGE_NAME "gaol")
set (PACKAGE ${PACKAGE_NAME})
set (PACKAGE_BUGREPORT "goualard@users.sourceforge.net")
set (PACKAGE_STRING "${PACKAGE_NAME} \"${GAOL_VERSION}\"")
set (PACKAGE_TARNAME "${PACKAGE_NAME}")
set (PACKAGE_VERSION "\"${GAOL_VERSION}\"")
set (VERSION "\"${GAOL_VERSION}\"")

message (STATUS "Running on system ${CMAKE_HOST_SYSTEM} with processor ${CMAKE_HOST_SYSTEM_PROCESSOR}")
if (CMAKE_CROSSCOMPILING)
  message (STATUS "Targeting system ${CMAKE_SYSTEM} with processor ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

# options and command-line arguments
option (BUILD_SHARED_LIBS "Set to ON to build shared libraries" OFF)
set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")

# Looking for headers (for gaol_configuration.in)
option (STDC_HEADERS "Set to OFF if you do not have standard headers" ON)
include (CheckIncludeFiles)
foreach (name "clock" "dlfcn" "fenv" "float" "inttypes" "memory" "stddef"
              "stdint" "stdlib" "stdio" "strings" "string" "sys/resource" "sys/stat"
              "sys/time" "sys/types" "unistd")
  string (TOUPPER "${name}" uppername)
  string (REPLACE "/" "_" uppername "${uppername}")
  set (header "${name}.h")
  set (var "HAVE_${uppername}_H")
  check_include_files ("${header}" "${var}")
endforeach ()

include (CheckSymbolExists)
if (HAVE_SYS_RESOURCE_H)
  check_symbol_exists (getrusage "sys/resource.h" GETRUSAGE_IN_HEADER)
else ()
  check_include_files ("time.h" HAVE_TIME_H)
  check_symbol_exists (clock "time.h" CLOCK_IN_HEADER)
endif ()

include (CheckIncludeFileCXX)
check_include_file_cxx (cassert HAVE_CASSERT)
check_include_file_cxx (limits HAVE_LIMITS)

include (CheckFunctionExists)
foreach (fct fesetround floor localeconv memset pow sqrt getrusage clock
          __signbit round rint finite malloc realloc)
  string (TOUPPER "${fct}" upperfct)
  set (var "HAVE_${upperfct}")
  check_function_exists (${fct} ${var})
endforeach ()
set (HAVE_NEXTAFTER ON) # we required C++11 for Ibex, so we are sure to have it

# set some macros depending on system
if (CMAKE_SYSTEM MATCHES "Linux")
  if (CMAKE_SYSTEM_PROCESSOR MATCHES "86_64")
    set (IX86_LINUX ON)
    set (X86_64BITS ON)
  elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
    set (IX86_LINUX ON)
  elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set (AARCH64_LINUX ON)
  else ()
    set (UNSUPPORTED_ARCH ON)
  endif ()
elseif (CMAKE_SYSTEM MATCHES "CYGWIN" AND CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
  set (IX86_CYGWIN ON)
elseif (CMAKE_SYSTEM MATCHES "Darwin")
  if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    set (AARCH64_LINUX ON)
  else ()
    set (IX86_MACOSX ON)
  endif ()
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "powerpc|ppc")
  set (POWERPC_UNIX ON)
else ()
  set (UNSUPPORTED_ARCH ON)
endif ()

if (UNSUPPORTED_ARCH)
  message (FATAL_ERROR "The target system is not supported by Gaol")
endif ()

# test endianness
if (NOT CMAKE_CROSSCOMPILING)
  include (TestBigEndian)
  test_big_endian (WORDS_BIGENDIAN)
elseif (NOT DEFINED WORDS_BIGENDIAN)
  message (FATAL_ERROR "In cross compiling mode, WORDS_BIGENDIAN should be "
                         "set on the command-line")
endif ()

if (NOT WIN32)
  if (NOT CMAKE_CROSSCOMPILING)
    if (NOT DEFINED USING_SSE2_INSTRUCTIONS
        AND NOT DEFINED USING_SSE3_INSTRUCTIONS)
      try_run (SSE_RUN SSE_COMPILE ${CMAKE_CURRENT_BINARY_DIR}
                ${CMAKE_CURRENT_SOURCE_DIR}/test_sse.cpp
                RUN_OUTPUT_VARIABLE SSE_OUTPUT)
      if (NOT SSE_COMPILE OR NOT SSE_RUN EQUAL 0)
        message (STATUS "Could not determine if SSE2 and SSE3 is available: "
          "set USING_SSE2_INSTRUCTIONS and/or USING_SSE3_INSTRUCTIONS on the "
          "command-line to use SSE2 and/or SSE3")
      else ()
        if (SSE_OUTPUT MATCHES "HAVE_SSE2=true")
          set (USING_SSE2_INSTRUCTIONS ON)
          message (STATUS "SSE2 instructions set detected")
        endif ()
        if (SSE_OUTPUT MATCHES "HAVE_SSE3=true")
          set (USING_SSE3_INSTRUCTIONS ON)
          message (STATUS "SSE3 instructions set detected")
        endif ()
      endif ()
    endif ()
  else ()
    if (NOT DEFINED USING_SSE2_INSTRUCTIONS)
      message (FATAL_ERROR "In cross compiling mode, USING_SSE2_INSTRUCTIONS "
                            "should be set on the command-line")
    endif ()
    if (NOT DEFINED USING_SSE3_INSTRUCTIONS)
      message (FATAL_ERROR "In cross compiling mode, USING_SSE3_INSTRUCTIONS "
                            "should be set on the command-line")
    endif ()
  endif ()
endif ()
if (USING_SSE3_INSTRUCTIONS)
  message (STATUS "Gaol will use SSE3 and SSE2 instructions")
elseif (USING_SSE2_INSTRUCTIONS)
  message (STATUS "Gaol will use SSE2 instructions")
else ()
  message (STATUS "Gaol will NOT use SSE instructions")
endif ()

#
set (GAOL_CERTAINLY_RELATIONS ON)
set (GAOL_EXCEPTIONS_ENABLED ON)
set (GAOL_PRESERVE_ROUNDING OFF)
set (GAOL_USING_ASM ON)
set (GAOL_VERBOSE_MODE OFF)

#
#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#  set (GAOL_DEBUGGING ON)
#endif ()

include (CheckTypeSize)
CHECK_TYPE_SIZE(int SIZEOF_INT BUILTIN_TYPES_ONLY)
CHECK_TYPE_SIZE(long SIZEOF_LONG BUILTIN_TYPES_ONLY)
CHECK_TYPE_SIZE("long long int" SIZEOF_LONG_LONG_INT BUILTIN_TYPES_ONLY)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag ("-fvisibility-inlines-hidden" COMPILER_SUPPORTS_VISI_1)
check_cxx_compiler_flag ("-fvisibility=hidden" COMPILER_SUPPORTS_VISI_2)
if (COMPILER_SUPPORTS_VISI_1 AND COMPILER_SUPPORTS_VISI_2)
  set (HAVE_VISIBILITY_OPTIONS ON)
  set (GAOL_CXXFLAGS "${GAOL_CXXFLAGS} -fvisibility-inlines-hidden")
  set (GAOL_CXXFLAGS "${GAOL_CXXFLAGS} -fvisibility=hidden")
endif ()

check_cxx_compiler_flag ("-funroll-loops" COMPILER_SUPPORTS_FUNROLL_LOOPS)
if (COMPILER_SUPPORTS_FUNROLL_LOOPS)
  set (GAOL_CXXFLAGS "${GAOL_CXXFLAGS} -funroll-loops")
endif ()
check_cxx_compiler_flag ("-fomit-frame-pointer" COMPILER_SUPPORTS_OMIT)
if (COMPILER_SUPPORTS_OMIT)
  set (GAOL_CXXFLAGS "${GAOL_CXXFLAGS} -fomit-frame-pointer")
endif ()
check_cxx_compiler_flag ("-fexpensive-optimizations" COMPILER_SUPPORTS_OPTIM)
if (COMPILER_SUPPORTS_OPTIM)
  set (GAOL_CXXFLAGS "${GAOL_CXXFLAGS} -fexpensive-optimizations")
endif ()
check_cxx_compiler_flag ("-frounding-math" HAVE_ROUNDING_MATH_OPTION)

if(NOT MSVC)
  set (CMAKE_C_FLAGS_RELEASE "-O2 -Wall -Wconversion")
  set (CMAKE_C_FLAGS_DEBUG "-O0 -g -pg -Wall -Wconversion")
  set (CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wconversion ${GAOL_CXXFLAGS}")
  set (CMAKE_CXX_FLAGS_DEBUG "-O -g -pg -Wall -Wconversion")
else()
  set (CMAKE_C_FLAGS_RELEASE "/O2")
  set (CMAKE_C_FLAGS_DEBUG "/Od")
  set (CMAKE_CXX_FLAGS_RELEASE "/O2")
  set (CMAKE_CXX_FLAGS_DEBUG "/O1")
endif()

### libgaol ####################################################################
# gaol/gaol_configuration.h.in -> gaol/gaol_configuration.h
configure_file (gaol/gaol_configuration.h.in gaol/gaol_configuration.h)

execute_process (COMMAND ${CMAKE_COMMAND} -E copy gaol/gaol_double_op_apmathlib.h gaol/gaol_double_op.h
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  RESULT_VARIABLE ret
                  OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/copy-out.log"
                  ERROR_FILE "${CMAKE_CURRENT_BINARY_DIR}/copy-err.log"
                 )
if (ret)
  message (FATAL_ERROR "An error occurs while copying a file\n"
                        "See also\n${CMAKE_CURRENT_BINARY_DIR}/copy-*.log\n")
endif ()

# source and header file
unset (SRCS)
foreach (file gaol gaol.h gaol_allocator.h gaol_assert.h gaol_common.cpp
              gaol_common.h gaol_config.h gaol_configuration.h gaol_double_op.h
              gaol_double_op_apmathlib.h gaol_eval_stack.h gaol_exact.c
              gaol_exact.h gaol_exceptions.cpp gaol_exceptions.h
              gaol_expr_eval.h gaol_expr_visitor.h gaol_expression.cpp
              gaol_expression.h gaol_flags.h gaol_fpu.h gaol_fpu_fenv.h
              gaol_init_cleanup.cpp gaol_init_cleanup.h gaol_interval.cpp
              gaol_interval.h gaol_interval_fpu.h gaol_interval_sse.h
              gaol_intervalf.cpp gaol_intervalf.h gaol_interval_lexer.cpp
              gaol_interval_parser.cpp gaol_interval_parser.h gaol_limits.h
              gaol_parameters.h gaol_parser.cpp gaol_parser.h gaol_port.cpp
              gaol_port.h gaol_profile.cpp gaol_profile.h gaol_version.h
              s_nextafter.c)
  list (APPEND SRCS "${CMAKE_CURRENT_SOURCE_DIR}/gaol/${file}")
endforeach ()
list (APPEND SRCS "${CMAKE_CURRENT_BINARY_DIR}/gaol/gaol_configuration.h")
if (USING_SSE3_INSTRUCTIONS)
  list (APPEND SRCS "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval2f.cpp")
  list (APPEND SRCS "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval2f.h")
endif ()

# Gaol library
add_library (gaol ${SRCS})
set_target_properties (gaol PROPERTIES VERSION 0.0.0 SOVERSION 0)

if (USING_SSE3_INSTRUCTIONS)
  target_compile_options (gaol PUBLIC "-msse3")
elseif (USING_SSE2_INSTRUCTIONS)
  target_compile_options (gaol PUBLIC "-msse2")
endif ()
if (HAVE_ROUNDING_MATH_OPTION)
  target_compile_options (gaol PUBLIC "-frounding-math")
endif ()

# Need source and binary dir (for gaol/gaol_configuration.) in include dirs
set (incdirs "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
                                           "${CMAKE_CURRENT_BINARY_DIR}/gaol")
target_include_directories (gaol PUBLIC "$<BUILD_INTERFACE:${incdirs}>")
target_include_directories (gaol PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(gaol PUBLIC ultim)

# install headers
install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_allocator.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_assert.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_common.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_config.h"
               "${CMAKE_CURRENT_BINARY_DIR}/gaol/gaol_configuration.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_double_op_apmathlib.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_double_op_crlibm.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_double_op.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_eval_stack.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_exact.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_exceptions.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_expression.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_expr_eval.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_expr_visitor.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_flags.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_fpu_fenv.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_fpu.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_init_cleanup.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval2f.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_intervalf.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval_fpu.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval_parser.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval_sse.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_limits.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_parameters.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_parser.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_port.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_profile.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_version.h"
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gaol COMPONENT devtools)
install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/gaol/sysdeps/gaol_exact_c99.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/gaol/sysdeps/gaol_exact_msvc.h"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gaol/sysdeps COMPONENT devtools)
if (USING_SSE3_INSTRUCTIONS)
  install (FILES "${CMAKE_CURRENT_SOURCE_DIR}/gaol/gaol_interval2f.h"
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gaol COMPONENT devtools)
endif ()

# install library (and add it to ibexExport)
install(TARGETS gaol EXPORT ibexExport DESTINATION ${CMAKE_INSTALL_LIBDIR}
                                                            COMPONENT devtools)
