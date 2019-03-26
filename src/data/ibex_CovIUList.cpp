//============================================================================
//                                  I B E X
// File        : ibex_CovIUList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Feb 28, 2019
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

CovIUList::CovIUList(size_t n) : CovList(n), data(new Data()), own_data(true) {

}

CovIUList::CovIUList(const char* filename) : CovIUList((size_t) 0 /* tmp */) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = CovIUList::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

CovIUList::CovIUList(const Cov& cov, bool copy) : CovList(cov, copy) {
	const CovIUList* covIUlist = dynamic_cast<const CovIUList*>(&cov);

	if (covIUlist) {
		if (copy) {
			data = new Data();
			data->_IU_inner   = covIUlist->data->_IU_inner;
			data->_IU_status  = covIUlist->data->_IU_status;
			data->_IU_unknown = covIUlist->data->_IU_unknown;
			own_data = true;
		} else {
			data = covIUlist->data;
			own_data = false;
		}
	} else {
		data = new Data();
		for (size_t i=0; i<size(); i++) {
			data->_IU_status.push_back(UNKNOWN);
			data->_IU_unknown.push_back(i);
		}
		own_data = true;
	}
}

void CovIUList::save(const char* filename) const {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=CovIUList::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
}

CovIUList::~CovIUList() {
	if (own_data) {
		delete data;
	}
}

void CovIUList::add_inner(const IntervalVector& x) {
	CovList::add(x);
	data->_IU_status.push_back(INNER);
	data->_IU_inner.push_back(size()-1);
}

void CovIUList::add_unknown(const IntervalVector& x) {
	CovList::add(x);
	data->_IU_status.push_back(UNKNOWN);
	data->_IU_unknown.push_back(size()-1);
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

		for (size_t i=0; i<nb_inner; i++) {
			uint32_t j=read_pos_int(*f);
			if (!cov.data->_IU_inner.empty()) { // check ordering
				if  (j<cov.data->_IU_inner.back())
					ibex_error("[CovIUList]: indices of inner boxes are not in increasing order.");
				if (j==cov.data->_IU_inner.back())
					ibex_error("[CovIUList]: duplicated index of inner box.");
			}
			cov.data->_IU_inner.push_back(j);
		}
	}

	vector<size_t>::const_iterator it=cov.data->_IU_inner.begin(); // iterator of inner boxes

	for (size_t i=0; i<cov.size(); i++) {
		if (it!=cov.data->_IU_inner.end() && i==*it) {
			cov.data->_IU_status.push_back(CovIUList::INNER);
			++it;
		}
		else {
			cov.data->_IU_unknown.push_back(i);
			cov.data->_IU_status.push_back(CovIUList::UNKNOWN);
		}
	}
	if (it!=cov.data->_IU_inner.end()) ibex_error("[CovIUList]: invalid inner box index.");

	return f;
}

ofstream* CovIUList::write(const char* filename, const CovIUList& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ofstream* f = CovList::write(filename, cov, format_id, format_version);

	write_pos_int(*f, cov.nb_inner());

	for (vector<size_t>::const_iterator it=cov.data->_IU_inner.begin(); it!=cov.data->_IU_inner.end(); ++it) {
		assert(*it<numeric_limits<uint32_t>::max());
		write_pos_int(*f, (uint32_t) *it);
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
	            " - Ni integers:   the indices of inner boxes in increasing\n"
	<< space << "                  order (a subset of CovList boxes).\n"
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
