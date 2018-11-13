//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Not 08, 2018
//============================================================================

#include "ibex_CovManifold.h"

using namespace std;

namespace ibex {

CovManifold::CovManifold(const CovManifoldFactory& fac) : CovIBUList(fac), m(fac.m), nb_ineq(fac.nb_ineq), nb_solution(0), nb_boundary(0),
		_manifold_is_solution(NULL), _manifold_solution(NULL), _manifold_boundary(NULL) {
	fac.build(*this);
}

CovManifold::~CovManifold() {
	assert(_manifold_is_solution);
	delete[] _manifold_is_solution;
	delete[] _manifold_solution;
	delete[] _manifold_boundary;
}

int CovManifold::subformat_number() const {
	return 0;
}

CovManifoldFactory::CovManifoldFactory() : nb_solution(0), m(0), nb_ineq(0) {

}

CovManifoldFactory::~CovManifoldFactory() {
}

void CovManifoldFactory::add_solution(const IntervalVector& x) {
	CovIBUListFactory::add_boundary(x);
	is_solution.push_back(true);
}

void CovManifoldFactory::add_boundary(const IntervalVector& x) {
	CovIBUListFactory::add_boundary(x);
	is_solution.push_back(false);
}

void CovManifoldFactory::build(CovManifold& manif) const {
	assert(manif.nb_inner == nb_inner);
	assert(manif.CovIBUList::nb_boundary == nb_boundary);
	(size_t&) manif.nb_solution = nb_solution;
	(size_t&) manif.nb_boundary = manif.CovIBUList::nb_boundary - nb_solution;
	manif._manifold_is_solution = new bool[manif.CovIBUList::nb_boundary];
	manif._manifold_solution    = new IntervalVector*[nb_solution];
	manif._manifold_boundary     = new IntervalVector*[manif.CovIBUList::nb_boundary - nb_solution];

	int i=0;    // count closed_set_boundary boxes
	int jsol=0; // count solution boxes
	int jbo=0;  // count boundary boxes
	for (vector<bool>::const_iterator it=is_solution.begin(); it!=is_solution.end(); ++it, i++) {
		manif._manifold_is_solution[i]=*it;
		if (*it)
			manif._manifold_solution[jsol++]=(IntervalVector*) &manif.boundary(i);
		else
			manif._manifold_boundary[jbo++]=(IntervalVector*) &manif.boundary(i);
	}
	assert(jsol==manif.nb_solution);
	assert(jbo==manif.nb_boundary);
}

} // end namespace

