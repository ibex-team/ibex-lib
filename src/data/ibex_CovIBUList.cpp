//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#include "ibex_CovIBUList.h"

using namespace std;

namespace ibex {

CovIBUList::CovIBUList(const CovIBUListFactory& fac) : CovIUList(fac), nb_boundary(0), nb_unknown(0),
		_closed_set_is_boundary(NULL), _closed_set_boundary(NULL), _closed_set_unknown(NULL) {
	fac.build(*this);
}

CovIBUList::~CovIBUList() {
	assert(_closed_set_is_boundary);
	delete[] _closed_set_is_boundary;
	delete[] _closed_set_boundary;
	delete[] _closed_set_unknown;
}

int CovIBUList::subformat_number() const {
	return 0;
}

CovIBUListFactory::CovIBUListFactory(size_t n) : CovIUListFactory(n), nb_boundary(0) {

}

CovIBUListFactory::~CovIBUListFactory() {
}

void CovIBUListFactory::add(const IntervalVector& x) {
	add_unknown(x);
}

void CovIBUListFactory::add_boundary(const IntervalVector& x) {
	CovIUListFactory::add_unknown(x);
	is_boundary.push_back(true);
}

void CovIBUListFactory::add_unknown(const IntervalVector& x) {
	CovIUListFactory::add_unknown(x);
	is_boundary.push_back(false);
}

void CovIBUListFactory::build(CovIBUList& set) const {
	assert(set.nb_inner == nb_inner);
	(size_t&) set.nb_boundary = nb_boundary;
	(size_t&) set.nb_unknown = set.CovIUList::nb_unknown - nb_boundary;
	set._closed_set_is_boundary = new bool[set.CovIUList::nb_unknown];
	set._closed_set_boundary    = new IntervalVector*[nb_boundary];
	set._closed_set_unknown     = new IntervalVector*[set.CovIUList::nb_unknown - nb_boundary];

	int i=0;   // count set_unknown boxes
	int jbo=0; // count boundary boxes
	int juk=0; // count unknown boxes
	for (vector<bool>::const_iterator it=is_boundary.begin(); it!=is_boundary.end(); ++it, i++) {
		set._closed_set_is_boundary[i]=*it;
		if (*it)
			set._closed_set_boundary[jbo++]=(IntervalVector*) &set.unknown(i);
		else
			set._closed_set_unknown[juk++]=(IntervalVector*) &set.unknown(i);
	}
	assert(jbo==set.nb_boundary);
	assert(juk==set.nb_unknown);
}

} // end namespace

