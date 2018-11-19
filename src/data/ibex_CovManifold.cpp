//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Nov 13, 2018
//============================================================================

#include "ibex_CovManifold.h"

using namespace std;

namespace ibex {

CovManifold::CovManifold(const CovManifoldFactory& fac) : CovIBUList(fac), m(fac.m), nb_ineq(fac.nb_ineq), nb_solution(0), nb_boundary(0),
		_manifold_status(NULL), _manifold_solution(NULL), _manifold_boundary(NULL), proofs(NULL) {

	fac.build(*this);
}

CovManifold::~CovManifold() {
	assert(_manifold_status);
	delete[] _manifold_status;
	delete[] _manifold_solution;
	delete[] _manifold_boundary;

	for (int j=0; j<nb_solution; j++)
		delete proofs[j];
	delete[] proofs;
}

int CovManifold::subformat_number() const {
	return 0;
}

CovManifoldFactory::CovManifoldFactory(size_t n) : CovIBUListFactory(n), nb_solution(0), m(0), nb_ineq(0) {

}

CovManifoldFactory::~CovManifoldFactory() {
}

void CovManifoldFactory::add_solution(const Certificate& c) {
	CovIBUListFactory::add_boundary(c.existence());
	is_solution.push_back(true);
	proofs.push_back(c);
	nb_solution++;

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
	manif._manifold_status      = new CovManifold::BoxStatus[manif.CovIBUList::nb_boundary];
	manif._manifold_solution    = new IntervalVector*[nb_solution];
	manif._manifold_boundary    = new IntervalVector*[manif.CovIBUList::nb_boundary - nb_solution];

	int i=0;    // count closed_set_boundary boxes
	vector<bool>::const_iterator it=is_solution.begin();
	int jsol=0; // count solution boxes
	int jbo=0;  // count boundary boxes
	for (int i=0; i<manif.size; i++) {
		if (manif.is_inner(i))
			manif._manifold_status[i]=CovManifold::INNER;
		else if (manif.is_unknown(i))
			manif._manifold_status[i]=CovManifold::UNKNOWN;
		else {
			if (*it) {
				manif._manifold_status[i]=CovManifold::SOLUTION;
				manif._manifold_solution[jsol++]=(IntervalVector*) &manif.boundary(i);
			} else {
				manif._manifold_status[i]=CovManifold::BOUNDARY;
				manif._manifold_boundary[jbo++]=(IntervalVector*) &manif.boundary(i);
			}
			++it;
		}
	}
	assert(it==is_solution.end());
	assert(jsol==manif.nb_solution);
	assert(jbo==manif.nb_boundary);
}

//----------------------------------------------------------------------------------------------------

CovManifoldFile::CovManifoldFile(const char* filename, CovManifoldFactory* _factory) :
							CovIBUListFile(filename, _factory? _factory : new CovManifoldFactory(0 /*tmp*/)) {

	CovManifoldFactory& fac = * ((CovManifoldFactory*) this->factory);

	assert(f); // file descriptor is open by CovFile constructor

	size_t nb_solution = read_pos_int(*f);

	size_t nb_boundary = read_pos_int(*f); // just for integrity check

	if (nb_solution + nb_boundary != fac.is_solution.size())
		ibex_error("[CovManifoldFile]: number of solution + boundary boxes <> number of CovIBUList boundary boxes");


	/* TODO */
}


} // end namespace

