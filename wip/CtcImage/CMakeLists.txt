cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


project(CtcImages3D)

set(SRCS ./src/Array3D.cpp ./src/Array3D.h ./src/Voxel.cpp ./src/Voxel.h ./src/IntegralImg3D.cpp ./src/IntegralImg3D.h)


# C++11 for GCC 4.6
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")

set(CMAKE_BUILD_TYPE Debug)

# C++11 GDAL wrapper
#find_package(GdalWrap REQUIRED)
#include_directories(include "./include/*")
#include_directories(${GDALWRAP_INCLUDE_DIRS})

# atlaas
#pkg_check_modules(ATLAAS REQUIRED atlaas)
#include_directories(${ATLAAS_INCLUDE_DIRS})

#others file
include_directories("./src")

FIND_PACKAGE( OpenMP REQUIRED)
   if(OPENMP_FOUND)
   message("OPENMP FOUND")
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package(PkgConfig)
pkg_check_modules(PC_IBEX REQUIRED ibex)
add_definitions(${PC_IBEX_CFLAGS_OTHER})
include_directories(${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${PC_IBEX_INCLUDES})
set(LIBS ${LIBS} -L/home/biop/programmation/morse_sim/devel/lib ${PC_IBEX_LIB_DIRS} ${PC_IBEX_LIBRARIES})


pkg_check_modules(PC_CPPTEST REQUIRED libcpptest)
add_definitions(${PC_CPPTEST_CFLAGS_OTHER})
include_directories(${PC_CPPTEST_INCLUDEDIR} ${PC_CPPTEST_INCLUDE_DIRS} ${PC_CPPTEST_INCLUDES})
set(LIBS ${LIBS} ${PC_CPPTEST_LIBRARIES})



add_executable (test_Array3D ./test_Array3D.cpp ./src/Array3D.h)
target_link_libraries(test_Array3D ${LIBS})

add_executable (test_CtcVoxelGrid ./test_CtcVoxelGrid.cpp ./src/Array3D.h ./src/ibex_CtcVoxelGrid.cpp ./src/ibex_CtcVoxelGrid.h ./src/ibex_IntInterval.cpp ./src/ibex_IntInterval.h)
target_link_libraries(test_CtcVoxelGrid ${LIBS})






