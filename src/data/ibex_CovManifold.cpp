//============================================================================
//                                  I B E X
// File        : ibex_CovManifold.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Dec 12, 2018
//============================================================================

#include "ibex_CovManifold.h"

using namespace std;

namespace ibex {

CovManifold::CovManifold(const CovManifoldFactory& fac) : CovIBUList(fac), m(fac.nb_eq), nb_ineq(fac.nb_ineq), nb_solution(0), nb_boundary(0),
		_manifold_status(NULL), _manifold_solution(NULL), _manifold_boundary(NULL), _unicity(1,1 /*tmp*/), _varset(NULL) {

	fac.build(*this);
}

CovManifold::CovManifold(const char* filename) : CovManifold(CovManifoldFactory(filename)) {

}

void CovManifold::save(const char* filename) {
	ofstream* of=CovManifoldFile::write(filename,*this);
	of->close();
	delete of;
}

CovManifold::~CovManifold() {
	assert(_manifold_status);

	delete[] _manifold_status;
	delete[] _manifold_solution;
	delete[] _manifold_boundary;

	if (m<n)
		for (int i=0; i<nb_solution; i++)
			delete _varset[i];
	else
		delete _varset[0];

	delete[] _varset;
}

int CovManifold::subformat_number() const {
	return 0;
}

CovManifoldFactory::CovManifoldFactory(size_t n) : CovIBUListFactory(n), nb_eq(0), nb_ineq(0) {

}

CovManifoldFactory::CovManifoldFactory(const char* filename) : CovManifoldFactory((size_t) 0 /* tmp*/) {
	ifstream* f = CovManifoldFile::read(filename, *this);
	f->close();
	delete f;
}

CovManifoldFactory::~CovManifoldFactory() {
}

void CovManifoldFactory::add_solution(const IntervalVector& existence, const IntervalVector& unicity) {
	if (nb_eq < n)
		ibex_error("CovManifoldFactory: a solution of under-constrained system requires \"VarSet\" structure (parameters/variables)");

	CovIBUListFactory::add_boundary(existence);
	this->solution.push_back(nb_boxes()-1);
	this->unicity.push_back(unicity);
}

void CovManifoldFactory::add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset) {
	CovIBUListFactory::add_boundary(existence);
	this->solution.push_back(nb_boxes()-1);
	this->unicity.push_back(unicity);
	this->varset.push_back(varset);
}

void CovManifoldFactory::build(CovManifold& manif) const {
	//assert(manif.nb_inner == nb_inner);
	//assert(manif.CovIBUList::nb_boundary == nb_boundary);
	(size_t&) manif.nb_solution = solution.size();
	(size_t&) manif.nb_boundary = manif.CovIBUList::nb_boundary - manif.nb_solution;
	manif._manifold_status      = new CovManifold::BoxStatus[manif.size];
	manif._manifold_solution    = new IntervalVector*[manif.nb_solution];
	manif._manifold_boundary    = new IntervalVector*[manif.nb_boundary];
	manif._unicity.resize(manif.nb_solution, manif.n);

	if (manif.m < manif.n)
		manif._varset               = new VarSet*[manif.nb_solution];
	else
		manif._varset               = new VarSet*[1];

	for (int i=0; i<manif.size; i++) {
		switch (manif.CovIBUList::status(i)) {
		case CovIBUList::INNER:   manif._manifold_status[i]=CovManifold::INNER;   break;
		case CovIBUList::UNKNOWN: manif._manifold_status[i]=CovManifold::UNKNOWN; break;
		default :                 manif._manifold_status[i]=CovManifold::BOUNDARY; // by default
		}
	}

	for (vector<unsigned int>::const_iterator it=solution.begin(); it!=solution.end(); ++it) {
		if (manif._manifold_status[*it]!=CovManifold::BOUNDARY)
			ibex_error("[CovManifoldFactoy]: a solution box cannot be inner or unknown at the same time.");
		else {
			manif._manifold_status[*it]=CovManifold::SOLUTION;
		}
	}

	size_t jsol=0; // count solution boxes
	size_t jbo=0;  // count boundary boxes

	if (manif.m >= manif.n)
		manif._varset[0] = new VarSet(manif.n,BitSet::all(manif.n),true);

	for (int i=0; i<manif.size; i++) {
		if (manif._manifold_status[i]==CovManifold::SOLUTION) {
			manif._manifold_solution[jsol] = (IntervalVector*) &manif[i];
			manif._unicity.row(jsol) = unicity[jsol];
			if (manif.m < manif.n)
				manif._varset[jsol] = new VarSet(varset[jsol]);
			jsol++;
		}
		else if (manif._manifold_status[i]==CovManifold::BOUNDARY) {
			manif._manifold_boundary[jbo++]=(IntervalVector*) &manif[i];
		}
	}
	assert(jsol==manif.nb_solution);
	assert(jbo==manif.nb_boundary);
}

//----------------------------------------------------------------------------------------------------

ifstream* CovManifoldFile::read(const char* filename, CovManifoldFactory& factory) {

	ifstream* f = CovIBUListFile::read(filename, factory);

	factory.nb_eq = read_pos_int(*f);
	factory.nb_ineq = read_pos_int(*f);
	size_t nb_solution = read_pos_int(*f);

	if (nb_solution > factory.nb_boundary())
		ibex_error("[CovManifoldFile]: number of solutions > number of CovIBUList boundary boxes");

	for (size_t i=0; i<nb_solution; i++) {
		factory.solution.push_back(read_pos_int(*f));
		if (factory.nb_eq < factory.n)
			factory.varset.push_back(read_varset(*f, factory.n, factory.nb_eq));
		factory.unicity.push_back(read_box(*f, factory.n));
	}

	return f;
}

ofstream* CovManifoldFile::write(const char* filename, const CovManifold& cov) {

	ofstream* f = CovIBUListFile::write(filename, cov);

	write_int(*f, cov.m);
	write_int(*f, cov.nb_ineq);
	write_int(*f, cov.nb_solution);

	// TODO: a complete scan could be avoided?
	int j=0;
	for (size_t i=0; i<cov.size; i++) {
		if (cov.status(i)==CovManifold::SOLUTION) {
			write_int(*f, (uint32_t) i);
			if (cov.m < cov.n) write_varset(*f, cov.varset(j));
			write_box(*f, cov.unicity(j));
			j++;
		}
	}
	return f;
}

VarSet CovManifoldFile::read_varset(ifstream& f, size_t n, size_t m) {

	BitSet params(n);

	for (unsigned int j=0; j<n-m; j++) {
		unsigned int v=read_pos_int(f);
		if (v>n) {
			ibex_error("[CovManifoldFile]: bad input file (bad parameter index)");
		}
		params.add(v-1); // index starting from 1 in the raw format
	}

	return VarSet(n,params,false);
}

void CovManifoldFile::write_varset(ofstream& f, const VarSet& varset) {
	for (int i=0; i<varset.nb_param; i++)
		write_int(f, varset.param(i));
}

void CovManifoldFile::format(stringstream& ss, const string& title) {
	CovIBUListFile::format(ss, title);

	ss <<   "- 2 integers:     the number m of equalities and the number of inequalities\n"
			"- 1 integer:      the number Ns of solution boxes (<= Nb)\n"
			"- Ns solutions:   each solution is the sequence of following information:\n"
			"            * 1 integer: the index of the solution boxes (in 0..N);\n"
			"            * [if m<n] n-m integers: the indices of parameters in the parametric proofs;\n"
			"            * 2*n real values: the unicity box in the proof (lb(x1), ub(x1),...,lb(xn), ub(xn));\n";
}

} // end namespace

