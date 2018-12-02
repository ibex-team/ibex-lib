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

CovManifold::CovManifold(const char* filename) : CovManifold(CovManifoldFactory(filename)) {

}

void CovManifold::save(const char* filename) {
	ofstream* of=CovManifoldFile::write(filename,*this);
	of->close();
	delete of;
}

CovManifold::~CovManifold() {
	assert(_manifold_status);
	delete[] _manifold_status;
	delete[] _manifold_solution;
	delete[] _manifold_boundary;

	for (size_t j=0; j<nb_solution; j++)
		delete proofs[j];
	delete[] proofs;
}

int CovManifold::subformat_number() const {
	return 0;
}

CovManifoldFactory::CovManifoldFactory(size_t n) : CovIBUListFactory(n), m(0), nb_ineq(0), nb_solution(0) {

}

CovManifoldFactory::CovManifoldFactory(const char* filename) : CovIBUListFactory((size_t) 0 /* tmp*/) {
	ifstream* f = CovManifoldFile::read(filename, *this);
	f->close();
	delete f;
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
	//assert(manif.nb_inner == nb_inner);
	//assert(manif.CovIBUList::nb_boundary == nb_boundary);
	(size_t&) manif.nb_solution = nb_solution;
	(size_t&) manif.nb_boundary = manif.CovIBUList::nb_boundary - nb_solution;
	manif._manifold_status      = new CovManifold::BoxStatus[manif.CovIBUList::nb_boundary];
	manif._manifold_solution    = new IntervalVector*[nb_solution];
	manif._manifold_boundary    = new IntervalVector*[manif.CovIBUList::nb_boundary - nb_solution];

	size_t i=0;    // count closed_set_boundary boxes
	vector<bool>::const_iterator it=is_solution.begin();
	size_t jsol=0; // count solution boxes
	size_t jbo=0;  // count boundary boxes
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
ifstream* CovManifoldFile::read(const char* filename, CovManifoldFactory& factory) {

	ifstream* f = CovIBUListFile::read(filename, factory);

	size_t nb_solution = read_pos_int(*f);

	if (nb_solution > factory.nb_boundary())
		ibex_error("[CovManifoldFile]: number of solutions > number of CovIBUList boundary boxes");

	/* TODO */

	return f;
}


ofstream* CovManifoldFile::write(const char* filename, const CovManifold& cov) {

	ofstream* f = CovIBUListFile::write(filename, cov);

	write_int(*f, cov.nb_solution);

	// TODO: a complete scan could be avoided
//	for (size_t i=0; i<cov.size; i++) {
//		if (cov.status(i)==CovIBUList::SOLUTION)
//			write_int(*f, (uint32_t) i);
//	}
	return f;
}


} // end namespace

