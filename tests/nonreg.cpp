//============================================================================
//                                  I B E X                                   
// File        : Non regression tests
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 3, 2014
//============================================================================

#include "ibex.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;
using namespace ibex;

const int NB_TESTS = 24;

const double TIME_LIMIT = 10;
const char* time_limit = "10";

const char* sample_size = "1";
const char* eq_eps      = "1e-08";

const double REGRESSION_RATIO = 1.1;

double _2dbl(const char* argname, const char* arg) {
	char* endptr;
	double val = strtod(arg,&endptr);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "error: " << argname << " must be a real number";
		ibex_error(s.str().c_str());
	}
	return val;
}

int _2int(const char* argname, const char* arg) {
	char* endptr;
	int val = strtol(arg,&endptr,10);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "UserFriendlyOptimizer: " << argname << " must be an integer";
		ibex_error(s.str().c_str());
	}
	return val;
}

int main (int argc, char** argv) {

	ifstream data;
	ifstream res;
	string word;

	// check the number of arguments
	if (argc<2) {
		ibex_error("usage: nonreg [file containing expected results ]");
	}

	data.open("nonreg.dat");
	res.open(argv[1]);

	const char* args[10];

	bool all_success=true;

	for (int n=0; n<NB_TESTS; n++) {
		for (int i=0; i<6; i++) {
			data >> word;
			args[i]=strdup(word.c_str());
		}
		cout << args[0] << "\t";

		args[6]=args[5];
		args[7]=sample_size;
		args[8]=time_limit;
		args[9]=eq_eps;

		bool ok=false;

		data >> word;
		double lb=_2dbl("objective lb",word.c_str());
		data >> word;
		double ub=_2dbl("objective ub",word.c_str());
		res >> word;
		double time=_2dbl("time",word.c_str());
		res >> word;
		int nb_cells=_2int("nb cells", word.c_str());

		if (time > TIME_LIMIT) { // too long... skip this bench
			for (int i=0; i<6; i++) free((char*) args[i]);
			cout << " (skip)" << endl;
			continue;
		}

		UserFriendlyOptimizer o(args);

		Optimizer::Status status=o.optimize();

		switch (status) {
		case Optimizer::INFEASIBLE :         cerr << "FAILED: infeasible"; break;
		case Optimizer::NO_FEASIBLE_FOUND :  cerr << "FAILED: no feasible point found"; break;
		case Optimizer::UNBOUNDED_OBJ :      cerr << "FAILED: unbounded objective"; break;
		case Optimizer::TIME_OUT :           cerr << "FAILED: timeout"; break;
		case Optimizer::SUCCESS : {
			if (o.loup < lb)                   {  cerr << "FAILED: upper bound (loup) is wrong"; }
			else if (o.uplo > ub)              {  cerr << "FAILED: lower bound (uplo) is wrong"; }
			else if (o.time > REGRESSION_RATIO*time)
			                                   {  cerr << "FAILED: time (" << o.time << "s) exceeds by more than 50% the reference time"; }
			else if (o.nb_cells> REGRESSION_RATIO*nb_cells)
			                                   {  cerr << "FAILED: number of cells (" << o.nb_cells << ") exceeds by more than 10% the reference value"; }
			else                               {  ok=true; cout << "SUCCESS"; }
		}
		}

		cout << endl;

		all_success &= ok;

		for (int i=0; i<6; i++)
			free((char*) args[i]);
	}

	return all_success ? 0 : -1;
}
