# FindSoPlex
# -----------
#
# The module defines the following variables:
#
#     SOPLEX_FOUND
#     SOPLEX_INCLUDE_DIRS
#     SOPLEX_LIBRARIES
#
# and the following imported target (if it does not already exist):
#
#  SoPlex::SoPlex - The SoPlex library
#
#
# Requires CMake >= 3.0

include(CheckCXXCompilerFlag)

set (SOPLEX_DIR "${SOPLEX_DIR}" CACHE PATH "Directory to search for SoPlex")

# Look for the library
find_library (SOPLEX_LIBRARY NAMES soplex HINTS "${SOPLEX_DIR}"
                                          PATH_SUFFIXES lib)

# Might want to look close to the library first for the includes.
get_filename_component (_libdir "${SOPLEX_LIBRARY}" PATH)

# Look for the include directory
find_path (SOPLEX_INC_DIR NAMES soplex.h
                         HINTS "${_libdir}/.." "${SOPLEX_DIR}"
                         PATH_SUFFIXES include)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (SoPlex DEFAULT_MSG SOPLEX_LIBRARY
                                                      SOPLEX_INC_DIR)

if (SOPLEX_FOUND)
	set (SOPLEX_LIBRARIES ${SOPLEX_LIBRARY})
	set (SOPLEX_INCLUDE_DIRS "${SOPLEX_INC_DIR}")
  mark_as_advanced (SOPLEX_DIR)
  if (NOT TARGET SoPlex::SoPlex)
    # For now we make the target global, because this file is included from a
    # CMakeLists.txt file in a subdirectory. With CMake >= 3.11, we could make
    # it global afterwards with
    # set_target_properties(SoPlex::SoPlex PROPERTIES IMPORTED_GLOBAL TRUE)
    add_library (SoPlex::SoPlex INTERFACE IMPORTED GLOBAL)
    set_target_properties (SoPlex::SoPlex PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${SOPLEX_INCLUDE_DIRS}"
                        INTERFACE_LINK_LIBRARIES "${SOPLEX_LIBRARIES}")
  endif()
endif()

mark_as_advanced(SOPLEX_INC_DIR SOPLEX_LIBRARY)
