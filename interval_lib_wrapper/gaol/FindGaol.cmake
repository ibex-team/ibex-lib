# FindGaol
# -----------
#
# The module defines the following variables:
#
#     GAOL_FOUND
#     GAOL_INCLUDE_DIRS
#     GAOL_LIBRARIES
#     GAOL_VERSION
#
# and the following imported target (if it does not already exist):
#
#  Gaol::Gaol - The Gaol library
#
#
# Requires CMake >= 3.0

include(CheckCXXCompilerFlag)

set (GAOL_DIR "${GAOL_DIR}" CACHE PATH "Directory to search for Gaol")

# Look for the library
find_library (GAOL_LIBRARY NAMES gaol HINTS "${GAOL_DIR}" PATH_SUFFIXES lib)

# Might want to look close to the library first for the includes.
get_filename_component (_libdir "${GAOL_LIBRARY}" PATH)

# Look for the include directory
find_path (GAOL_INC_DIR NAMES gaol/gaol.h
                         HINTS "${_libdir}/.." "${_libdir2}/.." "${GAOL_DIR}"
                         PATH_SUFFIXES include)

if (GAOL_INC_DIR)
  file (READ "${GAOL_INC_DIR}/gaol/gaol_configuration.h" _content)
  string (REGEX MATCH "gaol \"[0-9]+[.][0-9]+[.][0-9]+" _match "${_content}")
  string (SUBSTRING "${_match}" 6 -1 GAOL_VERSION)
endif ()

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (Gaol
                                   REQUIRED_VARS GAOL_LIBRARY GAOL_INC_DIR
                                   VERSION_VAR GAOL_VERSION)

if (GAOL_FOUND)
  set (GAOL_LIBRARIES ${GAOL_LIBRARY})
  set (GAOL_INCLUDE_DIRS "${GAOL_INC_DIR}")
  mark_as_advanced (GAOL_DIR)
  if (NOT TARGET Gaol::Gaol)
    # For now we make the target global, because this file is included from a
    # CMakeLists.txt file in a subdirectory. With CMake >= 3.11, we could make
    # it global afterwards with
    # set_target_properties(Gaol::Gaol PROPERTIES IMPORTED_GLOBAL TRUE)
    add_library (Gaol::Gaol INTERFACE IMPORTED GLOBAL)
    set_target_properties (Gaol::Gaol PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${GAOL_INCLUDE_DIRS}"
                        INTERFACE_LINK_LIBRARIES "${GAOL_LIBRARIES}")
  endif()
endif()

mark_as_advanced(GAOL_INC_DIR GAOL_LIBRARY)
