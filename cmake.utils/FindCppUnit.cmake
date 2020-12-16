# FindCppUnit
# -----------
#
# The module defines the following variables:
#
#     CPPUNIT_FOUND
#     CPPUNIT_INCLUDE_DIRS
#     CPPUNIT_LIBRARIES
#
# and the following imported target (if it does not already exist):
#
#  CppUnit::Cppunit - The CppUnit library
#
#
# Requires CMake >= 3.0

set (CPPUNIT_DIR "${CPPUNIT_DIR}" CACHE PATH "Directory to search for CppUnit")

# Look for the release library
find_library (CPPUNIT_LIBRARY_RELEASE NAMES cppunit
                                      HINTS "${CPPUNIT_DIR}"
                                      PATH_SUFFIXES lib)
# Look for the debug library
find_library (CPPUNIT_LIBRARY_DEBUG NAMES cppunitd
                                    HINTS "${CPPUNIT_DIR}"
                                    PATH_SUFFIXES lib)
# Select the correct library depending on current configuration
include (SelectLibraryConfigurations)
select_library_configurations (CPPUNIT)

# Might want to look close to the library first for the includes.
get_filename_component (_libdir "${CPPUNIT_LIBRARY}" PATH)

# Look for the include directory
find_path (CPPUNIT_INC_DIR NAMES cppunit/Test.h
                           HINTS "${_libdir}/.." "${CPPUNIT_DIR}"
                           PATH_SUFFIXES include)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (CppUnit DEFAULT_MSG CPPUNIT_LIBRARY
                                                       CPPUNIT_INC_DIR)

if (CPPUNIT_FOUND)
	set (CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY} ${CMAKE_DL_LIBS})
	set (CPPUNIT_INCLUDE_DIRS "${CPPUNIT_INC_DIR}")
  mark_as_advanced (CPPUNIT_DIR)
  if (NOT TARGET CppUnit::CppUnit)
    add_library (CppUnit::CppUnit INTERFACE IMPORTED)
    set_target_properties (CppUnit::CppUnit PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES "${CPPUNIT_INCLUDE_DIRS}"
                        INTERFACE_LINK_LIBRARIES "${CPPUNIT_LIBRARIES}")
  endif()
endif()

mark_as_advanced(CPPUNIT_INC_DIR CPPUNIT_LIBRARY_RELEASE CPPUNIT_LIBRARY_DEBUG)
