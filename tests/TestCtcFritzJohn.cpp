/* ============================================================================
 * I B E X - Fritz-John contractor Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 07, 2013
 * ---------------------------------------------------------------------------- */

#include "TestCtcFritzJohn.h"
#include "ibex_CtcFritzJohn.h"
#include "ibex_SystemFactory.h"
#include "ibex_SyntaxError.h"

using namespace std;

namespace ibex {

void TestCtcFritzJohn::test01() {
	SystemFactory fac;

	Variable x("x"),y("y"),z;
	fac.add_var(x);
	fac.add_var(y);

	fac.add_goal(-x-y);

	fac.add_ctr(sqr(x)+sqr(y)<=1);

	System sys(fac);

	CtcFritzJohn fj(sys);
	double s=::sqrt(2)/2.0;
	double _box[][2] = {{0.7,0.8},{0.7,0.8},{NEG_INFINITY,POS_INFINITY}};
	//double _box[][2] = {{s,s},{s,s}};
	IntervalVector box(3,_box);
	try {
	fj.contract(box);
	} catch(EmptyBoxException& e) {

	}

	TEST_ASSERT(!fj.ext_box.is_empty());

	//cout << fj.ext_box << endl;

	Interval& u=fj.ext_box[2];
	Interval& l=fj.ext_box[3];
	TEST_ASSERT(almost_eq(u+l, Interval(1,1), ERROR));
	TEST_ASSERT(almost_eq(-u+::sqrt(2)*l, Interval::ZERO, ERROR));
}

//void TestCtcFritzJohn::test01() {
//	SystemFactory fac;
//
//	Variable x,y,z;
//	fac.add_var(x);
//	fac.add_var(y);
//	fac.add_var(z);
//
//	fac.add_goal(z);
//
//	fac.add_ctr(sqr(x)+sqr(y)+sqr(z)<=1);
//	fac.add_ctr(x+y+z<=1);
//
//	System sys(fac);
//
//	CtcFritzJohn fj(sys);
//
//	double _box[][2] = {{0,1},{0,1},{0,1}};
//	IntervalVector box(3,_box);
//
//	fj.contract(box);
//
//	cout << fj.ext_box << endl;
//}

} // end namespace
