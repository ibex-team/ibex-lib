################################################################################
################################################################################
################################################################################
function (generators_expression_replace_for_build resultvar input_str)
  set (opt "")
  set (oneArgs INCLUDEDIR LIBDIR PREFIX)
  set (multiArgs "")
  
  cmake_parse_arguments(GERFB "${opt}" "${oneArgs}" "${multiArgs}" ${ARGN})

  if(GERFB_UNPARSED_ARGUMENTS)
    message (FATAL_ERROR "Unknown keywords given to generators_expression_replace_for_build(): \"${GERFB_UNPARSED_ARGUMENTS}\"")
  endif()

  if (NOT GERFB_PREFIX)
    set (GERFB_PREFIX "${CMAKE_INSTALL_PREFIX}")
  endif ()
  if (NOT GERFB_INCLUDEDIR)
    set (GERFB_INCLUDEDIR "${GERFB_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
  endif ()
  if (NOT GERFB_LIBDIR)
    set (GERFB_LIBDIR "${GERFB_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
  endif ()

  string(REPLACE "$<BUILD_INTERFACE:" "$<0:" input_str "${input_str}")
  string(REPLACE "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}" "$<1:${GERFB_INCLUDEDIR}" input_str "${input_str}")
  string(REPLACE "$<INSTALL_INTERFACE:${CMAKE_INSTALL_LIBDIR}" "$<1:${GERFB_LIBDIR}" input_str "${input_str}")
  string(REPLACE "$<INSTALL_INTERFACE:" "$<1:" input_str "${input_str}")

  string(REPLACE "$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}" "${GERFB_INCLUDEDIR}" input_str "${input_str}")
  string(REPLACE "$<INSTALL_PREFIX>/${CMAKE_INSTALL_LIBDIR}" "${GERFB_LIBDIR}" input_str "${input_str}")
  string(REPLACE "$<INSTALL_PREFIX>" "${GERFB_PREFIX}" input_str "${input_str}")

  set(${resultvar} ${input_str} PARENT_SCOPE)
endfunction ()

################################################################################
################################################################################
################################################################################
function (get_target_incdirs outvarname target)
  set (incdirs)

  get_target_property (_propvalue ${target} INTERFACE_INCLUDE_DIRECTORIES)
  if (_propvalue)
    list (APPEND incdirs ${_propvalue})
  endif ()

  get_target_property (_libs ${target} INTERFACE_LINK_LIBRARIES)
  foreach (lib ${_libs})
    if (TARGET ${lib})
      get_target_incdirs (_propvalue ${lib})
      if (_propvalue)
        list (APPEND incdirs ${_propvalue})
      endif ()
    endif ()
  endforeach ()

  set (${outvarname} ${incdirs} PARENT_SCOPE)
endfunction ()

################################################################################
################################################################################
################################################################################
function (get_target_libs outvarname target)
  set (Libs)

  if (TARGET ${target})
    get_target_property (is_imported ${target} IMPORTED)
    if (NOT is_imported)
      get_target_property (_propvalue ${target} NAME)
      if (NOT target STREQUAL "ibex")
        list (APPEND Libs "-L\${libdir}/ibex/3rd;-l${_propvalue}")
      else ()
        list (APPEND Libs "-L\${libdir};-l${_propvalue}")
      endif ()
    endif ()

    get_target_property (_libs ${target} INTERFACE_LINK_LIBRARIES)
    if (_libs)
      foreach (lib ${_libs})
        get_target_libs (_propvalue ${lib})
        if (_propvalue)
          list (APPEND Libs ${_propvalue})
        endif ()
      endforeach ()
    endif ()
  else ()
    list (APPEND Libs ${target})
  endif ()

  set (${outvarname} ${Libs} PARENT_SCOPE)
endfunction ()

################################################################################
################################################################################
################################################################################
function (ibex_generate_pkgconfig_file tgtname)
  get_target_incdirs (_incdirs ${tgtname})
  generators_expression_replace_for_build (incdirs "${_incdirs}"
                                                    PREFIX "\${prefix}"
                                                    INCLUDEDIR "\${includedir}")
  string (REPLACE "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}" "\${includedir}" incdirs "${incdirs}")

  get_target_libs (_Libs ${tgtname})
  list (REMOVE_DUPLICATES _Libs)
  generators_expression_replace_for_build (Libs "${_Libs}" PREFIX "\${prefix}"
                                                            LIBDIR "\${libdir}")
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
