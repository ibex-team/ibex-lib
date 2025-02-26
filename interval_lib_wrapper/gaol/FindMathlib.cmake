# FindMathlib
# -----------
#
# The module defines the following variables:
#
#     MATHLIB_FOUND
#     MATHLIB_INCLUDE_DIRS
#     MATHLIB_LIBRARIES
#     MATHLIB_VERSION
#
# and the following imported target (if it does not already exist):
#
#  Mathlib::Mathlib - The Mathlib library
#
#
# Requires CMake >= 3.0

include(CheckCXXCompilerFlag)

set (MATHLIB_DIR "${MATHLIB_DIR}" CACHE PATH "Directory to search for Mathlib")

# Look for the library
find_library (MATHLIB_LIBRARY NAMES ultim HINTS "${MATHLIB_DIR}"
                                          PATH_SUFFIXES lib)

# Might want to look close to the library first for the includes.
get_filename_component (_libdir "${MATHLIB_LIBRARY}" PATH)

# Look for the include directory
find_path (MATHLIB_INC_DIR NAMES MathLib.h
                         HINTS "${_libdir}/.." "${MATHLIB_DIR}"
                         PATH_SUFFIXES include)

if (MATHLIB_INC_DIR)
  file (READ "${MATHLIB_INC_DIR}/mathlib_configuration.h" _content)
  string (REGEX MATCH "mathlib [0-9]+[.][0-9]+[.][0-9]+" _match "${_content}")
  string (SUBSTRING "${_match}" 8 -1 MATHLIB_VERSION)
endif ()

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (Mathlib
                                   REQUIRED_VARS MATHLIB_LIBRARY MATHLIB_INC_DIR
                                   VERSION_VAR MATHLIB_VERSION)

if (MATHLIB_FOUND)
	set (MATHLIB_LIBRARIES ${MATHLIB_LIBRARY})
	set (MATHLIB_INCLUDE_DIRS "${MATHLIB_INC_DIR}")
  mark_as_advanced (MATHLIB_DIR)
  if (NOT TARGET Mathlib::Mathlib)
    # For now we make the target global, because this file is included from a
    # CMakeLists.txt file in a subdirectory. With CMake >= 3.11, we could make
    # it global afterwards with
    # set_target_properties(Mathlib::Mathlib PROPERTIES IMPORTED_GLOBAL TRUE)
    add_library (Mathlib::Mathlib INTERFACE IMPORTED GLOBAL)
    set_target_properties (Mathlib::Mathlib PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${MATHLIB_INCLUDE_DIRS}"
                        INTERFACE_LINK_LIBRARIES "${MATHLIB_LIBRARIES}")
  endif()
endif()

mark_as_advanced(MATHLIB_INC_DIR MATHLIB_LIBRARY)
