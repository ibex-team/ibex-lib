cmake_minimum_required (VERSION 3.5.1)

project (MATHLIB VERSION 2.1.1 LANGUAGES C)

message (STATUS "Running on system ${CMAKE_HOST_SYSTEM} with processor ${CMAKE_HOST_SYSTEM_PROCESSOR}")
if (CMAKE_CROSSCOMPILING)
  message (STATUS "Targeting system ${CMAKE_SYSTEM} with processor ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

# options and command-line arguments
option (BUILD_SHARED_LIBS "Set to ON to build shared libraries" OFF)
set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")

# Values for config.h.in
set (VERSION ${MATHLIB_VERSION})

# Looking for headers (for config.h.in)
option (STDC_HEADERS "Set to OFF if you do not have standard headers" ON)
include (CheckIncludeFiles)
foreach (name "dlfcn" "fenv" "inttypes" "memory" "stdint" "stdio" "stdlib" "strings" "string" "sys/stat" "sys/types" "unistd")
  string (TOUPPER "${name}" uppername)
  string (REPLACE "/" "_" uppername "${uppername}")
  set (header "${name}.h")
  set (var "HAVE_${uppername}_H")
  check_include_files ("${header}" "${var}")
endforeach ()

# set some macros depending on system
if (CMAKE_SYSTEM MATCHES "Linux")
  set (MATHLIB_LINUX ON)
  if (CMAKE_SYSTEM_PROCESSOR MATCHES "86_64")
    set (X86_64BITS ON)
    set (IX86_CPU ON)
  elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
    set (IX86_CPU ON)
  else ()
    set (UNSUPPORTED_ARCH ON)
  endif ()
elseif (CMAKE_SYSTEM MATCHES "CYGWIN" AND CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
  set (MATHLIB_CYGWIN ON)
  set (IX86_CPU ON)
elseif (CMAKE_SYSTEM MATCHES "Darwin")
  if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
    set (MATHLIB_AARCH64 ON)
  elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "i.86")
    set (MATHLIB_I86_MACOSX ON)
    set (IX86_CPU ON)
  else ()
    set (UNSUPPORTED_ARCH ON)
  endif ()
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "powerpc|ppc")
  # TODO also check for RS/6000 processor. Do not know the value of
  # CMAKE_SYSTEM_PROCESSOR for those.
  # ?)
  set (MATHLIB_AIX ON)
else ()
  # TODO check for SUN system. How to check it ?
  set (UNSUPPORTED_ARCH ON)
endif ()

if (UNSUPPORTED_ARCH)
  message (FATAL_ERROR "The target system is not supported by MathLib")
endif ()


# src/mathlib_configuration.h.in -> src/mathlib_configuration.h
configure_file (src/mathlib_configuration.h.in src/mathlib_configuration.h)

# move endian.h into utils/
file (MAKE_DIRECTORY "src/utils")
file (RENAME "src/endian.h" "src/utils/endian.h")

# source and header file
foreach (file atnat2.c halfulp.c mplog.c slowexp2.c ucot.c upow.c atnat.c mpa.c
              mpsqrt.c slowexp.c uexp2.c urem.c branred.c mpatan2.c mptan.c
              slowlog2.c uexp.c uroot.c doasin.c mpatan.c sincos32.c slowpow.c
              ulog2.c usncs.c dosincos.c mpexp.c slowcot.c uasncs.c ulog.c
              utan.c DPChange.c atnat2.h doasin.h mpa.h mpsqrt.h uasncs.h ulog.h
              usncs.h atnat.h dosincos.h mpatan.h mydefs.h ucot.h upow.h utan.h
              branred.h mpexp.h sincos32.h uexp.h urem.h dla.h mpa2.h mplog.h
              trigo.h ulog2.h uroot.h LINUX_DPChange.h SUN_DPChange.h
              AIX_DPChange.h IX86MACOSX_DPChange.h AIX_endian.h IX86_endian.h
              SUN_endian.h DPChange.h BCB_endian.h)
  list (APPEND SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/${file}")
endforeach ()
list (APPEND SRCS "${CMAKE_CURRENT_BINARY_DIR}/src/mathlib_configuration.h")
list (APPEND SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/endian.h")

# Mathlib library is called ultim
add_library (ultim ${SRCS})
set_target_properties (ultim PROPERTIES VERSION 0.0.0 SOVERSION 0)

# Need source and binary dir (for src/mathlib_configuration.h) in include dirs
set (incdirs "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/src")
target_include_directories (ultim PUBLIC "$<BUILD_INTERFACE:${incdirs}>")
target_include_directories (ultim PRIVATE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/utils>")
target_include_directories (ultim PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# install headers
install (FILES "${CMAKE_CURRENT_BINARY_DIR}/src/mathlib_configuration.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/MathLib.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/src/mathlib_config.h"
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
          COMPONENT devtools)

# install library (and add it to ibexExport)
install(TARGETS ultim EXPORT ibexExport DESTINATION ${CMAKE_INSTALL_LIBDIR}
                                                            COMPONENT devtools)
