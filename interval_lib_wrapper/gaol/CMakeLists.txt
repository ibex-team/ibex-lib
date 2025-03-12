set (GAOL_NAME "Gaol")
set (GAOL_NAME ${GAOL_NAME} PARENT_SCOPE)

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Look for Mathlib
find_package (Mathlib 2.1.1)

# Avoid overflowing display with warnings
if (NOT WIN32)
	add_compile_options(-w)
endif ()

if (MATHLIB_FOUND)
  set (MATHLIB_TARGET "Mathlib::Mathlib")
  # install FindMathlib.cmake file
  install (FILES FindMathlib.cmake
            DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}/cmake.utils
            COMPONENT devtools)
  # Write CMake file containing path hint and install it
  generate_path_hints_file (Mathlib INSTALL)

  # Look for Gaol
  find_package (Gaol 4.2.3)
  if (GAOL_FOUND)
    set (GAOL_TARGET "Gaol::Gaol;${MATHLIB_TARGET}" PARENT_SCOPE)
    # Install FindGaol.cmake file
    install (FILES FindGaol.cmake
              DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}/cmake.utils
              COMPONENT devtools)
    # Write CMake file containing path hint and install it
    generate_path_hints_file (Gaol INSTALL)
  endif ()
else ()
  message (STATUS "Will install and use library Mathlib from 3rd/ subdirectory")

  ###
  set (MATHLIB_NAME "Mathlib")
  set (MATHLIB_VERSION "2.1.1")
  set (mathlib_basename "mathlib-${MATHLIB_VERSION}")
  set (mathlib_archive_name "${mathlib_basename}.tar.gz")
  set (mathlib_archive "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${mathlib_archive_name}")
  set (wdir "${CMAKE_CURRENT_BINARY_DIR}/${mathlib_basename}")
  set (logdir "${CMAKE_CURRENT_BINARY_DIR}/log")

  file (MAKE_DIRECTORY "${logdir}")

  ### Extract
  execute_process_check (COMMAND ${CMAKE_COMMAND} -E tar xzf ${mathlib_archive}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                         MSG "extracting ${mathlib_archive_name}"
                         LOGBASENAME "${logdir}/extract"
                         STATUS_PREFIX "${MATHLIB_NAME}: ")

  ### Apply patch
  if (WIN32)
    patch ("${MATHLIB_NAME}: " "${CMAKE_CURRENT_SOURCE_DIR}/3rd" "${wdir}"
            "${logdir}" "${mathlib_basename}.win32.all.patch")
  endif ()

  ### Copy files
  copy ("${MATHLIB_NAME}: "
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${mathlib_basename}" "${wdir}"
        "${logdir}" "CMakeLists.txt" "src/mathlib_configuration.h.in")

  ### Configure
  message (STATUS "${MATHLIB_NAME}: configure")
  # Original values will be restored at the end of this file
  set (CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR_3RD})
  set (CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR_3RD})
  add_subdirectory (${wdir} ${wdir}-build)
  message (STATUS "${MATHLIB_NAME}: configure - done")

  ###
  set (MATHLIB_TARGET "ultim")
endif ()

if (NOT GAOL_FOUND)
  ###########################################################################
  message (STATUS "Will install and use library gaol from 3rd/ subdirectory")
  set (GAOL_VERSION "4.2.3alpha0")
  set (gaol_basename "gaol-${GAOL_VERSION}")
  set (gaol_archive_name "${gaol_basename}.tar.gz")
  set (gaol_archive "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${gaol_archive_name}")
  set (wdir "${CMAKE_CURRENT_BINARY_DIR}/${gaol_basename}")
  set (logdir "${CMAKE_CURRENT_BINARY_DIR}/log")

  file (MAKE_DIRECTORY "${logdir}")

  ### Extract
  execute_process_check (COMMAND ${CMAKE_COMMAND} -E tar xzf ${gaol_archive}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                         MSG "extracting ${gaol_archive_name}"
                         LOGBASENAME "${logdir}/extract"
                         STATUS_PREFIX "${GAOL_NAME}: ")

  ### Apply patch
  set (GAOL_PATCHES "${gaol_basename}.all.all.patch")
  patch ("${GAOL_NAME}: " "${CMAKE_CURRENT_SOURCE_DIR}/3rd" "${wdir}"
          "${logdir}" ${GAOL_PATCHES})

  ### Copy files
  copy ("${GAOL_NAME}: " "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${gaol_basename}"
        "${wdir}" "${logdir}" "CMakeLists.txt" "gaol/gaol_configuration.h.in"
                                                                "test_sse.cpp")

  ### Configure
  message (STATUS "${GAOL_NAME}: configure")
  # Original values will be restored at the end of this file
  set (CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR_3RD})
  set (CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR_3RD})
  add_subdirectory (${wdir} ${wdir}-build)
  message (STATUS "${GAOL_NAME}: configure - done")

  ###
  set (GAOL_TARGET "gaol;${MATHLIB_TARGET}" PARENT_SCOPE)
endif ()
set (GAOL_VERSION ${GAOL_VERSION} PARENT_SCOPE)
