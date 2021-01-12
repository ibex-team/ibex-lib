set (COINUTILS_VERSION_MAJOR 2)
set (COINUTILS_VERSION_MINOR 11)
set (COINUTILS_VERSION_RELEASE 4)
set (COINUTILS_VERSION ${COINUTILS_VERSION_MAJOR}.${COINUTILS_VERSION_MINOR}.${COINUTILS_VERSION_RELEASE})
set (VERSION ${COINUTILS_VERSION})
set (PACKAGE coinutils)
set (PACKAGE_BUGREPORT "http://projects.coin-or.org/CoinUtils")
set (PACKAGE_NAME CoinUtils)
set (PACKAGE_STRING "${PACKAGE_NAME} ${COINUTILS_VERSION}")
set (PACKAGE_TARNAME coinutils)
set (PACKAGE_VERSION ${COINUTILS_VERSION})

#
#if(PTHREADS_FOUND)
#  set(${NAME}_PTHREADS "1" CACHE INTERNAL "Use pthread")
#endif()

set (COIN_COINUTILS_CHECKLEVEL "0" CACHE INTERNAL "COINUTILS check level")
set (COIN_COINUTILS_VERBOSITY "0" CACHE INTERNAL "COINUTILS verbosity level")

set (COINUTILS_HAS_CSTDINT ${HAVE_CSTDINT})
set (COINUTILS_HAS_STDINT_H ${HAVE_STDINT_H})

# Configure file
configure_file (src/config_coinutils.h.in src/config_coinutils.h @ONLY)
configure_file (src/config.h.in src/config.h @ONLY)

# We DO NOT follow advice from https://cmake.org/cmake/help/v3.0/command/file.html,
# and we use the command FILE (GLOB ...) to find all headers and sources files
# because we assume that no file will be added in this directory.
# If one wants to use this CMakeLists.txt file to develop Clp, one can expand
# theses variables by hand as done in ibex/src/*/CMakeLists.txt files.
file (GLOB_RECURSE SRCS src/*.cpp)
file (GLOB_RECURSE HEADERS src/*.hpp)

# CoinUtils library
add_library (CoinUtils ${SRCS} ${HEADERS})
set_target_properties (CoinUtils PROPERTIES VERSION 3.11.4 SOVERSION 3)

# define some macros
target_compile_definitions (CoinUtils PUBLIC HAVE_CONFIG_H
                                      PRIVATE COINUTILS_BUILD)

# include directories
set (incdirs "${CMAKE_CURRENT_SOURCE_DIR}/src"
             "${CMAKE_CURRENT_BINARY_DIR}/src"
             "${CMAKE_CURRENT_SOURCE_DIR}/../BuildTools/headers")
target_include_directories (CoinUtils PUBLIC "$<BUILD_INTERFACE:${incdirs}>")
target_include_directories (CoinUtils PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# install library (and add it to ibexExport)
install (TARGETS CoinUtils
         EXPORT ibexExport
         DESTINATION ${CMAKE_INSTALL_LIBDIR}
         COMPONENT devtools)

# install headers
install (FILES ${HEADERS}
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         COMPONENT devtools)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/src/config_coinutils.h
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
         RENAME CoinUtilsConfig.h
         COMPONENT devtools)
