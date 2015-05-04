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

// Some parameters are chosen to be not configurable for the moment
const bool HC4_INCREMENTAL = false;

int main(int argc, char** argv) {

	try {
		// Load a system of equations
		// --------------------------
		if (argc<8) {
			cerr << "usage: solver filename ratiopropag filtering linearrelaxation bisection prec  timelimit "  << endl;
			exit(1);
		}

		StrategyParam p(
				argv[1],                     // filename
				argv[3],                     // contractor
				argv[4],                     // linear_relaxation
				argv[5],                     // bisector
				_2dbl("prec",argv[6]),       // precision
				_2int("timelimit", argv[7]), // time limit
				HC4_INCREMENTAL,             // hc4 incremental?
				_2dbl("ratiopropag",argv[2]),// propagation ratio
				CtcFixPoint::default_ratio   // fixpoint ratio
		);

		// Choose the way the search tree is explored
		// -------------------------------------
		// A "CellStack" means a depth-first search.
		CellStack buff;

		Solver s(p.get_ctc(), p.get_bsc(), buff);

		s.time_limit = p.time_limit;
		s.trace=1;  // solutions are printed as soon as found when trace=1

		// Solve the system and get the solutions
		vector<IntervalVector> sols=s.solve(p.get_sys().box);

		// Display the number of solutions
		cout << "number of solutions=" << sols.size() << endl;
		// Display the solutions : they are already displayed when s.trace=1, otherwise the following lines can be used.
		//	for (int i=0; i< sols.size() ; i++)
		//	  cout << " sol  "<< i+1 << " : " << sols[i] << endl;

		// Display the number of boxes (called "cells")
		// generated during the search
		cout << "number of cells=" << s.nb_cells << endl;
		// Display the cpu time used
		cout << "cpu time used=" << s.time << "s."<< endl;

	}

	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}

}


