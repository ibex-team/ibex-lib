# FindIpOpt
# -----------
#
# The module defines the following variables:
#
#     IPOPT_FOUND
#     IPOPT_LIBRARIES
#     IPOPT_LIBRARY_DIRS
#     IPOPT_LDFLAGS
#     IPOPT_LDFLAGS_OTHER
#     IPOPT_INCLUDE_DIRS
#     IPOPT_CFLAGS
#     IPOPT_CFLAGS_OTHER
#     IPOPT_VERSION
#
# and the following imported target (if it does not already exist):
#
#  IpOpt::IpOpt - The IpOpt library
#
#
# Requires CMake >= 3.0

include(CheckCXXCompilerFlag)
find_package (PkgConfig)

set (COINUTILS_DIR "${COIN_DIR}" CACHE PATH "Directory to search for CoinUtils")
set (IPOPT_DIR "${IPOPT_DIR}" CACHE PATH "Directory to search for IpOpt")

if (COINUTILS_DIR)
  set (ENV{PKG_CONFIG_PATH} "${COINUTILS_DIR}/lib/pkgconfig:${PKG_CONFIG_PATH}")
endif ()
if (IPOPT_DIR)
  set (ENV{PKG_CONFIG_PATH} "${IPOPT_DIR}/lib/pkgconfig:${PKG_CONFIG_PATH}")
endif ()

pkg_check_modules (IPOPT REQUIRED QUIET ipopt>=3)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (IpOpt REQUIRED_VARS IPOPT_FOUND
                                         VERSION_VAR IPOPT_VERSION)

if (IPOPT_FOUND)
  mark_as_advanced (COINUTILS_DIR)
  mark_as_advanced (IPOPT_DIR)
  if (NOT TARGET IpOpt::IpOpt)
    # For now we make the target global, because this file is included from a
    # CMakeLists.txt file in a subdirectory. With CMake >= 3.11, we could make
    # it global afterwards with
    # set_target_properties(IpOpt::IpOpt PROPERTIES IMPORTED_GLOBAL TRUE)
    add_library (IpOpt::IpOpt INTERFACE IMPORTED GLOBAL)
    set_target_properties (IpOpt::IpOpt PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${IPOPT_INCLUDE_DIRS}"
                        INTERFACE_COMPILE_OPTIONS "${IPOPT_CFLAGS_OTHER}"
                        INTERFACE_LINK_LIBRARIES "${IPOPT_LDFLAGS}")
  endif()
endif()
