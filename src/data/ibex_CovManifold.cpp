//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Feb 13, 2019
//============================================================================

#include "ibex_CovManifold.h"

#include <sstream>
#include <algorithm>

using namespace std;

namespace ibex {

const unsigned int CovManifold::FORMAT_VERSION = 1;

const unsigned int CovManifold::subformat_level = 4;

const unsigned int CovManifold::subformat_number = 0;

CovManifold::CovManifold(size_t n, size_t m, size_t nb_ineq, BoundaryType boundary_type) :
		CovIBUList(n, INNER_AND_OUTER_PT), m(m), nb_ineq(nb_ineq), boundary_type(boundary_type) {
}

CovManifold::CovManifold(const char* filename) : CovManifold(0,0,0 /* tmp */, EQU_ONLY /* by default */) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = CovManifold::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

void CovManifold::save(const char* filename) const {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=CovManifold::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
}

void CovManifold::add(const IntervalVector& x) {
	add_unknown(x);
}

void CovManifold::add_inner(const IntervalVector& x) {
	if (m>0)
		ibex_error("[CovManifold] inner boxes not allowed with equalities");
	CovIBUList::add_inner(x);
	_manifold_status.push_back(SOLUTION);
}

void CovManifold::add_boundary(const IntervalVector& x) {
	switch (boundary_type) {
	case EQU_ONLY  : CovIBUList::add_unknwon(x); break;
	case FULL_RANK : CovIBUList::add_unknwon(x); break;
	case HALF_BALL : CovIBUList::add_boundary(x); break;
	default        : assert(false);
	}

	_manifold_status.push_back(BOUNDARY);
	_manifold_boundary.push_back(list.size()-1);
}

void CovManifold::add_unknown(const IntervalVector& x) {
	CovIBUList::add_unknown(x);
	_manifold_status.push_back(UNKNOWN);
}

void CovManifold::add_solution(const IntervalVector& existence, const IntervalVector& unicity) {
	if (m==0)
		ibex_error("[CovManifold]: solution boxes not allowed without equalities");
	if (m > 0 && m < n)
		ibex_error("[CovManifold]: a solution of under-constrained system requires \"VarSet\" structure (parameters/variables)");

	CovIBUList::add_boundary(existence);
	_manifold_solution.push_back(list.size()-1);
	_manifold_unicity.push_back(unicity);
	_manifold_status.push_back(SOLUTION);
}

void CovManifold::add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset) {
	if (m==0)
			ibex_error("[CovManifold]: solution boxes not allowed without equalities");

	CovIBUList::add_boundary(existence);
	_manifold_solution.push_back(list.size()-1);
	_manifold_unicity.push_back(unicity);
	_manifold_status.push_back(SOLUTION);
	_manifold_varset.push_back(varset);
}

ostream& operator<<(ostream& os, const CovManifold& manif) {

	for (size_t i=0; i<manif.nb_solution(); i++) {
		os << " solution n°" << (i+1) << " = " << manif.solution(i) << endl;
	}

	for (size_t i=0; i<manif.nb_boundary(); i++) {
		os << " boundary n°" << (i+1) << " = " << manif.boundary(i) << endl;
	}

	for (size_t i=0; i<manif.nb_unknown(); i++) {
		os << " unknown n°" << (i+1) << " = " << manif.unknown(i) << endl;
	}

	return os;
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
		write_pos_int(f, varset.param(i));
}

ifstream* CovManifold::read(const char* filename, CovManifold& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	ifstream* f = CovIBUList::read(filename, cov, format_id, format_version);

	size_t nb_solution, nb_boundary;

	if (format_id.empty() || format_id.top()!=subformat_number || format_version.top()!=FORMAT_VERSION) {
		(size_t&) cov.m = 0;
		(size_t&) cov.nb_ineq = 0;
		nb_solution = 0;
		nb_boundary = 0;
	}
	else {
		format_id.pop();
		format_version.pop();
		(size_t&) cov.m = read_pos_int(*f);
		(size_t&) cov.nb_ineq = read_pos_int(*f);

		unsigned int _boundary_type = read_pos_int(*f);
		switch (_boundary_type) {
		case 0: (BoundaryType&) boundary_type = EQU_ONLY; break;
		case 1: (BoundaryType&) boundary_type = FULL_RANK; break;
		case 2: (BoundaryType&) boundary_type = HALF_BALL; break;
		default: ibex_error("[CovManifold]: unknown boundary type identifier.");
		}

		if (cov.m>0) {
			nb_solution = read_pos_int(*f);
			if (nb_solution > cov.CovIBUList::nb_boundary())
				ibex_error("[CovManifold]: number of solutions > number of CovIBUList boundary boxes (m>0)");

			// note: in principle, with some constraints like x=0 we could have
			// inner boxes. But this would create confusion. The solver only
			// produces inner boxes in the case m=0 anyway.
			if (cov.nb_inner()>0)
				ibex_error("[CovManifold]: the file should not contain 'solutions' in addition to inner boxes (m=0)");

			for (size_t i=0; i<nb_solution; i++) {
				uint32_t j=read_pos_int(*f);
				if (j<cov._manifold_solution.back())
					ibex_error("[CovManifold]: indices of solutions are not in increasing order.");
				if (j==cov._manifold_solution.back())
					ibex_error("[CovManifold]: duplicated index of solution.");
				cov._manifold_solution.push_back(j);

				if (cov.m < cov.n)
					cov._manifold_varset.push_back(read_varset(*f, cov.n, cov.m));

				cov._manifold_unicity.push_back(read_box(*f, cov.n));
			}
		}
		else {
			nb_solution = 0; // temporary since there may be inner boxes.
		}

		nb_boundary = read_pos_int(*f);

		// (check redundant with below)
		if (boundary_type != HALF_BALL && nb_boundary > cov.CovIBUList::nb_unknown())
			ibex_error("[CovManifold]: number of (uncertified) boundary boxes > number of CovIBUList unknown boxes");

		// (check redundant with below)
		if (boundary_type == HALF_BALL && nb_boundary > cov.CovIBUList::nb_boundary())
			ibex_error("[CovManifold]: number of boundary boxes > number of CovIBUList boundary boxes");

		for (size_t i=0; i<nb_boundary; i++) {
			uint32_t j=read_pos_int(*f);
			if (j<cov._manifold_boundary.back())
				ibex_error("[CovManifold]: indices of boundary boxes are not in increasing order.");
			if (j==cov._manifold_boundary.back())
				ibex_error("[CovManifold]: duplicated index of boundary box.");
			cov._manifold_boundary.push_back(j);

			if (cov.m < cov.n)
				cov._manifold_varset.push_back(read_varset(*f, cov.n, cov.m));
		}
	}


	// iterator of solution boxes
	vector<size_t>::const_iterator it_sol=cov._manifold_solution.begin();
	// iterator of boundary boxes
	vector<size_t>::const_iterator it_bnd=cov._manifold_boundary.begin();

	for (size_t i=0; i<cov.size(); i++) {
		if (it_sol!=cov._manifold_solution.end() && i==*it_sol) {
			if (m>0 && !cov.CovIBUList::is_boundary(i))
				ibex_error("[CovManifold]: a solution box must be a CovIBUList boundary box (if m>0).");
			cov._manifold_status.push_back(CovManifold::SOLUTION);
			++it_sol;
		} else if (it_bnd!=cov._manifold_boundary.end() && i==*it_bnd) {
			if (boundary_type != HALF_BALL && !cov.CovIBUList::is_unknown(i))
				ibex_error("[CovManifold]: a (uncertified) boundary box must be a CovIBUList unknown box.");
			if (boundary_type == HALF_BALL && !cov.CovIBUList::is_boundary(i))
				ibex_error("[CovManifold]: a boundary box must be a CovIBUList boundary box.");
			cov._manifold_status.push_back(CovManifold::BOUNDARY);
			++it_bnd;
		} else {
			switch(cov.CovIBUList::status(i)) {
			case CovIBUList::INNER :
				cov._manifold_solution.push_back(i);
				cov._manifold_status.push_back(CovManifold::SOLUTION);
				break;
			case CovIBUList::UNKNOWN :
				cov._manifold_status.push_back(CovManifold::UNKNOWN);
				break;
			default :
				// This case typically happens in the case of FULL_RANK
				// when a IBU boundary box meets a singularity of the inequality.
				cov._manifold_status.push_back(CovManifold::UNKNOWN);
			}
		}
	}

	if (it_sol!=cov._manifold_solution.end()) ibex_error("[CovManifold]: invalid solution box index.");

	if (it_bnd!=cov._manifold_boundary.end()) ibex_error("[CovManifold]: invalid boundary box index.");

	return f;
}

ofstream* CovManifold::write(const char* filename, const CovManifold& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ofstream* f = CovIBUList::write(filename, cov, format_id, format_version);

	write_pos_int(*f, cov.m);
	write_pos_int(*f, cov.nb_ineq);

	switch(cov.boundary_type) {
	case INNER_PT  : write_pos_int(*f, 0); break;
	case FULL_RANK : write_pos_int(*f, 1); break;
	case HALF_BALL : write_pos_int(*f, 2); break;
	default        : assert(false);
	}

	if (m>0) {
		write_pos_int(*f, cov.nb_solution());
		for (size_t i=0; i<cov.nb_solution(); i++) {
			assert(i<sizeof(uint32_t));
			write_pos_int(*f, (uint32_t) i);
			if (cov.m < cov.n) write_varset(*f, cov.varset(i));
			write_box(*f, cov.unicity(i));
		}
	}

	write_pos_int(*f, cov.nb_boundary());
	for (size_t i=0; i<cov.nb_boundary(); i++) {
		assert(i<sizeof(uint32_t));
		write_pos_int(*f, (uint32_t) i);
		if (cov.m < cov.n) write_varset(*f, cov.varset(i));
	}

	return f;
}

void CovManifold::format(stringstream& ss, const string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {
	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	CovIBUList::format(ss, title, format_id, format_version);

	ss
	<< space << " - 1 integer:     the number m of equalities\n"
	<< space << " - 1 integer:     the number of inequalities\n"
	<< space << " - 1 integer:     the type of boundary boxes:\n"
	<< space << "                  0=only equalities are certified.\n"
	<< space << "                  1=equalities are certified and the gradients\n"
	<< space << "                    of equalities and inequalities are\n"
	<< space << "                    linearly independent.\n"
	<< space << "                  2=the intersection of the manifold and the\n"
	<< space << "                    box is homeomorphic to a hall-ball of R^n.\n"
	<< space << " - [if m>0]\n"
	<< space << " | - 1 integer:   the number Ns of solution boxes (<= Nb)\n"
	<< space << " | - Ns solutions:each solution is the following sequence:\n"
	<< "|    CovManifold    |" <<
	            " |                - 1 integer: the index of the solution\n"
	<< space << " |                  (belongs to CovIBUList boundar boxes);\n"
	<< space << " |                - [if m<n] n-m integers: the indices of\n"
	<< space << " |                  parameters in the parametric proofs;\n"
	<< space << " |                - 2*n real values: the unicity box in the\n"
	<< space << " |                  proof (lb(x1), ub(x1),..., ub(xn));\n"
	<< space << " - 1 integer:     the number Nbb of boundary boxes\n"
	<< space << " - Nbb times      the following sequence:\n"
	<< space << "                  - 1 integer: the index of the boundary box\n"
	<< space << "                    (belongs to CovIBUList boundary or unknown"
	<< space << "                     boxes);\n"
	<< space << "                  - [if m<n] n-m integers: the indices of\n"
	<< space << "                    parameters in the parametric proofs;\n"
	<< separator;
}

string CovManifold::format() {
	stringstream ss;
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	format(ss, "CovManifold", format_id, format_version);
	return ss.str();
}


} // end namespace

