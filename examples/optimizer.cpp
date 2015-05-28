//============================================================================
//                                  I B E X                                   
// File        : optimizer04.cpp
// Author      : Gilles Chabert  Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================

#include "ibex.h"
#include <sstream>

using namespace std;
using namespace ibex;

/*
 * Convert char* --> int
 */
int _2int(const char* argname, const char* arg) {
	char* endptr;
	int val = strtol(arg,&endptr,10);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "optimizer: " << argname << " must be an integer";
		ibex_error(s.str().c_str());
	}
	return val;
}

/*
 * Convert char* --> double
 */
double _2dbl(const char* argname, const char* arg) {
	char* endptr;
	double val = strtod(arg,&endptr);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "optimizer: " << argname << " must be a real number";
		ibex_error(s.str().c_str());
	}
	return val;
}

// ----------------------------------------------------------------------------
// Parameterized Optimizer (with a system loaded from a file, and choice of
//  contractor, linearization  and bisector)
// ----------------------------------------------------------------------------

// Some parameters are chosen to be not configurable for the moment
const bool HC4_INCREMENTAL = true;
const double PROPAG_RATIO = 0.01;
const double FIXPOINT_RATIO = 0.2;
const int SAMPLE_SIZE = 1;
const double EQ_EPS= 1.e-8;

int main(int argc, char** argv) {

	if (argc<8) {
		cerr << "usage: optimizer filename filtering linear_relaxation bisection prec goal_prec timelimit "  << endl;
		exit(1);
	}

	try {
		OptimizerParam p(
				argv[1],                     // filename
				argv[2],                     // contractor
				argv[3],                     // linear_relaxation
				argv[4],                     // bisector
				_2dbl("prec",argv[5]),       // precision
				_2int("timelimit", argv[7]), // time limit
				HC4_INCREMENTAL,             // hc4 incremental?
				PROPAG_RATIO,                // propagation ratio
				FIXPOINT_RATIO,              // fixpoint ratio
				_2dbl("rel_prec", argv[6]),  // goal relative precision
				_2dbl("rel_prec", argv[6]),  // goal absolute precision (= relative)
				SAMPLE_SIZE,                 // sample size
				EQ_EPS                       // thickness of equation
				);

		Optimizer o(p.get_sys(), p.get_ctc(), p.get_bsc(), p.prec, p.goal_rel_prec, p.goal_abs_prec, p.sample_size, p.eq_eps);

		o.timeout = p.time_limit;

		// the trace
		o.trace=1;

		// the search itself
		Optimizer::Status result=o.optimize(p.get_sys().box);

		// printing the results
		o.report();

		return (result==Optimizer::SUCCESS? 0 : -1);

	}

	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
