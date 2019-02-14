//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Feb 13, 2019
//============================================================================

#include "ibex_CovIBUList.h"

#include <sstream>
#include <algorithm>

using namespace std;

namespace ibex {

const unsigned int CovIBUList::FORMAT_VERSION = 1;

const unsigned int CovIBUList::subformat_level = 3;

const unsigned int CovIBUList::subformat_number = 0;

CovIBUList::CovIBUList(size_t n, BoundaryType boundary_type) : CovIUList(n), boundary_type(boundary_type) {
}

CovIBUList::CovIBUList(const char* filename) : CovIUList((size_t) 0 /* tmp */), boundary_type(INNER_PT /* by default */) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = CovIBUList::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

void CovIBUList::save(const char* filename) const	 {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=CovIBUList::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
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
	_IBU_boundary.push_back(list.size()-1);
}

void CovIBUList::add_unknown(const IntervalVector& x) {
	CovIUList::add_unknown(x);
	_IBU_status.push_back(UNKNOWN);
	_IBU_unknown.push_back(list.size()-1);
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

ifstream* CovIBUList::read(const char* filename, CovIBUList& cov, stack<unsigned int>& format_id, stack<unsigned int>& format_version) {

	ifstream* f = CovIUList::read(filename, cov, format_id, format_version);

	size_t nb_boundary;

	if (format_id.empty() || format_id.top()!=subformat_number || format_version.top()!=FORMAT_VERSION) {
		nb_boundary = 0;
	}
	else {
		format_id.pop();
		format_version.pop();

		unsigned int _boundary_type = read_pos_int(*f);

		switch(_boundary_type) {
		case 0 :  (BoundaryType&) cov.boundary_type = INNER_PT; break;
		case 1 :  (BoundaryType&) cov.boundary_type = INNER_AND_OUTER_PT; break;
		default : ibex_error("[CovIBUList]: unknown boundary type identifier.");
		}

		nb_boundary = read_pos_int(*f);

		if (nb_boundary  > cov.CovIUList::nb_unknown())
			ibex_error("[CovIBUList]: number of boundary boxes > number of CovIUList unknown boxes!");

		for (size_t i=0; i<nb_boundary; i++) {
			uint32_t j=read_pos_int(*f);
			if (!cov._IBU_boundary.empty()) { // check ordering
				if (j<cov._IBU_boundary.back())
					ibex_error("[CovIBUList]: indices of boundary boxes are not in increasing order.");
				if (j==cov._IBU_boundary.back())
					ibex_error("[CovIBUList]: duplicated index of boundary box.");
			}
			cov._IBU_boundary.push_back(j);
		}
	}

	vector<size_t>::const_iterator it=cov._IBU_boundary.begin(); // iterator of boundary boxes

	for (size_t i=0; i<cov.size(); i++) {

		if (it!=cov._IBU_boundary.end() && i==*it) {
			if (!cov.CovIUList::is_unknown(i))
				ibex_error("[CovIBUList]: a boundary box must be a CovIUList unknown box.");
			cov._IBU_status.push_back(CovIBUList::BOUNDARY);
			++it;
		} else {
			switch(cov.CovIUList::status(i)) {
			case CovIUList::INNER :
				cov._IBU_status.push_back(CovIBUList::INNER);
				break;
			default :
				cov._IBU_unknown.push_back(i);
				cov._IBU_status.push_back(CovIBUList::UNKNOWN);
			}
		}
	}

	if (it!=cov._IBU_boundary.end()) ibex_error("[CovIBUList]: invalid boundary box index.");

	return f;
}

ofstream* CovIBUList::write(const char* filename, const CovIBUList& cov, stack<unsigned int>& format_id, stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ofstream* f = CovIUList::write(filename, cov, format_id, format_version);

	write_pos_int(*f, cov.boundary_type==INNER_PT? 0 : 1);

	write_pos_int(*f, cov.nb_boundary());

	for (vector<size_t>::const_iterator it=cov._IBU_boundary.begin(); it!=cov._IBU_boundary.end(); ++it) {
		assert(*it<numeric_limits<uint32_t>::max());
		write_pos_int(*f, (uint32_t) *it);
	}

	return f;
}

void CovIBUList::format(stringstream& ss, const string& title, stack<unsigned int>& format_id, stack<unsigned int>& format_version) {
	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	CovIUList::format(ss, title, format_id, format_version);

	ss
	<< space << " - 1 integer:     the type of boundary boxes:\n"
	<< space << "                  0=\"inner\": a boundary box contains at \n"
	<< space << "		             least an inner point.\n"
	<< space << "                  1=\"inner and outer\":  boundary box contains\n"
	<< space << "                    at least one inner and one outer point.\n"
	<< space << " - 1 integer:     the number Nb of boundary boxes (<= N-Ni)\n"
	<< "|     CovIBUList    |"
	            " - Nb integers:   the indices of boundary boxes in increasing order\n"
	<< space << "                  (a subset of CovIUList unknown boxes)\n"
	<< separator;
}

string CovIBUList::format() {
	stringstream ss;
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	format(ss, "CovIBUList", format_id, format_version);
	return ss.str();
}

} // end namespace

