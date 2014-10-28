cmake_minimum_required(VERSION 2.6 FATAL_ERROR)


project(tests)

# C++11 for GCC 4.6
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules")

set(CMAKE_BUILD_TYPE Debug)


find_package(PkgConfig)
pkg_check_modules(PC_IBEX REQUIRED ibex)
add_definitions(${PC_IBEX_CFLAGS_OTHER})
include_directories(${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${PC_IBEX_INCLUDES})
set(LIBS ${LIBS} -L/home/biop/programmation/morse_sim/devel/lib ${PC_IBEX_LIB_DIRS} ${PC_IBEX_LIBRARIES})


pkg_check_modules(PC_CPPTEST REQUIRED libcpptest)
add_definitions(${PC_CPPTEST_CFLAGS_OTHER})
include_directories(${PC_CPPTEST_INCLUDEDIR} ${PC_CPPTEST_INCLUDE_DIRS} ${PC_CPPTEST_INCLUDES})
set(LIBS ${LIBS} ${PC_CPPTEST_LIBRARIES})


# add Ctc lib
set(LIBS ${LIBS} CtcNDArray)

add_executable (utest ./TestNDArray.cpp ./TestCtcVoxelGrid.cpp ./TestCtcImage.cpp ./utest.cpp)
target_link_libraries(utest ${LIBS})

#add_executable (test_CtcImage ./test_CtcImage.cpp ./src/Array2D.h ./src/ibex_CtcImage.cpp ./src/ibex_CtcImage.h ./src/ibex_IntInterval.cpp ./src/ibex_IntInterval.h)
#target_link_libraries(test_CtcImage ${LIBS})





