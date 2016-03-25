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

#include "TestFritzJohn.h"
#include "ibex_FritzJohnCond.h"
#include "ibex_CtcHC4.h"
#include "ibex_SystemFactory.h"
#include "ibex_SyntaxError.h"

using namespace std;

namespace ibex {

void TestFritzJohn::test01() {
	SystemFactory fac;

	Variable x("x"),y("y");
	fac.add_var(x);
	fac.add_var(y);

	fac.add_goal(-x-y);

	fac.add_ctr(sqr(x)+sqr(y)<=1);

	System sys(fac);

	FritzJohnCond fj(sys);

	CtcHC4 hc4(fj.ctrs);
	//double s=::sqrt(2)/2.0;
	double _box[][2] = {{0.7,0.8},{0.7,0.8},{0,1},{NEG_INFINITY,POS_INFINITY}};

	IntervalVector box(4,_box);

	hc4.contract(box);

	CPPUNIT_ASSERT(!box.is_empty());

	Interval& u=box[2];
	Interval& l=box[3];

	CPPUNIT_ASSERT((u+l).contains(1));
	// we have
	//   u =1-l
	//   u =l*sqrt(2)
	// The solution is u=1/(1+1/sqrt(2)).
	//                 l=1/(sqrt(2)+1)

	CPPUNIT_ASSERT(u.contains(1/(1+::sqrt(2)/2)));
	CPPUNIT_ASSERT(l.contains(1/(1+::sqrt(2))));
	CPPUNIT_ASSERT((-u+::sqrt(2)*l).contains(0));
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
