set (OSI_VERSION_MAJOR 0)
set (OSI_VERSION_MINOR 108)
set (OSI_VERSION_RELEASE 6)
set (OSI_VERSION ${OSI_VERSION_MAJOR}.${OSI_VERSION_MINOR}.${OSI_VERSION_RELEASE})
set (VERSION ${OSI_VERSION})
set (PACKAGE osi)
set (PACKAGE_BUGREPORT "osi@list.coin-or.org")
set (PACKAGE_NAME Osi)
set (PACKAGE_STRING "${PACKAGE_NAME} ${OSI_VERSION}")
set (PACKAGE_TARNAME osi)
set (PACKAGE_VERSION ${OSI_VERSION})

set (COIN_OSI_CHECKLEVEL "0" CACHE INTERNAL "OSI check level")
set (COIN_OSI_VERBOSITY "0" CACHE INTERNAL "OSI verbosity level")

set (COIN_HAS_COINUTILS 1)

set (COIN_HAS_XPR 0)
set (COIN_HAS_CPX 0)
set (COIN_HAS_GLPK 0)
set (COIN_HAS_GRB 0)
set (COIN_HAS_MSK 0)
set (COIN_HAS_SOPLEX 0)

# Configure file
configure_file (src/Osi/config_osi.h.in src/Osi/config_osi.h @ONLY)
configure_file (src/Osi/config.h.in src/Osi/config.h @ONLY)

# We DO NOT follow advice from https://cmake.org/cmake/help/v3.0/command/file.html,
# and we use the command FILE (GLOB ...) to find all headers and sources files
# because we assume that no file will be added in this directory.
# If one wants to use this CMakeLists.txt file to develop Filib, one can expand
# theses variables by hand as done in ibex/src/*/CMakeLists.txt files.
file (GLOB_RECURSE SRCS src/Osi/*.cpp)
file (GLOB_RECURSE HEADERS src/Osi/*.hpp)

# Osi library
add_library (Osi ${SRCS} ${HEADERS})

# include directories
set (incdirs "${CMAKE_CURRENT_SOURCE_DIR}/src"
             "${CMAKE_CURRENT_SOURCE_DIR}/src/Osi"
             "${CMAKE_CURRENT_SOURCE_DIR}/../CoinUtils/src"
             "${CMAKE_CURRENT_BINARY_DIR}/src/Osi")
target_include_directories (Osi PUBLIC "$<BUILD_INTERFACE:${incdirs}>")
target_include_directories (Osi PUBLIC "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

#if (WIN32)
#  target_compile_definitions (Osi PRIVATE -D_CRT_SECURE_NO_WARNINGS
#                                                -D_CRT_SECURE_NO_DEPRECATE)
#endif ()

# install library (and add it to ibexExport)
install (TARGETS Osi
         EXPORT ibexExport
         DESTINATION ${CMAKE_INSTALL_LIBDIR}
         COMPONENT devtools)

# install headers
install (FILES ${HEADERS}
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coin
         COMPONENT devtools)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/src/Osi/config_osi.h
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/coin
         RENAME OsiConfig.h
         COMPONENT devtools)
