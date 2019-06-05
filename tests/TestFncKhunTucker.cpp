//============================================================================
//                                  I B E X                                   
// File        : TestFncKhunTucker.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 17, 2019
// Last Update : May 17, 2019
//============================================================================

#include "TestFncKhunTucker.h"
#include "ibex_FncKhunTucker.h"
#include "ibex_SystemFactory.h"

using namespace ibex;

void TestFncKhunTucker::one_var() {
	int n=1;

	const ExprSymbol& _x=ExprSymbol::new_("x");
	SystemFactory fac;
	fac.add_var(_x);
	fac.add_goal(sqr(_x));

	System sys(fac);
	sys.box[0]=Interval(-10,10);

	Function df(*sys.goal, Function::DIFF);

	NormalizedSystem nsys(sys);

	// --------------------------------------------------------------------
	IntervalVector x(1);
	x[0]=Interval(1,2);

	int m=1; // number of multipliers

	FncKhunTucker fkt(nsys,df,NULL,x,BitSet::empty(1));

	CPPUNIT_ASSERT(fkt.n == n);
	CPPUNIT_ASSERT(fkt.nb_mult==m);
	CPPUNIT_ASSERT(fkt.eq.empty());
	CPPUNIT_ASSERT(fkt.ineq.empty());
	CPPUNIT_ASSERT(fkt.bound_left.empty());
	CPPUNIT_ASSERT(fkt.bound_right.empty());

	IntervalVector lambda(m);
	lambda[0]=Interval(2,3);

	IntervalVector xlambda(n+m);
	xlambda.put(0,x);
	xlambda.put(n,lambda);

	IntervalVector fktx=fkt.eval_vector(xlambda,BitSet::all(n+m));

	CPPUNIT_ASSERT(fktx[0]==2*x[0]*lambda[0]);
	CPPUNIT_ASSERT(fktx[1]==lambda[0]-1); // normalization equation
	CPPUNIT_ASSERT(fkt.multiplier_domain()==IntervalVector(1,Interval(0,1)));

	IntervalMatrix J=((Fnc&) fkt).jacobian(xlambda);
	CPPUNIT_ASSERT(J[0][0]==2*lambda[0]);
	CPPUNIT_ASSERT(J[0][1]==2*x[0]);
	CPPUNIT_ASSERT(J[1][0]==Interval::zero());
	CPPUNIT_ASSERT(J[1][1]==Interval::one());

}

// Test n°2
// One variable, one active bound
void TestFncKhunTucker::one_var_1_bound() {
	int n=1;

	const ExprSymbol& _x=ExprSymbol::new_("x");
	SystemFactory fac;
	fac.add_var(_x);
	fac.add_goal(sqr(_x));

	System sys(fac);
	sys.box[0]=Interval(-10,10);

	Function df(*sys.goal, Function::DIFF);

	NormalizedSystem nsys(sys);

	// --------------------------------------------------------------------
	IntervalVector x(1);
	x[0]=Interval(9,10);

	int m=2; // number of multipliers
	FncKhunTucker fkt(nsys,df,NULL,x,BitSet::empty(1));

	CPPUNIT_ASSERT(fkt.n == n);
	CPPUNIT_ASSERT(fkt.nb_mult==m);
	CPPUNIT_ASSERT(fkt.eq.empty());
	CPPUNIT_ASSERT(fkt.ineq.empty());
	CPPUNIT_ASSERT(fkt.bound_left.empty());
	CPPUNIT_ASSERT(fkt.bound_right.size()==1);

	IntervalVector lambda(m);
	lambda[0]=Interval(2,3);
	lambda[1]=Interval(4,5);

	IntervalVector xlambda(n+m);
	xlambda.put(0,x);
	xlambda.put(n,lambda);

	IntervalVector fktx=fkt.eval_vector(xlambda,BitSet::all(n+m));
	CPPUNIT_ASSERT(fktx[0]==2*x[0]*lambda[0]+lambda[1]);
	CPPUNIT_ASSERT(fktx[1]==lambda[0]+lambda[1]-1); // normalization equation
	CPPUNIT_ASSERT(fktx[2]==lambda[1]*(x[0]-10));
	CPPUNIT_ASSERT(fkt.multiplier_domain()==IntervalVector(2,Interval(0,1)));

	IntervalMatrix J=((Fnc&) fkt).jacobian(xlambda);
	CPPUNIT_ASSERT(J[0][0]==2*lambda[0]);
	CPPUNIT_ASSERT(J[0][1]==2*x[0]);
	CPPUNIT_ASSERT(J[0][2]==1);
	CPPUNIT_ASSERT(J[1][0]==Interval::zero());
	CPPUNIT_ASSERT(J[1][1]==Interval::one());
	CPPUNIT_ASSERT(J[1][2]==Interval::one());
	CPPUNIT_ASSERT(J[2][0]==lambda[1]);
	CPPUNIT_ASSERT(J[2][1]==Interval::zero());
	CPPUNIT_ASSERT(J[2][2]==x[0]-10);
}

void TestFncKhunTucker::one_var_1_ineq_1_bound() {

	int n=1;

	const ExprSymbol& _x=ExprSymbol::new_("x");
	SystemFactory fac;
	fac.add_var(_x);
	fac.add_ctr(cos(_x)<=0);
	fac.add_goal(sqr(_x));

	System sys(fac);
	sys.box[0]=Interval(-10,10);

	Function df(*sys.goal, Function::DIFF);
	Function dg0(sys.f_ctrs[0], Function::DIFF);
	Function** dg = new Function*[1];
	dg[0]=&dg0;

	NormalizedSystem nsys(sys);

	// --------------------------------------------------------------------
	IntervalVector x(1);
	x[0]=Interval(9,10);

	int m=3; // number of multipliers
	FncKhunTucker fkt(nsys,df,dg,x,BitSet::singleton(1,0));

	CPPUNIT_ASSERT(fkt.n == n);
	CPPUNIT_ASSERT(fkt.nb_mult==m);
	CPPUNIT_ASSERT(fkt.eq.empty());
	CPPUNIT_ASSERT(fkt.ineq.size()==1);
	CPPUNIT_ASSERT(fkt.bound_left.empty());
	CPPUNIT_ASSERT(fkt.bound_right.size()==1);

	IntervalVector lambda(m);
	lambda[0]=Interval(2,3);
	lambda[1]=Interval(4,5);
	lambda[2]=Interval(6,7);

	IntervalVector xlambda(n+m);
	xlambda.put(0,x);
	xlambda.put(n,lambda);

	Function kkt_expected("x","l0","l1","l2","(2*x*l0+l1*(-sin(x))+l2,l0+l1+l2-1,l1*cos(x),l2*(x-10))");
	CPPUNIT_ASSERT(((Fnc&) fkt).eval_vector(xlambda)==kkt_expected.eval_vector(xlambda));

	CPPUNIT_ASSERT(fkt.multiplier_domain()==IntervalVector(3,Interval(0,1)));

	CPPUNIT_ASSERT(((Fnc&) fkt).jacobian(xlambda)==kkt_expected.jacobian(xlambda));
}
