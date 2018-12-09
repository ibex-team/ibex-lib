//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 01, 2018
//============================================================================

#include "ibex_CovIBUList.h"

using namespace std;

namespace ibex {

CovIBUList::CovIBUList(const CovIBUListFactory& fac) : CovIUList(fac), nb_boundary(0), nb_unknown(0),
		_IBU_status(NULL), _IBU_boundary(NULL), _IBU_unknown(NULL) {
	fac.build(*this);
}

CovIBUList::CovIBUList(const char* filename) : CovIBUList(CovIBUListFactory(filename)) {

}

void CovIBUList::save(const char* filename) {
	ofstream* of=CovIBUListFile::write(filename,*this);
	of->close();
	delete of;
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

CovIBUListFactory::CovIBUListFactory(size_t n) : CovIUListFactory(n) {

}

CovIBUListFactory::CovIBUListFactory(const char* filename) : CovIUListFactory((size_t) 0 /* tmp*/) {
	ifstream* f = CovIBUListFile::read(filename, *this);
	f->close();
	delete f;
}

CovIBUListFactory::~CovIBUListFactory() {
}

void CovIBUListFactory::add_boundary(const IntervalVector& x) {
	add_unknown(x);
	boundary.push_back(nb_boxes()-1);
}

void CovIBUListFactory::build(CovIBUList& set) const {
	//assert(set.nb_inner == nb_inner);
	(size_t&) set.nb_boundary = boundary.size();
	(size_t&) set.nb_unknown  = set.CovIUList::nb_unknown - set.nb_boundary;
	set._IBU_status           = new CovIBUList::BoxStatus[set.size];
	set._IBU_boundary         = new IntervalVector*[set.nb_boundary];
	set._IBU_unknown          = new IntervalVector*[set.nb_unknown];

	for (int i=0; i<set.size; i++) {
		if (set.CovIUList::status(i)==CovIUList::INNER)
			set._IBU_status[i]=CovIBUList::INNER;
		else
			set._IBU_status[i]=CovIBUList::UNKNOWN; // by default
	}

	for (vector<unsigned int>::const_iterator it=boundary.begin(); it!=boundary.end(); ++it) {
		if (set._IBU_status[*it]==CovIBUList::INNER)
			ibex_error("[CovIBUListFactoy]: a box cannot be inner and boundary at the same time.");
		else
			set._IBU_status[*it]=CovIBUList::BOUNDARY;
	}

	size_t jbo=0; // count boundary boxes
	size_t juk=0; // count unknown boxes

	for (int i=0; i<set.size; i++) {
		if (set._IBU_status[i]==CovIBUList::BOUNDARY)
			set._IBU_boundary[jbo++]=(IntervalVector*) &set[i];
		else if (set._IBU_status[i]==CovIBUList::UNKNOWN)
			set._IBU_unknown[juk++]=(IntervalVector*) &set[i];
	}
	assert(jbo==set.nb_boundary);
	assert(juk==set.nb_unknown);
}

//----------------------------------------------------------------------------------------------------

ifstream* CovIBUListFile::read(const char* filename, CovIBUListFactory& factory) {

	ifstream* f = CovIUListFile::read(filename, factory);

	size_t nb_boundary = read_pos_int(*f);

	if (nb_boundary  > (factory.nb_boxes()-factory.nb_inner()))
		ibex_error("[CovIBUListFile]: number of boundary boxes > number of CovIUList unknown boxes!");

	for (size_t i=0; i<nb_boundary; i++) {
		unsigned int j=read_pos_int(*f);
		if (j>=factory.nb_boxes()) ibex_error("[CovIBUListFile]: invalid boundary box index.");
		factory.boundary.push_back(j); // checking that j is not an index of inner box is done in CovIBUListFactory::build
	}

	if (factory.boundary.size() != nb_boundary)
		ibex_error("[CovIUListFile]: number of boundary boxes does not match.");

	return f;
}


ofstream* CovIBUListFile::write(const char* filename, const CovIBUList& cov) {

	ofstream* f = CovIUListFile::write(filename, cov);

	write_int(*f, cov.nb_boundary);

	// TODO: a complete scan could be avoided?
	for (size_t i=0; i<cov.size; i++) {
		if (cov.status(i)==CovIBUList::BOUNDARY)
			write_int(*f, (uint32_t) i);
	}
	return f;
}


void CovIBUListFile::format(stringstream& ss, const string& title) {
	CovIUListFile::format(ss, title);

	ss <<   "- 1 line:   1 integer: the number Nb of boundary boxes (among N-Ni)\n"
			"- 1 line:   Nb integers: the indices of boxes that are boundary boxes.\n";
}

} // end namespace

