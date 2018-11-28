//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Nov 13, 2018
//============================================================================

#include "ibex_CovIBUList.h"

using namespace std;

namespace ibex {

CovIBUList::CovIBUList(const CovIBUListFactory& fac) : CovIUList(fac), nb_boundary(0), nb_unknown(0),
		_IBU_status(NULL), _IBU_boundary(NULL), _IBU_unknown(NULL) {
	fac.build(*this);
}

CovIBUList::~CovIBUList() {
	assert(_IBU_status);
	delete[] _IBU_status;
	delete[] _IBU_boundary;
	delete[] _IBU_unknown;
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
	nb_boundary++;
}

void CovIBUListFactory::add_unknown(const IntervalVector& x) {
	CovIUListFactory::add_unknown(x);
	is_boundary.push_back(false);
}

void CovIBUListFactory::build(CovIBUList& set) const {
	//assert(set.nb_inner == nb_inner);
	//(size_t&) set.nb_boundary = nb_boundary;
	(size_t&) set.nb_unknown  = set.CovIUList::nb_unknown - nb_boundary;
	set._IBU_status           = new CovIBUList::BoxStatus[set.CovIUList::nb_unknown];
	set._IBU_boundary         = new IntervalVector*[nb_boundary];
	set._IBU_unknown          = new IntervalVector*[set.CovIUList::nb_unknown - nb_boundary];

	int i=0;   // count set_unknown boxes
	vector<bool>::const_iterator it=is_boundary.begin();
	int jbo=0; // count boundary boxes
	int juk=0; // count unknown boxes


	for (int i=0; i<set.size; i++) {
		if (set.is_inner(i)) {
			set._IBU_status[i]=CovIBUList::INNER;
		} else {
			if (*it) {
				set._IBU_status[i]=CovIBUList::BOUNDARY;
				set._IBU_boundary[jbo++]=(IntervalVector*) &set.unknown(i);
			} else {
				set._IBU_status[i]=CovIBUList::UNKNOWN;
				set._IBU_unknown[juk++]=(IntervalVector*) &set.unknown(i);
			}
			++it;
		}
	}
	assert(it==is_boundary.end());
	assert(jbo==set.nb_boundary);
	assert(juk==set.nb_unknown);
}

//----------------------------------------------------------------------------------------------------

CovIBUListFile::CovIBUListFile(const char* filename, CovIBUListFactory* _factory) :

		CovIUListFile(filename, _factory? _factory : new CovIBUListFactory(0 /*tmp*/)) {

	CovIBUListFactory& fac = * ((CovIBUListFactory*) this->factory);

	assert(f); // file descriptor is open by CovFile constructor

	size_t nb_boundary = read_pos_int(*f);

	size_t nb_unknown = read_pos_int(*f); // just for integrity check

	if (nb_boundary  + nb_unknown != fac.is_boundary.size())
		ibex_error("[CovIUListFile]: number of boundary + unknown boxes <> number of CovList boundary boxes");

	for (vector<bool>::iterator it=fac.is_boundary.begin(); it!=fac.is_boundary.end(); ++it) {
		unsigned int status=read_pos_int(*f);
		switch(status) {
		case 0:  *it = true; fac.nb_boundary++;  break;
		case 1:  *it = false; /*useless*/ break;
		default: ibex_error("[CovIBUListFile]: bad input file (bad status code).");
		}
	}

	if (fac.nb_boundary != nb_boundary)
		ibex_error("[CovIUListFile]: number of boundary boxes does not match");
}


} // end namespace

