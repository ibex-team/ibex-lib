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
#include "TestInterval.h"
#include "TestIntervalVector.h"

int main() {
    Test::TextOutput output(Test::TextOutput::Verbose);

    Test::Suite ts;
    ts.add(auto_ptr<Test::Suite>(new TestInterval()));
    ts.add(auto_ptr<Test::Suite>(new TestIntervalVector()));

    return ts.run(output,false) ? EXIT_SUCCESS : EXIT_FAILURE;
}
