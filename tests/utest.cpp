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

#include <stdlib.h>
#include <cpptest.h>
#include <memory>

// ================ tools ===============
#include "TestString.h"
#include "TestBitSet.h"
#include "TestSymbolMap.h"
#include "TestCellHeap.h"
#include "TestPixelMap.h"
#include "TestDoubleHeap.h"

// ================ arithmetic ===============
#include "TestInterval.h"
#include "TestIntervalVector.h"
#include "TestIntervalMatrix.h"
#include "TestDim.h"
#include "TestArith.h"
#include "TestInnerArith.h"
#include "TestAffine.h"
#include "TestAffine2.h"
//#include "TestDomain.h"

// ================ symbolic ===============
#include "TestExpr.h"
#include "TestExprCopy.h"
#include "TestExprDiff.h"
#include "TestExprCmp.h"
#include "TestExpr2DAG.h"
#include "TestExprSplitOcc.h"
#include "TestFunction.h"
#include "TestNumConstraint.h"
#include "TestEval.h"
#include "TestGradient.h"
#include "TestHC4Revise.h"
#include "TestInHC4Revise.h"

// ================ parser ===============
#include "TestParser.h"
#include "TestSystem.h"

// ================ system ===============
#include "TestFritzJohn.h"

// ================ numeric ===============
#include "TestLinear.h"
#include "TestNewton.h"

// ================ predicates ===============
#include "TestPdcHansenFeasibility.h"

// ================ contractor ===============
#include "TestCtcHC4.h"
#include "TestCtcInteger.h"
#include "TestCtcFwdBwd.h"
#include "TestCtcNotIn.h"
#include "TestCtcExist.h"
#include "TestCtcForAll.h"
#include "TestCtcPolytopeHull.h"
#include "TestCtcSegment.h"
#include "TestCtcPixelMap.h"

// ================ strategy ===============
#include "TestOptimizer.h"

// ================ set ===============
#include "TestSeparator.h"
#include "TestSepPolygon.h"

// ============= Conbinatorial ============
#include "TestQInter.h"

using namespace std;
using std::auto_ptr;

int main() {

	Interval x(Interval::EMPTY_SET);

    Test::TextOutput output(Test::TextOutput::Verbose);

    Test::Suite ts;

    ts.add(auto_ptr<Test::Suite>(new TestString()));
    ts.add(auto_ptr<Test::Suite>(new TestBitSet()));
    ts.add(auto_ptr<Test::Suite>(new TestSymbolMap()));
    ts.add(auto_ptr<Test::Suite>(new TestCellHeap()));
    ts.add(auto_ptr<Test::Suite>(new TestDoubleHeap()));
    ts.add(auto_ptr<Test::Suite>(new TestPixelMap()));

    ts.add(auto_ptr<Test::Suite>(new TestInterval()));
    ts.add(auto_ptr<Test::Suite>(new TestIntervalVector()));
    ts.add(auto_ptr<Test::Suite>(new TestIntervalMatrix()));
    ts.add(auto_ptr<Test::Suite>(new TestDim()));
    ts.add(auto_ptr<Test::Suite>(new TestArith()));
    ts.add(auto_ptr<Test::Suite>(new TestInnerArith()));
    //ts.add(auto_ptr<Test::Suite>(new TestDomain()));

    ts.add(auto_ptr<Test::Suite>(new TestAffine<AF_Linear>()));
    ts.add(auto_ptr<Test::Suite>(new TestAffine<AF_Default>()));
    ts.add(auto_ptr<Test::Suite>(new TestAffine2<AF_Linear>()));
    ts.add(auto_ptr<Test::Suite>(new TestAffine2<AF_Default>()));

    ts.add(auto_ptr<Test::Suite>(new TestExpr()));
    ts.add(auto_ptr<Test::Suite>(new TestExprCopy()));
    ts.add(auto_ptr<Test::Suite>(new TestExprDiff()));
    ts.add(auto_ptr<Test::Suite>(new TestExprCmp()));
    ts.add(auto_ptr<Test::Suite>(new TestExpr2DAG()));
    ts.add(auto_ptr<Test::Suite>(new TestExprSplitOcc()));
    ts.add(auto_ptr<Test::Suite>(new TestFunction()));
    ts.add(auto_ptr<Test::Suite>(new TestNumConstraint()));
    ts.add(auto_ptr<Test::Suite>(new TestEval()));

    ts.add(auto_ptr<Test::Suite>(new TestParser()));
    ts.add(auto_ptr<Test::Suite>(new TestSystem()));

    ts.add(auto_ptr<Test::Suite>(new TestHC4Revise()));
    ts.add(auto_ptr<Test::Suite>(new TestInHC4Revise()));
    ts.add(auto_ptr<Test::Suite>(new TestGradient()));

    ts.add(auto_ptr<Test::Suite>(new TestLinear()));
    ts.add(auto_ptr<Test::Suite>(new TestNewton()));

    ts.add(auto_ptr<Test::Suite>(new TestPdcHansenFeasibility()));

    ts.add(auto_ptr<Test::Suite>(new TestCtcHC4()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcInteger()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcFwdBwd()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcNotIn()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcExist()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcForAll()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcPolytopeHull()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcSegment()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcPixelMap()));
    ts.add(auto_ptr<Test::Suite>(new TestQInter()));

    ts.add(auto_ptr<Test::Suite>(new TestFritzJohn()));


    ts.add(auto_ptr<Test::Suite>(new TestOptimizer()));
    ts.add(auto_ptr<Test::Suite>(new TestSeparator()));
    ts.add(auto_ptr<Test::Suite>(new TestSepPolygon()));



    return ts.run(output,false) ? EXIT_SUCCESS : EXIT_FAILURE;

}
