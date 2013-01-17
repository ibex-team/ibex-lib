#include "ibex.h"

using namespace std;
using namespace ibex;


double convert(const char* argname, const char* arg) {
	char* endptr;
	double val = strtod(arg,&endptr);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "\"" << argname << "\" must be a real number";
		ibex_error(s.str().c_str());
	}
	return val;
}

int main(int argc, char** argv){
	try{

		// check the number of arguments
		if (argc<4) {
			ibex_error("usage: defaultsolver filename prec timelimit");
		}

		// Load a system of equations
		// --------------------------
		System sys(argv[1]);
		cout << "load file " << argv[1] << "." << endl;

		double prec       = convert("prec",argv[2]);
		double time_limit = convert("timelimit",argv[3]);

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
	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
