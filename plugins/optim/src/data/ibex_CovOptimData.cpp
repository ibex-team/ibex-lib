//============================================================================
//                                  I B E X
// File        : ibex_CovOptimData.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Nov 08, 2018
// Last update : Dec 24, 2018
//============================================================================

#include "ibex_CovOptimData.h"
#include "ibex_Optimizer.h"

#include <algorithm>

using namespace std;

namespace ibex {

const unsigned int CovOptimData::FORMAT_VERSION = 1;

const unsigned int CovOptimData::subformat_level = 5;

const unsigned int CovOptimData::subformat_number = 1;

CovOptimData::CovOptimData(size_t n, bool is_extended_space) : CovList(n),
		is_extended_space(is_extended_space),
		optimizer_status((unsigned int) Optimizer::SUCCESS /* ? */),
		uplo(NEG_INFINITY), uplo_of_epsboxes(POS_INFINITY), loup(POS_INFINITY), loup_point(n), time(-1), nb_cells(0) {
}

CovOptimData::CovOptimData(const char* filename) : CovList((size_t) 0 /* tmp */),
		is_extended_space(false /* tmp*/),
		optimizer_status((unsigned int) Optimizer::SUCCESS),
		uplo(NEG_INFINITY), uplo_of_epsboxes(POS_INFINITY), loup(POS_INFINITY), loup_point(1), time(-1), nb_cells(0) {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ifstream* f = CovOptimData::read(filename, *this, format_id, format_version);
	f->close();
	delete f;
}

void CovOptimData::save(const char* filename) const {
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	ofstream* of=CovOptimData::write(filename, *this, format_id, format_version);
	of->close();
	delete of;
}

ostream& operator<<(ostream& os, const CovOptimData& optim) {

	os << " f* in [" << optim.uplo << "," << optim.loup << "]" << endl;
	os << " x* ~  " << optim.loup_point << endl;

	for (size_t i=1; i<optim.size(); i++) {
		os << " pending n°" << (i+1) << " = " << optim[i] << endl;
	}

	return os;

}

void CovOptimData::read_vars(ifstream& f, size_t n, vector<string>& var_names) {
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

void CovOptimData::write_vars(ofstream& f, const vector<string>& var_names) {
	for (vector<string>::const_iterator it=var_names.begin(); it!=var_names.end(); it++) {
		f.write(it->c_str(),it->size()*sizeof(char));
		f.put('\0');
	}
}

ifstream* CovOptimData::read(const char* filename, CovOptimData& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	ifstream* f = CovList::read(filename, cov, format_id, format_version);

	cov.loup_point.resize((int) cov.n);

	if (format_id.empty() || format_id.top()!=subformat_number || format_version.top()!=FORMAT_VERSION) {
		cov.optimizer_status = (unsigned int) Optimizer::SUCCESS;
		cov.is_extended_space = false;
		cov.uplo = NEG_INFINITY;
		cov.loup = POS_INFINITY;
		cov.loup_point = IntervalVector::empty(cov.n);
		cov.time = -1;
		cov.nb_cells = 0;
	}
	else {
		format_id.pop();
		format_version.pop();

		read_vars(*f, cov.n, cov.var_names);

		unsigned int status = read_pos_int(*f);

		switch (status) {
		case 0: cov.optimizer_status = (unsigned int) Optimizer::SUCCESS;           break;
		case 1: cov.optimizer_status = (unsigned int) Optimizer::INFEASIBLE;        break;
		case 2: cov.optimizer_status = (unsigned int) Optimizer::NO_FEASIBLE_FOUND; break;
		case 3: cov.optimizer_status = (unsigned int) Optimizer::UNBOUNDED_OBJ;     break;
		case 4: cov.optimizer_status = (unsigned int) Optimizer::TIME_OUT;     		break;
		case 5: cov.optimizer_status = (unsigned int) Optimizer::UNREACHED_PREC;    break;
		default: ibex_error("[CovOptimData]: invalid optimizer status.");
		}

		cov.is_extended_space = (bool) read_pos_int(*f);
		cov.uplo              = read_double(*f);
		cov.uplo_of_epsboxes  = read_double(*f);
		cov.loup              = read_double(*f);

		uint32_t loup_found   = read_pos_int(*f);
		unsigned int nb_var   = cov.is_extended_space ? cov.n-1 : cov.n;
		cov.loup_point 		  = loup_found==1? cov[0].subvector(0,nb_var-1) : IntervalVector::empty(nb_var);

		cov.time              = read_double(*f);
		cov.nb_cells          = read_pos_int(*f);
	}

	return f;
}

ofstream* CovOptimData::write(const char* filename, const CovOptimData& cov, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {

	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	ofstream* f = CovList::write(filename, cov, format_id, format_version);

	write_vars  (*f, cov.var_names);
	write_pos_int   (*f, cov.optimizer_status);
	write_pos_int   (*f, (uint32_t) cov.is_extended_space);
	write_double(*f, cov.uplo);
	write_double(*f, cov.uplo_of_epsboxes);
	write_double(*f, cov.loup);

	if (!cov.loup_point.is_empty()) {
		unsigned int nb_var   = cov.is_extended_space ? cov.n-1 : cov.n;
		if (cov[0].subvector(0,nb_var-1)!=cov.loup_point) {
			ibex_error("[CovOptimData] the first box in the list must be the 'loup-point'.");
		}
		write_pos_int(*f, (uint32_t) 1);
	} else {
		write_pos_int(*f, (uint32_t) 0);
	}

	write_double(*f, cov.time);
	write_pos_int   (*f, cov.nb_cells);

	return f;
}

void CovOptimData::format(stringstream& ss, const string& title, std::stack<unsigned int>& format_id, std::stack<unsigned int>& format_version) {
	format_id.push(subformat_number);
	format_version.push(FORMAT_VERSION);

	CovList::format(ss, title, format_id, format_version);

	ss
	<< space << " - n strings:      the names of variables. Each string is\n"
	<< space << "                   terminated by the null character \'0\'.\n"
	<< space << " - 1 integer:      the status of the optimizer. Possible \n"
	<< space << "                   values are:\n"
	<< space << "                   - 0=success\n"
	<< space << "                   - 1=infeasible problem\n"
	<< "|   CovOptimData    |" <<
	            "                   - 2=no feasible point found\n"
	<< space << "                   - 3=unbounded objective\n"
	<< space << "                   - 4=time out\n"
	<< space << "                   - 5=unreached precision\n"
	<< space << " - 1 integer:      1 <=> covering of the extended space\n"
	<< space << "                    (vars+obj), 0 <=> covering of the \n"
	<< space << "                   original space (variables only).\n"
	<< space << " - 3 real values:  'uplo', 'uplo-of-epsboxes' and 'loup'\n"
	<< space << "                   'uplo' is the uppest lower bound of f*\n"
	<< space << "                   'loup' is the lowest upper bound of f*\n"
	<< space << "                   so we have:       uplo<= f* <=loup.\n"
	<< space << " - 1 integer:      1 <=> the first box is the 'loup point'\n"
	<< space << "                   (best feasible point found), possibly\n"
	<< space << "                   extended (see above). 0 <=> no feasible\n"
	<< space << "                   point found.\n"
	<< space << " - 1 real value:   time (in seconds)\n"
	<< space << " - 1 integer:      number of cells.\n"
	<< separator;
}

string CovOptimData::format() {
	stringstream ss;
	stack<unsigned int> format_id;
	stack<unsigned int> format_version;
	format(ss, "CovOptimData", format_id, format_version);
	return ss.str();
}

} // end namespace

