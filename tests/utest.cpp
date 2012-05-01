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

// ================ level 0 ===============
#include "TestInterval.h"
#include "TestIntervalVector.h"
#include "TestIntervalMatrix.h"
#include "TestDim.h"
//#include "TestDomain.h"

// ================ level 1 ===============
#include "TestExpr.h"
#include "TestFunction.h"
#include "TestEval.h"
#include "TestHC4Revise.h"
#include "TestHC4.h"

// ================ level 2 ===============
#include "TestHC4.h"



int main() {

    Test::TextOutput output(Test::TextOutput::Verbose);

    Test::Suite ts;
    ts.add(auto_ptr<Test::Suite>(new TestInterval()));
    ts.add(auto_ptr<Test::Suite>(new TestIntervalVector()));
    ts.add(auto_ptr<Test::Suite>(new TestIntervalMatrix()));
    ts.add(auto_ptr<Test::Suite>(new TestDim()));
    //ts.add(auto_ptr<Test::Suite>(new TestDomain()));

    ts.add(auto_ptr<Test::Suite>(new TestExpr()));
    ts.add(auto_ptr<Test::Suite>(new TestFunction()));
    ts.add(auto_ptr<Test::Suite>(new TestEval()));
    ts.add(auto_ptr<Test::Suite>(new TestHC4Revise()));

    ts.add(auto_ptr<Test::Suite>(new TestHC4()));

    return ts.run(output,false) ? EXIT_SUCCESS : EXIT_FAILURE;

}
