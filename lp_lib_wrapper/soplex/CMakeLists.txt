set (SOPLEX_NAME "SoPlex")
set (SOPLEX_NAME ${SOPLEX_NAME} PARENT_SCOPE)

# Look for SoPlex
# TODO: can we somehow use the fact that SoPlex 4 offers install a
# soplex-config.cmake file IF SoPlex was compiled with CMake. And use the
# following include as fallback.
include (FindSoPlex.cmake)

# Avoid overflowing display with warnings
if (NOT WIN32)
	add_compile_options(-w)
endif ()

if (SOPLEX_FOUND)
  set (SOPLEX_TARGET "SoPlex::SoPlex" PARENT_SCOPE)
  # Install FindSoPlex.cmake file
  install (FILES FindSoPlex.cmake
            DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}/cmake.utils
            COMPONENT devtools)
  # Write CMake file containing path hint and install it
  generate_path_hints_file (SoPlex INSTALL)
else ()
  message (STATUS "Will install and use library SoPlex from 3rd/ subdirectory")

  ###
  set (SOPLEX_VERSION "4.0.2")
  set (soplex_basename "soplex-${SOPLEX_VERSION}")
  set (soplex_archive_name "${soplex_basename}.tar")
  set (soplex_archive "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${soplex_archive_name}")
  set (wdir "${CMAKE_CURRENT_BINARY_DIR}/${soplex_basename}")
  set (logdir "${CMAKE_CURRENT_BINARY_DIR}/log")

  file (MAKE_DIRECTORY "${logdir}")

  ### Extract
  execute_process_check (COMMAND ${CMAKE_COMMAND} -E tar xzf ${soplex_archive}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                         MSG "extracting ${soplex_archive_name}"
                         LOGBASENAME "${logdir}/extract"
                         STATUS_PREFIX "${SOPLEX_NAME}: ")

  ### Apply patch
  patch ("${SOPLEX_NAME}: " "${CMAKE_CURRENT_SOURCE_DIR}/3rd" "${wdir}"
          "${logdir}" "${soplex_basename}.all.all.patch")
  
  patch ("${SOPLEX_NAME}: " "${CMAKE_CURRENT_SOURCE_DIR}/3rd" "${wdir}"
          "${logdir}" "${soplex_basename}.CMake.patch")

  ### Configure
  message (STATUS "${SOPLEX_NAME}: configure")
  # Original values will be restored at the end of this file
  set (CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR_3RD})
  set (CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR_3RD})
  add_subdirectory (${wdir} ${wdir}-build)
  message (STATUS "${SOPLEX_NAME}: configure - done")

  ###
  set (SOPLEX_TARGET "soplex" PARENT_SCOPE)
  set (SOPLEX_VERSION ${SOPLEX_VERSION} PARENT_SCOPE)
endif ()
