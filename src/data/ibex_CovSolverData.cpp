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

CovSolverData::CovSolverData(const CovSolverDataFactory& fac) : CovManifold(fac), var_names(NULL /*TODO */),
		status(fac.status), time(fac.time), nb_cells(fac.nb_cells),
		nb_pending(0), nb_unknown(0), _solver_status(NULL), _solver_pending(NULL), _solver_unknown(NULL) {
	fac.build(*this);
}

CovSolverData::~CovSolverData() {
	assert(_solver_status);
	delete[] _solver_status;
	delete[] _solver_pending;
	delete[] _solver_unknown;
}

int CovSolverData::subformat_number() const {
	return 0;
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
	(size_t&) solver.nb_pending = pending.size();
	(size_t&) solver.nb_unknown = solver.CovIBUList::nb_unknown - solver.nb_pending;
	solver._solver_status       = new CovSolverData::BoxStatus[solver.size];
	solver._solver_pending      = new IntervalVector*[solver.nb_pending];
	solver._solver_unknown      = new IntervalVector*[solver.nb_unknown];

	for (int i=0; i<solver.size; i++) {
		switch (solver.CovManifold::status(i)) {
		case CovManifold::INNER:    solver._solver_status[i]=CovSolverData::INNER;   break;
		case CovManifold::SOLUTION: solver._solver_status[i]=CovSolverData::SOLUTION;   break;
		case CovManifold::BOUNDARY: solver._solver_status[i]=CovSolverData::BOUNDARY; break;
		default:                    solver._solver_status[i]=CovSolverData::UNKNOWN; break; // by default
		}
	}

	for (vector<unsigned int>::const_iterator it=pending.begin(); it!=pending.end(); ++it) {
		if (!solver._solver_status[*it]==CovSolverData::UNKNOWN)
			ibex_error("[CovSolverDataFactoy]: a pending box cannot be inner, boundary or solution.");
		else {
			solver._solver_status[*it]=CovSolverData::PENDING;
		}
	}

	size_t junk=0; // count unknown boxes
	size_t jpen=0;  // count pending boxes

	for (int i=0; i<solver.size; i++) {
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

ifstream* CovSolverDataFile::read(const char* filename, CovSolverDataFactory& factory) {

	ifstream* f = CovManifoldFile::read(filename, factory);

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

	factory.time = read_pos_int(*f);
	factory.nb_cells = read_pos_int(*f);

	size_t nb_pending = read_pos_int(*f);

	if (nb_pending > factory.nb_unknown())
		ibex_error("[CovSolverDataFile]: number of pending boxes > number of CovIUList unknown boxes");

	for (size_t i=0; i<nb_pending; i++) {
		factory.pending.push_back(read_pos_int(*f));
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


void CovSolverDataFile::write_vars(ofstream& f, size_t n, string* var_names) {
	for (unsigned int i=0; i<n; i++) {
		f.write(var_names[i].c_str(),var_names[i].size()*sizeof(char));
		f.put('\0');
	}
}

void CovSolverDataFile::format(stringstream& ss, const string& title) {
	CovManifoldFile::format(ss, title);

	ss <<  	"n strings:      the names of variables. Each string is terminated by the null character \'0\'.\n"
			"1 integer:      the status of the search. Possible values are:\n"
			"\t\t* 0=complete search:   all output boxes are validated\n"
			"\t\t* 1=complete search:   infeasible problem\n"
			"\t\t* 2=incomplete search: minimal width (--eps-min) reached\n"
			"\t\t* 3=incomplete search: time out\n"
			"\t\t* 4=incomplete search: buffer overflow\n"
			"1 real value:   time (in seconds)\n"
			"1 integer:      the number of cells.\n"
			"1 value:        the number Np of pending boxes\n"
			"Np integers:    the indices of boxes that are pending boxes.\n";
}

} // end namespace

