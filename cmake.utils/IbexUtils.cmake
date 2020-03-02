include (ibex-config-utils)

################################################################################
################################################################################
################################################################################
function (FIND_HEADER_CUSTOM prefix hdrname hdr_paths)
  set (MSG "Looking for ${hdrname}")
  message (STATUS "${MSG}")

  # First look only in ${hdr_paths}
  find_path (${prefix}_INCDIR ${hdrname} PATHS ${hdr_paths}
                  DOC "Set to exact include directory to bypass internal test"
                  PATH_SUFFIXES include NO_DEFAULT_PATH)

  if (NOT ${prefix}_INCDIR)
    # Now look with system and cmake paths
    find_path (${prefix}_INCDIR ${hdrname} PATHS ${hdr_paths}
                    DOC "Set to exact include directory to bypass internal test"
                    PATH_SUFFIXES include)
    if (${prefix}_INCDIR)
      # If found in this case, the path is only used for building, it will not
      # be added in pkg-config and export files
      set (${prefix}_INCDIR $<BUILD_INTERFACE:${${prefix}_INCDIR}>)
    endif ()
  endif ()

  if (${prefix}_INCDIR)
    message (STATUS "${MSG} -- found at ${${prefix}_INCDIR}")
  else()
    message (STATUS "${MSG} -- not found")
  endif()

  set (${prefix}_INCDIR ${${prefix}_INCDIR} PARENT_SCOPE)
  mark_as_advanced (${prefix}_INCDIR)
endfunction ()

################################################################################
################################################################################
################################################################################
function (FIND_LIBRARY_CUSTOM prefix libname lib_paths)
  set (MSG "Looking for ${libname}")
  message (STATUS "${MSG}")

  # First look only in ${lib_paths}
  find_library (${prefix}_LIB ${libname} PATHS ${lib_paths}
                      DOC "Set to exact lib directory to bypass internal test"
                      PATH_SUFFIXES lib NO_DEFAULT_PATH)

  if (NOT ${prefix}_LIB)
    # Now look with system and cmake paths
    find_library (${prefix}_LIB ${libname} PATHS ${lib_paths}
                        DOC "Set to exact lib directory to bypass internal test"
                        PATH_SUFFIXES lib)
  endif ()

  if (${prefix}_LIB)
    message (STATUS "${MSG} -- found at ${${prefix}_LIB}")
  else()
    message (STATUS "${MSG} -- not found")
  endif()

  set (${prefix}_LIB ${${prefix}_LIB} PARENT_SCOPE)
  mark_as_advanced (${prefix}_LIB)
endfunction ()

################################################################################
################################################################################
################################################################################
function (IBEX_GENERATE_PKGCONFIG_FILE tgtname)
  get_target_incdirs (_incdirs ${tgtname})
  generators_expression_replace_for_build (incdirs "${_incdirs}"
                                                    PREFIX "\${prefix}"
                                                    INCLUDEDIR "\${includedir}")
  string (REPLACE "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}" "\${includedir}" incdirs "${incdirs}")

  get_target_libs (_Libs ${tgtname})
  list (REMOVE_DUPLICATES _Libs)
  generators_expression_replace_for_build (Libs "${_Libs}" PREFIX "\${prefix}"
                                                        LIBDIR "\${includedir}")
  string (REPLACE "\${prefix}/${CMAKE_INSTALL_LIBDIR}" "\${libdir}" Libs "${Libs}")
  
  file (GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ibex.pc
                 CONTENT "prefix=${CMAKE_INSTALL_PREFIX}
includedir=\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}
libdir=\${prefix}/${CMAKE_INSTALL_LIBDIR}

Name: ibex
Description: ${IBEX_DESCRIPTION}
Url: ${IBEX_URL}
Version: ${IBEX_VERSION}
Cflags: -I$<JOIN:${incdirs}, -I>
Libs: $<JOIN:${Libs}, >
")

  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ibex.pc
           DESTINATION ${CMAKE_INSTALL_PKGCONFIG} COMPONENT devtools)
endfunction ()
