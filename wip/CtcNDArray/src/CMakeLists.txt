cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


project(CtcNDArray)

# C++11 for GCC 4.6
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
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


find_package(PkgConfig)
pkg_check_modules(PC_IBEX REQUIRED ibex)
add_definitions(${PC_IBEX_CFLAGS_OTHER})
include_directories(${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${PC_IBEX_INCLUDES})
set(LIBS ${LIBS} -L/home/biop/programmation/morse_sim/devel/lib ${PC_IBEX_LIB_DIRS} ${PC_IBEX_LIBRARIES})


pkg_check_modules(PC_CPPTEST REQUIRED libcpptest)
add_definitions(${PC_CPPTEST_CFLAGS_OTHER})
include_directories(${PC_CPPTEST_INCLUDEDIR} ${PC_CPPTEST_INCLUDE_DIRS} ${PC_CPPTEST_INCLUDES})
set(LIBS ${LIBS} ${PC_CPPTEST_LIBRARIES})

add_library(CtcNDArray ibex_CtcImage.cpp ibex_CtcVoxelGrid.cpp ibex_IntInterval.cpp ibex_NDArray.h)


#add_executable (test_NDArray ./test_NDArray.cpp ./src/NDArray.h ./src/NDArray.cpp)
#target_link_libraries(test_NDArray ${LIBS})

#add_executable (test_CtcImage ./test_CtcImage.cpp ./src/Array2D.h ./src/ibex_CtcImage.cpp ./src/ibex_CtcImage.h ./src/ibex_IntInterval.cpp ./src/ibex_IntInterval.h)
#target_link_libraries(test_CtcImage ${LIBS})





