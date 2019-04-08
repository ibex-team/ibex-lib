/* ============================================================================
 * I B E X - Operators Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Oct 7, 2018
 * ---------------------------------------------------------------------------- */

#include "TestTrace.h"
#include "ibex_ExprOperators.h"

using namespace std;

namespace ibex {

void TestTrace::eval1() {
	Interval x(1,2);
	CPPUNIT_ASSERT(sinc(x)==(sin(x)/x));
}

} // end namespace
