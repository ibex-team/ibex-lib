/* ============================================================================
 * I B E X - CtcExist Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 05, 2014
 * ---------------------------------------------------------------------------- */

#include "TestCtcExist.h"
#include "ibex_CtcExist.h"

using namespace std;

namespace ibex {

void TestCtcExist::test01() {

	Variable x,y;
	Function f(x,y,1.5*sqr(x)+1.5*sqr(y)-x*y-0.2);

	CtcExist e(f,LEQ,0.001,IntervalVector(1,Interval(-10,10)));

	IntervalVector box(1,Interval(-10,10));
	try {
		e.contract(box);
	} catch(EmptyBoxException&) {
		TEST_ASSERT(false);
	}
	cout << "TEST EXIST=" << box << endl;
}

} // end namespace
