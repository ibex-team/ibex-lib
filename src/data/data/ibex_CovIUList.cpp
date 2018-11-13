//============================================================================
//                                  I B E X
// File        : ibex_BxsSet.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#include "ibex_BxsSet.h"

using namespace std;

namespace ibex {

BxsSet::BxsSet(const BxsSetFactory& fac) : BxsRawList(fac), set_nb_inner(0), set_nb_unknown(0),
		_set_is_inner(NULL), _set_inner(NULL), _set_unknown(NULL) {
	fac.build(*this);
}

BxsSet::~BxsSet() {
	assert(_set_is_inner);
	delete[] _set_is_inner;
	delete[] _set_inner;
	delete[] _set_unknown;
}

int BxsSet::subformat_number() const {
	return 0;
}

BxsSetFactory::BxsSetFactory() : nb_inner(0) {

}

void BxsSetFactory::add_inner(const IntervalVector& x) {
	add(x);
	is_inner.push_back(true);
	nb_inner++;
}

void BxsSetFactory::add_unknown(const IntervalVector& x) {
	add(x);
	is_inner.push_back(false);
}

void BxsSetFactory::build(BxsSet& set) const {
	assert(set.size == boxes.size());
	(size_t&) set.set_nb_inner = nb_inner;
	(size_t&) set.set_nb_unknown = set.size - nb_inner;
	set._set_is_inner = new bool[set.size];
	set._set_inner    = new IntervalVector*[nb_inner];
	set._set_unknown  = new IntervalVector*[set.size - nb_inner];

	int i=0;   // count boxes
	int jin=0; // count inner boxes
	int juk=0; // count unknown boxes
	for (vector<bool>::const_iterator it=is_inner.begin(); it!=is_inner.end(); ++it, i++) {
		set._set_is_inner[i]=*it;
		if (*it)
			set._set_inner[jin++]=(IntervalVector*) &set[i];
		else
			set._set_unknown[juk++]=(IntervalVector*) &set[i];
	}
	assert(jin==set.set_nb_inner);
	assert(juk==set.set_nb_unknown);
}

} /* namespace ibex */
