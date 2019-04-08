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
	Vector prec(2,1e-3);

	Solver solver(sys,hc4,rr,stack,prec,prec);
	solver.start(IntervalVector(2,Interval(-10,10)));

	CovSolverData::BoxStatus status;

	bool res=solver.next(status);
	CPPUNIT_ASSERT(res);
	CPPUNIT_ASSERT(status==CovSolverData::SOLUTION);
	CPPUNIT_ASSERT(solver.get_data().nb_solution()==1);
	CPPUNIT_ASSERT(solver.get_data().solution(0).is_superset(sol1));

	res=solver.next(status);
	CPPUNIT_ASSERT(res);
	CPPUNIT_ASSERT(status==CovSolverData::SOLUTION);
	CPPUNIT_ASSERT(solver.get_data().nb_solution()==2);
	CPPUNIT_ASSERT(solver.get_data().solution(1).is_superset(sol2));

	res=solver.next(status);
	CPPUNIT_ASSERT(!res);
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
	Vector prec(2,1e-3);
	Solver solver(sys,hc4,rr,stack,prec,prec);

	solver.start(IntervalVector(2,Interval(-10,10)));

	CovSolverData::BoxStatus status;

	bool res=solver.next(status);
	CPPUNIT_ASSERT(res);
	CPPUNIT_ASSERT(status==CovSolverData::UNKNOWN);
	CPPUNIT_ASSERT(solver.get_data().nb_unknown()==1);
	CPPUNIT_ASSERT(solver.get_data().unknown(0).is_superset(sol1));

	res=solver.next(status);
	CPPUNIT_ASSERT(!res);
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
	double sinpi6=(sqrt(Interval(3))/2).lb();
	f.add_ctr(4*y*abs(y)<=3); // a rigorous way to impose y<=sin(pi/6).

	double _sol1[]={cospi6,sinpi6};
	double _sol2[]={cospi6,-sinpi6};

	Vector sol1(2,_sol1);
	Vector sol2(2,_sol2);
	System sys(f);
	RoundRobin rr(1e-3);
	CellStack stack;
	CtcHC4 hc4(sys);
	Vector prec(2,1e-3);
	Solver solver(sys,hc4,rr,stack,prec,prec);

	solver.start(IntervalVector(2,Interval(-10,10)));


	CovSolverData::BoxStatus status;

	bool res=solver.next(status);
	CPPUNIT_ASSERT(res);
	CPPUNIT_ASSERT(status==CovSolverData::UNKNOWN);
	CPPUNIT_ASSERT(solver.get_data().nb_unknown()==1);
	CPPUNIT_ASSERT(solver.get_data().unknown(0).is_superset(sol1));

	res=solver.next(status);
	CPPUNIT_ASSERT(res);
	CPPUNIT_ASSERT(status==CovSolverData::SOLUTION);
	CPPUNIT_ASSERT(solver.get_data().nb_solution()==1);
	CPPUNIT_ASSERT(solver.get_data().solution(0).is_superset(sol2));

	res=solver.next(status);
	CPPUNIT_ASSERT(!res);
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

	VarSet params(sys.f_ctrs,sys.args[2],false);
	Vector prec(3,1e-3);
	Solver solver(sys,hc4,rr,stack,prec,prec);
	solver.set_params(params);
	IntervalVector box(3);
	box[0]=Interval(-10,10);
	box[1]=Interval(-10,10);
	box[2]=Interval(1,1);
	solver.start(box);

	CovSolverData::BoxStatus status;

	bool res=solver.next(status);
	CPPUNIT_ASSERT(res);
	CPPUNIT_ASSERT(status==CovSolverData::SOLUTION);
	CPPUNIT_ASSERT(solver.get_data().nb_solution()==1);
	CPPUNIT_ASSERT(solver.get_data().solution(0).is_superset(sol1));

	res=solver.next(status);
	CPPUNIT_ASSERT(status==CovSolverData::SOLUTION);
	CPPUNIT_ASSERT(solver.get_data().nb_solution()==2);
	CPPUNIT_ASSERT(solver.get_data().solution(1).is_superset(sol2));

	res=solver.next(status);
	CPPUNIT_ASSERT(!res);
}


} // end namespace
