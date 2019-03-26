//============================================================================
//                                  I B E X
// File        : ibex_CovIBUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Feb 28, 2019
//============================================================================

#include "ibex_CovIBUList.h"

#include <sstream>
#include <algorithm>

using namespace std;

namespace ibex {

const unsigned int CovIBUList::FORMAT_VERSION = 1;

const unsigned int CovIBUList::subformat_level = 3;

const unsigned int CovIBUList::subformat_number = 0;

CovIBUList::CovIBUList(size_t n, BoundaryType boundary_type) : CovIUList(n), data(new Data()), own_data(true) {
	data->_IBU_boundary_type = boundary_type;
}

CovIBUList::CovIBUList(const char* filename) : CovIBUList((size_t) 0, INNER_PT /* tmp */) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = CovIBUList::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

CovIBUList::CovIBUList(const Cov& cov, bool copy) : CovIUList(cov, copy) {
	const CovIBUList* covIBUlist = dynamic_cast<const CovIBUList*>(&cov);

	if (covIBUlist) {
		if (copy) {
			data = new Data();
			data->_IBU_boundary_type = covIBUlist->data->_IBU_boundary_type;
			data->_IBU_status   = covIBUlist->data->_IBU_status;
			data->_IBU_boundary = covIBUlist->data->_IBU_boundary;
			data->_IBU_unknown  = covIBUlist->data->_IBU_unknown;
			own_data = true;
		} else {
			data = covIBUlist->data;
			own_data = false;
		}
	} else {
		data = new Data();
		data->_IBU_boundary_type = INNER_PT; /* by default */
		for (size_t i=0; i<size(); i++) {
			switch(CovIUList::status(i)) {
			case CovIUList::INNER :
				data->_IBU_status.push_back(CovIBUList::INNER);
				break;
			default :
				data->_IBU_unknown.push_back(i);
				data->_IBU_status.push_back(CovIBUList::UNKNOWN);
			}
		}
		own_data = true;
	}
}

void CovIBUList::save(const char* filename) const	 {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=CovIBUList::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
}

CovIBUList::~CovIBUList() {
	if (own_data) {
		delete data;
	}
}

void CovIBUList::add(const IntervalVector& x) {
	add_unknown(x);
}

void CovIBUList::add_inner(const IntervalVector& x) {
	CovIUList::add_inner(x);
	data->_IBU_status.push_back(INNER);
}

void CovIBUList::add_boundary(const IntervalVector& x) {
	CovIUList::add_unknown(x);
	data->_IBU_status.push_back(BOUNDARY);
	data->_IBU_boundary.push_back(size()-1);
}

void CovIBUList::add_unknown(const IntervalVector& x) {
	CovIUList::add_unknown(x);
	data->_IBU_status.push_back(UNKNOWN);
	data->_IBU_unknown.push_back(size()-1);
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
		case 0 :  (BoundaryType&) cov.data->_IBU_boundary_type = INNER_PT; break;
		case 1 :  (BoundaryType&) cov.data->_IBU_boundary_type = INNER_AND_OUTER_PT; break;
		default : ibex_error("[CovIBUList]: unknown boundary type identifier.");
		}

		nb_boundary = read_pos_int(*f);

		if (nb_boundary  > cov.CovIUList::nb_unknown())
			ibex_error("[CovIBUList]: number of boundary boxes > number of CovIUList unknown boxes!");

		for (size_t i=0; i<nb_boundary; i++) {
			uint32_t j=read_pos_int(*f);
			if (!cov.data->_IBU_boundary.empty()) { // check ordering
				if (j<cov.data->_IBU_boundary.back())
					ibex_error("[CovIBUList]: indices of boundary boxes are not in increasing order.");
				if (j==cov.data->_IBU_boundary.back())
					ibex_error("[CovIBUList]: duplicated index of boundary box.");
			}
			cov.data->_IBU_boundary.push_back(j);
		}
	}

	vector<size_t>::const_iterator it=cov.data->_IBU_boundary.begin(); // iterator of boundary boxes

	for (size_t i=0; i<cov.size(); i++) {

		if (it!=cov.data->_IBU_boundary.end() && i==*it) {
			if (!cov.CovIUList::is_unknown(i))
				ibex_error("[CovIBUList]: a boundary box must be a CovIUList unknown box.");
			cov.data->_IBU_status.push_back(CovIBUList::BOUNDARY);
			++it;
		} else {
			switch(cov.CovIUList::status(i)) {
			case CovIUList::INNER :
				cov.data->_IBU_status.push_back(CovIBUList::INNER);
				break;
			default :
				cov.data->_IBU_unknown.push_back(i);
				cov.data->_IBU_status.push_back(CovIBUList::UNKNOWN);
			}
		}
	}

	if (it!=cov.data->_IBU_boundary.end()) ibex_error("[CovIBUList]: invalid boundary box index.");

	return f;
}

ofstream* CovIBUList::write(const char* filename, const CovIBUList& cov, stack<unsigned int>& format_id, stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ofstream* f = CovIUList::write(filename, cov, format_id, format_version);

	write_pos_int(*f, cov.boundary_type()==INNER_PT? 0 : 1);

	write_pos_int(*f, cov.nb_boundary());

	for (vector<size_t>::const_iterator it=cov.data->_IBU_boundary.begin(); it!=cov.data->_IBU_boundary.end(); ++it) {
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
	<< space << "                  -0=a boundary box contains at least\n"
	<< space << "                   an inner point.\n"
	<< space << "                  -1=a boundary box contains at least\n"
	<< space << "                   one inner and one outer point.\n"
	<< space << " - 1 integer:     the number Nb of boundary boxes (<= N-Ni)\n"
	<< "|     CovIBUList    |"
	            " - Nb integers:   indices of boundary boxes in increasing\n"
	<< space << "                  order (subset of CovIUList unknown boxes)\n"
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

