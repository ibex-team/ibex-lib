//============================================================================
//                                  I B E X
// File        : ibex_CovIUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 01, 2018
//============================================================================

#include "ibex_CovIUList.h"

#include <cassert>
#include <algorithm>

using namespace std;

namespace ibex {

CovIUList::CovIUList(size_t n) : CovList(n) {

}

CovIUList::CovIUList(const char* filename) : CovList((size_t) 0 /* tmp */) {
	stack<unsigned int> format_seq;
	ifstream* f = CovIUListFile::read(filename, *this, format_seq);
	f->close();
	delete f;
}

void CovIUList::save(const char* filename) const {
	stack<unsigned int> format_seq;
	ofstream* of=CovIUListFile::write(filename, *this, format_seq);
	of->close();
	delete of;
}

CovIUList::~CovIUList() {

}

void CovIUList::add_inner(const IntervalVector& x) {
	CovList::add(x);
	_IU_status.push_back(INNER);
	_IU_inner.push_back(&list.back());
}

void CovIUList::add_unknown(const IntervalVector& x) {
	CovList::add(x);
	_IU_status.push_back(UNKNOWN);
	_IU_unknown.push_back(&list.back());
}

void CovIUList::add(const IntervalVector& x) {
	add_unknown(x);
}

ostream& operator<<(ostream& os, const CovIUList& cov) {
	for (size_t i=0; i<cov.nb_inner(); i++) {
		os << " inner n°" << (i+1) << " = " << cov.inner(i) << endl;
	}

	for (size_t i=0; i<cov.nb_unknown(); i++) {
		os << " unknown n°" << (i+1) << " = " << cov.unknown(i) << endl;
	}

	return os;
}

//----------------------------------------------------------------------------------------------------

const unsigned int CovIUListFile::subformat_level = 2;

const unsigned int CovIUListFile::subformat_number = 0;

ifstream* CovIUListFile::read(const char* filename, CovIUList& cov, stack<unsigned int>& format_seq) {

	ifstream* f = CovListFile::read(filename, cov, format_seq);

	if (cov.size()==0) return f;

	size_t nb_inner;

	if (format_seq.empty() || format_seq.top()!=subformat_number) {
		nb_inner = 0;
	}
	else {
		format_seq.pop();

		nb_inner = read_pos_int(*f);

		if (nb_inner  > cov.size())
			ibex_error("[CovIUListFile]: number of inner boxes exceeds total!");
	}

	unsigned int indices[nb_inner];
	for (size_t i=0; i<nb_inner; i++) {
		indices[i]=read_pos_int(*f);
	}

	if (nb_inner>0)
		sort(indices,indices+nb_inner);

	size_t i2=0; // counter of inner boxes

	for (size_t i=0; i<cov.size(); i++) {
		if (i2<nb_inner && i==indices[i2]) {
			cov._IU_inner.push_back(cov.vec[i]);
			cov._IU_status.push_back(CovIUList::INNER);
			i2++;
		}
		else {
			cov._IU_unknown.push_back(cov.vec[i]);
			cov._IU_status.push_back(CovIUList::UNKNOWN);
		}
	}
	if (i2<nb_inner) ibex_error("[CovIUListFile]: invalid inner box index.");

	if (cov.nb_inner() != nb_inner)
		ibex_error("[CovIUListFile]: number of inner boxes does not match");

	return f;
}

ofstream* CovIUListFile::write(const char* filename, const CovIUList& cov, stack<unsigned int>& format_seq) {

	format_seq.push(subformat_number);

	ofstream* f = CovListFile::write(filename, cov, format_seq);

	write_int(*f, cov.nb_inner());

	// TODO: a complete scan could be avoided
	for (size_t i=0; i<cov.size(); i++) {
		if (cov.status(i)==CovIUList::INNER)
			write_int(*f, (uint32_t) i);
	}
	return f;
}

void CovIUListFile::format(stringstream& ss, const string& title, stack<unsigned int>& format_seq) {
	format_seq.push(subformat_number);

	CovListFile::format(ss, title, format_seq);

	ss
	<< space << " - 1 integer:     the number Ni of inner boxes (<= N)\n"
	<< "|     CovIUList     |"
	            " - Ni integers:   the indices of inner boxes\n"
	<< space << "                  (a subset of CovList boxes).\n"
	<< separator;
}

string CovIUListFile::format() {
	stringstream ss;
	stack<unsigned int> format_seq;
	format(ss, "CovIUList", format_seq);
	return ss.str();
}

} /* namespace ibex */
