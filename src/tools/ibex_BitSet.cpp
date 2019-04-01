//============================================================================
//                                  I B E X
// File        : ibex_BitSet.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 20, 2017
//============================================================================

#include "ibex_BitSet.h"

using namespace std;

namespace ibex {

void BitSet::resize(int n) {

	if (initialized()) {
		BitSet b(*this);
		bitset.destroy();
		bitset.initialise(n,0);
		(*this) |= b;
	} else
		bitset.initialise(n,0);
}

BitSet BitSet::compose(const BitSet& b) const {
	assert(initialized() && b.initialized());
	assert(b.bitset.max() < size());

	BitSet b2(size());
	int c;
	for (int i=0; i<size(); i++) {
		c = i==0? min() : next(c);
		if (b[i]) b2.add(c);
	}
	return b2;
}

ostream& operator<<(ostream& os, const BitSet& b) {
	int c;
	os << '(';
	for (int i=0; i<b.size(); i++) {
		c = (i==0? b.min() : b.next(c));
		if (i>0) os << ',';
		os << c;
	}
	os << ')';
	return os;
}

} // end namespace

