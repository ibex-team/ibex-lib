/* ============================================================================
 * I B E X - Main unit testing program
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 07, 2011
 * ---------------------------------------------------------------------------- */

#include "TestAgenda.h"
#include "TestArith.h"
#include "TestBitSet.h"
#include "TestBoolInterval.h"
#include "TestBxpSystemCache.h"
#include "TestCell.h"	
#include "TestCellHeap.h"
#include "TestCov.h"
#include "TestCross.h"
#include "TestCtcExist.h"
#include "TestCtcForAll.h"
#include "TestCtcFwdBwd.h"
#include "TestCtcHC4.h"
#include "TestCtcInteger.h"
#include "TestCtcNotIn.h"
#include "TestCtcPolytopeHull.h"
#include "TestDim.h"
#include "TestDomain.h"
#include "TestDoubleHeap.h"
#include "TestDoubleIndex.h"
#include "TestEval.h"
#include "TestExpr2DAG.h"
#include "TestExpr2Minibex.h"
#include "TestExprCmp.h"
#include "TestExprCopy.h"
#include "TestExpr.h"
#include "TestExprDiff.h"
#include "TestExprLinearity.h"
#include "TestExprMonomial.h"
#include "TestExprPolynomial.h"
#include "TestExprSimplify2.h"
#include "TestExprSimplify.h"
//#include "TestFncKuhnTucker.h"
#include "TestFunction.h"
#include "TestGradient.h"
#include "TestHC4Revise.h"
#include "TestInHC4Revise.h"
#include "TestInnerArith.h"
#include "TestInterval.h"
#include "TestIntervalMatrix.h"
#include "TestIntervalVector.h"
#include "TestKernel.h"
#include "TestKuhnTuckerSystem.h"
#include "TestLinear.h"
#include "TestLPSolver.h"
#include "TestNewton.h"
#include "TestNumConstraint.h"
#include "TestOptimizer.h"
#include "TestParser.h"
#include "TestPdcHansenFeasibility.h"
#include "TestRoundRobin.h"
#include "TestSeparator.h"
#include "TestSet.h"
#include "TestSinc.h"
#include "TestSolver.h"
#include "TestString.h"
#include "TestSymbolMap.h"
#include "TestSystem.h"
#include "TestTimer.h"
#include "TestTrace.h"
#include "TestUnconstrainedLocalSearch.h"
#include "TestVarSet.h"

bool SUCCESS;


int main() {

	SUCCESS = true;
	
	TestAgenda().testMe();
	TestArith().testMe();
	TestBitSet().testMe();
	TestBoolInterval().testMe();
	TestBxpSystemCache().testMe();
	TestCell().testMe();	
	TestCellHeap().testMe();
	TestCov().testMe();
	TestCrossProduct().testMe();
	TestCtcExist().testMe();
	TestCtcForAll().testMe();
	TestCtcFwdBwd().testMe();
	TestCtcHC4().testMe();
	TestCtcInteger().testMe();
	TestCtcNotIn().testMe();
	TestCtcPolytopeHull().testMe();
	TestDim().testMe();
	TestDomain().testMe();
	TestDoubleHeap().testMe();
	TestDoubleIndex().testMe();
	TestEval().testMe();
	TestExpr2DAG().testMe();
	TestExpr2Minibex().testMe();
	TestExprCmp().testMe();
	TestExprCopy().testMe();
	TestExpr().testMe();
	TestExprDiff().testMe();
	TestExprLinearity().testMe();
	TestExprMonomial().testMe();
	TestExprPolynomial().testMe();
	TestExprSimplify2().testMe();
	TestExprSimplify().testMe();
//	TestFncKuhnTucker().testMe();
	TestFunction().testMe();
	TestGradient().testMe();
	TestHC4Revise().testMe();
	TestInHC4Revise().testMe();
	TestInnerArith().testMe();
	TestInterval().testMe();
	TestIntervalMatrix().testMe();
	TestIntervalVector().testMe();
	TestKernel().testMe();
	TestKuhnTuckerSystem().testMe();
	TestLinear().testMe();
	TestLinearSolver().testMe();
	TestNewton().testMe();
	TestNumConstraint().testMe();
	TestOptimizer().testMe();
	TestParser().testMe();
	TestPdcHansenFeasibility().testMe();
	TestRoundRobin().testMe();
	TestSeparator().testMe();
	TestSet().testMe();
	TestSinc().testMe();
	TestSolver().testMe();
	TestString().testMe();
	TestSymbolMap().testMe();
	TestSystem().testMe();
	//TestTimer().testMe();
	TestTrace().testMe();
	TestUnconstrainedLocalSearch().testMe();
	TestVarSet().testMe();
	
	exit(SUCCESS ? 0 : 1);
}
