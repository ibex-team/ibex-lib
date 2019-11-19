################################################################################
# Options
################################################################################
set (MATHLIB_DIR "" CACHE PATH "Path to the Mathlib/ultim lib and include directories")
set (GAOL_DIR "" CACHE PATH "Path to the Gaol lib and include directories")

################################################################################
# May be needed to install missing dependencies
################################################################################
include (ExternalProject)
include (Utils3rd)

################################################################################
# Looking for Mathlib/libultim, install it if not found
################################################################################
set (MSG "Looking for Mathlib/ultim")
message (STATUS "${MSG}")
find_library (ultim_LIBRARY NAMES ultim HINTS ${MATHLIB_DIR} ${GAOL_DIR}
              PATH_SUFFIXES lib
              DOC "Set to exact lib directory to bypass internal test")
if (ultim_LIBRARY)
  message (STATUS "${MSG} -- found at ${ultim_LIBRARY}")
  get_filename_component (LIBULTIM_LIBDIR "${ultim_LIBRARY}" DIRECTORY)
  get_filename_component (LIBULTIM_INCDIR "${LIBULTIM_LIBDIR}" DIRECTORY)
  set (LIBULTIM_INCDIR "${LIBULTIM_INCDIR}/include")
else()
  message (STATUS "${MSG} -- not found")
  message (STATUS "Will install and use library ultim from 3rd/ subdirectory")
  ExternalProject_Add (libultim_3rd
                        PREFIX mathlib-2.1.0
                        URL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/mathlib-2.1.0.tar.gz
                        BUILD_IN_SOURCE 1
                        CONFIGURE_COMMAND ./configure --prefix=<INSTALL_DIR>
                        LOG_DOWNLOAD 1
                        LOG_CONFIGURE 1
                        LOG_BUILD 1
                        LOG_INSTALL 1
                      )
  ExternalProject_Get_Property (libultim_3rd INSTALL_DIR)
  set (LIBULTIM_LIBDIR "${INSTALL_DIR}/lib")
  set (LIBULTIM_INCDIR "${INSTALL_DIR}/include")
  lib_full_path (ultim_LIBRARY ${LIBULTIM_LIBDIR} ultim)
  install (FILES ${ultim_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR_3RD})
  unset (INSTALL_DIR)
endif()
mark_as_advanced (ultim_LIBRARY) # do not show var in list of options

################################################################################
# Looking for Gaol, install it if not found
################################################################################
# Looking for gaol/gaol.h
set (GAOL_HDR "gaol/gaol.h")
set (MSG "Looking for ${GAOL_HDR}")
message (STATUS "${MSG}")
find_path (gaol_INCLUDE_DIR ${GAOL_HDR} HINTS ${GAOL_DIR} PATH_SUFFIXES include
           DOC "Set to exact include directory to bypass internal test")
if (gaol_INCLUDE_DIR)
  message (STATUS "${MSG} -- found at ${gaol_INCLUDE_DIR}")
  if (gaol_INCLUDE_DIR MATCHES "^${GAOL_DIR}")
    set (gaol_INCLUDE_DIR ${gaol_INCLUDE_DIR})
  else ()
    set (gaol_INCLUDE_DIR $<BUILD_INTERFACE:${gaol_INCLUDE_DIR}>)
  endif ()
else()
  message (STATUS "${MSG} -- not found")
endif()

# Looking for libgaol and libdtoa
foreach (LIB gdtoa gaol)
  set (MSG "Looking for ${LIB}")
  message (STATUS "${MSG}")
  find_library (${LIB}_LIBRARY NAMES ${LIB} HINTS ${GAOL_DIR} PATH_SUFFIXES lib
                DOC "Set to exact lib directory to bypass internal test")
  if (${LIB}_LIBRARY)
    message (STATUS "${MSG} -- found at ${${LIB}_LIBRARY}")
  else()
    message (STATUS "${MSG} -- not found")
  endif()
endforeach ()

if (NOT gaol_INCLUDE_DIR OR NOT gaol_LIBRARY OR NOT gdtoa_LIBRARY)
  message (STATUS "Will install and use library gaol from 3rd/ subdirectory")
  if (TARGET libultim_3rd)
    set (_gaol_depends "DEPENDS" "libultim_3rd")
  endif ()
  set (GAOL_CONFIG_ARGS "--disable-preserve-rounding" "--enable-optimize" "--disable-verbose-mode")
  if (WIN32)
    # On windows, we disable SSE instructions (it can generate failures during
    # execution). You can enable SSE instructions by deleting the next line,
    # do it at your own risk.
    set (GAOL_CONFIG_ARGS "${GAOL_CONFIG_ARGS}" "--disable-simd")
  endif ()
  ExternalProject_Add (libgaol_3rd
                        PREFIX gaol-4.2.0
                        ${_gaol_depends}
                        URL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/gaol-4.2.0.tar.gz
                        BUILD_IN_SOURCE 1
                        CONFIGURE_COMMAND ./configure --prefix=<INSTALL_DIR>
                                      --with-mathlib-include=${LIBULTIM_INCDIR}
                                      --with-mathlib-lib=${LIBULTIM_LIBDIR}
                                      ${GAOL_CONFIG_ARGS}
                        LOG_DOWNLOAD 1
                        LOG_CONFIGURE 1
                        LOG_BUILD 1
                        LOG_INSTALL 1
                      )
  ExternalProject_Get_Property (libgaol_3rd INSTALL_DIR)
  # install headers and set build and install include directory
  set (_incdir ${INSTALL_DIR}/include)
  install (DIRECTORY ${_incdir}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR_3RD})
  set (gaol_INCLUDE_DIR $<BUILD_INTERFACE:${_incdir}>$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR_3RD}>)
  file (MAKE_DIRECTORY "${_incdir}") # need to exist during generation step
  #
  lib_full_path (gaol_LIBRARY ${INSTALL_DIR}/lib gaol)
  lib_full_path (gdtoa_LIBRARY ${INSTALL_DIR}/lib gdtoa)
  install (FILES ${gaol_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR_3RD})
  install (FILES ${gdtoa_LIBRARY} DESTINATION ${CMAKE_INSTALL_LIBDIR_3RD})
  unset (INSTALL_DIR)
endif ()
# do not show vars in list of options
mark_as_advanced (gaol_INCLUDE_DIR gdtoa_LIBRARY gaol_LIBRARY)

# set build and install interfaces for LINK_LIBRARIES
set (build_interface)
list (APPEND install_interface "-lgaol" "-lgdtoa" "-lultim")
set (build_interface ${gaol_LIBRARY};${gdtoa_LIBRARY};${ultim_LIBRARY})
set (install_interface)
if (MATHLIB_DIR)
  list (APPEND install_interface "-L${MATHLIB_DIR}")
endif ()
if (GAOL_DIR)
  list (APPEND install_interface "-L${GAOL_DIR}")
endif ()
if (TARGET libultim_3rd OR TARGET libgaol_3rd)
  list (APPEND install_interface "-L$<INSTALL_PREFIX>/${CMAKE_INSTALL_LIBDIR_3RD}")
endif ()
list (APPEND install_interface "-lgaol" "-lgdtoa" "-lultim")
set (link_libraries $<BUILD_INTERFACE:${build_interface}>$<INSTALL_INTERFACE:${install_interface}>)

################################################################################
# Create an interface imported target
################################################################################
add_library (gaol INTERFACE IMPORTED GLOBAL)
set_target_properties (gaol PROPERTIES
                            INTERFACE_INCLUDE_DIRECTORIES ${gaol_INCLUDE_DIR}
                            INTERFACE_LINK_LIBRARIES "${link_libraries}"
                      )

# With Gaol, we add "-Wno-deprecated" to remove lots of warnings generated by
# a function definition in gaol_interval.h
CHECK_CXX_COMPILER_FLAG ("-Wno-deprecated" COMPILER_SUPPORTS_WNO_DEPRECATED)
if (COMPILER_SUPPORTS_WNO_DEPRECATED)
  set_target_properties (gaol PROPERTIES
                              INTERFACE_COMPILE_OPTIONS -Wno-deprecated)
endif()
