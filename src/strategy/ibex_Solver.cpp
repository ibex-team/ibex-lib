//============================================================================
//                                  I B E X                                   
// File        : ibex_Solver.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 13, 2012
// Last Update : May 13, 2012
//============================================================================

#include "ibex_Solver.h"
#include "ibex_Newton.h"
#include "ibex_NoBisectableVariableException.h"
#include "ibex_LinearException.h"

#include <cassert>

using namespace std;

namespace ibex {

Solver::Solver(Ctc& ctc, Bsc& bsc, CellBuffer& buffer) :
		  ctc(ctc), bsc(bsc), buffer(buffer), time_limit(-1), cell_limit(-1), trace(0), nb_cells(0), time(0), impact(BitSet::all(ctc.nb_var)),
		  solve_init_box(IntervalVector::empty(ctc.nb_var)), eqs(NULL), ineqs(NULL), params(NULL) {

	n=ctc.nb_var;
	m=-1;
}

Solver::Solver(const System& sys, Ctc& ctc, Bsc& bsc, CellBuffer& buffer) :
		  ctc(ctc), bsc(bsc), buffer(buffer), time_limit(-1), cell_limit(-1), trace(0), nb_cells(0), time(0), impact(BitSet::all(ctc.nb_var)),
		  solve_init_box(IntervalVector::empty(ctc.nb_var)), eqs(NULL), ineqs(NULL), params(NULL) {

	init(sys, NULL);

}

Solver::Solver(const System& sys, const BitSet& _params, Ctc& ctc, Bsc& bsc, CellBuffer& buffer) :
		  ctc(ctc), bsc(bsc), buffer(buffer), time_limit(-1), cell_limit(-1), trace(0), nb_cells(0), time(0), impact(BitSet::all(ctc.nb_var)),
		  solve_init_box(IntervalVector::empty(ctc.nb_var)), eqs(NULL), ineqs(NULL), params(NULL) {

	init(sys,&_params);

}

void Solver::init(const System& sys, const BitSet* _params) {

	int nb_eq=0;

	if (_params) {
		this->params=new BitSet();
		((BitSet*) this->params)->clone(*_params);
	}

	// count the dimension of equalities
	for (int i=0; i<sys.nb_ctr; i++) {
		if (sys.ctrs[i].op==EQ) nb_eq+=sys.ctrs[i].f.image_dim();
	}

	if (nb_eq==sys.f.image_dim())
		eqs=&sys; // useless to create a new one
	else {
		ineqs=new System(sys,System::INEQ_ONLY);

		if (nb_eq>0) {
			eqs=new System(sys,System::EQ_ONLY);
		}
		// else: strange to use the solver for inequalities only...
	}

	n=sys.nb_var;
	m=eqs? eqs->f.image_dim() : 0;

}

Solver::~Solver() {
	if (params)
		delete params;

	if (ineqs) {
		delete ineqs;
		if (eqs) {
			delete eqs;
		}
	}
}

void Solver::start(const IntervalVector& init_box) {
	buffer.flush();
	solutions.clear();
	solve_init_box = init_box;
	nb_cells=0;
	time=0;

	assert(init_box.size()==ctc.nb_var);

	Cell* root=new Cell(init_box);

	// add data required by this solver
	root->add<BisectedVar>();

	// add data required by the bisector
	bsc.add_backtrackable(*root);

	buffer.push(root);

	int nb_var=init_box.size();

	IntervalVector tmpbox(ctc.nb_var);

	Timer::start();

}

bool Solver::next(const Solution*& sol) {
	try  {
		while (!buffer.empty()) {

			if (trace==2) cout << buffer << endl;

			Cell* c=buffer.top();

			int v=c->get<BisectedVar>().var;      // last bisected var.

			if (v!=-1)                          // no root node :  impact set to 1 for last bisected var only
				impact.add(v);
			else                                // root node : impact set to 1 for all variables
				impact.fill(0,ctc.nb_var-1);

			ctc.contract(c->box,impact);

			if (c->box.is_empty()) {
				delete buffer.pop();
				impact.remove(v); // note: in case of the root node, we should clear the bitset
				// instead but since the search is over, the impact is not used anymore.
				continue;
			}

			if (v!=-1)
				impact.remove(v);
			else                              // root node : impact set to 0 for all variables after contraction
				impact.clear();

			// certification is performed at each intermediate step
			// if the system is under constrained
			if (certification() && m<n) {
				Solution new_sol(n);
				if (check_sol(c->box, new_sol)) {
					if (new_sol.status==SOLUTION) {
						delete buffer.pop();
						store_sol(new_sol);
						sol = &solutions.back();
						return true;
					}
				} // otherwise: continue search...
			}

			try {

				pair<Cell*,Cell*> new_cells=bsc.bisect_cell(*c);

				delete buffer.pop();
				buffer.push(new_cells.first);
				buffer.push(new_cells.second);
				nb_cells+=2;
				if (cell_limit >=0 && nb_cells>=cell_limit) throw CellLimitException();}

			catch (NoBisectableVariableException&) {
				Solution new_sol(n);
				bool is_sol=check_sol(c->box,new_sol);
				delete buffer.pop();
				if (is_sol) {
					store_sol(new_sol);
					sol = &solutions.back();
					return true;
				}
				// note that we skip time_limit_check() here.
				// In the case where "next" is called by "solve",
				// and if time has exceeded, the exception will be raised by the
				// very next call to "next" anyway. This holds, unless "next" finds
				// new solutions again and again endlessly. So there is a little risk
				// of uncaught timeout in this case (but this case is probably already
				// an error case).
			}
			time_limit_check();
		}
	}
	catch (TimeOutException&) {
		cout << "time limit " << time_limit << "s. reached " << endl;
	}
	catch (CellLimitException&) {
		cout << "cell limit " << cell_limit << " reached " << endl;
	}

	Timer::stop();
	time+= Timer::VIRTUAL_TIMELAPSE();

	return false;

}

vector<IntervalVector> Solver::solve(const IntervalVector& init_box) {
	vector<IntervalVector> sols;
	start(init_box);
	while (next(sols)) { }
	return sols;
}

void Solver::time_limit_check () {
	Timer::stop();
	time += Timer::VIRTUAL_TIMELAPSE();
	if (time_limit >0 &&  time >=time_limit) throw TimeOutException();
	Timer::start();
}

bool Solver::check_sol(IntervalVector& box, Solution& sol) {

	bool proved=true;

	if (eqs) {

		sol._unicity=new IntervalVector(n);

		if (m>n)
			not_implemented("Solver: certification of over-constrained systems");
		else if (m<n) {
			// ====== under-constrained =========
			try {
				VarSet varset=get_newton_vars(eqs->f,box.mid(),params? *params: BitSet::empty(n));

				proved=inflating_newton(eqs->f, varset, box, sol._existence, *sol._unicity);

				if (params && params->size()<n-m)
					sol.varset = new VarSet(varset);

			} catch(SingularMatrixException& e) {
				proved=false;
			}

			if (!proved) {
				sol._existence=box;
				delete sol._unicity;
				sol._unicity=NULL;
			}
		} else {
			// ====== well-constrained =========
			proved=inflating_newton(eqs->f, box.mid(), sol._existence, *sol._unicity);

			if (!proved) {
				sol._existence=box;
				delete sol._unicity;
				sol._unicity=NULL;
			}
		}
	} else {
		sol._existence=box;
		sol._unicity=NULL;
	}

	if (ineqs) {
		Interval y,r;
		for (int i=0; i<ineqs->nb_ctr; i++) {
			NumConstraint& c=ineqs->ctrs[i];
			assert(c.f.image_dim()==1);
			y=c.f.eval(sol._existence);
			r=c.right_hand_side().i();
			if (y.is_disjoint(r)) return false;
			if (proved && !y.is_subset(r)) proved=false;
		}
	}

	if (proved) {
		// the following cases can only happen if proved==true because only the inflating
		// Newton may cause a box produced by the search to be outside the initial box

		if (solve_init_box.is_disjoint(sol._existence)) {
			return false;
		}

		if (!solve_init_box.is_superset(sol._existence)) {
			proved=false; // don't return now, the box may be be discarded later
		}
	}

	if (eqs && n==m) {
		// Check if the solution is new, that is, that the solution is not included in the unicity
		// box of a previously found solution. For efficiency reason, this test is not performed in
		// the case of under-constrained systems (m<n).

		for (vector<Solution>::iterator it=solutions.begin(); it!=solutions.end(); it++) {
			if (it->unicity().is_superset(sol._existence))
				return false;
		}
	}

	(sol_status&) sol.status = (certification() && proved)? SOLUTION : UNKNOWN;

	return true;
}

void Solver::store_sol(const Solver::Solution& sol) {

	solutions.push_back(sol);

	cout.precision(12);

	if (trace >=1) {
		cout << " sol n°" << solutions.size() << " = " << solutions.back().existence() << " ";
		switch(sol.status) {
		case SOLUTION :   cout << "[certified]"; break;
		case UNKNOWN :    cout << "[unknown]"; break;
		}
		cout << endl;
	}
}

} // end namespace ibex
