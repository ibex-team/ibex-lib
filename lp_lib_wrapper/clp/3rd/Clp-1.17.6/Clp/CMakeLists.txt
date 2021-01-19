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
# If one wants to use this CMakeLists.txt file to develop Clp, one can expand
# theses variables by hand as done in ibex/src/*/CMakeLists.txt files.
file (GLOB_RECURSE SRCS src/*.cpp)
file (GLOB_RECURSE HEADERS src/*.hpp)

set (SRCS_SOLVER src/ClpSolver.cpp src/CbcOrClpParam.cpp src/MyEventHandler.cpp
	               src/MyMessageHandler.cpp src/unitTest.cpp src/Clp_ampl.cpp)
set (HEADERS_SOLVER src/CbcOrClpParam.hpp src/MyEventHandler.hpp
                    src/MyMessageHandler.hpp src/Clp_ampl.h)
list (REMOVE_ITEM SRCS ${SRCS_SOLVER})
list (REMOVE_ITEM HEADERS ${HEADERS_SOLVER})

# remove file from libClpSolver and other unused files
file (GLOB_RECURSE _rem src/ClpCholeskyTaucs.* src/OsiClp/*)

list (REMOVE_ITEM SRCS ${_rem})
list (REMOVE_ITEM HEADERS ${_rem})

# remove other files
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
if (NOT COIN_HAS_ASL)
  file (GLOB_RECURSE _rem src/Clp_ampl.*)
  list (REMOVE_ITEM SRCS ${_rem})
  list (REMOVE_ITEM HEADERS ${_rem})
endif ()

# Clp and ClpSolver libraries
add_library (Clp ${SRCS} ${HEADERS})
set_target_properties (Clp PROPERTIES VERSION 1.14.6 SOVERSION 1)
add_library (ClpSolver ${SRCS_SOLVER} ${HEADERS_SOLVER})
set_target_properties (ClpSolver PROPERTIES VERSION 1.14.6 SOVERSION 1)

target_link_libraries (Clp PUBLIC CoinUtils)
target_link_libraries (ClpSolver PUBLIC Clp)

# define some macros
if (HAVE_CMATH)
  target_compile_definitions (Clp PRIVATE HAVE_CMATH)
  target_compile_definitions (ClpSolver PRIVATE HAVE_CMATH)
endif ()
if (HAVE_MATH_H)
  target_compile_definitions (Clp PRIVATE HAVE_MATH_H)
  target_compile_definitions (ClpSolver PRIVATE HAVE_MATH_H)
endif ()
target_compile_definitions (Clp PRIVATE COIN_HAS_CLP)
target_compile_definitions (ClpSolver PRIVATE COIN_HAS_CLP)

# include directories
set (incdirs "${CMAKE_CURRENT_SOURCE_DIR}/src"
             "${CMAKE_CURRENT_BINARY_DIR}/src"
             "${CMAKE_CURRENT_BINARY_DIR}/../Osi/src/Osi"
             "${CMAKE_CURRENT_BINARY_DIR}/../CoinUtils/src")
target_include_directories (Clp PUBLIC "$<BUILD_INTERFACE:${incdirs}>")
target_include_directories (Clp PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# install library (and add it to ibexExport)
install (TARGETS Clp ClpSolver
         EXPORT ibexExport
         DESTINATION ${CMAKE_INSTALL_LIBDIR}
         COMPONENT devtools)

# install headers
install (FILES ${HEADERS}
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT devtools)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/src/config_clp.h
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         RENAME ClpConfig.h
         COMPONENT devtools)
