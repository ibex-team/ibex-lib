//============================================================================
//                                  I B E X                                   
// File        : ibex_Solver.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : Oct 01, 2017
//============================================================================

#include "ibex_Solver.h"
#include "ibex_Newton.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_LinearException.h"
#include "ibex_CovSolverData.h"

#include <cassert>

using namespace std;

namespace ibex {

namespace {
	class EmptyBoxException : Exception { };
}

Solver::Solver(const System& sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer,
		const Vector& eps_x_min, const Vector& eps_x_max) :
		  ctc(ctc), bsc(bsc), buffer(buffer), eps_x_min(eps_x_min), eps_x_max(eps_x_max),
		  boundary_test(ALL_TRUE), time_limit(-1), cell_limit(-1), trace(0),
		  solve_init_box(sys.box), eqs(NULL), ineqs(NULL),
		  params(sys.nb_var,BitSet::empty(sys.nb_var),false) /* no forced parameter by default */,
		  manif(NULL), time(0), nb_cells(0) {

	assert(sys.box.size()==ctc.nb_var);

	int nb_eq=0;

	// count the dimension of equalities
	for (int i=0; i<sys.nb_ctr; i++) {
		if (sys.ctrs[i].op==EQ) nb_eq+=sys.ctrs[i].f.image_dim();
	}

	if (nb_eq==sys.f_ctrs.image_dim())
		eqs=&sys; // useless to create a new one
	else {
		ineqs=new System(sys,System::INEQ_ONLY);

		if (nb_eq>0) {
			eqs=new System(sys,System::EQ_ONLY);
		}
		// else: strange to use the solver for inequalities only...
	}

	n=sys.nb_var;
	m=eqs? eqs->f_ctrs.image_dim() : 0;
	nb_ineq=ineqs? ineqs->f_ctrs.image_dim() : 0;

	if (m==0 || m==n) {
		boundary_test=ALL_FALSE;
	}
	if (m>n)
		ibex_warning("Certification not implemented for over-constrained systems ");
}

void Solver::set_params(const VarSet& _params) {
	params=_params;
}

Solver::~Solver() {
	if (ineqs) {
		delete ineqs;
		if (eqs) {
			delete eqs;
		}
	}

	if (manif) delete manif;
}

void Solver::start(const IntervalVector& init_box) {

	buffer.flush();

	if (manif) delete manif;

	manif = new CovSolverData(n, m, nb_ineq, CovManifold::EQU_ONLY, eqs? eqs->var_names() : ineqs->var_names());

	Cell* root=new Cell(init_box);

	// add data required by the bisector
	bsc.add_property(init_box, root->prop);

	// add data required by the contractor
	ctc.add_property(init_box, root->prop);

	buffer.add_property(init_box, root->prop);

	buffer.push(root);

	time = 0;
	manif->set_time(0);

	nb_cells = 1;
	manif->set_nb_cells(0);

	timer.restart();
}

void Solver::start(const CovSolverData& data) {
	buffer.flush();

	if (manif) delete manif;
	manif = new CovSolverData(n, m, nb_ineq);

	// may erase former variable names if the input paving was actually
	// not calculated with the same Minibex file.
	manif->var_names() = eqs? eqs->var_names() : ineqs->var_names();

	// just copy inner, solution and boundary boxes
	for (size_t i=0; i<data.nb_inner(); i++)
		manif->add_inner(data.inner(i));

	for (size_t i=0; i<data.nb_solution(); i++)
		if (m==n)
			manif->add_solution(data.solution(i), data.unicity(i));
		else
			manif->add_solution(data.solution(i), data.unicity(i), data.solution_varset(i));

	for (size_t i=0; i<data.nb_boundary(); i++)
		manif->add_boundary(data.boundary(i), data.boundary_varset(i));

	// the unknown and pending boxes have to be processed
	for (size_t i=0; i<data.CovManifold::nb_unknown(); i++) {

		const IntervalVector& box=data.CovManifold::unknown(i);

		Cell* cell=new Cell(box);

		// add data required by the cell buffer
		buffer.add_property(box, cell->prop);

		// add data required by the bisector
		bsc.add_property(box, cell->prop);

		// add data required by the contractor
		ctc.add_property(box, cell->prop);

		buffer.push(cell);

	}

	time = 0;
	manif->set_time(data.time());

	nb_cells=0; // no new cell created!
	manif->set_nb_cells(data.nb_cells());

	timer.restart();
}

void Solver::start(const char* input_paving) {
	CovSolverData data(input_paving);
	start(data);
}

bool Solver::next(CovSolverData::BoxStatus& status, const IntervalVector** sol) {

	while (!buffer.empty()) {

		if (time_limit >0) {
			try {
				timer.check(time_limit);
			}
			catch(TimeOutException& e) {
				flush();
				if (sol) *sol=NULL;
				throw e;
			}
		}

		if (trace==2) cout << buffer << endl;

		Cell* c=buffer.top();

		ContractContext context(c->prop);

		int v=c->bisected_var; // last bisected var.

		if (v!=-1) { // not the root node :  impact set to the last bisected variable only
			context.impact = BitSet::singleton(n,v);
		}

		try {
			ctc.contract(c->box,context);

			if (c->box.is_empty()) throw EmptyBoxException();

			// 2nd condition: certification is performed at
			// each intermediate step only if the system is under constrained
			if (m==0 || (m<n && !is_too_large(c->box))) {
				// note: cannot return PENDING status
				status=check_sol(c->box);
				if (status!=CovSolverData::UNKNOWN) { // <=> solution or boundary
					delete buffer.pop();
					if (sol) *sol=&(*manif)[manif->size()-1];
					return true;
				} // otherwise: continue search...
			} // else: otherwise: continue search...

			try {
				if (is_too_small(c->box))
					throw NoBisectableVariableException();

				// next line may also throw NoBisectableVariableException
				pair<Cell*,Cell*> new_cells=bsc.bisect(*c);

				delete buffer.pop();
				buffer.push(new_cells.first);
				buffer.push(new_cells.second);
				nb_cells+=2;
				if (cell_limit >=0 && nb_cells>=cell_limit) {
					flush();
					if (sol) *sol=NULL;
					throw CellLimitException();
				}
			}

			catch (NoBisectableVariableException&) {
				status=check_sol(c->box);
				if (status==CovSolverData::UNKNOWN) {
					if (trace >=1) cout << " [unknown] " << c->box << endl;
					manif->add_unknown(c->box);
				}
				delete buffer.pop();
				if (sol) *sol=&(*manif)[manif->size()-1];
				return true;
			}
		}
		catch (EmptyBoxException&) {
			delete buffer.pop();
			//impact.remove(v); // note: in case of the root node, we should clear the bitset
			// instead but since the search is over, the impact is not used anymore.
			// JN: that make a bug with Mingw

			continue;
		}
	}

	if (sol) *sol=NULL;
	return false;
}

Solver::Status Solver::solve(const IntervalVector& init_box) {
	start(init_box);
	return solve();
}

Solver::Status Solver::solve(const CovSolverData& data) {
	start(data);
	return solve();
}

Solver::Status Solver::solve(const char* init_paving) {
	start(init_paving);
	return solve();
}

Solver::Status Solver::solve() {

	Solver::Status final_status;

	// initialization...
	if (manif->nb_inner()==0 && manif->nb_solution()==0 && manif->nb_boundary()==0)
		final_status = INFEASIBLE;
	else
		final_status = SUCCESS;

	CovSolverData::BoxStatus status;

	try {
		while (next(status)) {
			if (final_status==INFEASIBLE) // first solution found
				final_status=SUCCESS; // by default... may be changed right after

			if (status==CovSolverData::UNKNOWN)
				final_status=NOT_ALL_VALIDATED;
		}
	} catch(CellLimitException&) {
		final_status=CELL_OVERFLOW;
	} catch(TimeOutException&) {
		final_status=TIME_OUT;
	}

	manif->set_solver_status(final_status);

	timer.stop();
	time = timer.get_time();

	manif->set_time(manif->time() + time);

	manif->set_nb_cells(manif->nb_cells() + nb_cells);

	return final_status;
}

bool Solver::check_ineq(const IntervalVector& box) {
	if (!ineqs)
		return true;

	Interval y,r;

	bool not_inner=false;

	for (int i=0; i<ineqs->nb_ctr; i++) {
		NumConstraint& c=ineqs->ctrs[i];
		assert(c.f.image_dim()==1);
		y=c.f.eval(box);
		r=c.right_hand_side().i();

		if (y.is_disjoint(r)) {
			throw EmptyBoxException();
		}
		else if (!y.is_subset(r)) {
			not_inner=true;
		}
	}

	return !not_inner;
}

CovSolverData::BoxStatus Solver::check_sol(const IntervalVector& box) {

	if (!eqs) {
		if (check_ineq(box)) {
			if (trace >=1) cout << " [solution] " << box << endl;
			manif->add_inner(box);
			return CovSolverData::SOLUTION;
		} else if (is_boundary(box)) {
			manif->add_boundary(box);
			return CovSolverData::BOUNDARY;
		} else
			return CovSolverData::UNKNOWN;
	} else {

		if (m>n) {
			// Certification not implemented for over-constrained systems
			return CovSolverData::UNKNOWN;
		}

		IntervalVector existence(box);
		IntervalVector unicity(n);
		VarSet varset(n,BitSet::empty(n));

		if (m<n) {
			// ====== under-constrained =========
			try {

				varset=get_newton_vars(eqs->f_ctrs,box.mid(),params);

				if (!inflating_newton(eqs->f_ctrs, varset, box, existence, unicity)) {
					return CovSolverData::UNKNOWN;
				}

			} catch(SingularMatrixException& e) {
				return CovSolverData::UNKNOWN;
			}
		} else {
			// ====== well-constrained =========
			if (!inflating_newton(eqs->f_ctrs, box.mid(), existence, unicity)) {
				return CovSolverData::UNKNOWN;
			}
		}

		// The inflating Newton may cause the existence box to be disjoint from the input box.

		// Note that the following line also tests the case of an existence box outside
		// the initial box of the search
		if (box.is_disjoint(existence)) {
			throw EmptyBoxException();
		}

		bool solution = existence.is_subset(solve_init_box);

		solution &= check_ineq(existence);

		if (eqs && n==m) {
			// Check if the solution is new, that is, that the solution is not included in the unicity
			// box of a previously found solution. For efficiency reason, this test is not performed in
			// the case of under-constrained systems (m<n).

			// TODO
			//		for (vector<QualifiedBox>::iterator it=manif->inner.begin(); it!=manif->inner.end(); it++) {
			//			if (it->unicity().is_superset(existence)) {
			//				if (unicity) delete unicity;
			//				if (varset) delete varset;
			//				throw EmptyBoxException();
			//			}
			//		}
		}

		if (solution) {
			if (trace >=1) cout << " [solution] " << existence << endl;
			manif->add_solution(existence, unicity, varset);
			return CovSolverData::SOLUTION;
		} else {
			if (is_boundary(existence)) {
				if (trace >=1) cout << " [boundary] " << existence << endl;
				manif->add_boundary(existence, varset);
				return CovSolverData::BOUNDARY;
			} else
				return CovSolverData::UNKNOWN;
		}
	}
}

bool Solver::is_boundary(const IntervalVector& box) {

	switch (boundary_test) {
	case ALL_TRUE : return true;
	case ALL_FALSE : return false;
	case FULL_RANK : {
		// get active boundary constraints
		BitSet bound(n);
		for (int i=0; i<n; i++) {
			if (solve_init_box[i].lb()>=box[i].lb()) bound.add(i);
			if (solve_init_box[i].ub()<=box[i].ub()) {
				if (bound[i]) return false;
				else bound.add(i);
			}
		}

		// get active inequalities
		BitSet ineq_active=ineqs? ineqs->active_ctrs(box) : BitSet::empty(n);

		int size = bound.size() + m + ineq_active.size();

		IntervalMatrix J(size, n);
		int i=0;
		int v;
		for (; i<bound.size(); i++) {
			J[i].clear();
			v=(i==0? bound.min() : bound.next(v));
			J[i][v]=1.0;
		}
		if (m>0) {
			J.put(i,0,eqs->f_ctrs.jacobian(box));
			i+=m;
		}
		if (ineqs!=NULL) {
			J.put(i,0,ineqs->f_ctrs.jacobian(box,ineq_active));
		}
		return full_rank(J);
	}
	case HALF_BALL:
	default:
		not_implemented("\"half-ball\" boundary test");
		return false;
	}
}

bool Solver::is_too_small(const IntervalVector& box) {
	for (int i=0; i<n; i++)
		if (box[i].diam()>eps_x_min[i]) return false;
	return true;
}

bool Solver::is_too_large(const IntervalVector& box) {
	for (int i=0; i<n; i++)
		if (box[i].diam()>eps_x_max[i]) return true;
	return false;
}

void Solver::flush() {
	while (!buffer.empty()) {
		Cell* cell=buffer.top();
		if (trace >=1) cout << " [pending] " << cell->box << endl;
		manif->add_pending(cell->box);
		delete buffer.pop();
	}
}

namespace {
const char* green() {
#ifndef _WIN32
	return "\033[32m";
#else
	return "";
#endif
}

const char* red(){
#ifndef _WIN32
	return "\033[31m";
#else
	return "";
#endif
}

const char* white() {
#ifndef _WIN32
	return "\033[0m";
#else
	return "";
#endif
}

}
void Solver::report() {

	switch ((Status) manif->solver_status()) {
	case SUCCESS: 
		cout << green() << " solving successful!" << endl;
		break;
	case INFEASIBLE: 
		cout << red() << " infeasible problem" << endl;
		break;
	case NOT_ALL_VALIDATED: 
		cout << red() << " done! but some boxes have 'unknown' status." << endl;
		break;
	case TIME_OUT: 
		cout << red() << " time limit " << time_limit << "s. reached " << endl;
		break;
	case CELL_OVERFLOW: 
		cout << red() << " cell overflow" << endl;
	}

	cout << white() << endl;

	cout << " number of solution boxes:\t";
	if (manif->nb_solution()==0) cout << "--"; else cout << manif->nb_solution();
	cout << endl;
	cout << " number of boundary boxes:\t";
	if (manif->nb_boundary()==0) cout << "--"; else cout << manif->nb_boundary();
	cout << endl;
	cout << " number of unknown boxes:\t";
	if (manif->nb_unknown()==0) cout << "--"; else cout << manif->nb_unknown();
	cout << endl;
	cout << " number of pending boxes:\t";
	if (manif->nb_pending()==0) cout << "--"; else cout << manif->nb_pending();
	cout << endl;
	cout << " cpu time used:\t\t\t" << time << "s";
	if (manif->time()!=time)
		cout << " [total=" << manif->time() << "]";
	cout << endl;
	cout << " number of cells:\t\t" << nb_cells;
	if (manif->nb_cells()!=nb_cells)
		cout << " [total=" << manif->nb_cells() << "]";
	cout << endl << endl;
}

} // end namespace ibex
