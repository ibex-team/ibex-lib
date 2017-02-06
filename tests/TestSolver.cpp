/* ============================================================================
 * I B E X - Solver Tests
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Mar 2, 2012
 * ---------------------------------------------------------------------------- */

#include "TestSolver.h"
#include "ibex_SystemFactory.h"
#include "ibex_Solver.h"
#include "ibex_RoundRobin.h"
#include "ibex_CellStack.h"
#include "ibex_CtcHC4.h"

using namespace std;

namespace ibex {

void TestSolver::circle1() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");

	SystemFactory f;
	f.add_var(x);
	f.add_var(y);
	f.add_ctr(sqr(x)+sqr(y)=1);
	f.add_ctr(sqr(x-1)+sqr(y)=1);
	double cospi6=0.5;
	double sinpi6=::sqrt(3)/2;
	double _sol1[]={cospi6,sinpi6};
	double _sol2[]={cospi6,-sinpi6};

	Vector sol1(2,_sol1);
	Vector sol2(2,_sol2);
	System sys(f);
	RoundRobin rr(1e-3);
	CellStack stack;
	CtcHC4 hc4(sys);

	Solver solver(sys,hc4,rr,stack);
	solver.start(IntervalVector(2,Interval(-10,10)));
	const Solver::Solution* sol;
	bool res;

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol1));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol2));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==false);
}

void TestSolver::circle2() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");

	SystemFactory f;
	f.add_var(x);
	f.add_var(y);
	f.add_ctr(sqr(x)+sqr(y)=1);
	f.add_ctr(sqr(x-2)+sqr(y)=1);
	double _sol1[]={1,0};
	Vector sol1(2,_sol1);
	System sys(f);
	RoundRobin rr(1e-3);
	CellStack stack;
	CtcHC4 hc4(sys);
	Solver solver(sys,hc4,rr,stack);

	solver.start(IntervalVector(2,Interval(-10,10)));
	const Solver::Solution* sol;
	bool res;

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol1));
	CPPUNIT_ASSERT(sol->status==Solver::UNKNOWN);

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==false);
}

void TestSolver::circle3() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");

	SystemFactory f;
	f.add_var(x);
	f.add_var(y);
	f.add_ctr(sqr(x)+sqr(y)=1);
	f.add_ctr(sqr(x-1)+sqr(y)=1);

	double cospi6=0.5;
	double sinpi6=::sqrt(3)/2;
	f.add_ctr(y<=sinpi6);

	double _sol1[]={cospi6,sinpi6};
	double _sol2[]={cospi6,-sinpi6};

	Vector sol1(2,_sol1);
	Vector sol2(2,_sol2);
	System sys(f);
	RoundRobin rr(1e-3);
	CellStack stack;
	CtcHC4 hc4(sys);
	Solver solver(sys,hc4,rr,stack);

	solver.start(IntervalVector(2,Interval(-10,10)));
	const Solver::Solution* sol;
	bool res;

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol1));
	CPPUNIT_ASSERT(sol->status==Solver::UNKNOWN);

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol2));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==false);
}

void TestSolver::circle4() {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprSymbol& r2=ExprSymbol::new_("r2");

	SystemFactory f;
	f.add_var(x);
	f.add_var(y);
	f.add_var(r2);
	f.add_ctr(sqr(x)+sqr(y)=r2);
	f.add_ctr(sqr(x-1)+sqr(y)=r2);

	double cospi6=0.5;
	double sinpi6=::sqrt(3)/2;

	double _sol1[]={cospi6,sinpi6,1};
	double _sol2[]={cospi6,-sinpi6,1};

	Vector sol1(3,_sol1);
	Vector sol2(3,_sol2);

	System sys(f);
	RoundRobin rr(1e-3);
	CellStack stack;
	CtcHC4 hc4(sys);

	BitSet params(BitSet::empty(3));
	params.add(2);
	Solver solver(sys,params,hc4,rr,stack);

	IntervalVector box(3);
	box[0]=Interval(-10,10);
	box[1]=Interval(-10,10);
	box[2]=Interval(1,1);
	solver.start(box);
	const Solver::Solution* sol;
	bool res;

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol1));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol2));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==false);
}


} // end namespace
