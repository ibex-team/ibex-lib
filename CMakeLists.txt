cmake_minimum_required (VERSION 3.5.1)
# CMake doc can be found at https://cmake.org/cmake/help/v3.5/

message (WARNING "CMake support is in beta, please report all bugs to https://github.com/ibex-team/ibex-lib/issues/")

project (IBEX VERSION 2.8.8 LANGUAGES CXX)
set (IBEX_DESCRIPTION "A C++ library for interval-based algorithm design")
set (IBEX_URL "http://www.ibex-lib.org/")

################################################################################
# Add cmake.utils to the list of CMAKE_MODULE_PATH
################################################################################
set (MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake.utils")
list (APPEND CMAKE_MODULE_PATH "${MODULE_DIR}")

################################################################################
# Run common init function and install the module so plugins can use it
################################################################################
message (STATUS "Configuring Ibex ${IBEX_VERSION}")
include (ibex-config-utils)
ibex_init_common ()

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
include (IbexUtils)
ibex_generate_pkgconfig_file (ibex)

################################################################################
# Tests
################################################################################
add_make_target_for_ctest (check)

################################################################################
# TODO benchs
################################################################################
#add_subdirectory (benchs)

################################################################################
# We add this file last in the hope that the install script will run it last.
# Specifically, we want this to be run after the line
# install (EXPORT ibex...
# from src/CMakeLists.txt because this line can, in some cases, remove files
# matching ${CMAKE_INSTALL_CONFIGCMAKE}/ibex-config-*.cmake even if they were
# just installed.
################################################################################
add_subdirectory (cmake.install)

################################################################################
# archives and packages
################################################################################
if (CMAKE_HOST_SYSTEM_NAME EQUAL "Linux")
  set (CPACK_GENERATOR "TGZ" "ZIP" "DEB")
else ()
  set (CPACK_GENERATOR "TGZ" "ZIP")
endif ()
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
