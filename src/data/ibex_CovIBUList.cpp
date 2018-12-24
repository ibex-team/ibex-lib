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

#include <algorithm>

using namespace std;

namespace ibex {

CovIBUList::CovIBUList(size_t n) : CovIUList(n) {
}

CovIBUList::CovIBUList(const char* filename) : CovIUList((size_t) 0 /* tmp */) {
	stack<unsigned int> format_seq;
	ifstream* f = CovIBUListFile::read(filename, *this, format_seq);
	f->close();
	delete f;
}

void CovIBUList::save(const char* filename) const	 {
	stack<unsigned int> format_seq;
	ofstream* of=CovIBUListFile::write(filename, *this, format_seq);
	of->close();
	delete of;
}

CovIBUList::~CovIBUList() {

}

void CovIBUList::add(const IntervalVector& x) {
	add_unknown(x);
}

void CovIBUList::add_inner(const IntervalVector& x) {
	CovIUList::add_inner(x);
	_IBU_status.push_back(INNER);
}

void CovIBUList::add_boundary(const IntervalVector& x) {
	CovIUList::add_unknown(x);
	_IBU_status.push_back(BOUNDARY);
	_IBU_boundary.push_back(&list.back());
}

void CovIBUList::add_unknown(const IntervalVector& x) {
	CovIUList::add_unknown(x);
	_IBU_status.push_back(UNKNOWN);
	_IBU_unknown.push_back(&list.back());
}

ostream& operator<<(ostream& os, const CovIBUList& cov) {

	for (size_t i=0; i<cov.nb_inner(); i++) {
		os << " inner n°" << (i+1) << " = " << cov.inner(i) << endl;
	}

	for (size_t i=0; i<cov.nb_boundary(); i++) {
		os << " boundary n°" << (i+1) << " = " << cov.boundary(i) << endl;
	}

	for (size_t i=0; i<cov.nb_unknown(); i++) {
		os << " unknown n°" << (i+1) << " = " << cov.unknown(i) << endl;
	}

	return os;
}

//----------------------------------------------------------------------------------------------------

const unsigned int CovIBUListFile::subformat_level = 3;

const unsigned int CovIBUListFile::subformat_number = 0;

ifstream* CovIBUListFile::read(const char* filename, CovIBUList& cov, stack<unsigned int>& format_seq) {

	ifstream* f = CovIUListFile::read(filename, cov, format_seq);

	size_t nb_boundary;

	if (format_seq.empty() || format_seq.top()!=subformat_number) {
		nb_boundary = 0;
	}
	else {
		format_seq.pop();
		nb_boundary = read_pos_int(*f);
	}

	if (nb_boundary  > cov.CovIUList::nb_unknown())
		ibex_error("[CovIBUListFile]: number of boundary boxes > number of CovIUList unknown boxes!");

	unsigned int indices[nb_boundary];
	for (size_t i=0; i<nb_boundary; i++) {
		indices[i]=read_pos_int(*f);
	}

	if (nb_boundary>0)
		sort(indices,indices+nb_boundary);

	size_t i2=0; // counter of boundary boxes

	for (size_t i=0; i<cov.size(); i++) {

		if (i2<nb_boundary && i==indices[i2]) {
			if (!cov.CovIUList::is_unknown(i))
				ibex_error("[CovIBUListFile]: a boundary box must be a CovIUList unknown box.");
			cov._IBU_boundary.push_back(cov.vec[i]);
			cov._IBU_status.push_back(CovIBUList::BOUNDARY);
			i2++;
		} else {
			switch(cov.CovIUList::status(i)) {
			case CovIUList::INNER :
				cov._IBU_status.push_back(CovIBUList::INNER);
				break;
			default :
				cov._IBU_unknown.push_back(cov.vec[i]);
				cov._IBU_status.push_back(CovIBUList::UNKNOWN);
			}
		}
	}

	if (i2<nb_boundary) ibex_error("[CovIBUListFile]: invalid boundary box index.");

	if (cov.nb_boundary() != nb_boundary)
		ibex_error("[CovIBUListFile]: number of boundary boxes does not match.");

	return f;
}


ofstream* CovIBUListFile::write(const char* filename, const CovIBUList& cov, stack<unsigned int>& format_seq) {

	format_seq.push(subformat_number);

	ofstream* f = CovIUListFile::write(filename, cov, format_seq);

	write_int(*f, cov.nb_boundary());

	// TODO: a complete scan could be avoided?
	for (size_t i=0; i<cov.size(); i++) {
		if (cov.status(i)==CovIBUList::BOUNDARY)
			write_int(*f, (uint32_t) i);
	}
	return f;
}

void CovIBUListFile::format(stringstream& ss, const string& title, stack<unsigned int>& format_seq) {
	format_seq.push(subformat_number);

	CovIUListFile::format(ss, title, format_seq);

	ss
	<< space << " - 1 integer:     the number Nb of boundary boxes (<= N-Ni)\n"
	<< "|     CovIBUList    |"
	            " - Nb integers:   the indices of boundary boxes\n"
	<< space << "                  (a subset of CovIUList unknown boxes)\n"
	<< separator;
}

string CovIBUListFile::format() {
	stringstream ss;
	stack<unsigned int> format_seq;
	format(ss, "CovIBUList", format_seq);
	return ss.str();
}

} // end namespace

