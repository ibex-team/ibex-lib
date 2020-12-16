################################################################################
################################################################################
################################################################################
# subdir_list (RESULTVAR [ WDIR wdir] [ RELATIVE ])
# return the list of directories of wdir
# If not given wdir = CMAKE_CURRENT_SOURCE_DIR
# By default, RELATIVE is not set and absolute path are returned. If RELATIVE
# is given, only the names of the directories are returned.
function (subdir_list resultvar)
  set (opt RELATIVE)
  set (oneArgs WDIR)
  set (multiArgs "")

  cmake_parse_arguments(SL "${opt}" "${oneArgs}" "${multiArgs}" ${ARGN})

  if(SL_UNPARSED_ARGUMENTS)
    message (FATAL_ERROR "Unknown keywords given to subdir_list(): \"${SL_UNPARSED_ARGUMENTS}\"")
  endif()

  if (SL_WDIR)
    set (wdir ${SL_WDIR})
  else ()
    set (wdir ${CMAKE_CURRENT_SOURCE_DIR})
  endif ()

  #
  file (GLOB children RELATIVE ${wdir} ${wdir}/*)
  set (dirlist "")
  foreach (child ${children})
    if (IS_DIRECTORY ${wdir}/${child})
      if (SL_RELATIVE)
        list (APPEND dirlist ${child})
      else()
        list (APPEND dirlist ${wdir}/${child})
      endif()
    endif()
  endforeach()
  set (${resultvar} ${dirlist} PARENT_SCOPE)
endfunction()

################################################################################
################################################################################
################################################################################
function (generate_path_hints_file targetname)
  string (TOLOWER ${targetname} _tgt_lower)
  string (TOUPPER ${targetname} _tgt_upper)
  set (_ofile "${CMAKE_CURRENT_BINARY_DIR}/${IBEX_CMAKE_PATH_FILES_PREFIX}-${_tgt_lower}.cmake")
  file (WRITE ${_ofile} "if (NOT ${_tgt_upper}_DIR)
  set (${_tgt_upper}_DIR \"${${_tgt_upper}_DIR}\")
endif ()
")
  if (ARGC EQUAL 2 AND ARGV1 STREQUAL "INSTALL")
    install (FILES ${_ofile} DESTINATION ${CMAKE_INSTALL_CONFIGCMAKE}
                                                            COMPONENT devtools)
  endif ()
endfunction ()

################################################################################
################################################################################
################################################################################
function (execute_process_check)
  set (opt "")
  set (oneArgs WORKING_DIRECTORY MSG LOGBASENAME STATUS_PREFIX)
  set (multiArgs COMMAND)

  cmake_parse_arguments(EPC "${opt}" "${oneArgs}" "${multiArgs}" ${ARGN})

  if(EPC_UNPARSED_ARGUMENTS)
    message (FATAL_ERROR "Unknown keywords given to execute_process_check(): \"${EPC_UNPARSED_ARGUMENTS}\"")
  endif()

  # Check mandatory arguments
  foreach (arg "COMMAND" "MSG" "LOGBASENAME")
    if (NOT EPC_${arg})
      message (FATAL_ERROR "Missing mandatory argument ${arg} in execute_process_check")
    endif ()
  endforeach ()

  # use working dir if given
  if (EPC_WORKING_DIRECTORY)
    set (_workingdir WORKING_DIRECTORY ${EPC_WORKING_DIRECTORY})
  endif ()

  message (STATUS "${EPC_STATUS_PREFIX}${EPC_MSG}")
  execute_process (COMMAND ${EPC_COMMAND} ${_workingdir}
                   RESULT_VARIABLE ret
                   OUTPUT_FILE "${EPC_LOGBASENAME}-out.log"
                   ERROR_FILE "${EPC_LOGBASENAME}-err.log"
                   )

  if (ret)
    message (FATAL_ERROR "An error occurs while ${EPC_MSG}\n"
                          "See also\n${EPC_LOGBASENAME}-*.log\n")
  endif ()
endfunction ()

################################################################################
################################################################################
################################################################################
# Apply all patches in ARGN.
# Assume patch binary is available. Only used when installing 3rd-party
# libraries. If a patch binary is not available, the user may have to install
# the 3rd-party libraries itself.
function (patch prefix patchdir wdir logdir)
  foreach (_patchfile ${ARGN})
    string (REPLACE "/" "_" logbase "${_patchfile}")
    execute_process_check (COMMAND patch -p1 -i "${patchdir}/${_patchfile}"
                         WORKING_DIRECTORY ${wdir}
                         MSG "applying patch ${_patchfile}"
                         LOGBASENAME "${logdir}/patch-${logbase}"
                         STATUS_PREFIX ${prefix})
  endforeach ()
endfunction ()

################################################################################
################################################################################
################################################################################
# Copy files in ARGN
function (copy prefix indir outdir logdir)
  foreach (_file ${ARGN})
    set (_ifile "${indir}/${_file}")
    set (_ofile "${outdir}/${_file}")
    string (REPLACE "/" "_" logbase "${_file}")
    execute_process_check (COMMAND ${CMAKE_COMMAND} -E copy ${_ifile} ${_ofile}
                           MSG "copying ${_file}"
                           LOGBASENAME "${logdir}/copy-${logbase}"
                           STATUS_PREFIX "${prefix}")
  endforeach ()
endfunction ()
