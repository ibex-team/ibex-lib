#set (PACKAGE coinutils)
#set (PACKAGE_BUGREPORT "http://projects.coin-or.org/CoinUtils")
#set (PACKAGE_STRING "${PACKAGE_NAME} ${CLP_VERSION}")
#set (PACKAGE_TARNAME coinutils)
#set (PACKAGE_VERSION ${CLP_VERSION})
set (CLP_VERSION_RELEASE ${CLP_MICRO_VERSION})

set (CLP_HAS_ABC OFF)
set (COIN_HAS_COINUTILS ON)
set (COIN_HAS_OSI ON)

set (COIN_CLP_CHECKLEVEL "0" CACHE INTERNAL "Clp check level")
set (COIN_CLP_VERBOSITY "0" CACHE INTERNAL "Clp verbosity level")

# Configure file
configure_file (src/config_clp.h.in src/config_clp.h @ONLY)
configure_file (src/config.h.in src/config.h @ONLY)

# We DO NOT follow advice from https://cmake.org/cmake/help/v3.0/command/file.html,
# and we use the command FILE (GLOB ...) to find all headers and sources files
# because we assume that no file will be added in this directory.
# If one wants to use this CMakeLists.txt file to develop Filib, one can expand
# theses variables by hand as done in ibex/src/*/CMakeLists.txt files.
file (GLOB_RECURSE SRCS src/*.cpp)
# TODO remove OsiClpSolverInterface ??
file (GLOB_RECURSE HEADERS src/*.h src/*.hpp)

if (NOT COIN_HAS_AMD AND NOT COIN_HAS_CHOLMOD AND NOT COIN_HAS_GLPK)
  file (GLOB_RECURSE _rem src/ClpCholeskyUfl.*)
  list (REMOVE_ITEM SRCS ${_rem})
  list (REMOVE_ITEM HEADERS ${_rem})
endif ()
if (NOT COIN_HAS_MUMPS)
  file (GLOB_RECURSE _rem src/ClpCholeskyMumps.*)
  list (REMOVE_ITEM SRCS ${_rem})
  list (REMOVE_ITEM HEADERS ${_rem})
endif ()
if (NOT COIN_HAS_WSMP)
  file (GLOB_RECURSE _rem src/ClpCholeskyWssmp*.*)
  list (REMOVE_ITEM SRCS ${_rem})
  list (REMOVE_ITEM HEADERS ${_rem})
endif ()

if (NOT COIN_HAS_ABC)
  file (GLOB_RECURSE _rem src/Abc* src/CoinAbc*)
  list (REMOVE_ITEM SRCS ${_rem})
  list (REMOVE_ITEM HEADERS ${_rem})
endif ()

# Clp and OsiClp libraries
add_library (Clp ${SRCS} ${HEADERS})
add_library (OsiClp src/OsiClp/OsiClpSolverInterface.cpp
                    src/OsiClp/OsiClpSolverInterface.hpp)

target_link_libraries (Clp PUBLIC CoinUtils Osi)
target_link_libraries (OsiClp PUBLIC CoinUtils Osi)

# define some macros
if (HAVE_CMATH)
  target_compile_definitions (Clp PRIVATE HAVE_CMATH)
endif ()
if (HAVE_MATH_H)
  target_compile_definitions (Clp PRIVATE HAVE_MATH_H)
endif ()
target_compile_definitions (Clp PRIVATE COIN_HAS_CLP)

# include directories
set (incdirs "${CMAKE_CURRENT_SOURCE_DIR}/src"
             "${CMAKE_CURRENT_BINARY_DIR}/src")
target_include_directories (Clp PUBLIC "$<BUILD_INTERFACE:${incdirs}>")
target_include_directories (Clp PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

list (APPEND incdirs "${CMAKE_CURRENT_SOURCE_DIR}/src/OsiClp")
target_include_directories (OsiClp PUBLIC "$<BUILD_INTERFACE:${incdirs}>")
target_include_directories (OsiClp PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# install library (and add it to ibexExport)
install (TARGETS Clp
         EXPORT ibexExport
         DESTINATION ${CMAKE_INSTALL_LIBDIR}
         COMPONENT devtools)

# install headers
install (FILES ${HEADERS}
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coin
         COMPONENT devtools)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/src/config_clp.h
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coin
         RENAME ClpConfig.h
         COMPONENT devtools)
