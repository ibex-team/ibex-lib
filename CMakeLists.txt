cmake_minimum_required(VERSION 2.8.3)
project(ibex-lib)
# build type
IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE "Release")
	#SET(CMAKE_BUILD_TYPE "DEBUG")
	#SET(CMAKE_BUILD_TYPE "RELWITHDEBINFO")
	#SET(CMAKE_BUILD_TYPE "MINSIZEREL")
ENDIF()

set(PACKAGE_VERSION "2.1.12")

# option declaration
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLE "Build example" ON)
option(WITH_FILIB ON)
option(WITH_CLP OFF)
option(GENERATE_PARSER_FILES OFF)
option(BUILD_SHARED OFF)

set(_IBEX_WITH_FILIB_ "1")
if(WITH_CLP)
	set(_IBEX_WITH_CLP_ "1")
else(WITH_CLP)
	set(_IBEX_WITH_NOLP_ "1")
endif(WITH_CLP)
# set(_IBEX_WITH_CXSC_ "1")
include(CheckCXXCompilerFlag)	
check_cxx_compiler_flag(-fPIC SUPPORT_FPIC)
if(SUPPORT_FPIC)
	set(CMAKE_CXX_FLAGS ${CMAKE_CSS_FLAGS} "-fPIC")
endif(SUPPORT_FPIC)

#disable warnings
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")

if(WIN32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

  STRING(REGEX REPLACE "\\\\" "/" CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
	set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()
endif(WIN32)



# Load the 'install_pkg_config_file' macro
include(InstallPkgConfigFile)

# RPath management http://www.cmake.org/Wiki/CMake_RPATH_handling
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
if("${isSystemDir}" STREQUAL "-1")
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

# if(WITH_FILIB)
	# if( NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/filibsrc)
		execute_process(
			COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_SOURCE_DIR}/3rd/filibsrc-3.0.2.2.tar.gz
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rd
		)
	# endif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/3rd/filibsrc)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/filib_config.h.in 
			${CMAKE_CURRENT_SOURCE_DIR}/3rd/filibsrc/config.h.in COPYONLY
	)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/filib_CMakeLists.txt.in
			${CMAKE_CURRENT_SOURCE_DIR}/3rd/filibsrc/CMakeLists.txt COPYONLY
	)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/filib_rounding_control_config.hpp.in
			${CMAKE_CURRENT_SOURCE_DIR}/3rd/filibsrc/rounding_control/rounding_control_config.hpp.in COPYONLY
	)

	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd/filibsrc)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/3rd/filibsrc)
# endif(WITH_FILIB)

add_subdirectory(src)

if (BUILD_TESTS)
	ENABLE_TESTING()
	include( CTest )
	INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src/include)
	MESSAGE(STATUS ${CTEST_ENVIRONMENT})
	add_subdirectory(tests)
endif(BUILD_TESTS)


#########################################################################
### Uninstall ###
#########################################################################
configure_file("${CMAKE_HOME_DIRECTORY}/CMakeModules/cmake_uninstall.cmake.in" "cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake")

# if (BUILD_EXAMPLE)
	# add_subdirectory(examples)
# endif(BUILD_EXAMPLE)

set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "IbexTeam")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Interval Library")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "12")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "ibex-lib")

#set(CPACK_COMPONENTS_ALL applications CTData instructionData)
#set(CPACK_COMPONENT_CTDATA_GROUP "data")
#set(CPACK_COMPONENT_INSTRUCTIONDATA_GROUP "data")
#
#set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "applications (hello & world)")
#set(CPACK_COMPONENT_GROUP_DATA_DESCRIPTION "data (CT and instructions)")

# SET(CPACK_GENERATOR "DEB")
# SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Benoit Desrochers") #required

INCLUDE(CPack)
