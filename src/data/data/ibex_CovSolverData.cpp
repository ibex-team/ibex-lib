//============================================================================
//                                  I B E X
// File        : ibex_BxsSolver.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Not 08, 2018
//============================================================================

#include "ibex_BxsSolver.h"

using namespace std;

namespace ibex {

BxsSolver::BxsSolver(const BxsSolverFactory& fac) : BxsManifold(fac), var_names(NULL /*TODO */),
		status(fac.status), time(fac.time), nb_cells(fac.nb_cells),
		nb_pending(0), solver_nb_unknown(0), _closed_set_is_pending(NULL), _solver_pending(NULL), _solver_unknown(NULL) {
	fac.build(*this);
}

BxsSolver::~BxsSolver() {
	assert(_closed_set_is_pending);
	delete[] _closed_set_is_pending;
	delete[] _solver_pending;
	delete[] _solver_unknown;
}

int BxsSolver::subformat_number() const {
	return 0;
}

/**
 * TODO: default solver status should be something like Solver::UNKNOWN
 */
BxsSolverFactory::BxsSolverFactory() : nb_pending(0), var_names(NULL), status(Solver::INFEASIBLE), time(-1), nb_cells(0) {

}

BxsSolverFactory::~BxsSolverFactory() {
}

void BxsSolverFactory::add_pending(const IntervalVector& x) {
	BxsClosedSetFactory::add_unknown(x);
	is_pending.push_back(true);
}

void BxsSolverFactory::add_unknown(const IntervalVector& x) {
	BxsClosedSetFactory::add_unknown(x);
	is_pending.push_back(false);
}

void BxsSolverFactory::build(BxsSolver& solver) const {
	assert(solver.set_nb_inner == nb_inner);
	assert(solver.closed_set_nb_boundary == nb_boundary);
	assert(solver.nb_solution == nb_solution);
	(size_t&) solver.nb_pending = nb_pending;
	(size_t&) solver.solver_nb_unknown= solver.closed_set_nb_unknown - nb_pending;
	solver._closed_set_is_pending = new bool[solver.closed_set_nb_unknown];
	solver._solver_pending    = new IntervalVector*[nb_pending];
	solver._solver_unknown     = new IntervalVector*[solver.closed_set_nb_unknown - nb_pending];

	int i=0;    // count closed_set_boundary boxes
	int jpen=0; // count solution boxes
	int junk=0;  // count boundary boxes
	for (vector<bool>::const_iterator it=is_pending.begin(); it!=is_pending.end(); ++it, i++) {
		solver._closed_set_is_pending[i]=*it;
		if (*it)
			solver._solver_pending[jpen++]=(IntervalVector*) &solver.closed_set_unknown(i);
		else
			solver._solver_unknown[junk++]=(IntervalVector*) &solver.closed_set_unknown(i);
	}
	assert(jpen==solver.nb_pending);
	assert(junk==solver.solver_nb_unknown);
}

} // end namespace

