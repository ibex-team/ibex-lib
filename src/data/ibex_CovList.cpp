//============================================================================
//                                  I B E X
// File        : ibex_CovList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#include "ibex_CovList.h"

#include <cassert>

using namespace std;

namespace ibex {

CovList::CovList(const CovListFactory& fac) : Cov(fac.n), size(0), list(NULL) {
	fac.build(*this);
}


CovList::CovList(const char* filename) : CovList(*CovListFile(filename).factory) {

}

CovList::~CovList() {
	if (list) delete[] list;
}

//----------------------------------------------------------------------------------------------------

CovListFactory::CovListFactory(size_t n) : CovFactory(n), n(n) {

}

void CovListFactory::add(const IntervalVector& x) {
	if (boxes.empty())
		n=x.size();
	else {
		if (n!=x.size())
			ibex_error("[CovListFactory] boxes must have all the same size.");
	}
	boxes.push_back(x);
}

void CovListFactory::build(CovList& l) const {
	assert(l.n == n);
	(size_t&) l.size = boxes.size();
	if (l.size>0) {
		l.list = new IntervalVector[boxes.size()];
		int i=0;
		for (vector<IntervalVector>::const_iterator it=boxes.begin(); it!=boxes.end(); ++it, i++) {
			l.list[i].resize(n);
			l.list[i]=*it;
		}
	}
}

//----------------------------------------------------------------------------------------------------

CovListFile::CovListFile(const char* filename): //, CovListFactory* factory) :
		CovFile(filename,factory? factory : new CovListFactory(0 /*tmp*/)), factory(factory) {

	if (factory==NULL)
		factory = (CovListFactory*) CovFile::factory;

	// note: factory reference re-stored in CovListFile field to avoid casts.

	assert(f); // file is open by Cov constructor

	size_t size = read_pos_int(*f);

	for (unsigned int i=0; i<size; i++) {
		factory->add(read_box(*f));
	}
}

IntervalVector CovListFile::read_box(ifstream& f) {

	IntervalVector box(factory->n);

	for (unsigned int j=0; j<factory->n; j++) {
		double lb=read_double(f);
		double ub=read_double(f);
		box[j]=Interval(lb,ub);
	}

	return box;
}

void CovListFile::write_box(ofstream& f, const IntervalVector& box) const {
	for (int i=0; i<box.size(); i++) {
		write_double(f,box[i].lb());
		write_double(f,box[i].ub());
	}
}

//int CovListFile::subformat_number() const {
//	return 0;
//}


} /* namespace ibex */
