//============================================================================
//                                  I B E X
// File        : ibex_CovIUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Not 07, 2018
//============================================================================

#include "ibex_CovIUList.h"

#include <cassert>

using namespace std;

namespace ibex {

CovIUList::CovIUList(const CovIUListFactory& fac) : CovList(fac), nb_inner(0), nb_unknown(0),
		_set_status(NULL), _set_inner(NULL), _set_unknown(NULL) {
	fac.build(*this);
}

CovIUList::CovIUList(const char* filename) : CovIUList(*CovIUListFile(filename).factory) {

}

CovIUList::~CovIUList() {
	if (_set_status) {
		delete[] _set_status;
		delete[] _set_inner;
		delete[] _set_unknown;
	}
}

//----------------------------------------------------------------------------------------------------

CovIUListFactory::CovIUListFactory(size_t n) : CovListFactory(n), nb_inner(0) {

}

void CovIUListFactory::add_inner(const IntervalVector& x) {
	add(x);
	is_inner.push_back(true);
	nb_inner++;
}

void CovIUListFactory::add_unknown(const IntervalVector& x) {
	add(x);
	is_inner.push_back(false);
}

void CovIUListFactory::build(CovIUList& set) const {
	assert(set.size == boxes.size());
	(size_t&) set.nb_inner = nb_inner;
	(size_t&) set.nb_unknown = set.size - nb_inner;
	set._set_status = new CovIUList::BoxStatus[set.size];
	set._set_inner    = new IntervalVector*[nb_inner];
	set._set_unknown  = new IntervalVector*[set.size - nb_inner];

	int i=0;   // count boxes
	int jin=0; // count inner boxes
	int juk=0; // count unknown boxes
	for (vector<bool>::const_iterator it=is_inner.begin(); it!=is_inner.end(); ++it, i++) {

		if (*it) {
			set._set_status[i]=CovIUList::INNER;
			set._set_inner[jin++]=(IntervalVector*) &set[i];
		} else {
			set._set_status[i]=CovIUList::UNKNOWN;
			set._set_unknown[juk++]=(IntervalVector*) &set[i];
		}
	}
	assert(jin==set.nb_inner);
	assert(juk==set.nb_unknown);
}


//----------------------------------------------------------------------------------------------------

CovIUListFile::CovIUListFile(const char* filename, CovIUListFactory* factory=NULL) :

		CovListFile(filename,factory? factory : new CovIUListFactory(0 /*tmp*/)), factory(factory) {

	if (factory==NULL)
		factory = (CovIUListFactory*) CovListFile::factory;

	assert(f); // file is open by Cov constructor

	size_t nb_inner = read_pos_int(*f);

	size_t nb_unknown = read_pos_int(*f);

	if (nb_inner  + nb_unknown != factory->boxes.size())
		ibex_error("[CovIUListFile]: number of inner + boundary boxes <> total");

	if (factory->boxes.size()==0) return;

//	if (nb_inner>0)
//		_set_inner    = new IntervalVector*[nb_inner];
//	if (nb_unknown>0)
//		_set_unknown  = new IntervalVector*[size() - nb_inner];
//
//	_set_status = new BoxStatus[size()];

	for (unsigned int i=0; i<factory->boxes.size(); i++) {
		unsigned int status=read_pos_int(*f);
		switch(status) {
		case 0:  factory->is_inner.push_back(true); factory->nb_inner++;  break;
		case 1:  factory->is_inner.push_back(false); break;
		default: ibex_error("[CovIUListFile]: bad input file (bad status code).");
		}
	}

	if (factory->nb_inner != nb_inner)
		ibex_error("[CovIUListFile]: number of inner boxes does not match");
}

int CovIUListFile::subformat_number() const {
	return 0;
}

} /* namespace ibex */
