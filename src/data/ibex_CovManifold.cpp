//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Feb 28, 2019
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
		CovIBUList(n, INNER_AND_OUTER_PT), data(new Data()), own_data(true) {

	data->_manifold_nb_eq = m;
	data->_manifold_nb_ineq = nb_ineq;
	data->_manifold_boundary_type =  boundary_type;

	if (n>0) // if well initialized
		// create once for all varset structure for variables and parameters
		if (m==0)
			data->_manifold_solution_varset.push_back(VarSet(n,BitSet::all(n),false)); // all parameters
		else if (m==n)
			data->_manifold_solution_varset.push_back(VarSet(n,BitSet::empty(n),false)); // no parameter
}

CovManifold::CovManifold(const char* filename) : CovManifold(0,0,0 /* tmp */, EQU_ONLY /* by default */) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = CovManifold::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

CovManifold::CovManifold(const Cov& cov, bool copy) : CovIBUList(cov, copy) {
	const CovManifold* covManifold = dynamic_cast<const CovManifold*>(&cov);

	if (covManifold) {
		if (copy) {
			data = new Data(*covManifold->data); // implicit constructor is fine
//			data->_manifold_nb_eq= covManifold->data->_manifold_nb_eq;
//			data->_manifold_nb_ineq= covManifold->data->_manifold_nb_ineq;
//			data->_manifold_boundary_type= covManifold->data->_manifold_boundary_type;
//			data->_manifold_status= covManifold->data->_manifold_status;
//			data->_manifold_solution= covManifold->data->_manifold_solution;
//			data->_manifold_boundary= covManifold->data->_manifold_boundary;
//			data->_manifold_unknown= covManifold->data->_manifold_unknown;
//			data->_manifold_unicity= covManifold->data->_manifold_unicity;
			own_data = true;
		} else {
			data = covManifold->data;
			own_data = false;
		}
	} else {
		data = new Data();
		data->_manifold_nb_eq = 0;
		data->_manifold_nb_ineq = 0;
		data->_manifold_boundary_type = EQU_ONLY; /* by default */
		for (size_t i=0; i<size(); i++) {
			switch(CovIBUList::status(i)) {
			case CovIBUList::INNER :
				data->_manifold_status.push_back(CovManifold::SOLUTION);
				data->_manifold_solution.push_back(i);
				if (data->_manifold_solution_varset.empty())
					data->_manifold_solution_varset.push_back(VarSet(n,BitSet::all(n),false)); // all parameters
				break;
			case CovIBUList::BOUNDARY :
			case CovIBUList::UNKNOWN :
				data->_manifold_status.push_back(CovManifold::UNKNOWN);
				data->_manifold_unknown.push_back(i);
				break;
			default :
				assert(false);
			}
		}
		own_data = true;
	}
}

void CovManifold::save(const char* filename) const {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=CovManifold::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
}

CovManifold::~CovManifold() {
	if (own_data) {
		delete data;
	}
}

void CovManifold::add(const IntervalVector& x) {
	add_unknown(x);
}

void CovManifold::add_inner(const IntervalVector& x) {
	if (nb_eq()>0)
		ibex_error("[CovManifold] inner boxes not allowed with equalities");
	CovIBUList::add_inner(x);
	data->_manifold_solution.push_back(size()-1);
	data->_manifold_status.push_back(SOLUTION);
}

void CovManifold::add_boundary(const IntervalVector& x) {
	if (nb_eq()>0 && nb_eq()<n)
		ibex_error("[CovManifold]: a boundary box for an under-constrained system requires \"VarSet\" structure (parameters/variables)");

	add_boundary(x,data->_manifold_solution_varset[0]);
}

void CovManifold::add_boundary(const IntervalVector& x, const VarSet& varset) {
	switch (boundary_type()) {
	case EQU_ONLY  : CovIBUList::add_unknown(x); break;
	case FULL_RANK : CovIBUList::add_unknown(x); break;
	case HALF_BALL : CovIBUList::add_boundary(x); break;
	default        : assert(false);
	}

	data->_manifold_status.push_back(BOUNDARY);
	data->_manifold_boundary.push_back(size()-1);

	if (nb_eq()>0 && nb_eq()<n) // useless otherwise
		data->_manifold_boundary_varset.push_back(varset);
}

void CovManifold::add_unknown(const IntervalVector& x) {
	CovIBUList::add_unknown(x);
	data->_manifold_unknown.push_back(size()-1);
	data->_manifold_status.push_back(UNKNOWN);
}

void CovManifold::add_solution(const IntervalVector& existence, const IntervalVector& unicity) {
	if (nb_eq() < n)
		ibex_error("[CovManifold]: a solution of under-constrained system requires \"VarSet\" structure (parameters/variables)");

	add_solution(existence, unicity, data->_manifold_solution_varset[0]);
}

void CovManifold::add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset) {
	if (nb_eq()==0)
		ibex_error("[CovManifold]: solution boxes not allowed without equalities");

	CovIBUList::add_boundary(existence);
	data->_manifold_solution.push_back(size()-1);
	data->_manifold_unicity.push_back(unicity);
	data->_manifold_status.push_back(SOLUTION);

	if (nb_eq()<n) // useless otherwise
		data->_manifold_solution_varset.push_back(varset);
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
		(size_t&) cov.data->_manifold_nb_eq = 0;
		(size_t&) cov.data->_manifold_nb_ineq = 0;
		nb_solution = 0;
		nb_boundary = 0;
	}
	else {
		format_id.pop();
		format_version.pop();
		(size_t&) cov.data->_manifold_nb_eq = read_pos_int(*f);
		(size_t&) cov.data->_manifold_nb_ineq = read_pos_int(*f);

		unsigned int _boundary_type = read_pos_int(*f);
		switch (_boundary_type) {
		case 0: (BoundaryType&) cov.data->_manifold_boundary_type = EQU_ONLY; break;
		case 1: (BoundaryType&) cov.data->_manifold_boundary_type = FULL_RANK; break;
		case 2: (BoundaryType&) cov.data->_manifold_boundary_type = HALF_BALL; break;
		default: ibex_error("[CovManifold]: unknown boundary type identifier.");
		}

		if (cov.nb_eq()>0) {
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
				if (!cov.data->_manifold_solution.empty()) { // check ordering
					if (j<cov.data->_manifold_solution.back())
						ibex_error("[CovManifold]: indices of solutions are not in increasing order.");
					if (j==cov.data->_manifold_solution.back())
						ibex_error("[CovManifold]: duplicated index of solution.");
				}
				cov.data->_manifold_solution.push_back(j);

				if (cov.nb_eq() < cov.n)
					cov.data->_manifold_solution_varset.push_back(read_varset(*f, cov.n, cov.nb_eq()));

				cov.data->_manifold_unicity.push_back(read_box(*f, cov.n));
			}
		}

		nb_boundary = read_pos_int(*f);

		// (check redundant with below)
		if (cov.boundary_type() != HALF_BALL && nb_boundary > cov.CovIBUList::nb_unknown())
			ibex_error("[CovManifold]: number of (uncertified) boundary boxes > number of CovIBUList unknown boxes");

		// (check redundant with below)
		if (cov.boundary_type() == HALF_BALL && nb_boundary > cov.CovIBUList::nb_boundary())
			ibex_error("[CovManifold]: number of boundary boxes > number of CovIBUList boundary boxes");

		for (size_t i=0; i<nb_boundary; i++) {
			uint32_t j=read_pos_int(*f);
			if (!cov.data->_manifold_boundary.empty()) { // check ordering
				if (j<cov.data->_manifold_boundary.back())
					ibex_error("[CovManifold]: indices of boundary boxes are not in increasing order.");
				if (j==cov.data->_manifold_boundary.back())
					ibex_error("[CovManifold]: duplicated index of boundary box.");
			}
			cov.data->_manifold_boundary.push_back(j);

			if (cov.nb_eq()>0 && cov.nb_eq() < cov.n)
				cov.data->_manifold_boundary_varset.push_back(read_varset(*f, cov.n, cov.nb_eq()));
		}
	}

	if (cov.nb_eq()==0) {
		// comes from either a CovManifold with no equality or a CovManifold
		// loaded from a CovIBUList file (or lesser).
		nb_solution = cov.CovIUList::nb_inner();
		for (size_t i=0; i<cov.CovIUList::nb_inner(); i++) {
			cov.data->_manifold_solution.push_back(cov.CovIUList::data->_IU_inner[i]);
		}
	}

	// iterator of solution boxes
	vector<size_t>::const_iterator it_sol=cov.data->_manifold_solution.begin();
	// iterator of boundary boxes
	vector<size_t>::const_iterator it_bnd=cov.data->_manifold_boundary.begin();

	for (size_t i=0; i<cov.size(); i++) {
		if (it_sol!=cov.data->_manifold_solution.end() && i==*it_sol) {
			if (cov.nb_eq()>0 && !cov.CovIBUList::is_boundary(i))
				ibex_error("[CovManifold]: a solution box must be a CovIBUList boundary box (if m>0).");
			cov.data->_manifold_status.push_back(CovManifold::SOLUTION);
			++it_sol;
		} else if (it_bnd!=cov.data->_manifold_boundary.end() && i==*it_bnd) {
			if (cov.boundary_type() != HALF_BALL && !cov.CovIBUList::is_unknown(i))
				ibex_error("[CovManifold]: a (uncertified) boundary box must be a CovIBUList unknown box.");
			if (cov.boundary_type() == HALF_BALL && !cov.CovIBUList::is_boundary(i))
				ibex_error("[CovManifold]: a boundary box must be a CovIBUList boundary box.");
			cov.data->_manifold_status.push_back(CovManifold::BOUNDARY);
			++it_bnd;
		} else {
			switch(cov.CovIBUList::status(i)) {
			case CovIBUList::BOUNDARY :
				// This case typically happens in the case of FULL_RANK
				// when a IBU boundary box meets a singularity of the inequality.
				cov.data->_manifold_unknown.push_back(i);
				cov.data->_manifold_status.push_back(CovManifold::UNKNOWN);
				break;
			case CovIBUList::UNKNOWN :
				cov.data->_manifold_unknown.push_back(i);
				cov.data->_manifold_status.push_back(CovManifold::UNKNOWN);
				break;
			default :
				assert(false);
			}
		}
	}

	if (it_sol!=cov.data->_manifold_solution.end()) ibex_error("[CovManifold]: invalid solution box index.");

	if (it_bnd!=cov.data->_manifold_boundary.end()) ibex_error("[CovManifold]: invalid boundary box index.");

	// create once for all varset structure for variables and parameters
	if (cov.n>0 && cov.nb_eq()==0)
		cov.data->_manifold_solution_varset.push_back(VarSet(cov.n,BitSet::all(cov.n),false)); // all parameters
	else if (cov.n>0 && cov.nb_eq()==cov.n)
		cov.data->_manifold_solution_varset.push_back(VarSet(cov.n,BitSet::empty(cov.n),false)); // no parameter

	return f;
}

ofstream* CovManifold::write(const char* filename, const CovManifold& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ofstream* f = CovIBUList::write(filename, cov, format_id, format_version);

	write_pos_int(*f, cov.nb_eq());
	write_pos_int(*f, cov.nb_ineq());

	switch(cov.boundary_type()) {
	case INNER_PT  : write_pos_int(*f, 0); break;
	case FULL_RANK : write_pos_int(*f, 1); break;
	case HALF_BALL : write_pos_int(*f, 2); break;
	default        : assert(false);
	}

	if (cov.nb_eq()>0) {
		write_pos_int(*f, cov.nb_solution());
		std::vector<VarSet>::const_iterator it_varset=cov.data->_manifold_solution_varset.begin();
		std::vector<IntervalVector>::const_iterator it_unicity=cov.data->_manifold_unicity.begin();

		for (vector<size_t>::const_iterator it=cov.data->_manifold_solution.begin(); it!=cov.data->_manifold_solution.end(); ++it) {
			assert(*it<numeric_limits<uint32_t>::max());
			write_pos_int(*f, (uint32_t) *it);
			if (cov.nb_eq() < cov.n) {
				write_varset(*f, *it_varset);
				++it_varset;
			}
			write_box(*f, *it_unicity);
			++it_unicity;
		}
	}

	write_pos_int(*f, cov.nb_boundary());
	std::vector<VarSet>::const_iterator it_varset=cov.data->_manifold_boundary_varset.begin();

	for (vector<size_t>::const_iterator it=cov.data->_manifold_boundary.begin(); it!=cov.data->_manifold_boundary.end(); ++it) {
		assert(*it<numeric_limits<uint32_t>::max());
		write_pos_int(*f, (uint32_t) *it);

		if (cov.nb_eq()>0 && cov.nb_eq() < cov.n) {
			write_varset(*f, *it_varset);
			++it_varset;
		}
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
	<< space << "                  - 0=only equalities are certified.\n"
	<< space << "                  - 1=equalities are certified and the \n"
	<< space << "                    gradients of all active constraints\n"
	<< space << "                    are linearly independent.\n"
	<< space << "                  - 2=the intersection of the manifold and\n"
	<< space << "                    the box is homeomorphic to a hall-ball\n"
	<< space << "                    of R^n.\n"
	<< space << " +----[if m>0]----\n"
	<< space << " | - 1 integer:   the number Ns of solution boxes (<= Nb)\n"
	<< space << " | - Ns solutions:each solution is the following sequence:\n"
	<< "|    CovManifold    |" <<
	            " |                - 1 integer: the index of the solution\n"
	<< space << " |                  (belongs to CovIBUList boundar boxes);\n"
	<< space << " |                - [if m<n] n-m integers: the indices of\n"
	<< space << " |                  parameters in the parametric proofs\n"
	<< space << " |                - 2*n real values: the unicity box in the\n"
	<< space << " |                  proof (lb(x1), ub(x1),..., ub(xn))\n"
	<< space << " +----------------\n"
	<< space << " - 1 integer:     the number Nbb of boundary boxes\n"
	<< space << " - Nbb times      the following sequence:\n"
	<< space << "                  - 1 integer: the index of the boundary \n"
	<< space << "                    box (belongs to CovIBUList boundary or \n"
	<< space << "                    unknown boxes)\n"
	<< space << "                  - [if m>0 and m<n]: \n"
	<< space << "                    n-m integers: the indices of parameters\n"
	<< space << "                    in the parametric proofs\n"
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

