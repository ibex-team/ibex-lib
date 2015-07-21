/* ============================================================================
 * I B E X - Linear Relaxation Tests
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : July, 20 2015
 * ---------------------------------------------------------------------------- */

#include "TestLinearRelax.h"
#include "ibex_SystemFactory.h"

using namespace std;

namespace ibex {


void TestLinearRelax::test1_AF2(IntervalVector& box) {
	SystemFactory f;
	const ExprSymbol& x=ExprSymbol::new_();
	const ExprSymbol& y=ExprSymbol::new_();
	f.add_var(x);
	f.add_var(y);
	f.add_ctr( -x+2*y<=10);
	f.add_ctr(  x+2*y<=18);
	f.add_ctr(2*x+  y<=18);
	f.add_ctr(2*x-  y<=10);
	f.add_ctr(  x-2*y<= 2);
	f.add_ctr( -x-2*y<=-6);
	f.add_ctr(-2*x- y<=-6);
	f.add_ctr(-2*x+ y<=2);

	f.add_goal(x+y);

	ExtendedSystem sys(f);

	LinearRelaxAffine2 lin(sys);

	CtcPolytopeHull ctc(lin);


	ctc.contract(box);
}


void TestLinearRelax::test1() {
	IntervalVector res(3,Interval(1,7));
	res[2] = Interval(4,POS_INFINITY);

	IntervalVector box(3,Interval::ALL_REALS);
	test1_AF2(box);

	TEST_ASSERT(almost_eq(box, res, 1.e-8));


}



} // end namespace
