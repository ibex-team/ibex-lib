set (CLP_NAME "Clp")
set (CLP_NAME ${CLP_NAME} PARENT_SCOPE)

# Look for Clp
include (FindClp.cmake)

message (STATUS "######## ${CLP_FOUND} ${CLP_VERSION}")

if (CLP_FOUND)
  set (CLP_TARGET "Clp::Clp" PARENT_SCOPE)
  # Install FindClp.cmake file
  install (FILES FindClp.cmake
            DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}/cmake.utils
            COMPONENT devtools)
  # Write CMake file containing path hint and install it
  generate_path_hints_file (Clp INSTALL)
else ()
  message (STATUS "Will install and use library Clp from 3rd/ subdirectory")

  ###
  set (CLP_VERSION "4.0.2")
  set (clp_basename "clp-${CLP_VERSION}")
  set (clp_archive_name "${clp_basename}.tar")
  set (clp_archive "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${clp_archive_name}")
  set (wdir "${CMAKE_CURRENT_BINARY_DIR}/${clp_basename}")
  set (logdir "${CMAKE_CURRENT_BINARY_DIR}/log")

  file (MAKE_DIRECTORY "${logdir}")

  ### Extract
  execute_process_check (COMMAND ${CMAKE_COMMAND} -E tar xzf ${clp_archive}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                         MSG "extracting ${clp_archive_name}"
                         LOGBASENAME "${logdir}/extract"
                         STATUS_PREFIX "${CLP_NAME}: ")

  ### Apply patch
  patch ("${CLP_NAME}: " "${CMAKE_CURRENT_SOURCE_DIR}/3rd" "${wdir}"
          "${logdir}" "${clp_basename}.CMake.patch")

  ### Configure
  message (STATUS "${CLP_NAME}: configure")
  # Original values will be restored at the end of this file
  set (CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR_3RD})
  set (CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR_3RD})
  add_subdirectory (${wdir} ${wdir}-build)
  message (STATUS "${CLP_NAME}: configure - done")

  ###
  set (CLP_TARGET "clp" PARENT_SCOPE)
  set (CLP_VERSION ${CLP_VERSION} PARENT_SCOPE)
endif ()
