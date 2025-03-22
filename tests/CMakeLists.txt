
add_executable (utest utest.cpp utils.cpp Ponts30.cpp 
TestAgenda.cpp
TestArith.cpp
TestBitSet.cpp
TestBoolInterval.cpp
TestBxpSystemCache.cpp
TestCell.cpp	
TestCellHeap.cpp
TestCov.cpp
TestCross.cpp
TestCtcExist.cpp
TestCtcForAll.cpp
TestCtcFwdBwd.cpp
TestCtcHC4.cpp
TestCtcInteger.cpp
TestCtcNotIn.cpp
TestCtcPolytopeHull.cpp
TestDim.cpp
TestDomain.cpp
TestDoubleHeap.cpp
TestDoubleIndex.cpp
TestEval.cpp
TestExpr2DAG.cpp
TestExpr2Minibex.cpp
TestExprCmp.cpp
TestExprCopy.cpp
TestExpr.cpp
TestExprDiff.cpp
TestExprLinearity.cpp
TestExprMonomial.cpp
TestExprPolynomial.cpp
TestExprSimplify2.cpp
TestExprSimplify.cpp
TestFncKuhnTucker.cpp
TestFunction.cpp
TestGradient.cpp
TestHC4Revise.cpp
TestInHC4Revise.cpp
TestInnerArith.cpp
TestInterval.cpp
TestIntervalMatrix.cpp
TestIntervalVector.cpp
TestKernel.cpp
TestKuhnTuckerSystem.cpp
TestLinear.cpp
TestLPSolver.cpp
TestNewton.cpp
TestNumConstraint.cpp
TestOptimizer.cpp
TestParser.cpp
TestPdcHansenFeasibility.cpp
TestRoundRobin.cpp
TestSeparator.cpp
TestSet.cpp
TestSinc.cpp
TestSolver.cpp
TestString.cpp
TestSymbolMap.cpp
TestSystem.cpp
TestTimer.cpp
TestTrace.cpp
TestUnconstrainedLocalSearch.cpp
TestVarSet.cpp
)

target_link_libraries (utest PUBLIC ibex)
  
set (srcdir_test_flag -DSRCDIR_TESTS="${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_definitions (utest PRIVATE ${srcdir_test_flag})

add_dependencies (check utest)

add_test(utest utest)


