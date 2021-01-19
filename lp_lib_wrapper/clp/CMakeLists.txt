set (CLP_NAME "Clp")
set (CLP_NAME ${CLP_NAME} PARENT_SCOPE)

# Look for Clp
include (FindClp.cmake)

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
  set (CLP_VERSION "1.17.6")
  set (clp_basename "Clp-${CLP_VERSION}")
  set (clp_archive_name "${clp_basename}.tgz")
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

  ### Copy files
  copy ("${CLP_NAME}: "
        "${CMAKE_CURRENT_SOURCE_DIR}/3rd/${clp_basename}" "${wdir}"
        "${logdir}" "CMakeLists.txt" "CoinUtils/CMakeLists.txt"
        "CoinUtils/src/config.h.in" "CoinUtils/src/config_coinutils.h.in"
        "Clp/CMakeLists.txt" "Clp/src/config.h.in" "Clp/src/config_clp.h.in"
        )

  ### Configure
  message (STATUS "${CLP_NAME}: configure")
  # Original values will be restored at the end of this file
  set (CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR_3RD}/coin)
  set (CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR_3RD})
  add_subdirectory (${wdir} ${wdir}-build)
  message (STATUS "${CLP_NAME}: configure - done")

  ###
  set (CLP_TARGET "ClpSolver" PARENT_SCOPE)
  set (CLP_VERSION ${CLP_VERSION} PARENT_SCOPE)
endif ()
