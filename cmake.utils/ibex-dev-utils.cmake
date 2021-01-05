################################################################################
################################################################################
################################################################################
# filter the input list(s) and return only elements matching regex
function (ibex_list_filter resultvar regex)
  foreach (filelist ${ARGN})
    foreach (file ${filelist})
      if (file MATCHES "${regex}")
        list (APPEND _list ${file})
      endif ()
    endforeach()
  endforeach()
  set (${resultvar} ${_list} PARENT_SCOPE)
endfunction ()

function (ibex_list_filter_header resultvar)
  ibex_list_filter (${resultvar} "\\.(h|hpp)$" ${ARGN})
  set (${resultvar} ${${resultvar}} PARENT_SCOPE)
endfunction ()

################################################################################
################################################################################
################################################################################
# Note: we need this one to be a macro, not a function, as we need to execute
# the following command in the same scope as the main CMakeLists.txt file
macro (ibex_add_make_target_for_ctest tgtname)
  include (CTest)

  if (ARGN)
    set (_depends DEPENDS ${ARGN})
  else ()
    set (_depends "")
  endif ()

  if ("${CMAKE_GENERATOR}" STREQUAL "Unix Makefiles")
    set (_cmd ${CMAKE_CTEST_COMMAND} --output-on-failure $(ARGS))
  else ()
    set (_cmd ${CMAKE_CTEST_COMMAND} --output-on-failure)
  endif ()

  if (BUILD_TESTING)
    add_custom_target (${tgtname} COMMAND ${_cmd} ${_depends}
                                  COMMENT "Running the tests")
    add_subdirectory (tests EXCLUDE_FROM_ALL)
  endif ()
endmacro ()

################################################################################
################################################################################
################################################################################
function (ibex_init_common)
  ##############################################################################
  # Options for install directory
  ##############################################################################
  set (CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C++ header files (include)")
  set (CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")
  set (CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
  set (CMAKE_INSTALL_PKGCONFIG "share/pkgconfig" CACHE PATH "pkg files (share/pkgconfig)")
  set (CMAKE_INSTALL_JARDIR "share/java" CACHE PATH "jar archives (share/java)")

  set (CMAKE_INSTALL_INCLUDEDIR_3RD ${CMAKE_INSTALL_INCLUDEDIR}/ibex/3rd PARENT_SCOPE)
  set (CMAKE_INSTALL_LIBDIR_3RD ${CMAKE_INSTALL_LIBDIR}/ibex/3rd PARENT_SCOPE)
  set (CMAKE_INSTALL_CONFIGCMAKE "share/ibex/cmake" PARENT_SCOPE)

  ##############################################################################
  # Print information (to ease debugging)
  ##############################################################################
  message (STATUS "Running on system ${CMAKE_HOST_SYSTEM} with processor ${CMAKE_HOST_SYSTEM_PROCESSOR}")
  if (CMAKE_CROSSCOMPILING)
    message (STATUS "Targeting system ${CMAKE_SYSTEM} with processor ${CMAKE_SYSTEM_PROCESSOR}")
  endif ()
  message (STATUS "Using CMake ${CMAKE_VERSION}")
  message (STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

  ##############################################################################
  # Options for shared or static library
  ##############################################################################
  option (BUILD_SHARED_LIBS "Set to ON to build shared libraries" OFF)
  if (BUILD_SHARED_LIBS)
    message (STATUS "Will build shared libraries")
  else ()
    message (STATUS "Will build static libraries")
  endif ()

  ##############################################################################
  # Ibex and its plugins need c++11
  ##############################################################################
  # we do not override (nor check!) the value if a choice was already made
  if (NOT CMAKE_CXX_STANDARD)
    set (CMAKE_CXX_STANDARD 11 PARENT_SCOPE)
    set (CMAKE_CXX_STANDARD_REQUIRED ON PARENT_SCOPE)
  endif ()

  ##############################################################################
  # Set flags and build type (release or debug)
  ##############################################################################
  include(CheckCXXCompilerFlag)
  if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message (STATUS "Setting build type to 'Release' as none was specified.")
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose type of build" FORCE)
    set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
  endif ()

  if(MSVC)
    set (CMAKE_CXX_FLAGS_RELEASE "/O2 /D NDEBUG /D _CRT_SECURE_NO_WARNINGS" PARENT_SCOPE)
    set (CMAKE_CXX_FLAGS_DEBUG "/Od /DEBUG /D DEBUG /D _CRT_SECURE_NO_WARNINGS" PARENT_SCOPE)
  else()
    set (CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" PARENT_SCOPE)
    set (CMAKE_CXX_FLAGS_DEBUG "-O0 -g -pg -Wall -DDEBUG" PARENT_SCOPE)
  endif()

  ##############################################################################
  # add uninstall command
  ##############################################################################
  if(NOT TARGET uninstall)
    set (_outfile ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
    # looking for file cmake_uninstall.cmake.in'
    if (EXISTS "cmake_uninstall.cmake.in")
      set (_infile cmake_uninstall.cmake.in)
    else ()
      foreach (_dir ${CMAKE_MODULE_PATH})
        set (_f ${_dir}/cmake_uninstall.cmake.in)
        if (EXISTS "${_f}")
          set (_infile ${_f})
          break ()
        endif ()
      endforeach ()
    endif ()

    if (_infile)
      configure_file (${_infile} ${_outfile} @ONLY)
      add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${_outfile})
    endif ()
  endif()
endfunction ()
