set (IBEX_PKGCONFIG_LINK_FLAGS)
set (IBEX_PKGCONFIG_LIBS)
set (IBEX_PKGCONFIG_LINK_DIRS)
set (IBEX_PKGCONFIG_INCDIRS)

# link flags
get_target_property (_tmp ibex LINK_FLAGS)
if (_tmp)
  set (IBEX_PKGCONFIG_LINK_FLAGS ${_tmp})
endif ()

# include directories, libs and link_dirs
get_target_property (_req_lib ibex INTERFACE_LINK_LIBRARIES)
if (_req_lib)
  foreach (_lib ${_req_lib})
    get_target_property(${_lib}_interface ${_lib} INTERFACE_LINK_LIBRARIES)
    if (${_lib}_interface)
      foreach (_l ${${_lib}_interface})
        string (REGEX REPLACE "(.?:?/[^ ]*)/lib([^ ]*)\\.(a|so|dylib|dll)" "\\1"
                      _dir "${_l}")
        string (REGEX REPLACE "(.?:?/[^ ]*)/lib([^ ]*)\\.(a|so|dylib|dll)" "\\2"
                      _libname "${_l}")
        string(REPLACE "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
                       "\${libdir}" _dir ${_dir})
        string(REPLACE "${CMAKE_INSTALL_PREFIX}" "\${prefix}" _dir ${_dir})
        list (APPEND IBEX_PKGCONFIG_LINK_DIRS "-L${_dir}")
        list (APPEND IBEX_PKGCONFIG_LIBS "-l${_libname}")
      endforeach ()
    endif ()

    get_target_property(${_lib}_incdir ${_lib} INTERFACE_INCLUDE_DIRECTORIES)
    if (${_lib}_incdir)
      foreach (_d ${${_lib}_incdir})
        string(REPLACE "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}"
                       "\${includedir}" _d ${_d})
        string(REPLACE "${CMAKE_INSTALL_PREFIX}" "\${prefix}" _d ${_d})
        list (APPEND IBEX_PKGCONFIG_INCDIRS "-I${_d}")
      endforeach ()
    endif ()
  endforeach ()
endif ()
list (REMOVE_DUPLICATES IBEX_PKGCONFIG_LINK_DIRS)
list (REMOVE_DUPLICATES IBEX_PKGCONFIG_INCDIRS)

string (REPLACE ";" " " IBEX_PKGCONFIG_LINK_FLAGS "${IBEX_PKGCONFIG_LINK_FLAGS}")
string (REPLACE ";" " " IBEX_PKGCONFIG_LIBS "${IBEX_PKGCONFIG_LIBS}")
string (REPLACE ";" " " IBEX_PKGCONFIG_LINK_DIRS "${IBEX_PKGCONFIG_LINK_DIRS}")
string (REPLACE ";" " " IBEX_PKGCONFIG_INCDIRS "${IBEX_PKGCONFIG_INCDIRS}")
