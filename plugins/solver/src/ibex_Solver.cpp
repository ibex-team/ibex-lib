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
#include "ibex_Manifold.h"

#include <cassert>

using namespace std;

namespace ibex {

namespace {
	class EmptyBoxException : Exception { };
}

Solver::Solver(const System& sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer,
		const Vector& eps_x_min, const Vector& eps_x_max) :
		  ctc(ctc), bsc(bsc), buffer(buffer), eps_x_min(eps_x_min), eps_x_max(eps_x_max),
		  boundary_test(ALL_TRUE), time_limit(-1), cell_limit(-1), trace(0), impact(BitSet::all(ctc.nb_var)),
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

	if (m==0 || m==n)
		boundary_test=ALL_FALSE;

	manif = new Manifold(n,m,nb_ineq);
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

	manif = new Manifold(n,m,nb_ineq);

	Cell* root=new Cell(init_box);

	// add data required by this solver
	root->add<BisectedVar>();

	// add data required by the bisector
	bsc.add_backtrackable(*root);

	buffer.push(root);
	nb_cells = 1;

	time = 0;

	timer.restart();
}

void Solver::start(const char* input_paving) {
	buffer.flush();

	if (manif) delete manif;
	manif = new Manifold(n,m,nb_ineq);
	manif->load(input_paving);

	vector<SolverOutputBox>::const_iterator it=manif->unknown.begin();

	// the unknown and pending boxes have to be processed
	while (it!=manif->pending.end()) {
		if (it==manif->unknown.end())
			it=manif->pending.begin();
		if (it==manif->pending.end())
			break;

		Cell* cell=new Cell(it->existence());

		// add data required by this solver
		cell->add<BisectedVar>();

		// add data required by the bisector
		bsc.add_backtrackable(*cell);

		buffer.push(cell);

		it++;
	}

	nb_cells=0; // no new cell created!

	manif->unknown.clear();
	manif->pending.clear();

	timer.restart();
}

SolverOutputBox* Solver::next() {
	while (!buffer.empty()) {
		if (time_limit >0) timer.check(time_limit);

		if (trace==2) cout << buffer << endl;

		Cell* c=buffer.top();

		int v=c->get<BisectedVar>().var;      // last bisected var.

		if (v!=-1)                          // no root node :  impact set to 1 for last bisected var only
			impact.add(v);
		else                                // root node : impact set to 1 for all variables
			impact.fill(0,ctc.nb_var-1);

		try {
			ctc.contract(c->box,impact);

			if (c->box.is_empty()) throw EmptyBoxException();

			if (v!=-1)
				impact.remove(v);
			else                              // root node : impact set to 0 for all variables after contraction
				impact.clear();

			// certification is performed at each intermediate step
			// if the system is under constrained
			if (!c->box.is_empty() && m<n) {
				// note: cannot return PENDING status
				SolverOutputBox new_sol=check_sol(c->box);
				if (new_sol.status!=SolverOutputBox::UNKNOWN) {
					if ((m==0 && new_sol.status==SolverOutputBox::INNER) ||
							!is_too_large(new_sol.existence())) {
						delete buffer.pop();
						return &store_sol(new_sol);
					} else {
						// otherwise: continue search...
					}
				}
				else {
					// otherwise: continue search...
				}
			}

			try {
				if (is_too_small(c->box))
					throw NoBisectableVariableException();

				// next line may also throw NoBisectableVariableException
				pair<Cell*,Cell*> new_cells=bsc.bisect(*c);

				delete buffer.pop();
				buffer.push(new_cells.first);
				buffer.push(new_cells.second);
				nb_cells+=2;
				if (cell_limit >=0 && nb_cells>=cell_limit) throw CellLimitException();
			}

			catch (NoBisectableVariableException&) {
				SolverOutputBox new_sol=check_sol(c->box);
				delete buffer.pop();
				return &store_sol(new_sol);
			}
		}
		catch (EmptyBoxException&) {
			delete buffer.pop();
			//impact.remove(v); // note: in case of the root node, we should clear the bitset
			// instead but since the search is over, the impact is not used anymore.
			// JN: that make a bug with Mingw
			if (v!=-1)                          // no root node :  impact set to 1 for last bisected var only
				impact.remove(v);
			else                                // root node : impact set to 1 for all variables
				impact.clear();

			continue;
		}
	}

	return NULL;
}

Solver::Status Solver::solve(const IntervalVector& init_box) {
	start(init_box);
	return solve();
}

Solver::Status Solver::solve(const char* init_paving) {
	start(init_paving);
	return solve();
}

Solver::Status Solver::solve() {

	try {

		while (next()!=NULL) { }

		if (manif->unknown.size()>0)
			manif->status = NOT_ALL_VALIDATED;
		else if (manif->inner.size()>0 || manif->boundary.size()>0)
			manif->status = SUCCESS;
		else
			manif->status = INFEASIBLE;

	} catch (TimeOutException&) {
		flush();
		manif->status = TIME_OUT;
	}
	catch (CellLimitException&) {
		flush();
		manif->status = CELL_OVERFLOW;
	}

	timer.stop();
	time = timer.get_time();

	manif->time += time;
	manif->nb_cells += nb_cells;

	return manif->status;
}


SolverOutputBox Solver::check_sol(const IntervalVector& box) {

	SolverOutputBox sol(n);

	(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::INNER; // by default

	if (eqs) {

		sol._unicity=new IntervalVector(n);

		if (m>n) {
			ibex_warning("Certification not implemented for over-constrained systems ");
			(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;
		} else if (m<n) {
			// ====== under-constrained =========
			try {
				VarSet varset=get_newton_vars(eqs->f_ctrs,box.mid(),params);

				if (inflating_newton(eqs->f_ctrs, varset, box, sol._existence, *sol._unicity)) {
					if (params.nb_param<n-m)
						sol.varset = new VarSet(varset);
				} else
					(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;

			} catch(SingularMatrixException& e) {
				(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;
			}
		} else {
			// ====== well-constrained =========
			if (!inflating_newton(eqs->f_ctrs, box.mid(), sol._existence, *sol._unicity)) {
				(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;
			}
		}
	} else {
		sol._existence=box;
		sol._unicity=NULL;
	}

	if (sol.status==SolverOutputBox::UNKNOWN) {
		sol._existence=box;
		if (sol._unicity!=NULL) delete sol._unicity;
		sol._unicity=NULL;
	} else {
		// The inflating Newton may cause the existence box to be disjoint from the input box.

		// Note that the following line also tests the case of an existence box outside
		// the initial box of the search
		if (box.is_disjoint(sol.existence())) {
			throw EmptyBoxException();
		}
	}

	if (sol.status==SolverOutputBox::INNER && !sol.existence().is_subset(solve_init_box))
		// BOUNDARY by default: will be verified later
		(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::BOUNDARY;

	if (ineqs) {
		Interval y,r;
		for (int i=0; i<ineqs->nb_ctr; i++) {
			NumConstraint& c=ineqs->ctrs[i];
			assert(c.f.image_dim()==1);
			y=c.f.eval(sol.existence());
			r=c.right_hand_side().i();
			if (y.is_disjoint(r)) throw EmptyBoxException();
			if (sol.status==SolverOutputBox::INNER && !y.is_subset(r)) {
				// BOUNDARY by default: will be verified later
				(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::BOUNDARY;
			}
		}
	}

	if (sol.status==SolverOutputBox::BOUNDARY) {
		if (!is_boundary(sol.existence()))
			(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::UNKNOWN;

	}

	if (eqs && n==m) {
		// Check if the solution is new, that is, that the solution is not included in the unicity
		// box of a previously found solution. For efficiency reason, this test is not performed in
		// the case of under-constrained systems (m<n).

		for (vector<SolverOutputBox>::iterator it=manif->inner.begin(); it!=manif->inner.end(); it++) {
			if (it->unicity().is_superset(sol._existence))
				throw EmptyBoxException();
		}
	}

	return sol;
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

SolverOutputBox& Solver::store_sol(const SolverOutputBox& sol) {

	if (trace >=1) cout << sol << endl;

	switch (sol.status) {
	case SolverOutputBox::INNER    :
		manif->inner.push_back(sol);
		return manif->inner.back();
	case SolverOutputBox::BOUNDARY :
		manif->boundary.push_back(sol);
		return manif->boundary.back();
	case SolverOutputBox::UNKNOWN  :
		manif->unknown.push_back(sol);
		return manif->unknown.back();
	case SolverOutputBox::PENDING :
	default:
		manif->pending.push_back(sol);
		return manif->pending.back();
	}
}

void Solver::flush() {
	while (!buffer.empty()) {
		Cell* cell=buffer.top();
		SolverOutputBox sol(n);
		(SolverOutputBox::sol_status&) sol.status = SolverOutputBox::PENDING;
		sol._existence=cell->box;
		sol._unicity=NULL;
		store_sol(sol);
		delete buffer.pop();
	}
}

void Solver::report() {

	switch(manif->status) {
	case SUCCESS: cout << "\033[32m" << " solving successful!" << endl;
	break;
	case INFEASIBLE: cout << "\033[31m" << " infeasible problem" << endl;
	break;
	case NOT_ALL_VALIDATED: cout << "\033[31m" << " done! but some boxes have 'unknown' status." << endl;
	break;
	case TIME_OUT: cout << "\033[31m" << " time limit " << time_limit << "s. reached " << endl;
	break;
	case CELL_OVERFLOW: cout << "\033[31m" << " cell overflow" << endl;
	}

	cout << "\033[0m" << endl;

	cout << " number of inner boxes:\t\t" << manif->inner.size() << endl;
	cout << " number of boundary boxes:\t" << manif->boundary.size() << endl;
	cout << " number of unknown boxes:\t" << manif->unknown.size() << endl;
	cout << " number of pending boxes:\t" << manif->pending.size() << endl;
	cout << " cpu time used:\t\t\t" << time << "s";
	if (manif->time!=time)
		cout << " [total=" << manif->time << "]";
		cout << endl;
	cout << " number of cells:\t\t" << nb_cells;
	if (manif->nb_cells!=nb_cells)
		cout << " [total=" << manif->nb_cells << "]";
	cout << endl << endl;
}

} // end namespace ibex
