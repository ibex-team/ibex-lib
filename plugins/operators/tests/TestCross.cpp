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

#include "TestCross.h"
#include "ibex_ExprOperators.h"

using namespace std;

namespace ibex {

void TestCrossProduct::eval1() {
	double _x1[]={1,2,3};
	double _x2[]={4,5,6};
	double _y[]= {-3,6,-3};
	Vector x1(3,_x1);
	Vector x2(3,_x2);
	Vector y(3,_y);
	CPPUNIT_ASSERT(cross(x1,x2)==y);
}

} // end namespace
