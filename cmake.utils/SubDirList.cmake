function (subdir_list resultvar)
  # subdir_list (RESULTVAR [ WDIR wdir] [ RELATIVE ])
  # return the list of directories of wdir
  # If not given wdir = CMAKE_CURRENT_SOURCE_DIR
  # By default, RELATIVE is not set and absolute path are returned. If RELATIVE
  # is given, only the names of the directories are returned.

  # default values of args
  set (wdir ${CMAKE_CURRENT_SOURCE_DIR})
  set (relative FALSE)

  # parse args
  foreach (x ${ARGN})
    if (x STREQUAL "WDIR")
      set (current "wdir")
    elseif (x STREQUAL "RELATIVE")
      set (relative TRUE)
      unset (current)
    elseif (current)
      set (current ${x})
      unset (current)
    else ()
      message (FATAL_ERROR "incorrect call to subdir_list")
    endif ()
  endforeach ()

  #
  file (GLOB children RELATIVE ${wdir} ${wdir}/*)
  set (dirlist "")
  foreach (child ${children})
    if (IS_DIRECTORY ${wdir}/${child})
      if (${relative})
        list (APPEND dirlist ${child})
      else()
        list (APPEND dirlist ${wdir}/${child})
      endif()
    endif()
  endforeach()
  set (${resultvar} ${dirlist} PARENT_SCOPE)
endfunction()
