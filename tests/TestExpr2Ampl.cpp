/* ============================================================================
 * I B E X - Expression/Function serialization tests
 * ============================================================================
 * Copyright   : ENSTA Bretagne (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan NININ
 * Created     : Jul 05, 2019
 * ---------------------------------------------------------------------------- */

#include "TestExpr2Ampl.h"
#include "ibex_Expr2Ampl.h"
#include "ibex_System.h"
#include "ibex_SystemFactory.h"

#include <sstream>

using namespace std;

namespace ibex {

static System* amplex1() {
	SystemFactory fac;
	Variable x(3,"x");
	Variable A(3,3,"A");
	Variable y("y");

	fac.add_var(x);
	fac.add_var(A);
	fac.add_var(y);
	fac.add_goal(y-cos(x[1]));
	IntervalVector a(3,0);
	fac.add_ctr(A*x=a);
	fac.add_ctr(y>=x[0]);

	return new System(fac);
}

static System* amplex2() {
	SystemFactory fac;
	Variable x(3,"x");
	Variable y("y");

	fac.add_var(x);
	fac.add_var(y);
	fac.add_goal(y-cos(x[1]));
	IntervalVector a(3,0);
	fac.add_ctr(x[0]+x[1]>=0);
	fac.add_ctr(x[1]+x[2]<=y);
	fac.add_ctr(y>=x[0]);

	return new System(fac);
}

static System* amplex3() {
	SystemFactory fac;
	Variable x("x");
	Variable y("y");

	fac.add_var(x);
	fac.add_var(y);
	fac.add_ctr(x+y=Interval(-1,1));
	fac.add_ctr(x-y<=1);
	fac.add_ctr(x-y>=-1);
	fac.add_ctr(x=y);

	return new System(fac);
}



void TestExpr2Ampl::expr01() {

	System& sys(*amplex1());
	cout << sys.toAmpl();
	CPPUNIT_ASSERT(true);
}

void TestExpr2Ampl::expr02() {


	System& sys(*amplex2());
	cout << sys.toAmpl();
	CPPUNIT_ASSERT(true);
}

void TestExpr2Ampl::expr03() {



	System& sys(*amplex3());
	cout << sys.toAmpl();
	CPPUNIT_ASSERT(false);
}

} // end namespace

