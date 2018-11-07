//============================================================================
//                                  I B E X
// File        : ibex_BxsList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#include "ibex_BxsList.h"

using namespace std;

namespace ibex {

BxsRawList::BxsRawList(const BxsRawListFactory& fac) : Bxs(fac.n), size(0), list(NULL) {
	fac.build(*this);
}

BxsRawList::~BxsRawList() {
	delete[] list;
}

int BxsRawList::subformat_number() const {
	return 0;
}

void BxsRawListFactory::add(const IntervalVector& x) {
	if (boxes.empty())
		n=x.size();
	else {
		if (n!=x.size())
			ibex_error("[BxsRawListFactory] boxes must have all the same size.");
	}
	boxes.push_back(x);
}

void BxsRawListFactory::build(BxsRawList& rwl) const {
	(size_t&) rwl.n = n;
	(size_t&)  rwl.size = boxes.size();
	rwl.list = new IntervalVector[boxes.size()];
	int i=0;
	for (vector<IntervalVector>::const_iterator it=boxes.begin(); it!=boxes.end(); ++it, i++) {
		rwl.list[i].resize(n);
		rwl.list[i]=*it;
	}
}

} /* namespace ibex */
