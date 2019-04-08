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

#include "TestSinc.h"
#include "ibex_ExprOperators.h"
#include "ibex_DefaultSolver.h"
#include "ibex_SystemFactory.h"

using namespace std;

namespace ibex {

void TestSinc::eval1() {
	Interval x(1,2);
	CPPUNIT_ASSERT(sinc(x)==(sin(x)/x));
}

void TestSinc::callbacks() {
	Domain a(Dim::scalar());
	a.i()=Interval(1,2);
	Domain b(Dim::scalar());
	b.i()=sinc(a.i());
  	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprGenericUnaryOp& y=ExprGenericUnaryOp::new_("sinc",x);

	CPPUNIT_ASSERT(y.size==2);
	CPPUNIT_ASSERT(y.dim.type()==Dim::SCALAR);
	CPPUNIT_ASSERT(y.eval(a)==b);
	CPPUNIT_ASSERT(sameExpr(y,"sinc(x)"));

	Interval a_save=a.i();
	y.bwd(b,a);
	CPPUNIT_ASSERT(a.i().is_subset(a_save));
	CPPUNIT_ASSERT(b.i().is_subset(sinc(a.i())));

	Domain g(Dim::scalar());
	g.i()=Interval::ONE;
	CPPUNIT_ASSERT(y.num_diff(a,g).i()==(a.i()*cos(a.i())-sin(a.i()))/sqr(a.i()));

	const ExprNode& gnode=ExprConstant::new_scalar(Interval::ONE);
	const ExprNode& e=y.symb_diff(x,gnode);
	CPPUNIT_ASSERT(e.size == 9);
	cleanup(y,true);
}

void TestSinc::simplify() {
    Interval cst(2,2);
  	const ExprConstant& x=ExprConstant::new_scalar(cst);
	const ExprNode& y=ExprGenericUnaryOp::new_("sinc",x);
	const ExprNode& z=y.simplify();
	const ExprConstant* zz=dynamic_cast<const ExprConstant*>(&z);
	CPPUNIT_ASSERT(zz!=NULL);
	CPPUNIT_ASSERT(zz->get_value()==sinc(Interval(2,2)));
	cleanup(z,true);
}

void TestSinc::gradient() {
    const ExprSymbol& _x=ExprSymbol::new_("x");
	const ExprGenericUnaryOp& y=ExprGenericUnaryOp::new_("sinc",_x);
    const ExprNode& e=y*_x;
    Function f(_x,e);

    IntervalVector x(1,Interval(2,2));
    IntervalVector g=f.gradient(x);
    CPPUNIT_ASSERT(g[0]==x[0]*(x[0]*cos(x[0])-sin(x[0]))/sqr(x[0]) + sinc(x[0]));
}


void TestSinc::solve() {
    const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprNode& y=ExprGenericUnaryOp::new_("sinc",x);

    SystemFactory fac;
    fac.add_var(x);
    fac.add_ctr(y=Interval(0.5,0.5));
    System sys(fac);

    DefaultSolver solver(sys,1e-7,1e-7);
	solver.solve(IntervalVector(1,Interval(-100,100)));

    CPPUNIT_ASSERT(solver.get_data().nb_solution()==2);
    CPPUNIT_ASSERT(solver.get_data().nb_unknown()==1);
    Interval sol=solver.get_data().solution(1)[0];
    CPPUNIT_ASSERT(almost_eq(sin(sol),0.5*sol,1e-6));
}



} // end namespace
