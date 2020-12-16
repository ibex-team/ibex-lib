# FindFilib
# -----------
#
# The module defines the following variables:
#
#     FILIB_FOUND
#     FILIB_INCLUDE_DIRS
#     FILIB_LIBRARIES
#
# and the following imported target (if it does not already exist):
#
#  Filib::Filib - The Filib library
#
#
# Requires CMake >= 3.0

include(CheckCXXCompilerFlag)

set (FILIB_DIR "${FILIB_DIR}" CACHE PATH "Directory to search for Filib")

# Look for the library
find_library (FILIB_LIBRARY NAMES prim HINTS "${FILIB_DIR}" PATH_SUFFIXES lib)

# Might want to look close to the library first for the includes.
get_filename_component (_libdir "${FILIB_LIBRARY}" PATH)

# Look for the include directory
find_path (FILIB_INC_DIR NAMES interval/interval.hpp
                         HINTS "${_libdir}/.." "${FILIB_DIR}"
                         PATH_SUFFIXES include)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (Filib DEFAULT_MSG FILIB_LIBRARY
                                                     FILIB_INC_DIR)

if (FILIB_FOUND)
	set (FILIB_LIBRARIES ${FILIB_LIBRARY})
	set (FILIB_INCLUDE_DIRS "${FILIB_INC_DIR}")
  mark_as_advanced (FILIB_DIR)
  if (NOT TARGET Filib::Filib)
    # For now we make the target global, because this file is included from a
    # CMakeLists.txt file in a subdirectory. With CMake >= 3.11, we could make
    # it global afterwards with
    # set_target_properties(Filib::Filib PROPERTIES IMPORTED_GLOBAL TRUE)
    add_library (Filib::Filib INTERFACE IMPORTED GLOBAL)
    set_target_properties (Filib::Filib PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${FILIB_INCLUDE_DIRS}"
                        INTERFACE_LINK_LIBRARIES "${FILIB_LIBRARIES}")

    # Checking for -frounding-math and -ffloat-store
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag ("-frounding-math" COMPILER_SUPPORTS_FROUNDING_MATH)
    if (COMPILER_SUPPORTS_FROUNDING_MATH)
      list (APPEND FILIB_COMPILE_OPTIONS "-frounding-math")
    endif ()
    if (NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
      check_cxx_compiler_flag ("-ffloat-store" COMPILER_SUPPORTS_FFLOAT_STORE)
      if (COMPILER_SUPPORTS_FROUNDING_MATH)
        list (APPEND FILIB_COMPILE_OPTIONS "-ffloat-store")
      endif ()
    endif ()
    set_target_properties (Filib::Filib PROPERTIES
                          INTERFACE_COMPILE_OPTIONS "${FILIB_COMPILE_OPTIONS}")
  endif()
endif()

mark_as_advanced(FILIB_INC_DIR FILIB_LIBRARY)
