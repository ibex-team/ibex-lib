//============================================================================
//                                  I B E X
// File        : ibex_CovSolverData.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Not 08, 2018
//============================================================================

#include "ibex_CovSolverData.h"

using namespace std;

namespace ibex {

CovSolverData::CovSolverData(const CovSolverDataFactory& fac) : CovManifold(fac),
		solver_status(fac.status), time(fac.time), nb_cells(fac.nb_cells),
		nb_pending(0), nb_unknown(0), _solver_status(NULL), _solver_pending(NULL), _solver_unknown(NULL) {
	fac.build(*this);
}

CovSolverData::CovSolverData(const char* filename) : CovSolverData(CovSolverDataFactory(filename)) {

}

CovSolverData::~CovSolverData() {
	assert(_solver_status);
	delete[] _solver_status;
	delete[] _solver_pending;
	delete[] _solver_unknown;
}

void CovSolverData::save(const char* filename) {
	stack<unsigned int> format_seq;
	ofstream* of=CovSolverDataFile::write(filename, *this, format_seq);
	of->close();
	delete of;
}

CovSolverDataFactory::CovSolverDataFactory(size_t n) : CovManifoldFactory(n), status(Solver::SUCCESS /* ? */), time(-1), nb_cells(0) {

}

CovSolverDataFactory::CovSolverDataFactory(const char* filename) : CovSolverDataFactory((size_t) 0 /* tmp*/) {
	stack<unsigned int> format_seq;
	ifstream* f = CovSolverDataFile::read(filename, *this, format_seq);
	f->close();
	delete f;
}

CovSolverDataFactory::~CovSolverDataFactory() {
}

void CovSolverDataFactory::add_pending(const IntervalVector& x) {
	CovIUListFactory::add_unknown(x);
	pending.push_back(nb_boxes()-1);
}

void CovSolverDataFactory::build(CovSolverData& solver) const {
//	assert(solver.nb_inner == nb_inner);
//  assert(solver.nb_boundary == nb_boundary);
//	assert(solver.nb_solution == nb_solution);

	for (vector<string>::const_iterator it=var_names.begin(); it!=var_names.end(); it++)
		solver.var_names.push_back(*it);

	(size_t&) solver.nb_pending = pending.size();
	(size_t&) solver.nb_unknown = solver.CovIBUList::nb_unknown - solver.nb_pending;
	solver._solver_status       = new CovSolverData::BoxStatus[solver.size];
	solver._solver_pending      = new IntervalVector*[solver.nb_pending];
	solver._solver_unknown      = new IntervalVector*[solver.nb_unknown];

	for (size_t i=0; i<solver.size; i++) {
		switch (solver.CovManifold::status(i)) {
		case CovManifold::INNER:    solver._solver_status[i]=CovSolverData::INNER;   break;
		case CovManifold::SOLUTION: solver._solver_status[i]=CovSolverData::SOLUTION;   break;
		case CovManifold::BOUNDARY: solver._solver_status[i]=CovSolverData::BOUNDARY; break;
		default:                    solver._solver_status[i]=CovSolverData::UNKNOWN; break; // by default
		}
	}

	for (vector<unsigned int>::const_iterator it=pending.begin(); it!=pending.end(); ++it) {
		if (solver._solver_status[*it]!=CovSolverData::UNKNOWN)
			ibex_error("[CovSolverDataFactoy]: a pending box cannot be inner, boundary or solution.");
		else {
			solver._solver_status[*it]=CovSolverData::PENDING;
		}
	}

	size_t junk=0; // count unknown boxes
	size_t jpen=0;  // count pending boxes

	for (size_t i=0; i<solver.size; i++) {
		if (solver._solver_status[i]==CovSolverData::PENDING) {
			solver._solver_pending[jpen] = (IntervalVector*) &solver[i];
			jpen++;
		}
		else if (solver._solver_status[i]==CovSolverData::UNKNOWN) {
			solver._solver_unknown[junk] = (IntervalVector*) &solver[i];
			junk++;
		}
	}
	assert(junk==solver.nb_unknown);
	assert(jpen==solver.nb_pending);
}


//----------------------------------------------------------------------------------------------------

const unsigned int CovSolverDataFile::subformat_level = 5;

const unsigned int CovSolverDataFile::subformat_number = 0;

ifstream* CovSolverDataFile::read(const char* filename, CovSolverDataFactory& factory, stack<unsigned int>& format_seq) {

	ifstream* f = CovManifoldFile::read(filename, factory, format_seq);

	if (format_seq.empty() || format_seq.top()!=subformat_number) return f;
	else format_seq.pop();

	read_vars(*f, factory.n, factory.var_names);

	unsigned int status = read_pos_int(*f);

	switch (status) {
	case 0: factory.status = Solver::SUCCESS;           break;
	case 1: factory.status = Solver::INFEASIBLE;        break;
	case 2: factory.status = Solver::NOT_ALL_VALIDATED; break;
	case 3: factory.status = Solver::TIME_OUT;          break;
	case 4: factory.status = Solver::CELL_OVERFLOW;     break;
	default: ibex_error("[CovSolverDataFile]: invalid solver status.");
	}

	factory.time = read_double(*f);
	factory.nb_cells = read_pos_int(*f);

	size_t nb_pending = read_pos_int(*f);

	if (nb_pending > factory.nb_unknown())
		ibex_error("[CovSolverDataFile]: number of pending boxes > number of CovIUList unknown boxes");

	for (size_t i=0; i<nb_pending; i++) {
		factory.pending.push_back(read_pos_int(*f));
	}

	return f;
}

ofstream* CovSolverDataFile::write(const char* filename, const CovSolverData& cov, stack<unsigned int>& format_seq) {

	format_seq.push(subformat_number);

	ofstream* f = CovManifoldFile::write(filename, cov, format_seq);

	write_vars(*f, cov.var_names);
	write_int(*f, cov.solver_status);
	write_double(*f, cov.time);
	write_int(*f, cov.nb_cells);
	write_int(*f, cov.nb_pending);

	// TODO: a complete scan could be avoided?
	for (size_t i=0; i<cov.size; i++) {
		if (cov.status(i)==CovSolverData::PENDING)
			write_int(*f, (uint32_t) i);
	}
	return f;
}

void CovSolverDataFile::read_vars(ifstream& f, size_t n, vector<string>& var_names) {
	char x;
	for (size_t i=0; i<n; i++) {
		stringstream s;
		do {
			f.read(&x, sizeof(char));
			if (f.eof()) ibex_error("[CovManifoldFile]: unexpected end of file.");
			if (x!='\0') s << x;
		} while(x!='\0');
		var_names.push_back(s.str());
	}
}

void CovSolverDataFile::write_vars(ofstream& f, const vector<string>& var_names) {
	for (vector<string>::const_iterator it=var_names.begin(); it!=var_names.end(); it++) {
		f.write(it->c_str(),it->size()*sizeof(char));
		f.put('\0');
	}
}

void CovSolverDataFile::format(stringstream& ss, const string& title, stack<unsigned int>& format_seq) {
	format_seq.push(subformat_number);

	CovManifoldFile::format(ss, title, format_seq);

	ss
	<< space << " - n strings:      the names of variables. Each string is\n"
	<< space << "                   terminated by the null character \'0\'.\n"
	<< space << " - 1 integer:      the status of the search. Possible \n"
	<< space << "                   values are:\n"
	<< space << "                   - 0=complete search: all output boxes\n"
	<< space << "                     are validated\n"
	<< space << "                   - 1=complete search: infeasible problem\n"
	<< "|   CovSolverData   |" <<
	            "                   - 2=incomplete search: minimal width\n"
	<< space << "                     (--eps-min) reached\n"
	<< space << "                   - 3=incomplete search: time out\n"
	<< space << "                   - 4=incomplete search: buffer overflow\n"
	<< space << " - 1 real value:   time (in seconds)\n"
	<< space << " - 1 integer:      the number of cells.\n"
	<< space << " - 1 value:        the number Np of pending boxes\n"
	<< space << " - Np integers:    the indices of pending boxes\n"
	<< space << "                   (a subset of CovIUList unknown boxes).\n"
	<< separator;
}

string CovSolverDataFile::format() {
	stringstream ss;
	stack<unsigned int> format_seq;
	format(ss, "CovSolverData", format_seq);
	return ss.str();
}

} // end namespace

