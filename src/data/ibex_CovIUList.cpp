//============================================================================
//                                  I B E X
// File        : ibex_CovIUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Nov 13, 2018
//============================================================================

#include "ibex_CovIUList.h"

#include <cassert>

using namespace std;

namespace ibex {

CovIUList::CovIUList(const CovIUListFactory& fac) : CovList(fac), nb_inner(0), nb_unknown(0),
		_IU_status(NULL), _IU_inner(NULL), _IU_unknown(NULL) {
	fac.build(*this);
}

CovIUList::CovIUList(const char* filename) : CovIUList((CovIUListFactory&) *CovIUListFile(filename).factory) {

}

CovIUList::~CovIUList() {
	if (_IU_status) {
		delete[] _IU_status;
		delete[] _IU_inner;
		delete[] _IU_unknown;
	}
}

//----------------------------------------------------------------------------------------------------

CovIUListFactory::CovIUListFactory(size_t n) : CovListFactory(n), nb_inner(0) {

}
void CovIUListFactory::add(const IntervalVector& x) {
	add_unknown(x);
}

void CovIUListFactory::add_inner(const IntervalVector& x) {
	CovListFactory::add(x);
	is_inner.push_back(true);
	nb_inner++;
}

void CovIUListFactory::add_unknown(const IntervalVector& x) {
	CovListFactory::add(x);
	is_inner.push_back(false);
}

void CovIUListFactory::build(CovIUList& set) const {
	assert(set.size == boxes.size());
	(size_t&) set.nb_inner = nb_inner;
	(size_t&) set.nb_unknown = set.size - nb_inner;
	set._IU_status = new CovIUList::BoxStatus[set.size];
	set._IU_inner    = new IntervalVector*[nb_inner];
	set._IU_unknown  = new IntervalVector*[set.size - nb_inner];

	int i=0;   // count boxes
	int jin=0; // count inner boxes
	int juk=0; // count unknown boxes
	for (vector<bool>::const_iterator it=is_inner.begin(); it!=is_inner.end(); ++it, i++) {

		if (*it) {
			set._IU_status[i]=CovIUList::INNER;
			set._IU_inner[jin++]=(IntervalVector*) &set[i];
		} else {
			set._IU_status[i]=CovIUList::UNKNOWN;
			set._IU_unknown[juk++]=(IntervalVector*) &set[i];
		}
	}
	assert(jin==set.nb_inner);
	assert(juk==set.nb_unknown);
}


//----------------------------------------------------------------------------------------------------

CovIUListFile::CovIUListFile(const char* filename, CovIUListFactory* _factory) :

		CovListFile(filename, _factory? _factory : new CovIUListFactory(0 /*tmp*/)) {

	CovIUListFactory& fac = * ((CovIUListFactory*) this->factory);

	assert(f); // file descriptor is open by CovFile constructor

	size_t nb_inner = read_pos_int(*f);

	size_t nb_unknown = read_pos_int(*f);

	if (nb_inner  + nb_unknown != fac.nb_boxes())
		ibex_error("[CovIUListFile]: number of inner + boundary boxes <> total");

	if (fac.nb_boxes()==0) return;

	for (vector<bool>::iterator it=fac.is_inner.begin(); it!=fac.is_inner.end(); ++it) {
	//for (unsigned int i=0; i<factory.boxes.size(); i++) {
		unsigned int status=read_pos_int(*f);
		switch(status) {
		case 0:  *it = true; fac.nb_inner++;  break;
		case 1:  *it = false; /*useless*/ break;
		default: ibex_error("[CovIUListFile]: bad input file (bad status code).");
		}
	}

	if (fac.nb_inner != nb_inner)
		ibex_error("[CovIUListFile]: number of inner boxes does not match");
}

int CovIUListFile::subformat_number() const {
	return 0;
}

} /* namespace ibex */
