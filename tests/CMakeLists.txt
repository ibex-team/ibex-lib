# Looking for cppunit
find_package (CppUnit)

# Only add the test targets if cppunit was found
if (CPPUNIT_FOUND)
  # Compile common stuff for the tests
  add_library (test_common utest.cpp utils.cpp utils.h Ponts30.cpp Ponts30.h)
  target_link_libraries (test_common PUBLIC ibex CppUnit::CppUnit)
  
  set (srcdir_test_flag -DSRCDIR_TESTS="${CMAKE_CURRENT_SOURCE_DIR}")

  set (TESTS_LIST TestAgenda TestArith TestBitSet TestBoolInterval
                  TestBxpSystemCache TestCell TestCov TestCross TestCtcExist
                  TestCtcForAll TestCtcFwdBwd TestCtcHC4 TestCtcInteger
                  TestCtcNotIn TestDim TestDomain TestDoubleHeap TestDoubleIndex
                  TestEval TestExpr2DAG TestExpr2Minibex TestExprCmp
                  TestExprCopy TestExpr TestExprDiff TestExprLinearity
                  TestExprMonomial TestExprPolynomial TestExprSimplify
                  TestExprSimplify2 TestFncKuhnTucker TestKuhnTuckerSystem
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
    target_link_libraries (${test} test_common ibex CppUnit::CppUnit)
    target_compile_definitions (${test} PRIVATE ${srcdir_test_flag})
    add_dependencies (check ${test})
    add_test (${test} ${test})
  endforeach ()
else ()
  message (STATUS "Will not run tests, required cppunit library was not found")
  set (MSG "No tests will be run as CMake failed to find the cppunit library \
during the configuration step. If you want to run the tests, you need to \
install cppunit. Then, you need to re-run the configuration step of CMake. You \
can use the CPPUNIT_DIR variable to indicate to CMake were to find it")
  add_custom_target (notest COMMAND ${CMAKE_COMMAND} -E echo ${MSG})
  add_dependencies (check notest)
endif ()
