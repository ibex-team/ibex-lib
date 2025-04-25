cmake_minimum_required (VERSION 3.0.2)

project (IBEX_EXAMPLES LANGUAGES CXX)

include (FindPkgConfig)
pkg_search_module (IBEX REQUIRED ibex)
message (STATUS "Found Ibex version ${IBEX_VERSION}")

add_compile_options(-O3 -DNDEBUG)

if (WIN32)
  add_definitions(-U__STRICT_ANSI__)
endif ()

#
set (LAB_EXAMPLES lab1 lab2 lab3 lab4 lab5 lab6 lab7 lab8)
set (DOC_EXAMPLES doc-arithmetic doc-contractor doc-covfiles doc-modeling
                  doc-separator doc-set doc-sivia doc-solver doc-strategy
                  doc-tutorial)
if (IBEX_LP_LIB_NAME)
  list (APPEND DOC_EXAMPLES doc-optim)
else ()
  set (MSG "Cannot compile example 'doc-optim', it requires Ibex with a"
           "linear programming library.")
  add_custom_target (doc-optim COMMAND ${CMAKE_COMMAND} -E echo ${MSG})
endif ()

foreach (ex_name ${LAB_EXAMPLES} ${DOC_EXAMPLES})
  add_executable (${ex_name} ${ex_name}.cpp)
  target_compile_options (${ex_name} PUBLIC ${IBEX_CFLAGS})
  target_include_directories (${ex_name} PUBLIC ${IBEX_INCLUDE_DIRS})
  target_link_libraries (${ex_name} PUBLIC ${IBEX_LDFLAGS})
endforeach ()

#
set (SLAM_EXAMPLES slam1 slam2 slam3)

foreach (ex_name ${SLAM_EXAMPLES})
  add_executable (${ex_name} slam/${ex_name}.cpp slam/data.cpp slam/data.h)
  target_compile_options (${ex_name} PUBLIC ${IBEX_CFLAGS})
  target_include_directories (${ex_name} PUBLIC ${IBEX_INCLUDE_DIRS})
  target_link_libraries (${ex_name} PUBLIC ${IBEX_LDFLAGS})
endforeach ()
