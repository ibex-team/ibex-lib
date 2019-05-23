# TODO look for cppunit

# Compile common stuff for the tests
add_library (test_common STATIC utest.cpp utest.h
                                utils.cpp utils.h
                                ExFunction.cpp ExFunction.h
                                Instance.cpp Instance.h
                                Ponts30.cpp Ponts30.h
                                )
target_link_libraries (test_common PUBLIC cppunit)
target_link_libraries (test_common PUBLIC ibex)
target_compile_definitions (test_common PUBLIC -DSRCDIR_TESTS="${CMAKE_CURRENT_SOURCE_DIR}")

set (TESTS_LIST TestAgenda TestArith TestBitSet TestBoolInterval
                TestBxpSystemCache TestCell
                #TestCov #plugin/solver dependent
                #TestCtcExist #plugin/solver dependent
                #TestCtcForAll #plugin/solver dependent
                TestCtcFwdBwd TestCtcHC4 TestCtcInteger TestCtcNotIn TestDim
                TestDomain TestDoubleHeap TestDoubleIndex TestEval TestExpr2DAG
                TestExpr2Minibex TestExprCmp TestExprCopy TestExpr TestExprDiff
                TestExprLinearity TestExprSimplify
                #TestFncKhunTucker #plugin/solver dependent
                TestFritzJohn TestFunction TestGradient TestHC4Revise
                TestInHC4Revise TestInnerArith TestInterval TestIntervalMatrix
                TestIntervalVector TestKernel TestLinear TestLPSolver
                TestNewton TestNumConstraint TestParser TestPdcHansenFeasibility
                TestRoundRobin TestSeparator TestSet
                #TestSolver #plugin/solver dependent
                TestString TestSymbolMap TestSystem TestTimer TestVarSet
                )

foreach (test ${TESTS_LIST})
  # /!\ The test and the target building the executable have the same name
  add_executable (${test} ${test}.cpp ${test}.h)
  target_link_libraries (${test} test_common)
  add_dependencies (check ${test})
  add_test (${test} ${test})
endforeach ()
