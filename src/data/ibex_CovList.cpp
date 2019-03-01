//============================================================================
//                                  I B E X
// File        : ibex_CovList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Feb 28, 2019
//============================================================================

#include "ibex_CovList.h"

#include <sstream>
#include <cassert>

using namespace std;

namespace ibex {

const unsigned int CovList::FORMAT_VERSION = 1;

const unsigned int CovList::subformat_level = 1;

const unsigned int CovList::subformat_number = 0;

CovList::CovList(size_t n) : Cov(n), data(new Data()), own_data(true) {

}

CovList::CovList(const char* filename) : CovList((size_t) 0 /*tmp*/) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = CovList::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

CovList::CovList(const Cov& cov, bool copy) : Cov(cov.n) {
	const CovList* covlist = dynamic_cast<const CovList*>(&cov);

	if (covlist) {
		if (copy) {
			data = new Data();
			data->lst  = covlist->data->lst;
			for (list<IntervalVector>::iterator it=data->lst.begin(); it!=data->lst.end(); ++it)
				data->vec.push_back(&(*it));
			own_data = true;
		} else {
			data = covlist->data;
			own_data = false;
		}
	} else {
		data = new Data();
		// nothing to add
		own_data = true;
	}
}

void CovList::save(const char* filename) const {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=CovList::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
}

CovList::~CovList() {
	if (own_data) {
		delete data;
	}
}

void CovList::add(const IntervalVector& x) {
//	if (list.empty())
//		n=x.size();
//	else {
		if (n!=x.size())
			ibex_error("[CovList] boxes must have all the same size.");
//	}
	data->lst.push_back(x);
	data->vec.push_back(&data->lst.back());
}

ostream& operator<<(ostream& os, const CovList& cov) {

	for (size_t i=0; i<cov.size(); i++) {
		os << " " << cov[i] << endl;
	}
	os << endl;

	return os;
}

IntervalVector CovList::read_box(ifstream& f, size_t n) {

	IntervalVector box(n);

	for (unsigned int j=0; j<n; j++) {
		double lb=read_double(f);
		double ub=read_double(f);
		box[j]=Interval(lb,ub);
	}

	return box;
}

void CovList::write_box(ofstream& f, const IntervalVector& box) {
	for (int i=0; i<box.size(); i++) {
		write_double(f,box[i].lb());
		write_double(f,box[i].ub());
	}
}

ifstream* CovList::read(const char* filename, CovList& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	ifstream* f = Cov::read(filename, cov, format_id, format_version);

	if (format_id.empty() || format_id.top()!=subformat_number || format_version.top()!=FORMAT_VERSION) return f;
	else {
		format_id.pop();
		format_version.pop();
	}

	size_t size = read_pos_int(*f);

	for (unsigned int i=0; i<size; i++) {
		cov.CovList::add(read_box(*f, cov.n));
	}

	return f;
}

ofstream* CovList::write(const char* filename, const CovList& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ofstream* f = Cov::write(filename, cov, format_id, format_version);

	write_pos_int(*f, cov.size());

	for (std::list<IntervalVector>::const_iterator it=cov.data->lst.begin(); it!=cov.data->lst.end(); ++it) {
		write_box(*f, *it);
	}

	return f;
}

void CovList::format(stringstream& ss, const string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {
	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	Cov::format(ss, title, format_id, format_version);

	ss      << space << " - 1 integer:     the total number N of boxes\n"
			<< "|      CovList      |" << " - N boxes:       a box is a sequence of 2*n real values:\n"
			<< space << "                  lb(x1), ub(x1),...,lb(xn), ub(xn)\n"
			<< separator;
}

string CovList::format() {
	stringstream ss;
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	format(ss, "CovList", format_id, format_version);
	return ss.str();
}


} /* namespace ibex */
