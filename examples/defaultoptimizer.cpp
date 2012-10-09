#include "ibex.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv){
  try{
// Load a system of equations
	// --------------------------

    if (argc<3) {
		cerr << "usage: defaultoptimizer filename  goal_prec timelimit "  << endl;
		exit(1);
    }
    System sys(argv[1]);
    cout << "file " << argv[1] << endl;
    System ext_sys(sys,System::EXTEND);
    srandom(1);
    double prec= atof(argv[2]);  // the precision for splitting the boxes
    double goal_prec= atof(argv[3]);  // the required precision for the objective
    double time_limit=atof(argv[4]); 

    DefaultOptimizer o(sys,ext_sys,prec,goal_prec);
    o.timeout=time_limit;
    o.trace=1;  // each better feasible point is printed when it is found
    cout.precision(12);
    // Searching for the optimum
    o.optimize(sys.box);
    o.report();
    return 0;

  }
  catch(ibex::SyntaxError& e) {
    cout << e << endl;
  }
}
