/* ============================================================================
 * I B E X - Optimizer Tests
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * Last update : Oct 17, 2017
 * ---------------------------------------------------------------------------- */

#include "TestOptimizer.h"
#include "ibex_Optimizer.h"
#include "ibex_DefaultOptimizer.h"
#include "ibex_SystemFactory.h"

using namespace std;

namespace ibex {

void TestOptimizer::vec_problem01() {

	const ExprSymbol& x=ExprSymbol::new_(Dim::col_vec(3));

	SystemFactory f;
	f.add_var(x);
	f.add_ctr(x[0]*x[1]*x[2]>=1);
	f.add_goal(x*x);
	System sys(f);

	double prec=1e-3;
	DefaultOptimizer o(sys,
			Optimizer::default_rel_eps_f,
			Optimizer::default_abs_eps_f,
			NormalizedSystem::default_eps_h, false, false); // no INHC4
	Optimizer::Status status=o.optimize(IntervalVector(3,Interval(0,10)));

	CPPUNIT_ASSERT(status==Optimizer::SUCCESS);
	CPPUNIT_ASSERT(o.get_loup()>=3 && o.get_uplo()<=3);
	CPPUNIT_ASSERT(almost_eq(o.get_loup_point(),Vector::ones(3),0.1));
}

void TestOptimizer::vec_problem02() {
	const ExprSymbol& alpha=ExprSymbol::new_();
	const ExprSymbol& x=ExprSymbol::new_(Dim::col_vec(2));

	const ExprNode& R=Return(Return(cos(alpha),-sin(alpha),ExprVector::ROW), Return(sin(alpha),cos(alpha),ExprVector::ROW), ExprVector::COL);

	Vector v=Vector::ones(2);

	SystemFactory f;
	f.add_var(alpha);
	f.add_var(x);
	f.add_ctr(R*x=-x);
	f.add_goal(v*x);
	System sys(f);

	double prec=1e-3;
	DefaultOptimizer o(sys,
			Optimizer::default_rel_eps_f,
			Optimizer::default_abs_eps_f,
			NormalizedSystem::default_eps_h, false, false); // no INHC4

	IntervalVector box=cart_prod(Interval(-1,1)*Interval::PI, Interval(-10,10), Interval(-10,10));
	Optimizer::Status status=o.optimize(box);
	o.report();

	CPPUNIT_ASSERT(status==Optimizer::SUCCESS);
	CPPUNIT_ASSERT(o.get_loup()>=-20 && o.get_uplo()<=-20);
	Vector sol=o.get_loup_point().mid();
	CPPUNIT_ASSERT(almost_eq(fabs(sol[0]),Interval::PI.mid(),1e-3));
	CPPUNIT_ASSERT(almost_eq(sol[1],-10,1e-3));
	CPPUNIT_ASSERT(almost_eq(sol[2],-10,1e-3));
}

// true minimum is 0.
Optimizer::Status issue50(double init_loup, double prec) {
	SystemFactory f;
	const ExprSymbol& x=ExprSymbol::new_();
	f.add_var(x);
	f.add_ctr(x>=0);
	f.add_goal(x);

	System sys(f);
	DefaultOptimizer o(sys,prec,prec,prec);

	IntervalVector init_box(1,Interval::ALL_REALS);
	Optimizer::Status st=o.optimize(init_box,init_loup);
	o.report(); //cout << "status=" << st << endl;
	return o.optimize(init_box,init_loup);
}

void TestOptimizer::issue50_1() {
	CPPUNIT_ASSERT(issue50(1e-10, 0.1)==Optimizer::NO_FEASIBLE_FOUND);
}

void TestOptimizer::issue50_2() {
	CPPUNIT_ASSERT(issue50(1e-10, 0)==Optimizer::SUCCESS);
}

void TestOptimizer::issue50_3() {
	CPPUNIT_ASSERT(issue50(-1e-10, 0.1)==Optimizer::NO_FEASIBLE_FOUND);
}

void TestOptimizer::issue50_4() {
	CPPUNIT_ASSERT(issue50(-1e-10, 0)==Optimizer::INFEASIBLE);
}

void TestOptimizer::unconstrained() {
	const ExprSymbol& x=ExprSymbol::new_();
	SystemFactory fac;
	fac.add_var(x);
	fac.add_goal(sqr(x));
	System sys(fac);
	DefaultOptimizer o(sys);
	o.optimize(IntervalVector(1,Interval(-10,10)));
	CPPUNIT_ASSERT(o.get_loup()>=0 && o.get_uplo()<=0);
}

} // end namespace
