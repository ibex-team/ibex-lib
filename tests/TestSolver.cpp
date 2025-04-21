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
#include "ibex_DefaultSolver.h"
#include "ibex_RoundRobin.h"
#include "ibex_CellStack.h"
#include "ibex_CtcHC4.h"

#include <regex>

using namespace std;

namespace ibex {

void TestSolver::empty() {
	System sys(SRCDIR_TESTS "/minibex/empty.mbx");
	DefaultSolver solver(sys);
	Solver::Status status=solver.solve(sys.box);
	CPPUNIT_ASSERT(status==Solver::SUCCESS);
	CPPUNIT_ASSERT(solver.get_data().nb_solution()==1);
	CPPUNIT_ASSERT(solver.get_data().solution(0)==sys.box);
}

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
	double _sol1[]={cospi6,-sinpi6};
	double _sol2[]={cospi6,sinpi6};

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

	double _sol1[]={cospi6,-sinpi6};
	double _sol2[]={cospi6,sinpi6};

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
	CPPUNIT_ASSERT(status==CovSolverData::UNKNOWN);
	CPPUNIT_ASSERT(solver.get_data().nb_unknown()==1);
	CPPUNIT_ASSERT(solver.get_data().unknown(0).is_superset(sol2));

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

	double _sol1[]={cospi6,-sinpi6,1};
	double _sol2[]={cospi6,sinpi6,1};

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


static const unsigned int NB_BENCHS = 28;
 
static const char* BENCHS[NB_BENCHS] = {
"/../benchs/solver/non-polynom/Bratu-0030.bch",
"/../benchs/solver/non-polynom/Bratu-0050.bch",
"/../benchs/solver/non-polynom/Designsp.bch",
"/../benchs/solver/non-polynom/Kin1.bch",
"/../benchs/solver/non-polynom/SjirkBoon.bch",
"/../benchs/solver/non-polynom/Trigexp1-020.bch",
"/../benchs/solver/non-polynom/Trigexp1-022.bch",
"/../benchs/solver/non-polynom/Trigexp1-024.bch",
"/../benchs/solver/non-polynom/Trigexp1-030.bch",
"/../benchs/solver/non-polynom/Trigexp1-100.bch",
"/../benchs/solver/non-polynom/Trigo1-0005.bch",
"/../benchs/solver/non-polynom/Trigo1-0006.bch",
"/../benchs/solver/non-polynom/Trigo1-0010.bch",
"/../benchs/solver/non-polynom/Trigo1-0010m1.bch",
"/../benchs/solver/non-polynom/Trigo1-0010sp.bch",
"/../benchs/solver/non-polynom/Troesch10.bch",
"/../benchs/solver/non-polynom/troesch10-sh.bch",
"/../benchs/solver/non-polynom/Troesch20.bch",
"/../benchs/solver/others/cyclohexan3D.bch",
"/../benchs/solver/others/hayes1.bch",
"/../benchs/solver/others/kolev36.bch",
"/../benchs/solver/others/transistor-icse.bch",
"/../benchs/solver/polynom/Neveu1.bch",
"/../benchs/solver/polynom/ponts-geo.bch",
"/../benchs/solver/polynom/Redeco8.bch",
"/../benchs/solver/polynom/yamamura8a.bch",
"/../benchs/solver/polynom/yamamura/Yamamua1-0010.bch",
"/../benchs/solver/polynom/yamamura/Yamamua1-0012sp.bch"
};

void  TestSolver::benchs() {
	for (unsigned int i=0; i<NB_BENCHS; i++) {
		System sys((string(SRCDIR_TESTS)+BENCHS[i]).c_str());
		DefaultSolver solver(sys);
		solver.time_limit = 10;
		Solver::Status status = solver.solve(sys.box);	
		const CovSolverData& data = solver.get_data();

		CPPUNIT_ASSERT(data.solver_status() == Solver::SUCCESS);

		// load reference COV file of the bench
		string cov_file_name = regex_replace(BENCHS[i], std::regex("/../benchs/"), "");
		cov_file_name = regex_replace(cov_file_name, std::regex("/"), "_");
		cov_file_name = regex_replace(cov_file_name, std::regex("\\.bch"), ".cov");
		CovSolverData ref_data((string(SRCDIR_TESTS)+"/benchs-results/"+cov_file_name).c_str()); 

		unsigned int n = ref_data.nb_solution();
		CPPUNIT_ASSERT(data.nb_solution() == n);
			
		// build the vector of solutions indices (0,1,...,n-1)
		vector<unsigned int> ref_sols_indices;
		for (unsigned int i=0; i<n; i++) ref_sols_indices.push_back(i);
			
		// check solutions are the same
		for (unsigned int i=0; i<n; i++) {
			// find solution in ref_data matching solution n°i in data
			bool solution_found = false;
			vector<unsigned int>::iterator it = ref_sols_indices.begin();
			while (it!=ref_sols_indices.end()) {
				if (ref_data.solution(*it).intersects(data.solution(i))) {
					// remove this solution from the candidates
					ref_sols_indices.erase(it);
					solution_found = true;
					break;
				} else ++it;
			}				
			CPPUNIT_ASSERT(solution_found);
		}
	}
}

} // end namespace
