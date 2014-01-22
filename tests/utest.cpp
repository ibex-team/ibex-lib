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
#include "TestSymbolMap.h"

// ================ arithmetic ===============
#include "TestInterval.h"
#include "TestIntervalVector.h"
#include "TestIntervalMatrix.h"
#include "TestDim.h"
#include "TestArith.h"
#include "TestInnerArith.h"
#include "TestAffine2.h"
//#include "TestDomain.h"

// ================ symbolic ===============
#include "TestExpr.h"
#include "TestExprDiff.h"
#include "TestExprSplitOcc.h"
#include "TestFunction.h"
#include "TestNumConstraint.h"
#include "TestEval.h"
#include "TestGradient.h"
#include "TestHC4Revise.h"
#include "TestInHC4Revise.h"
#include "TestHC4.h"

// ================ parser ===============
#include "TestParser.h"
#include "TestSystem.h"

// ================ numeric ===============
#include "TestLinear.h"
#include "TestNewton.h"

// ================ predicates ===============
#include "TestPdcHansenFeasibility.h"

// ================ contractor ===============
#include "TestHC4.h"
#include "TestCtcInteger.h"
//#include "TestCtcSubBox.h"
#include "TestCtcNotIn.h"
#include "TestCtcFritzJohn.h"

#include "TestAffine2.h"


using namespace std;
using std::auto_ptr;

int main() {

	Interval x(Interval::EMPTY_SET);

    Test::TextOutput output(Test::TextOutput::Verbose);

    Test::Suite ts;

    ts.add(auto_ptr<Test::Suite>(new TestString()));
    ts.add(auto_ptr<Test::Suite>(new TestSymbolMap()));

    ts.add(auto_ptr<Test::Suite>(new TestInterval()));
    ts.add(auto_ptr<Test::Suite>(new TestIntervalVector()));
    ts.add(auto_ptr<Test::Suite>(new TestIntervalMatrix()));
    ts.add(auto_ptr<Test::Suite>(new TestDim()));
    ts.add(auto_ptr<Test::Suite>(new TestArith()));
    ts.add(auto_ptr<Test::Suite>(new TestInnerArith()));
    //ts.add(auto_ptr<Test::Suite>(new TestDomain()));

    ts.add(auto_ptr<Test::Suite>(new TestAffine2()));

    ts.add(auto_ptr<Test::Suite>(new TestExpr()));
    ts.add(auto_ptr<Test::Suite>(new TestExprDiff()));
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

    ts.add(auto_ptr<Test::Suite>(new TestHC4()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcInteger()));
 //   ts.add(auto_ptr<Test::Suite>(new TestCtcSubBox()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcNotIn()));
    ts.add(auto_ptr<Test::Suite>(new TestCtcFritzJohn()));

    return ts.run(output,false) ? EXIT_SUCCESS : EXIT_FAILURE;

}
