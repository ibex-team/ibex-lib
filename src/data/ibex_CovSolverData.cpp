//============================================================================
//                                  I B E X
// File        : ibex_CovSolverData.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Not 08, 2018
//============================================================================

#include "ibex_CovSolverData.h"

using namespace std;

namespace ibex {

CovSolverData::CovSolverData(const CovSolverDataFactory& fac) : CovManifold(fac), var_names(NULL /*TODO */),
		status(fac.status), time(fac.time), nb_cells(fac.nb_cells),
		nb_pending(0), nb_unknown(0), _closed_set_is_pending(NULL), _solver_pending(NULL), _solver_unknown(NULL) {
	fac.build(*this);
}

CovSolverData::~CovSolverData() {
	assert(_closed_set_is_pending);
	delete[] _closed_set_is_pending;
	delete[] _solver_pending;
	delete[] _solver_unknown;
}

int CovSolverData::subformat_number() const {
	return 0;
}

/**
 * TODO: default solver status should be something like Solver::UNKNOWN
 */
CovSolverDataFactory::CovSolverDataFactory(size_t n) : CovManifoldFactory(n), nb_pending(0), var_names(NULL), status(Solver::INFEASIBLE), time(-1), nb_cells(0) {

}

CovSolverDataFactory::~CovSolverDataFactory() {
}

void CovSolverDataFactory::add_pending(const IntervalVector& x) {
	CovIBUListFactory::add_unknown(x);
	is_pending.push_back(true);
}

void CovSolverDataFactory::add_unknown(const IntervalVector& x) {
	CovIBUListFactory::add_unknown(x);
	is_pending.push_back(false);
}

void CovSolverDataFactory::build(CovSolverData& solver) const {
	assert(solver.nb_inner == nb_inner);
	assert(solver.nb_boundary == nb_boundary);
	assert(solver.nb_solution == nb_solution);
	(size_t&) solver.nb_pending = nb_pending;
	(size_t&) solver.nb_unknown= solver.CovIBUList::nb_unknown - nb_pending;
	solver._closed_set_is_pending = new bool[solver.CovIBUList::nb_unknown];
	solver._solver_pending    = new IntervalVector*[nb_pending];
	solver._solver_unknown     = new IntervalVector*[solver.CovIBUList::nb_unknown - nb_pending];

	int i=0;    // count closed_set_boundary boxes
	int jpen=0; // count solution boxes
	int junk=0;  // count boundary boxes
	for (vector<bool>::const_iterator it=is_pending.begin(); it!=is_pending.end(); ++it, i++) {
		solver._closed_set_is_pending[i]=*it;
		if (*it)
			solver._solver_pending[jpen++]=(IntervalVector*) &solver.unknown(i);
		else
			solver._solver_unknown[junk++]=(IntervalVector*) &solver.unknown(i);
	}
	assert(jpen==solver.nb_pending);
	assert(junk==solver.nb_unknown);
}

} // end namespace

