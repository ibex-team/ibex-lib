//============================================================================
//                                  I B E X
// File        : ibex_CovIUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 24, 2018
//============================================================================

#include "ibex_CovIUList.h"

#include <sstream>
#include <cassert>
#include <algorithm>

using namespace std;

namespace ibex {

const unsigned int CovIUList::FORMAT_VERSION = 1;

const unsigned int CovIUList::subformat_level = 2;

const unsigned int CovIUList::subformat_number = 0;

CovIUList::CovIUList(size_t n) : CovList(n) {

}

CovIUList::CovIUList(const char* filename) : CovList((size_t) 0 /* tmp */) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = CovIUList::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

void CovIUList::save(const char* filename) const {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=CovIUList::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
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

ifstream* CovIUList::read(const char* filename, CovIUList& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	ifstream* f = CovList::read(filename, cov, format_id, format_version);

	if (cov.size()==0) return f;

	size_t nb_inner;

	if (format_id.empty() || format_id.top()!=subformat_number || format_version.top()!=FORMAT_VERSION) {
		nb_inner = 0;
	}
	else {
		format_id.pop();
		format_version.pop();

		nb_inner = read_pos_int(*f);

		if (nb_inner  > cov.size())
			ibex_error("[CovIUList]: number of inner boxes exceeds total!");
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
	if (i2<nb_inner) ibex_error("[CovIUList]: invalid inner box index.");

	if (cov.nb_inner() != nb_inner)
		ibex_error("[CovIUList]: number of inner boxes does not match");

	return f;
}

ofstream* CovIUList::write(const char* filename, const CovIUList& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ofstream* f = CovList::write(filename, cov, format_id, format_version);

	write_pos_int(*f, cov.nb_inner());

	// TODO: a complete scan could be avoided
	for (size_t i=0; i<cov.size(); i++) {
		if (cov.status(i)==CovIUList::INNER)
			write_pos_int(*f, (uint32_t) i);
	}
	return f;
}

void CovIUList::format(stringstream& ss, const string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {
	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	CovList::format(ss, title, format_id, format_version);

	ss
	<< space << " - 1 integer:     the number Ni of inner boxes (<= N)\n"
	<< "|     CovIUList     |"
	            " - Ni integers:   the indices of inner boxes\n"
	<< space << "                  (a subset of CovList boxes).\n"
	<< separator;
}

string CovIUList::format() {
	stringstream ss;
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	format(ss, "CovIUList", format_id, format_version);
	return ss.str();
}

} /* namespace ibex */
