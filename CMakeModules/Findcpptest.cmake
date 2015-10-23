# - Try to find CPPTEST
# Once done this will define
#  LIBXML2_FOUND - System has LibXml2
#  LIBXML2_INCLUDE_DIRS - The LibXml2 include directories
#  LIBXML2_LIBRARIES - The libraries needed to use LibXml2
#  LIBXML2_DEFINITIONS - Compiler switches required for using LibXml2

find_package(PkgConfig)
pkg_check_modules(PC_CPPTEST QUIET cpptest)
message(STATUS "CPPTEST_DIR ${CPPTEST_DIR}")
set(CPPTEST_DEFINITIONS ${PC_CPPTESTCFLAGS_OTHER})
find_path(CPPTEST_INCLUDE_DIR cpptest.h
          HINTS ${PC_CPPTESTINCLUDEDIR} ${PC_CPPTESTINCLUDE_DIRS} ${CPPTEST_DIR}
          PATH_SUFFIXES cpptest include)

find_library(CPPTEST_LIBRARY NAMES cpptest
             HINTS ${PC_CPPTESTLIBDIR} ${PC_CPPTESTLIBRARY_DIRS}  ${CPPTEST_DIR}
			 PATH_SUFFIXES lib64
			 )

set(CPPTEST_LIBRARIES ${CPPTEST_LIBRARY} )
set(CPPTEST_INCLUDE_DIRS ${CPPTEST_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CPPTEST_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(CPPTEST  DEFAULT_MSG
                                  CPPTEST_LIBRARY CPPTEST_INCLUDE_DIR)

mark_as_advanced(CPPTEST_INCLUDE_DIR CPPTEST_LIBRARY )
