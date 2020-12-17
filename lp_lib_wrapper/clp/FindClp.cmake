# FindClp
# -----------
#
# The module defines the following variables:
#
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

set (CLP_DIR "${CLP_DIR}" CACHE PATH "Directory to search for Clp")

# Look for the library
find_library (CLP_LIBRARY NAMES Clp HINTS "${CLP_DIR}" PATH_SUFFIXES lib)

# Might want to look close to the library first for the includes.
get_filename_component (_libdir "${CLP_LIBRARY}" PATH)

# Look for the include directory
find_path (CLP_INC_DIR NAMES ClpConfig.h
                         HINTS "${_libdir}/.." "${CLP_DIR}"
                         PATH_SUFFIXES include include/coin)

if (CLP_INC_DIR)
  file (READ "${CLP_INC_DIR}/ClpConfig.h" _content)
  string (REGEX MATCH "CLP_VERSION \"[0-9]+\.[0-9]+\.[0-9]+" _match "${_content}")
  string (SUBSTRING "${_match}" 13 -1 CLP_VERSION)
endif ()

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (Clp DEFAULT_MSG CLP_LIBRARY CLP_INC_DIR)

if (CLP_FOUND)
  set (CLP_LIBRARIES ${CLP_LIBRARY})
	set (CLP_INCLUDE_DIRS "${CLP_INC_DIR}")
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

mark_as_advanced(CLP_INC_DIR CLP_LIBRARY)
