//============================================================================
//                                  I B E X
// File        : ibex_CovList.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 07, 2018
// Last update : Dec 01, 2018
//============================================================================

#include "ibex_CovList.h"

#include <cassert>

using namespace std;

namespace ibex {

CovList::CovList(const CovListFactory& fac) : Cov(fac), size(0), list(NULL) {
	fac.build(*this);
}


CovList::CovList(const char* filename) : CovList(CovListFactory(filename)) {

}

void CovList::save(const char* filename) {
	stack<unsigned int> format_seq;
	ofstream* of=CovListFile::write(filename, *this, format_seq);
	of->close();
	delete of;
}

CovList::~CovList() {
	if (list) delete[] list;
}

//----------------------------------------------------------------------------------------------------

CovListFactory::CovListFactory(size_t n) : CovFactory(n) {

}

CovListFactory::CovListFactory(const char* filename) : CovFactory((size_t) 0 /* tmp*/) {
	stack<unsigned int> format_seq;
	ifstream* f = CovListFile::read(filename, *this, format_seq);
	f->close();
	delete f;
}

CovListFactory::~CovListFactory() {

}

void CovListFactory::add(const IntervalVector& x) {
	if (boxes.empty())
		n=x.size();
	else {
		if (n!=x.size())
			ibex_error("[CovListFactory] boxes must have all the same size.");
	}
	boxes.push_back(x);
}

void CovListFactory::build(CovList& l) const {
	assert(l.n == n);
	(size_t&) l.size = boxes.size();
	if (l.size>0) {
		l.list = new IntervalVector[boxes.size()];
		size_t i=0;
		for (vector<IntervalVector>::const_iterator it=boxes.begin(); it!=boxes.end(); ++it, i++) {
			l.list[i].resize(n);
			l.list[i]=*it;
		}
	}
}

//----------------------------------------------------------------------------------------------------

const unsigned int CovListFile::subformat_level = 1;

const unsigned int CovListFile::subformat_number = 0;

ifstream* CovListFile::read(const char* filename, CovListFactory& factory, stack<unsigned int>& format_seq) {

	ifstream* f = CovFile::read(filename, factory, format_seq);

	if (format_seq.empty() || format_seq.top()!=subformat_number) return f;
	else format_seq.pop();

	size_t size = read_pos_int(*f);

	for (unsigned int i=0; i<size; i++) {
		factory.add(read_box(*f, factory.n));
	}

	return f;
}

ofstream* CovListFile::write(const char* filename, const CovList& cov, stack<unsigned int>& format_seq) {

	format_seq.push(subformat_number);

	ofstream* f = CovFile::write(filename, cov, format_seq);

	write_int(*f, cov.size);

	for (unsigned int i=0; i<cov.size; i++) {
		write_box(*f, cov[i]);
	}

	return f;
}

IntervalVector CovListFile::read_box(ifstream& f, size_t n) {

	IntervalVector box(n);

	for (unsigned int j=0; j<n; j++) {
		double lb=read_double(f);
		double ub=read_double(f);
		box[j]=Interval(lb,ub);
	}

	return box;
}

void CovListFile::write_box(ofstream& f, const IntervalVector& box) {
	for (int i=0; i<box.size(); i++) {
		write_double(f,box[i].lb());
		write_double(f,box[i].ub());
	}
}

void CovListFile::format(stringstream& ss, const string& title, stack<unsigned int>& format_seq) {
	format_seq.push(subformat_number);

	CovFile::format(ss, title, format_seq);

	ss      << space << " - 1 integer:     the total number N of boxes\n"
			<< "|      CovList      |" << " - N boxes:       a box is a sequence of 2*n real values:\n"
			<< space << "                  lb(x1), ub(x1),...,lb(xn), ub(xn)\n"
			<< separator;
}

string CovListFile::format() {
	stringstream ss;
	stack<unsigned int> format_seq;
	format(ss, "CovList", format_seq);
	return ss.str();
}


} /* namespace ibex */
