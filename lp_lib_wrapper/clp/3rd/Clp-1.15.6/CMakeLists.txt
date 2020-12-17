cmake_minimum_required (VERSION 3.5.1)

set (CLP_MAJOR_VERSION 1)
set (CLP_MINOR_VERSION 15)
set (CLP_MICRO_VERSION 6)
set (CLP_VERSION "${CLP_MAJOR_VERSION}.${CLP_MINOR_VERSION}.${CLP_MICRO_VERSION}")
project (CLP VERSION ${CLP_VERSION} LANGUAGES CXX)

set (PACKAGE_NAME "Clp")
set (PACKAGE_BUGREPORT "clp@list.coin-or.org")
set (PACKAGE_STRING "${PACKAGE_NAME} ${CLP_VERSION}")
set (PACKAGE_TARNAME "clp")
set (PACKAGE_VERSION ${CLP_VERSION})

message (STATUS "Running on system ${CMAKE_HOST_SYSTEM} with processor ${CMAKE_HOST_SYSTEM_PROCESSOR}")
if (CMAKE_CROSSCOMPILING)
  message (STATUS "Targeting system ${CMAKE_SYSTEM} with processor ${CMAKE_SYSTEM_PROCESSOR}")
endif ()

# options and command-line arguments
option (BUILD_SHARED_LIBS "Set to ON to build shared libraries" OFF)
set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")

message (FATAL_ERROR "Automatic installation of Clp not yet supported")
