#include "ibex.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv){

// Load a system of equations
	// --------------------------
	System sys(argv[1]);
	srandom(1);
	double prec= atof(argv[2]);  // the required precision
        double time_limit=atof(argv[3]); 

	DefaultSolver s(sys,prec);
	s.time_limit=time_limit;
	s.trace=1;  // the solutions are printed when they are found
	cout.precision(12);
	// Get the solutions
	vector<IntervalVector> sols=s.solve(sys.box);
	cout << "number of solutions=" << sols.size() << endl;

	cout << "cpu time used=" << s.time << "s."<< endl;
	cout << "number of cells=" << s.nb_cells << endl;
}
