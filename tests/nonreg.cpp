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

const double TIME_LIMIT = 500;
const char* time_limit = "500";

const double REGRESSION_CELLS_RATIO = 1.1;
const double REGRESSION_TIME_RATIO = 1.4;

// Some parameters are chosen to be not configurable for the moment
const bool HC4_INCREMENTAL = true;
const double PROPAG_RATIO = 0.01;
const double FIXPOINT_RATIO = 0.2;
const int SAMPLE_SIZE = 1;
const double EQ_EPS= 1.e-8;


double _2dbl(const char* argname, const char* arg) {
	char* endptr;
	double val = strtod(arg,&endptr);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "nonreg: " << argname << " must be a real number";
		ibex_error(s.str().c_str());
	}
	return val;
}

int _2int(const char* argname, const char* arg) {
	char* endptr;
	int val = strtol(arg,&endptr,10);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "nonreg: " << argname << " must be an integer";
		ibex_error(s.str().c_str());
	}
	return val;
}


int main (int argc, char** argv) {

	ifstream data;
	ifstream res;
	string word;
	string filename, contractor, linear_relax, bisector;
	double prec, goal_prec;

	// check the number of arguments
	if (argc<2) {
		ibex_error("usage: nonreg [file containing expected results ]");
	}

	res.open(argv[1]);

	// ==================================================================================
	double ref_perf, current_perf;

	res >> word; ref_perf=_2dbl("ref perf time", word.c_str());

	cout << "measuring machine performance..." << endl;
	cout << "\treference time=" << ref_perf << endl;

	RNG::srand(1);
	volatile double a=RNG::rand();
	volatile double b=RNG::rand();
	Timer::start();
	for (int i=0; i<100000000; i++) {
		a *= b;
	}
	Timer::stop();
	current_perf = Timer::VIRTUAL_TIMELAPSE();

	cout << "\tcurrent time=" << current_perf << endl;
	double ratio_perf = current_perf/ref_perf;
	cout << "\tratio=" << ratio_perf << endl;
	// ==================================================================================

	data.open("nonreg.dat");


	bool all_success=true;

	for (int n=0; n<NB_TESTS; n++) {

		data >> filename;
		data >> contractor;
		data >> linear_relax;
		data >> bisector;
		data >> word; prec=_2dbl("prec",word.c_str());
		data >> word; goal_prec=_2dbl("goal_prec",word.c_str());

		OptimizerParam p(
					filename.c_str(), contractor.c_str(), linear_relax.c_str(), bisector.c_str(), prec,
					TIME_LIMIT, HC4_INCREMENTAL, PROPAG_RATIO, FIXPOINT_RATIO, goal_prec, goal_prec,
					SAMPLE_SIZE, EQ_EPS
					);

		cout << filename << "\t";

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
			cout << " (skip)" << endl;
			continue;
		}

		Optimizer o(p.get_sys(), p.get_ctc(), p.get_bsc(), p.prec, p.goal_rel_prec, p.goal_abs_prec, p.sample_size, p.eq_eps);

		Optimizer::Status status=o.optimize(p.get_sys().box);

		//cerr << "number of cells=" << o.nb_cells << " time=" << o.time << endl;
		switch (status) {
		case Optimizer::INFEASIBLE :         cerr << "FAILED: infeasible"; break;
		case Optimizer::NO_FEASIBLE_FOUND :  cerr << "FAILED: no feasible point found"; break;
		case Optimizer::UNBOUNDED_OBJ :      cerr << "FAILED: unbounded objective"; break;
		case Optimizer::TIME_OUT :           cerr << "FAILED: timeout"; break;
		case Optimizer::SUCCESS : {
			if (o.loup < lb)                   {  cerr.precision(20); cerr << "FAILED: upper bound (loup=" << o.loup << ") is wrong"; }
			else if (o.uplo > ub)              {  cerr.precision(20); cerr << "FAILED: lower bound (uplo=" << o.uplo << ") is wrong"; }
			else if (o.time > REGRESSION_TIME_RATIO*time)
			                                   {  cerr << "FAILED: time (" << o.time << "s) exceeds by more than 10% the reference time"; }
			else if (o.nb_cells> REGRESSION_CELLS_RATIO*nb_cells)
			                                   {  cerr << "FAILED: number of cells (" << o.nb_cells << ") exceeds by more than 10% the reference value"; }
			else                               {  ok=true; cout << "SUCCESS (time=" << o.time << " nb cells=" << o.nb_cells << ")"; }
		}
      }

		cout << endl;

		all_success &= ok;
	}

	return all_success ? 0 : -1;
}
