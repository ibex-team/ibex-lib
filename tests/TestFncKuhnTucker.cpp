//============================================================================
//                                  I B E X                                   
// File        : TestFncKuhnTucker.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 17, 2019
// Last Update : Jun 05, 2019
//============================================================================

#include "TestFncKuhnTucker.h"
#include "ibex_FncKuhnTucker.h"
#include "ibex_SystemFactory.h"

using namespace ibex;

void TestFncKuhnTucker::one_var() {
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

	FncKuhnTucker fkt(nsys,df,NULL,x,BitSet::empty(1));

	CPPUNIT_ASSERT(fkt.n == n);
	CPPUNIT_ASSERT(fkt.nb_mult==m);
	CPPUNIT_ASSERT(fkt.eq().empty());
	CPPUNIT_ASSERT(fkt.ineq().empty());
	CPPUNIT_ASSERT(fkt.left_bound().empty());
	CPPUNIT_ASSERT(fkt.right_bound().empty());

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
void TestFncKuhnTucker::one_var_1_bound() {
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
	FncKuhnTucker fkt(nsys,df,NULL,x,BitSet::empty(1));

	CPPUNIT_ASSERT(fkt.n == n);
	CPPUNIT_ASSERT(fkt.nb_mult==m);
	CPPUNIT_ASSERT(fkt.eq().empty());
	CPPUNIT_ASSERT(fkt.ineq().empty());
	CPPUNIT_ASSERT(fkt.left_bound().empty());
	CPPUNIT_ASSERT(fkt.right_bound().size()==1);

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

void TestFncKuhnTucker::one_var_1_ineq_1_rbound() {

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
	FncKuhnTucker fkt(nsys,df,dg,x,BitSet::singleton(1,0));

	CPPUNIT_ASSERT(fkt.n == n);
	CPPUNIT_ASSERT(fkt.nb_mult==m);
	CPPUNIT_ASSERT(fkt.eq().empty());
	CPPUNIT_ASSERT(fkt.ineq().size()==1);
	CPPUNIT_ASSERT(fkt.left_bound().empty());
	CPPUNIT_ASSERT(fkt.right_bound().size()==1);

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


void TestFncKuhnTucker::one_var_1_eq_1_lbound() {
	int n=1;

	const ExprSymbol& _x=ExprSymbol::new_("x");
	SystemFactory fac;
	fac.add_var(_x);
	fac.add_ctr(cos(_x)=0);
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
	x[0]=Interval(-11,-9);

	int m=3; // number of multipliers
	FncKuhnTucker fkt(nsys,df,dg,x,BitSet::singleton(1,0));

	CPPUNIT_ASSERT(fkt.n == n);
	CPPUNIT_ASSERT(fkt.nb_mult==m);
	CPPUNIT_ASSERT(fkt.eq().size()==1);
	CPPUNIT_ASSERT(fkt.ineq().empty());
	CPPUNIT_ASSERT(fkt.left_bound().size()==1);
	CPPUNIT_ASSERT(fkt.right_bound().empty());

	IntervalVector lambda(m);
	lambda[0]=Interval(2,3);
	lambda[1]=Interval(4,5);
	lambda[2]=Interval(6,7);

	IntervalVector xlambda(n+m);
	xlambda.put(0,x);
	xlambda.put(n,lambda);

	Function kkt_expected("x","l0","l1","l2","(2*x*l0+l1*(-sin(x))-l2,l0+l1^2+l2-1,cos(x),l2*(-10-x))");

	CPPUNIT_ASSERT(((Fnc&) fkt).eval_vector(xlambda)==kkt_expected.eval_vector(xlambda));

	double _multiplier_domain[][2]= { {0,1},{-1,1},{0,1} };

	CPPUNIT_ASSERT(fkt.multiplier_domain()==IntervalVector(m,_multiplier_domain));

	CPPUNIT_ASSERT(((Fnc&) fkt).jacobian(xlambda)==kkt_expected.jacobian(xlambda));
}


void TestFncKuhnTucker::two_vars_1_eq_1_ineq_2bounds() {
	int n=2;

	const ExprSymbol& _x=ExprSymbol::new_("x");
	const ExprSymbol& _y=ExprSymbol::new_("y");
	SystemFactory fac;
	fac.add_var(_x);
	fac.add_var(_y);

	// we declare the equality first on purpose,
	// to see if it is robust to constraint declaration
	// order
	fac.add_ctr(cos(_x*_y)=0);
	fac.add_ctr(sin(_x*_y)<=0);
	fac.add_ctr(exp(_x-_y)<=0);

	fac.add_goal(sqr(_x+_y));

	System sys(fac);
	sys.box[0]=Interval(3,4);
	sys.box[1]=Interval(1,2);

	Function df(*sys.goal, Function::DIFF);
	Function dg0(sys.f_ctrs[0], Function::DIFF);
	Function dg1(sys.f_ctrs[1], Function::DIFF);
	Function dg2(sys.f_ctrs[2], Function::DIFF);
	Function** dg = new Function*[3];
	dg[0]=&dg0;
	dg[1]=&dg1;
	dg[2]=&dg2;

	NormalizedSystem nsys(sys);

	// --------------------------------------------------------------------
	IntervalVector x(n);
	x[0]=Interval(3,3.5);
	x[1]=Interval(1.5,2);

	int m=6; // number of multipliers
	FncKuhnTucker fkt(nsys,df,dg,x,BitSet::all(3));

	CPPUNIT_ASSERT(fkt.n == n);
	CPPUNIT_ASSERT(fkt.nb_mult==m);
	CPPUNIT_ASSERT(fkt.eq().size()==1);
	CPPUNIT_ASSERT(fkt.ineq().size()==2);
	CPPUNIT_ASSERT(fkt.left_bound().size()==1);
	CPPUNIT_ASSERT(fkt.right_bound().size()==1);

	IntervalVector lambda(m);
	lambda[0]=Interval(2,3);
	lambda[1]=Interval(4,5);
	lambda[2]=Interval(6,7);
	lambda[3]=Interval(8,9);
	lambda[4]=Interval(10,11);
	lambda[5]=Interval(12,13);

	IntervalVector xlambda(n+m);
	xlambda.put(0,x);
	xlambda.put(n,lambda);

	Function kkt_expected("x","y","l0","l1","l2","l3","l4","l5",
			"(2*(x+y)*l0 + cos(x*y)*y*l1 + exp(x-y)*l2 - sin(x*y)*y*l3 - l4, \
  2*(x+y)*l0 + cos(x*y)*x*l1 - exp(x-y)*l2 - sin(x*y)*x*l3 + l5, \
  l0 + l1 + l2 + l3^2 + l4 + l5 -1, \
  l1*sin(x*y), \
  l2*exp(x-y), \
  cos(x*y), \
  l4*(3-x), \
  l5*(y-2))");

	CPPUNIT_ASSERT(((Fnc&) fkt).eval_vector(xlambda)==kkt_expected.eval_vector(xlambda));

	double _multiplier_domain[][2]= { {0,1},{0,1},{0,1},{-1,1},{0,1},{0,1} };

	CPPUNIT_ASSERT(fkt.multiplier_domain()==IntervalVector(m,_multiplier_domain));

	IntervalMatrix J1=((Fnc&) fkt).jacobian(xlambda);
	IntervalMatrix J2=kkt_expected.jacobian(xlambda);

	CPPUNIT_ASSERT(J1==J2);

	/** check selecting just one column of the jacobian (useful for Newton) ***/
	for (int i=0; i<n+m; i++) {
		J1=((Fnc&) fkt).jacobian(xlambda,i);
		for (int j=0; j<n+m; j++) {
			if (j==i)
				CPPUNIT_ASSERT(J1.col(j)==J2.col(j));
			else
				// check that other columns are not
				// uselessly computed:
				CPPUNIT_ASSERT(J1.col(j)==IntervalVector(n+m));
			}
	}
	// test selecting specific rows

}
