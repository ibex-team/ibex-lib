set (FILIB_NAME "Filib")
set (FILIB_NAME ${FILIB_NAME} PARENT_SCOPE)

# Look for Filib
include (FindFilib.cmake)

# Avoid overflowing display with warnings
if (MSVC)
	add_compile_options("/wd4244") # disable warning conversion from 'double' to '___', possible loss of data
else ()
	add_compile_options(-w)
endif ()

if (FILIB_FOUND)
  set (FILIB_TARGET "Filib::Filib" PARENT_SCOPE)
  # Install FindFilib.cmake file
  install (FILES FindFilib.cmake
            DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}/cmake.utils
            COMPONENT devtools)
  # Write CMake file containing path hint and install it
  generate_path_hints_file (Filib INSTALL)
else ()
  message (STATUS "Will install and use library Filib from 3rd/ subdirectory")

  ###
  set (FILIB_VERSION "3.0.2.2")
  set (filib_basename "filibsrc-${FILIB_VERSION}")
  set (filib_archive_name "${filib_basename}.tar.gz")
  set (filib_archive "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${filib_archive_name}")
  set (wdir "${CMAKE_CURRENT_BINARY_DIR}/${filib_basename}")
  set (logdir "${CMAKE_CURRENT_BINARY_DIR}/log")

  file (MAKE_DIRECTORY "${logdir}")

  ### Extract
  execute_process_check (COMMAND ${CMAKE_COMMAND} -E tar xzf ${filib_archive}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                         MSG "extracting ${filib_archive_name}"
                         LOGBASENAME "${logdir}/extract"
                         STATUS_PREFIX "${FILIB_NAME}: ")

  ### Apply patch
  patch ("${FILIB_NAME}: " "${CMAKE_CURRENT_SOURCE_DIR}/3rd" "${wdir}"
          "${logdir}" "${filib_basename}.all.all.patch"
                      "${filib_basename}_remove-throw.patch")

  ### Copy files
  copy ("${FILIB_NAME}: " "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${filib_basename}"
          "${wdir}" "${logdir}" "CMakeLists.txt" "config.h.in"
          "rounding_control/rounding_control_config.hpp.in")

  ### Configure
  message (STATUS "${FILIB_NAME}: configure")
  # Original values will be restored at the end of this file
  set (CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR_3RD})
  set (CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR_3RD})
  add_subdirectory (${wdir} ${wdir}-build)
  message (STATUS "${FILIB_NAME}: configure - done")

  ###
  set (FILIB_TARGET "prim" PARENT_SCOPE)
  set (FILIB_VERSION ${FILIB_VERSION} PARENT_SCOPE)
endif ()

