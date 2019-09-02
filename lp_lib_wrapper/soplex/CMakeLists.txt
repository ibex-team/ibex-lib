################################################################################
# Options
################################################################################
set (SOPLEX_DIR "" CACHE PATH "Path to the Soplex lib and include directories")

################################################################################
# May be needed to install missing dependencies
################################################################################
include (ExternalProject)
include (Utils3rd)

# Looking for soplex.h
set (SOPLEX_HDR "soplex.h")
set (MSG "Looking for ${SOPLEX_HDR}")
message (STATUS "${MSG}")
find_path (soplex_INCLUDE_DIR ${SOPLEX_HDR} HINTS ${SOPLEX_DIR}
           PATH_SUFFIXES include
           DOC "Set to exact include directory to bypass internal test")
if (soplex_INCLUDE_DIR)
  message (STATUS "${MSG} -- found at ${soplex_INCLUDE_DIR}")
  if (soplex_INCLUDE_DIR MATCHES "^${SOPLEX_DIR}")
    set (soplex_INCLUDE_DIR ${soplex_INCLUDE_DIR})
  else ()
    set (soplex_INCLUDE_DIR $<BUILD_INTERFACE:${soplex_INCLUDE_DIR}>)
  endif ()
else()
  message (STATUS "${MSG} -- not found")
endif()

# Looking for libsoplex
set (SOPLEX_LIB "soplex")
set (MSG "Looking for ${SOPLEX_LIB}")
message (STATUS "${MSG}")
find_library (soplex_LIBRARY NAMES ${SOPLEX_LIB} HINTS ${SOPLEX_DIR}
              PATH_SUFFIXES lib
              DOC "Set to exact lib directory to bypass internal test")
if (soplex_LIBRARY)
  message (STATUS "${MSG} -- found at ${soplex_LIBRARY}")
  set (_tmp "$<BUILD_INTERFACE:${soplex_LIBRARY}>$<INSTALL_INTERFACE:")
  if (soplex_LIBRARY MATCHES "^${SOPLEX_DIR}")
    get_filename_component (_libdir "${soplex_LIBRARY}" DIRECTORY)
    set (_tmp "${_tmp}-L${_libdir} ")
  endif ()
  set (soplex_LIBRARY "${_tmp}-lsoplex>")
else()
  message (STATUS "${MSG} -- not found")
endif()


if (NOT soplex_INCLUDE_DIR OR NOT soplex_LIBRARY)
  message (STATUS "Will install and use library Filib from 3rd/ subdirectory")
  # The installation of soplex via CMake does not work for us (linking error
  # when compiling ibexsolve)
  ExternalProject_Add (libsoplex_3rd
                        PREFIX soplex-3.1.1
                        URL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/soplex-3.1.1.tar
                        CONFIGURE_COMMAND ""
                        BUILD_COMMAND make GMP=false ZLIB=false USRCPPFLAGS=-fPIC INSTALLDIR=<INSTALL_DIR> install
                        INSTALL_COMMAND ""
                        BUILD_IN_SOURCE 1
                        )
  ExternalProject_Get_Property (libsoplex_3rd INSTALL_DIR)
  # install headers and set build and install include directory
  set (_incdir ${INSTALL_DIR}/include)
  install (DIRECTORY ${_incdir}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR_3RD})
  set (soplex_INCLUDE_DIR $<BUILD_INTERFACE:${_incdir}>$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR_3RD}>)
  file (MAKE_DIRECTORY "${_incdir}") # need to exist during generation step
  #
  lib_full_path (soplex_LIBRARY ${INSTALL_DIR}/lib soplex)
  install (FILES ${soplex_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR_3RD})
  set (soplex_LIBRARY "$<BUILD_INTERFACE:${soplex_LIBRARY}>$<INSTALL_INTERFACE:-L$<INSTALL_PREFIX>/${CMAKE_INSTALL_LIBDIR_3RD} -lsoplex>")
  unset (INSTALL_DIR)
endif ()

#
mark_as_advanced (soplex_INCLUDE_DIR soplex_LIBRARY)

# Create an interface imported target
add_library (soplex INTERFACE IMPORTED GLOBAL)
set_target_properties (soplex PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${soplex_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES ${soplex_LIBRARY}
  )
