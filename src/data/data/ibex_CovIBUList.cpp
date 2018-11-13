//============================================================================
//                                  I B E X
// File        : ibex_BxsClosedSet.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#include "ibex_BxsClosedSet.h"

using namespace std;

namespace ibex {

BxsClosedSet::BxsClosedSet(const BxsClosedSetFactory& fac) : BxsSet(fac), closed_set_nb_boundary(0), closed_set_nb_unknown(0),
		_closed_set_is_boundary(NULL), _closed_set_boundary(NULL), _closed_set_unknown(NULL) {
	fac.build(*this);
}

BxsClosedSet::~BxsClosedSet() {
	assert(_closed_set_is_boundary);
	delete[] _closed_set_is_boundary;
	delete[] _closed_set_boundary;
	delete[] _closed_set_unknown;
}

int BxsClosedSet::subformat_number() const {
	return 0;
}

BxsClosedSetFactory::BxsClosedSetFactory() : nb_boundary(0) {

}

BxsClosedSetFactory::~BxsClosedSetFactory() {
}

void BxsClosedSetFactory::add_boundary(const IntervalVector& x) {
	BxsSetFactory::add_unknown(x);
	is_boundary.push_back(true);
}

void BxsClosedSetFactory::add_unknown(const IntervalVector& x) {
	BxsSetFactory::add_unknown(x);
	is_boundary.push_back(false);
}

void BxsClosedSetFactory::build(BxsClosedSet& set) const {
	assert(set.set_nb_inner == nb_inner);
	(size_t&) set.closed_set_nb_boundary = nb_boundary;
	(size_t&) set.closed_set_nb_unknown = set.set_nb_unknown - nb_boundary;
	set._closed_set_is_boundary = new bool[set.set_nb_unknown];
	set._closed_set_boundary    = new IntervalVector*[nb_boundary];
	set._closed_set_unknown     = new IntervalVector*[set.set_nb_unknown - nb_boundary];

	int i=0;   // count set_unknown boxes
	int jbo=0; // count boundary boxes
	int juk=0; // count unknown boxes
	for (vector<bool>::const_iterator it=is_boundary.begin(); it!=is_boundary.end(); ++it, i++) {
		set._closed_set_is_boundary[i]=*it;
		if (*it)
			set._closed_set_boundary[jbo++]=(IntervalVector*) &set.set_unknown(i);
		else
			set._closed_set_unknown[juk++]=(IntervalVector*) &set.set_unknown(i);
	}
	assert(jbo==set.closed_set_nb_boundary);
	assert(juk==set.closed_set_nb_unknown);
}

} // end namespace

