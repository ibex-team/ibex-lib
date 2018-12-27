//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Dec 24, 2018
//============================================================================

#include "ibex_CovManifold.h"

#include <sstream>
#include <algorithm>

using namespace std;

namespace ibex {

const unsigned int CovManifold::subformat_level = 4;

const unsigned int CovManifold::subformat_number = 0;

CovManifold::CovManifold(size_t n, size_t m, size_t nb_ineq) : CovIBUList(n), m(m), nb_ineq(nb_ineq) {
}

CovManifold::CovManifold(const char* filename) : CovManifold(0,0,0 /* tmp */) {
	stack<unsigned int> format_seq;
	ifstream* f = CovManifold::read(filename, *this, format_seq);
	f->close();
	delete f;
}

void CovManifold::save(const char* filename) const {
	stack<unsigned int> format_seq;
	ofstream* of=CovManifold::write(filename, *this, format_seq);
	of->close();
	delete of;
}

void CovManifold::add(const IntervalVector& x) {
	add_unknown(x);
}

void CovManifold::add_inner(const IntervalVector& x) {
	CovIBUList::add_inner(x);
	_manifold_status.push_back(INNER);
}

void CovManifold::add_boundary(const IntervalVector& x) {
	CovIBUList::add_boundary(x);
	_manifold_status.push_back(BOUNDARY);
	_manifold_boundary.push_back(&list.back());
}

void CovManifold::add_unknown(const IntervalVector& x) {
	CovIBUList::add_unknown(x);
	_manifold_status.push_back(UNKNOWN);
}

void CovManifold::add_solution(const IntervalVector& existence, const IntervalVector& unicity) {
	if (m < n)
		ibex_error("CovManifold: a solution of under-constrained system requires \"VarSet\" structure (parameters/variables)");

	CovIBUList::add_boundary(existence);
	_manifold_solution.push_back(&list.back());
	_manifold_unicity.push_back(unicity);
	_manifold_status.push_back(SOLUTION);
}

void CovManifold::add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset) {
	CovIBUList::add_boundary(existence);
	_manifold_solution.push_back(&list.back());
	_manifold_unicity.push_back(unicity);
	_manifold_status.push_back(SOLUTION);
	_manifold_varset.push_back(varset);
}

ostream& operator<<(ostream& os, const CovManifold& manif) {

	for (size_t i=0; i<manif.nb_solution(); i++) {
		os << " solution n°" << (i+1) << " = " << manif.solution(i) << endl;
	}

	for (size_t i=0; i<manif.nb_inner(); i++) {
		os << " inner n°" << (i+1) << " = " << manif.inner(i) << endl;
	}

	for (size_t i=0; i<manif.nb_boundary(); i++) {
		os << " boundary n°" << (i+1) << " = " << manif.boundary(i) << endl;
	}

	for (size_t i=0; i<manif.nb_unknown(); i++) {
		os << " unknown n°" << (i+1) << " = " << manif.unknown(i) << endl;
	}

	return os;
}

ifstream* CovManifold::read(const char* filename, CovManifold& cov, stack<unsigned int>& format_seq) {

	ifstream* f = CovIBUList::read(filename, cov, format_seq);

	size_t nb_solution;

	if (format_seq.empty() || format_seq.top()!=subformat_number) {
		(size_t&) cov.m = 0;
		(size_t&) cov.nb_ineq = 0;
		nb_solution = 0;
	}
	else {
		format_seq.pop();
		(size_t&) cov.m = read_pos_int(*f);
		(size_t&) cov.nb_ineq = read_pos_int(*f);
		nb_solution = read_pos_int(*f);
	}

	if (nb_solution > cov.CovIBUList::nb_boundary())
		ibex_error("[CovManifold]: number of solutions > number of CovIBUList boundary boxes");

	unsigned int indices[nb_solution];
	for (size_t i=0; i<nb_solution; i++) {
		indices[i]=read_pos_int(*f);
		if (cov.m < cov.n)
			cov._manifold_varset.push_back(read_varset(*f, cov.n, cov.m));
		cov._manifold_unicity.push_back(read_box(*f, cov.n));
	}

	if (nb_solution>0)
		sort(indices,indices+nb_solution);


	size_t i2=0; // counter of solution boxes

	for (size_t i=0; i<cov.size(); i++) {

		if (i2<nb_solution && i==indices[i2]) {
			if (!cov.CovIBUList::is_boundary(i))
				ibex_error("[CovManifold]: a solution box must be a CovIBUList boundary box.");
			cov._manifold_solution.push_back(cov.vec[i]);
			cov._manifold_status.push_back(CovManifold::SOLUTION);
			i2++;
		} else {
			switch(cov.CovIBUList::status(i)) {
			case CovIBUList::INNER :
				cov._manifold_status.push_back(CovManifold::INNER);
				break;
			case CovIBUList::UNKNOWN :
				cov._manifold_status.push_back(CovManifold::UNKNOWN);
				break;
			default :
				cov._manifold_boundary.push_back(cov.vec[i]);
				cov._manifold_status.push_back(CovManifold::BOUNDARY);
			}
		}
	}

	if (i2<nb_solution) ibex_error("[CovManifold]: invalid solution box index.");

	if (cov.nb_solution() != nb_solution)
		ibex_error("[CovManifoldFiile]: number of solution boxes does not match.");

	return f;
}

ofstream* CovManifold::write(const char* filename, const CovManifold& cov, stack<unsigned int>& format_seq) {

	format_seq.push(subformat_number);

	ofstream* f = CovIBUList::write(filename, cov, format_seq);

	write_int(*f, cov.m);
	write_int(*f, cov.nb_ineq);
	write_int(*f, cov.nb_solution());

	// TODO: a complete scan could be avoided?
	int j=0;
	for (size_t i=0; i<cov.size(); i++) {
		if (cov.status(i)==CovManifold::SOLUTION) {
			write_int(*f, (uint32_t) i);
			if (cov.m < cov.n) write_varset(*f, cov.varset(j));
			write_box(*f, cov.unicity(j));
			j++;
		}
	}
	return f;
}

VarSet CovManifold::read_varset(ifstream& f, size_t n, size_t m) {

	BitSet params(n);

	for (unsigned int j=0; j<n-m; j++) {
		unsigned int v=read_pos_int(f);
		if (v>n) {
			ibex_error("[CovManifold]: bad input file (bad parameter index)");
		}
		params.add(v); // index starting from 1 in the raw format
	}

	return VarSet(n,params,false);
}

void CovManifold::write_varset(ofstream& f, const VarSet& varset) {
	for (int i=0; i<varset.nb_param; i++)
		write_int(f, varset.param(i));
}

void CovManifold::format(stringstream& ss, const string& title, stack<unsigned int>& format_seq) {
	format_seq.push(subformat_number);

	CovIBUList::format(ss, title, format_seq);

	ss
	<< space << " - 1 integer:     the number m of equalities\n"
	<< space << " - 1 integer:     the number of inequalities\n"
	<< space << " - 1 integer:     the number Ns of solution boxes (<= Nb)\n"
	<< space << " - Ns solutions:  each solution is the following sequence:\n"
	<< "|    CovManifold    |" <<
	            "                  - 1 integer: the index of the solution\n"
	<< space << "                    (belongs to CovIBUList boundary boxes);\n"
	<< space << "                  - [if m<n] n-m integers: the indices of\n"
	<< space << "                    parameters in the parametric proofs;\n"
	<< space << "                  - 2*n real values: the unicity box in the\n"
	<< space << "                    proof (lb(x1), ub(x1),..., ub(xn));\n"
	<< separator;
}

string CovManifold::format() {
	stringstream ss;
	stack<unsigned int> format_seq;
	format(ss, "CovManifold", format_seq);
	return ss.str();
}


} // end namespace

