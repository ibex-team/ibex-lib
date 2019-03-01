//============================================================================
//                                  I B E X
// File        : ibex_CovSolverData.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Feb 28, 2019
//============================================================================

#include "ibex_CovSolverData.h"
#include "ibex_Solver.h"

#include <algorithm>

using namespace std;

namespace ibex {

const unsigned int CovSolverData::FORMAT_VERSION = 1;

const unsigned int CovSolverData::subformat_level = 5;

const unsigned int CovSolverData::subformat_number = 0;

CovSolverData::CovSolverData(size_t n, size_t m, size_t nb_ineq, BoundaryType boundary_type, const vector<string>& var_names) :
		CovManifold(n, m, nb_ineq, boundary_type), data(new Data()), own_data(true) {

	data->_solver_var_names = var_names;
	data->_solver_solver_status = Solver::SUCCESS; /* ? */
	data->_solver_time = -1;
	data->_solver_nb_cells = 0;
}

CovSolverData::CovSolverData(const char* filename) : CovSolverData(0, 0, 0 /* tmp */) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = CovSolverData::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

CovSolverData::CovSolverData(const Cov& cov, bool copy) : CovManifold(cov, copy) {
	const CovSolverData* covSolverData = dynamic_cast<const CovSolverData*>(&cov);

	if (covSolverData) {
		if (copy) {
			data = new Data(*covSolverData->data); // implicit constructor is fine
			own_data = true;
		} else {
			data = covSolverData->data;
			own_data = false;
		}
	} else {
		data = new Data();
		data->_solver_solver_status = Solver::SUCCESS; /* ? */
		data->_solver_time = -1;
		data->_solver_nb_cells = 0;

		for (size_t i=0; i<size(); i++) {
			switch(CovManifold::status(i)) {
			case CovManifold::SOLUTION :
				data->_solver_status.push_back(CovSolverData::SOLUTION);
				break;
			case CovManifold::BOUNDARY :
				data->_solver_status.push_back(CovSolverData::BOUNDARY);
				break;
			case CovManifold::UNKNOWN :
				data->_solver_status.push_back(CovSolverData::UNKNOWN);
				data->_solver_unknown.push_back(i);
				break;
			}
		}
		own_data = true;
	}
}

CovSolverData::~CovSolverData() {
	if (own_data) {
		delete data;
	}
}

void CovSolverData::save(const char* filename) const {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=CovSolverData::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
}

void CovSolverData::add(const IntervalVector& x) {
	add_unknown(x);
}

void CovSolverData::add_inner(const IntervalVector& x) {
	CovManifold::add_inner(x);
	data->_solver_status.push_back(SOLUTION);
}

void CovSolverData::add_boundary(const IntervalVector& x, const VarSet& varset) {
	CovManifold::add_boundary(x,varset);
	data->_solver_status.push_back(BOUNDARY);
}

void CovSolverData::add_unknown(const IntervalVector& x) {
	CovManifold::add_unknown(x);
	data->_solver_status.push_back(UNKNOWN);
	data->_solver_unknown.push_back(size()-1);
}

void CovSolverData::add_solution(const IntervalVector& existence, const IntervalVector& unicity, const VarSet& varset) {
	CovManifold::add_solution(existence, unicity, varset);
	data->_solver_status.push_back(SOLUTION);
}

void CovSolverData::add_pending(const IntervalVector& x) {
	CovManifold::add_unknown(x);
	data->_solver_status.push_back(PENDING);
	data->_solver_pending.push_back(size()-1);
}

ostream& operator<<(ostream& os, const CovSolverData& solver) {

	for (size_t i=0; i<solver.nb_solution(); i++) {
		os << " solution n°" << (i+1) << " = " << solver.solution(i) << endl;
	}

	for (size_t i=0; i<solver.nb_inner(); i++) {
		os << " inner n°" << (i+1) << " = " << solver.inner(i) << endl;
	}

	for (size_t i=0; i<solver.nb_boundary(); i++) {
		os << " boundary n°" << (i+1) << " = " << solver.boundary(i) << endl;
	}

	for (size_t i=0; i<solver.nb_unknown(); i++) {
		os << " unknown n°" << (i+1) << " = " << solver.unknown(i) << endl;
	}

	for (size_t i=0; i<solver.nb_pending(); i++) {
		os << " pending n°" << (i+1) << " = " << solver.pending(i) << endl;
	}

	return os;

}

void CovSolverData::read_vars(ifstream& f, size_t n, vector<string>& var_names) {
	char x;
	for (size_t i=0; i<n; i++) {
		stringstream s;
		do {
			f.read(&x, sizeof(char));
			if (f.eof()) ibex_error("[CovManifold]: unexpected end of file.");
			if (x!='\0') s << x;
		} while(x!='\0');
		var_names.push_back(s.str());
	}
}

void CovSolverData::write_vars(ofstream& f, const vector<string>& var_names) {
	for (vector<string>::const_iterator it=var_names.begin(); it!=var_names.end(); it++) {
		f.write(it->c_str(),it->size()*sizeof(char));
		f.put('\0');
	}
}

ifstream* CovSolverData::read(const char* filename, CovSolverData& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	ifstream* f = CovManifold::read(filename, cov, format_id, format_version);

	size_t nb_pending;

	if (format_id.empty() || format_id.top()!=subformat_number || format_version.top()!=FORMAT_VERSION) {
		cov.data->_solver_solver_status = (unsigned int) Solver::SUCCESS;
		cov.data->_solver_time = -1;
		cov.data->_solver_nb_cells = 0;
		nb_pending = 0;
	}
	else {
		format_id.pop();
		format_version.pop();

		read_vars(*f, cov.n, cov.data->_solver_var_names);

		unsigned int status = read_pos_int(*f);

		switch (status) {
		case 0: cov.data->_solver_solver_status = (unsigned int) Solver::SUCCESS;           break;
		case 1: cov.data->_solver_solver_status = (unsigned int) Solver::INFEASIBLE;        break;
		case 2: cov.data->_solver_solver_status = (unsigned int) Solver::NOT_ALL_VALIDATED; break;
		case 3: cov.data->_solver_solver_status = (unsigned int) Solver::TIME_OUT;          break;
		case 4: cov.data->_solver_solver_status = (unsigned int) Solver::CELL_OVERFLOW;     break;
		default: ibex_error("[CovSolverData]: invalid solver status.");
		}

		cov.data->_solver_time = read_double(*f);
		cov.data->_solver_nb_cells = read_pos_int(*f);

		nb_pending = read_pos_int(*f);

		if (nb_pending > cov.CovManifold::nb_unknown())
			ibex_error("[CovSolverData]: number of pending boxes > number of CovManifold unknown boxes");

		for (size_t i=0; i<nb_pending; i++) {
			uint32_t j=read_pos_int(*f);
			if (!cov.data->_solver_pending.empty()) { // check ordering
				if (j<cov.data->_solver_pending.back())
					ibex_error("[CovSolverData]: indices of pending boxes are not in increasing order.");
				if (j==cov.data->_solver_pending.back())
					ibex_error("[CovSolverData]: duplicated index of pending box.");
			}
			cov.data->_solver_pending.push_back(j);
		}
	}


	vector<size_t>::const_iterator it=cov.data->_solver_pending.begin(); // iterator of pending boxes

	for (size_t i=0; i<cov.size(); i++) {

		if (it!=cov.data->_solver_pending.end() && i==*it) {
			if (!cov.CovManifold::is_unknown(i))
				ibex_error("[CovSolverData]: a pending box must be a CovManifold unknown box.");
			cov.data->_solver_status.push_back(CovSolverData::PENDING);
			++it;
		} else {
			switch(cov.CovManifold::status(i)) {
			case CovManifold::SOLUTION :
				cov.data->_solver_status.push_back(CovSolverData::SOLUTION);
				break;
			case CovManifold::BOUNDARY :
				cov.data->_solver_status.push_back(CovSolverData::BOUNDARY);
				break;
			default :
				cov.data->_solver_unknown.push_back(i);
				cov.data->_solver_status.push_back(CovSolverData::UNKNOWN);
			}
		}

	}
	if (it!=cov.data->_solver_pending.end()) ibex_error("[CovSolverData]: invalid pending box index.");

	return f;
}

ofstream* CovSolverData::write(const char* filename, const CovSolverData& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ofstream* f = CovManifold::write(filename, cov, format_id, format_version);

	write_vars(*f, cov.data->_solver_var_names);
	write_pos_int(*f, cov.data->_solver_solver_status);
	write_double(*f, cov.data->_solver_time);
	write_pos_int(*f, cov.data->_solver_nb_cells);
	write_pos_int(*f, cov.nb_pending());


	for (vector<size_t>::const_iterator it=cov.data->_solver_pending.begin(); it!=cov.data->_solver_pending.end(); ++it) {
		assert(*it<numeric_limits<uint32_t>::max());
		write_pos_int(*f, (uint32_t) *it);
	}

	return f;
}

void CovSolverData::format(stringstream& ss, const string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {
	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	CovManifold::format(ss, title, format_id, format_version);

	ss
	<< space << " - n strings:      the names of variables. Each string is\n"
	<< space << "                   terminated by the null character \'\\0\'.\n"
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

string CovSolverData::format() {
	stringstream ss;
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	format(ss, "CovSolverData", format_id, format_version);
	return ss.str();
}

} // end namespace

