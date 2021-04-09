# FindClp
# -----------
#
# The module defines the following variables:
#
#     COINUTILS_FOUND
#     COINUTILS_INCLUDE_DIRS
#     COINUTILS_LIBRARIES
#     COINUTILS_VERSION
#     CLP_FOUND
#     CLP_INCLUDE_DIRS
#     CLP_LIBRARIES
#     CLP_VERSION
#
# and the following imported target (if it does not already exist):
#
#  Clp::Clp - The Clp library
#
#
# Requires CMake >= 3.0

include(CheckCXXCompilerFlag)
find_package (PkgConfig)

set (COINUTILS_DIR "${COIN_DIR}" CACHE PATH "Directory to search for CoinUtils")
set (CLP_DIR "${CLP_DIR}" CACHE PATH "Directory to search for Clp")

if (COINUTILS_DIR)
  set (ENV{PKG_CONFIG_PATH} "${COINUTILS_DIR}/lib/pkgconfig:${PKG_CONFIG_PATH}")
endif ()
if (CLP_DIR)
  set (ENV{PKG_CONFIG_PATH} "${CLP_DIR}/lib/pkgconfig:${PKG_CONFIG_PATH}")
endif ()

pkg_check_modules (CLP_PKG QUIET clp)

# Look for the library
foreach (_lib ${CLP_PKG_LIBRARIES})
  string (TOUPPER "${_lib}" _upperlib)
  find_library (${_upperlib}_LIBRARY NAMES ${_lib}
                                     HINTS ${CLP_PKG_LIBRARY_DIRS}
                                     PATH_SUFFIXES lib)
  list (APPEND _req_vars ${_upperlib}_LIBRARY)
  list (APPEND CLP_LIBRARIES "${${_upperlib}_LIBRARY}")
  mark_as_advanced (${_upperlib}_LIBRARY)
  unset (_upperlib)
endforeach ()
unset (_lib)

# Look for the include directory
find_path (COINUTILS_INC_DIR NAMES CoinUtilsConfig.h
                             HINTS ${CLP_PKG_INCLUDE_DIRS}
                             PATH_SUFFIXES include include/coin)
find_path (CLP_INC_DIR NAMES ClpConfig.h
                       HINTS ${CLP_PKG_INCLUDE_DIRS}
                       PATH_SUFFIXES include include/coin)

# Look for the version
set (_version_match "VERSION \"[0-9]+[.][0-9]+[.][0-9]+")
if (COINUTILS_INC_DIR)
  file (READ "${COINUTILS_INC_DIR}/CoinUtilsConfig.h" _content)
  string (REGEX MATCH "COINUTILS_${_version_match}" _match "${_content}")
  string (SUBSTRING "${_match}" 19 -1 COINUTILS_VERSION)
endif ()

if (CLP_INC_DIR)
  file (READ "${CLP_INC_DIR}/ClpConfig.h" _content)
  string (REGEX MATCH "CLP_${_version_match}" _match "${_content}")
  string (SUBSTRING "${_match}" 13 -1 CLP_VERSION)
endif ()
unset (_content)
unset (_match)
unset (_version_match)

include (FindPackageHandleStandardArgs)
list (APPEND _req_vars CLP_INC_DIR)
list (APPEND _req_vars COINUTILS_INC_DIR)
find_package_handle_standard_args (Clp REQUIRED_VARS ${_req_vars}
                                       VERSION_VAR CLP_VERSION)
unset (_req_vars)

if (CLP_FOUND)
  set (CLP_INCLUDE_DIRS ${CLP_PKG_INCLUDE_DIRS})
  mark_as_advanced (COINUTILS_DIR)
  mark_as_advanced (CLP_DIR)
  if (NOT TARGET Clp::Clp)
    # For now we make the target global, because this file is included from a
    # CMakeLists.txt file in a subdirectory. With CMake >= 3.11, we could make
    # it global afterwards with
    # set_target_properties(Clp::Clp PROPERTIES IMPORTED_GLOBAL TRUE)
    add_library (Clp::Clp INTERFACE IMPORTED GLOBAL)
    set_target_properties (Clp::Clp PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${CLP_INCLUDE_DIRS}"
                        INTERFACE_LINK_LIBRARIES "${CLP_LIBRARIES}")
  endif()
endif()

mark_as_advanced (CLP_INC_DIR COINUTILS_INC_DIR)
