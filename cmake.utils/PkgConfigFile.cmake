set (IBEX_PKGCONFIG_LINK_FLAGS)
set (IBEX_PKGCONFIG_LIBS)
set (IBEX_PKGCONFIG_LINK_DIRS)
set (IBEX_PKGCONFIG_INCDIRS)

function (pkgconfig_preprocess_property VAR TARGET PROP)
  get_target_property (OUT_PROP ${TARGET} ${PROP})
  string(REPLACE "$<BUILD_INTERFACE:" "$<0:" OUT_PROP "${OUT_PROP}")
  string(REPLACE "$<INSTALL_INTERFACE:" "$<1:" OUT_PROP "${OUT_PROP}")

  string(REPLACE "$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}" "\${includedir}" OUT_PROP "${OUT_PROP}")
  string(REPLACE "$<INSTALL_PREFIX>/${CMAKE_INSTALL_LIBDIR}" "\${libdir}" OUT_PROP "${OUT_PROP}")
  string(REPLACE "$<INSTALL_PREFIX>" "\${prefix}" OUT_PROP "${OUT_PROP}")

  set(${VAR} ${OUT_PROP} PARENT_SCOPE)
endfunction ()

# link flags
get_target_property (link_flags ibex LINK_FLAGS)
if (link_flags)
  set (Libs ${link_flags})
endif ()

# -I, -L and -l
pkgconfig_preprocess_property (incdirs ibex INTERFACE_INCLUDE_DIRECTORIES)
list (APPEND Libs "-L\${libdir}")
list (APPEND Libs "-libex")
get_target_property (req_lib ibex INTERFACE_LINK_LIBRARIES)
foreach (lib ${req_lib})
  pkgconfig_preprocess_property (incdir ${lib} INTERFACE_INCLUDE_DIRECTORIES)
  if (incdir)
    list (APPEND incdirs ${incdir})
  endif ()

  pkgconfig_preprocess_property (linklib ${lib} INTERFACE_LINK_LIBRARIES)
  if (linklib)
    list (APPEND Libs ${linklib})
  endif ()
endforeach ()

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
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ibex.pc DESTINATION ${CMAKE_INSTALL_PKGCONFIG})
