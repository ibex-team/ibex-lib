cmake_minimum_required (VERSION 3.5.1)
# CMake doc can be found at https://cmake.org/cmake/help/v3.5/

project (IBEX VERSION 2.9.1.0 LANGUAGES CXX)
set (IBEX_DESCRIPTION "A C++ library for interval-based algorithm design")
set (IBEX_URL "http://www.ibex-lib.org/")
set (IBEX_NAMESPACE "Ibex") # TODO use it everywhere
set (IBEX_CMAKE_PATH_FILES_PREFIX ibex-path-hints)

################################################################################
# Add cmake.utils to the list of CMAKE_MODULE_PATH
################################################################################
set (MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake.utils")
list (APPEND CMAKE_MODULE_PATH "${MODULE_DIR}")

################################################################################
# Run common init function (the module is installed later so plugins can use it)
################################################################################
message (STATUS "Configuring Ibex ${IBEX_VERSION}")
set (IBEX_DEV_UTILS_FILENAME ibex-dev-utils)
include (${IBEX_DEV_UTILS_FILENAME})
ibex_init_common ()

################################################################################
# Configure the libraries for interval arithmetic and linear programming
################################################################################

if (MSVC)
  message(STATUS "Forcing /fp:strict")
  # Filib looses correct rounding with /fp:fast or /fp:precise
  add_compile_options("/fp:strict")
endif()

add_subdirectory (interval_lib_wrapper)
add_subdirectory (lp_lib_wrapper)

#option (USING_IPOPT "Set to ON to use IpOpt" OFF)
#if (USING_IPOPT)
#  find_package (IpOpt)
#endif()

################################################################################
# Go to src subdirectory that builds the libibex target
################################################################################
add_subdirectory (src)

################################################################################
# Generate ibex.pc
################################################################################
include (ibex-gen-pkgconfig)
ibex_generate_pkgconfig_file (ibex)

################################################################################
# Install some CMake utils files that are use for detecting dependencies in
# cmake-config.cmake module and can be useful in plugins.
################################################################################
install (FILES "${MODULE_DIR}/${IBEX_DEV_UTILS_FILENAME}.cmake"
               "${MODULE_DIR}/FindCppUnit.cmake"
          DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}/cmake.utils
          COMPONENT devtools)

################################################################################
# Tests
################################################################################
ibex_add_make_target_for_ctest (check)

################################################################################
# TODO benchs
################################################################################
#add_subdirectory (benchs)

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
