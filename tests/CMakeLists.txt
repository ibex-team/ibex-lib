cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(tests)

file(GLOB Test_SRC
	# TestInterval.cpp
	# TestIntervalVector.cpp
	# TestIntervalMatrix.cpp
	# TestDim.cpp
	# TestArith.cpp
	# TestInnerArith.cpp
	# TestInterval.h
	# TestIntervalVector.h
	# TestIntervalMatrix.h
	# TestDim.h
	# TestArith.h
	# TestInnerArith.h
	# utils.cpp
	# utils.h
	# utest.cpp
	# utest.h
    "*.h"
    "*.cpp"
)
if( NOT WITH_CLP)
	list(REMOVE_ITEM Test_SRC ${CMAKE_CURRENT_SOURCE_DIR}/TestCtcPolytopHull.cpp)
	list(REMOVE_ITEM Test_SRC ${CMAKE_CURRENT_SOURCE_DIR}/TestFritzJohn.cpp)
else(NOT WITH_CLP)
	pkg_check_modules(PC_CLP REQUIRED clp)
	add_definitions(${PC_CLP_CFLAGS_OTHER})
	include_directories(${PC_CLP_INCLUDEDIR} ${PC_CLP_INCLUDE_DIRS} ${PC_CLP_INCLUDES})
	set(LIBS ${LIBS} ${PC_CLP_LIBRARIES})
endif(NOT WITH_CLP)



list(REMOVE_ITEM Test_SRC ${CMAKE_CURRENT_SOURCE_DIR}/nonreg.cpp)
#pkg_check_modules(PC_IBEX REQUIRED ibex)
#add_definitions(${PC_IBEX_CFLAGS_OTHER})
#include_directories(${PC_IBEX_INCLUDEDIR} ${PC_IBEX_INCLUDE_DIRS} ${PC_IBEX_INCLUDES})
#set(LIBS ${LIBS} -L/home/biop/programmation/devel/lib ${PC_IBEX_LIB_DIRS} ${PC_IBEX_LIBRARIES})
#if(NOT WIN32) # [TODO] better to chech retrun of find_package(PkgConfig)
#find_package(PkgConfig)
#pkg_check_modules(PC_CPPTEST REQUIRED libcpptest)
#add_definitions(${PC_CPPTEST_CFLAGS_OTHER})
#include_directories(${PC_CPPTEST_INCLUDEDIR} ${PC_CPPTEST_INCLUDE_DIRS} ${PC_CPPTEST_INCLUDES})
#set(LIBS ${LIBS} ${PC_CPPTEST_LIBRARIES})
find_package(cpptest)
if(CPPTEST_FOUND)
  message(STATUS ${CPPTEST_INCLUDE_DIRS} ${CPPTEST_LIBRARIES})
  INCLUDE_DIRECTORIES(${CPPTEST_INCLUDE_DIRS})
  set(LIBS ${LIBS} ${CPPTEST_LIBRARIES})
endif(CPPTEST_FOUND)


file(COPY quimper DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ../benchs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#endif(NOT WIN32)

add_executable (utest ${Test_SRC})
message(STATUS ${LIBS})

target_link_libraries(utest ${LIBS} ibex prim)
# target_link_libraries(utest -L/home/biop/programmation/devel/lib -lprim)
add_test(unitTest utest)


