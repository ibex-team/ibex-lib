# Looking for cppunit
set (DO_TEST ON)
set (CPPUNIT_DIR "" CACHE PATH "Path to cppunit lib and include directories")

# first try with pkg-config
include (FindPkgConfig)
pkg_check_modules(CPPUNIT cppunit)

# otherwise do it ourself
if (NOT CPPUNIT_FOUND)
  # Looking for cppunit/Test.h, result is written in CPPUNIT_INCDIR
  find_header_custom (CPPUNIT "cppunit/Test.h" PATHS "${CPPUNIT_DIR}")
  # Looking for cppunit library, result is written in CPPUNIT_LIB
  find_library_custom (CPPUNIT "cppunit" PATHS "${CPPUNIT_DIR}")

  if (NOT CPPUNIT_INCDIR OR NOT CPPUNIT_LIB)
    set (DO_TEST OFF)
    message (STATUS "Will not run tests, needed cppunit library was not found")
  endif ()
endif ()

# Only add the test targets if cppunit was found
if (DO_TEST)
  # Compile common stuff for the tests
  add_library (test_common STATIC utest.cpp utest.h utils.cpp utils.h
                                  ExFunction.cpp ExFunction.h Instance.cpp
                                  Instance.h Ponts30.cpp Ponts30.h)
  target_link_libraries (test_common PUBLIC ibex)
  set (srcdir_test_flag -DSRCDIR_TESTS="${CMAKE_CURRENT_SOURCE_DIR}")

  set (TESTS_LIST TestAgenda TestArith TestBitSet TestBoolInterval
                  TestBxpSystemCache TestCell TestCov TestCross TestCtcExist
                  TestCtcForAll TestCtcFwdBwd TestCtcHC4 TestCtcInteger
                  TestCtcNotIn TestDim TestDomain TestDoubleHeap TestDoubleIndex
                  TestEval TestExpr2DAG TestExpr2Minibex TestExprCmp
                  TestExprCopy TestExpr TestExprDiff TestExprLinearity TestExprMonomial
                  TestExprPolynomial TestExprSimplify TestExprSimplify2 TestFncKuhnTucker TestKuhnTuckerSystem
                  TestFunction TestGradient TestHC4Revise TestInHC4Revise
                  TestInnerArith TestInterval TestIntervalMatrix
                  TestIntervalVector TestKernel TestLinear TestLPSolver
                  TestNewton TestNumConstraint TestParser
                  TestPdcHansenFeasibility TestRoundRobin TestSeparator TestSet
                  TestSinc TestSolver TestString TestSymbolMap TestSystem
                  TestTimer TestTrace TestVarSet
                  TestCellHeap TestCtcPolytopeHull TestOptimizer TestUnconstrainedLocalSearch)

  foreach (test ${TESTS_LIST})
    # /!\ The test and the target building the executable have the same name
    add_executable (${test} ${test}.cpp ${test}.h)
    target_link_libraries (${test} test_common)
    if (CPPUNIT_FOUND) # found via pkg-config
      target_link_libraries (${test} ${CPPUNIT_LIBRARIES})
      target_include_directories (${test} PUBLIC ${CPPUNIT_INCLUDE_DIRS})
      target_compile_options (${test} PUBLIC ${CPPUNIT_CFLAGS_OTHER})
    else () # found ourself
      target_link_libraries (${test} ${CPPUNIT_LIB})
      target_include_directories (${test} PUBLIC ${CPPUNIT_INCDIR})
    endif ()
    target_compile_definitions (${test} PRIVATE ${srcdir_test_flag})
    add_dependencies (check ${test})
    add_test (${test} ${test})
  endforeach ()
else ()
  set (MSG "No tests will be run as CMake failed to find the cppunit library \
during the configuration step. If you want to run the tests, you need to \
install cppunit. Then, you need to re-run the configuration step of CMake. You \
can use the CPPUNIT_DIR variable to indicate to CMake were to find it")
  add_custom_target (notest COMMAND ${CMAKE_COMMAND} -E echo ${MSG})
  add_dependencies (check notest)
endif ()
