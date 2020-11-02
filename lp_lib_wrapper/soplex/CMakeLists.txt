################################################################################
# Options
################################################################################
set (SOPLEX_DIR "" CACHE PATH "Path to the Soplex lib and include directories")

################################################################################
# May be needed to install missing dependencies
################################################################################
include (ExternalProject)
include (IbexUtils)

################################################################################
# Looking for Soplex, install it if not found
################################################################################
# Looking for soplex.h, result is written in SOPLEX_INCDIR
find_header_custom (SOPLEX "soplex.h" PATHS "${SOPLEX_DIR}")
# Looking for soplex library, result is written in SOPLEX_LIB
find_library_custom (SOPLEX "soplex" PATHS "${SOPLEX_DIR}")

if (SOPLEX_INCDIR AND SOPLEX_LIB)
  create_target_import_and_export (soplex ${SOPLEX_LIB} SOPLEX_EXPORTFILE
                                          NAMESPACE Ibex::
                                          INCLUDE_DIRECTORIES ${SOPLEX_INCDIR})
else ()
  message (STATUS "Will install and use library Filib from 3rd/ subdirectory")
  # The installation of soplex via CMake does not work for us (linking error
  # when compiling ibexsolve)
  set (SOPLEX_PATCH "${CMAKE_CURRENT_SOURCE_DIR}/3rd/soplex-4.0.2.all.all.patch")
  if (BUILD_SHARED_LIBS)
    set (_shared "SHARED=true")
  endif ()
  ExternalProject_Add (libsoplex_3rd
                        PREFIX soplex-4.0.2
                        URL ${CMAKE_CURRENT_SOURCE_DIR}/3rd/soplex-4.0.2.tar
                        PATCH_COMMAND patch -p1 -i ${SOPLEX_PATCH}
                        CONFIGURE_COMMAND ""
                        BUILD_COMMAND $(MAKE) USRCXXFLAGS=${CMAKE_CXX_FLAGS} GMP=false ZLIB=false ${_shared} INSTALLDIR=<INSTALL_DIR> install
                        INSTALL_COMMAND ""
                        BUILD_IN_SOURCE 1
                        LOG_DOWNLOAD 1
                        LOG_BUILD 1
                      )
  ExternalProject_Get_Property (libsoplex_3rd INSTALL_DIR)
  set (SOPLEX_LIBDIR "${INSTALL_DIR}/lib")
  set (SOPLEX_INCDIR "${INSTALL_DIR}/include")

  # absolute path to the library
  lib_get_abspath_from_name (SOPLEX_LIB ${SOPLEX_LIBDIR} soplex)

  # this directory needs to exist during generation step to avoid CMake errors
  file (MAKE_DIRECTORY "${SOPLEX_INCDIR}")

  # install headers
  install (DIRECTORY ${SOPLEX_INCDIR}/ COMPONENT devtools
                                    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR_3RD})

  # 
  set (_incdir $<BUILD_INTERFACE:${SOPLEX_INCDIR}>$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR_3RD}>)
  create_target_import_and_export (soplex ${SOPLEX_LIB} SOPLEX_EXPORTFILE
                                        INSTALL NAMESPACE Ibex::
                                        INCLUDE_DIRECTORIES ${_incdir}
                                        COMPONENT devtools
                                        DEPENDS libsoplex_3rd)
  unset (INSTALL_DIR)
endif ()

list (APPEND EXPORTFILES "${SOPLEX_EXPORTFILE}")
set (EXPORTFILES "${EXPORTFILES}" PARENT_SCOPE)
