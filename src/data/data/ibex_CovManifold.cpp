//============================================================================
//                                  I B E X
// File        : ibex_BxsManifold.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Not 08, 2018
//============================================================================

#include "ibex_BxsManifold.h"

using namespace std;

namespace ibex {

BxsManifold::BxsManifold(const BxsManifoldFactory& fac) : BxsClosedSet(fac), m(fac.m), nb_ineq(fac.nb_ineq), nb_solution(0), manifold_nb_boundary(0),
		_manifold_is_solution(NULL), _manifold_solution(NULL), _manifold_boundary(NULL) {
	fac.build(*this);
}

BxsManifold::~BxsManifold() {
	assert(_manifold_is_solution);
	delete[] _manifold_is_solution;
	delete[] _manifold_solution;
	delete[] _manifold_boundary;
}

int BxsManifold::subformat_number() const {
	return 0;
}

BxsManifoldFactory::BxsManifoldFactory() : nb_solution(0), m(0), nb_ineq(0) {

}

BxsManifoldFactory::~BxsManifoldFactory() {
}

void BxsManifoldFactory::add_solution(const IntervalVector& x) {
	BxsClosedSetFactory::add_boundary(x);
	is_solution.push_back(true);
}

void BxsManifoldFactory::add_boundary(const IntervalVector& x) {
	BxsClosedSetFactory::add_boundary(x);
	is_solution.push_back(false);
}

void BxsManifoldFactory::build(BxsManifold& manif) const {
	assert(manif.set_nb_inner == nb_inner);
	assert(manif.closed_set_nb_boundary == nb_boundary);
	(size_t&) manif.nb_solution = nb_solution;
	(size_t&) manif.manifold_nb_boundary = manif.closed_set_nb_boundary - nb_solution;
	manif._manifold_is_solution = new bool[manif.closed_set_nb_boundary];
	manif._manifold_solution    = new IntervalVector*[nb_solution];
	manif._manifold_boundary     = new IntervalVector*[manif.closed_set_nb_boundary - nb_solution];

	int i=0;    // count closed_set_boundary boxes
	int jsol=0; // count solution boxes
	int jbo=0;  // count boundary boxes
	for (vector<bool>::const_iterator it=is_solution.begin(); it!=is_solution.end(); ++it, i++) {
		manif._manifold_is_solution[i]=*it;
		if (*it)
			manif._manifold_solution[jsol++]=(IntervalVector*) &manif.closed_set_boundary(i);
		else
			manif._manifold_boundary[jbo++]=(IntervalVector*) &manif.closed_set_boundary(i);
	}
	assert(jsol==manif.nb_solution);
	assert(jbo==manif.manifold_nb_boundary);
}

} // end namespace

