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
#include "math.h"

using namespace std;

namespace ibex {

void TestSolver::circle1() {
	try {
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
cout << "1.1"<< endl;
	SystemFactory f;
	f.add_var(x);
	f.add_var(y);
	cout << "1.2"<< endl;
	f.add_ctr(sqr(x)+sqr(y)=1);
	f.add_ctr(sqr(x-1)+sqr(y)=1);
	cout << "1.3"<< endl;
	double cospi6=0.5;
	double sinpi6=::sqrt(3)/2;
	cout << "1.4"<< endl;
	double _sol1[]={cospi6,sinpi6};
	double _sol2[]={cospi6,-sinpi6};
	cout << "1.5"<< endl;

	Vector sol1(2,_sol1);
	Vector sol2(2,_sol2);
	System sys(f);
	cout << "1.6"<< endl;
	RoundRobin rr(1e-3);
	CellStack stack;
	CtcHC4 hc4(sys);

	cout << "1.7"<< endl;
	Solver solver(sys,hc4,rr,stack);
	solver.start(IntervalVector(2,Interval(-10,10)));
	const Solver::Solution* sol;
	bool res;
	cout << "1.8"<< endl;

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol1));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);
	cout << "1.9"<< endl;

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol2));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);

	cout << "1.10"<< endl;
	res=solver.next(sol);
	cout << "1.11"<< endl;
	CPPUNIT_ASSERT(res==false);
	cout << "1.12"<< endl;

	delete &x;
	delete &y;
} catch (Exception& e) {
	cout << typeid(e).name() << endl;

}
}

void TestSolver::circle2() {
	try {
	cout << "2.1"<< endl;
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	cout << "2.2"<< endl;

	SystemFactory f;
	f.add_var(x);
	f.add_var(y);
	f.add_ctr(sqr(x)+sqr(y)=1);
	f.add_ctr(sqr(x-2)+sqr(y)=1);
	cout << "2.3"<< endl;
	double _sol1[]={1,0};
	Vector sol1(2,_sol1);
	System sys(f);
	RoundRobin rr(1e-3);
	CellStack stack;
	CtcHC4 hc4(sys);
	cout << "2.4"<< endl;
	Solver solver(sys,hc4,rr,stack);
	cout << "2.5"<< endl;

	solver.start(IntervalVector(2,Interval(-10,10)));
	const Solver::Solution* sol;
	bool res;
	cout << "2.6"<< endl;

	res=solver.next(sol);
	cout << "2.7"<< endl;
	CPPUNIT_ASSERT(res==true);
	cout << "2.8"<< endl;
	CPPUNIT_ASSERT(sol->existence().is_superset(sol1));
	cout << "2.9"<< endl;
	CPPUNIT_ASSERT(sol->status==Solver::UNKNOWN);
	cout << "2.10"<< endl;

	res=solver.next(sol);
	cout << "2.11"<< endl;
	CPPUNIT_ASSERT(res==false);
	cout << "2.12"<< endl;

	delete &x;
	delete &y;
} catch (Exception& e) {
	cout << typeid(e).name() << endl;

}
}

void TestSolver::circle3() {
	try {
	cout << "3.1"<< endl;
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	cout << "3.2"<< endl;

	SystemFactory f;
	f.add_var(x);
	f.add_var(y);
	f.add_ctr(sqr(x)+sqr(y)=1);
	f.add_ctr(sqr(x-1)+sqr(y)=1);
	cout << "3.3"<< endl;

	double cospi6=0.5;
	double sinpi6=(sqrt(Interval(3))/2).lb();
	f.add_ctr(4*y*abs(y)<=3); // a rigorous way to impose y<=sin(pi/6).

	cout << "3.4"<< endl;
	double _sol1[]={cospi6,sinpi6};
	double _sol2[]={cospi6,-sinpi6};
	cout << "3.5"<< endl;

	Vector sol1(2,_sol1);
	Vector sol2(2,_sol2);
	System sys(f);
	RoundRobin rr(1e-3);
	CellStack stack;
	CtcHC4 hc4(sys);
	Solver solver(sys,hc4,rr,stack);
	cout << "3.6"<< endl;

	solver.start(IntervalVector(2,Interval(-10,10)));
	const Solver::Solution* sol;
	bool res;
	cout << "3.7"<< endl;

	res=solver.next(sol);
	cout << "3.8"<< endl;
	CPPUNIT_ASSERT(res==true);
	cout << "3.9"<< endl;
	CPPUNIT_ASSERT(sol->existence().is_superset(sol1));
	CPPUNIT_ASSERT(sol->status==Solver::UNKNOWN);
	cout << "3.10"<< endl;

	res=solver.next(sol);
	cout << "3.11"<< endl;
	CPPUNIT_ASSERT(res==true);
	CPPUNIT_ASSERT(sol->existence().is_superset(sol2));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);
	cout << "3.12"<< endl;

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==false);
	cout << "3.13"<< endl;
	delete &x;
	delete &y;
} catch (Exception& e) {
	cout << typeid(e).name() << endl;

}
}

void TestSolver::circle4() {
	try {
	cout << "4.1"<< endl;
	const ExprSymbol& x=ExprSymbol::new_("x");
	const ExprSymbol& y=ExprSymbol::new_("y");
	const ExprSymbol& r2=ExprSymbol::new_("r2");
	cout << "4.2"<< endl;

	SystemFactory f;
	f.add_var(x);
	f.add_var(y);
	f.add_var(r2);
	f.add_ctr(sqr(x)+sqr(y)=r2);
	f.add_ctr(sqr(x-1)+sqr(y)=r2);
	cout << "4.3"<< endl;

	double cospi6=0.5;
	double sinpi6=::sqrt(3)/2;
	cout << "4.4"<< endl;

	double _sol1[]={cospi6,sinpi6,1};
	double _sol2[]={cospi6,-sinpi6,1};

	cout << "4.5"<< endl;
	Vector sol1(3,_sol1);
	Vector sol2(3,_sol2);

	System sys(f);
	RoundRobin rr(1e-3);
	CellStack stack;
	CtcHC4 hc4(sys);
	cout << "4.6"<< endl;

	BitSet params(BitSet::empty(3));
	params.add(2);
	Solver solver(sys,params,hc4,rr,stack);
	cout << "4.7"<< endl;

	IntervalVector box(3);
	box[0]=Interval(-10,10);
	box[1]=Interval(-10,10);
	box[2]=Interval(1,1);
	solver.start(box);
	const Solver::Solution* sol;
	bool res;
	cout << "4.8"<< endl;

	res=solver.next(sol);
	cout << "4.9"<< endl;
	CPPUNIT_ASSERT(res==true);
	cout << "4.10"<< endl;
	CPPUNIT_ASSERT(sol->existence().is_superset(sol1));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);
	cout << "4.11"<< endl;

	res=solver.next(sol);
	CPPUNIT_ASSERT(res==true);
	cout << "4.12"<< endl;
	CPPUNIT_ASSERT(sol->existence().is_superset(sol2));
	CPPUNIT_ASSERT(sol->status==Solver::SOLUTION);

	cout << "4.13"<< endl;
	res=solver.next(sol);
	CPPUNIT_ASSERT(res==false);
	cout << "4.14"<< endl;
	delete &x;
	delete &y;
	delete &r2;
} catch (Exception& e) {
	cout << typeid(e).name() << endl;

}
}


} // end namespace
