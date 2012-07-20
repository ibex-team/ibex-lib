#include "ibex.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

	// Load a system of equations
	// --------------------------
	System sys(argv[1]);
	double ratio_propag= atof(argv[2]);
	string filtering= argv[3];
        string bisection= argv[4];
	double prec= atof(argv[5]);
        double time_limit=atof(argv[6]);
	// Build contractor #1:
	// --------------------------
	// A "constraint propagation" loop.
	// Each constraint in sys.ctrs is an
	// equation.
	CtcHC4 hc4(sys.ctrs,ratio_propag);
	CtcHC4 hc44cid(sys.ctrs,0.1,true);
	//CtcHC4 hc44cid(sys.ctrs,0.1,false);
	hc4.accumulate=false;
	CtcAcid acidhc4(sys,BoolMask(sys.nb_var,1),hc44cid);
	Ctc3BCid c3bcidhc4(BoolMask(sys.nb_var,1),hc44cid,10,1,sys.nb_var);
	// Build contractor #2:
	// --------------------------
	// An interval Newton iteration
	// for solving f(x)=0 where f is
	// a vector-valued function representing
	// the system.
	CtcNewton newton(sys.f,2e8,prec,1.e-4);

	// Build the main contractor:
	// ---------------------------
	// A composition of the two previous
	// contractors
   
	CtcCompo cacid(hc4,acidhc4);
	CtcCompo c3bcid(hc4,c3bcidhc4);
	CtcCompo hc4n(hc4,newton);
	CtcCompo cacidn(cacid,newton);
	CtcCompo c3bcidn(c3bcid,newton);

	Ctc* ctc;
	if (filtering== "hc4")
	  ctc = & hc4;
	else if (filtering== "acidhc4")
	  ctc = & cacid;
	else if (filtering== "3bcidhc4")
	  ctc = &c3bcid;
	else if (filtering == "hc4n")
	  ctc = &hc4n;
	else if (filtering== "acidhc4n")
	  ctc = & cacidn;
	else if (filtering== "3bcidhc4n")
	  ctc = & c3bcidn;


	// Build the bisection heuristic
	// --------------------------
	// Round-robin means that the domain
	// of each variable is bisected in turn
	Bsc * bs;
	cout << "bisection " << bisection << endl;
	if (bisection=="roundrobin")
	  bs = new RoundRobin (prec);
	else if (bisection== "largestfirst")
          bs= new LargestFirst();
	else if (bisection=="smearsum")
	  bs = new SmearSum(sys,prec);
	else if (bisection=="smearmax")
	  bs = new SmearMax(sys,prec);
	else if (bisection=="smearsumrel")
	  bs = new SmearSumRelative(sys,prec);
	else if (bisection=="smearmaxrel")
	  bs = new SmearMaxRelative(sys,prec);
	  
	cout << " donnees lues " << endl;

	// Chose the way the search tree is explored
	// -------------------------------------
	// A "CellStack" means a depth-first search.
	CellStack buff;

	// Build a solver
	// -------------
	// The last parameter (1e-07) is the precision
	// required for the solutions

	Solver s(*ctc,*bs,buff,prec);
	s.time_limit = time_limit;;
	s.trace=1;  // solutions are printed 
	// Get the solutions
	vector<IntervalVector> sols=s.solve(sys.box);

	// Display the number of solutions
	cout << "number of solutions=" << sols.size() << endl;
	//	for (int i=0; i< sols.size() ; i++)
	//	  cout << " sol  "<< i+1 << " : " << sols[i] << endl;
	// Display the number of boxes (called "cells")
	// generated during the search
	cout << "number of cells=" << s.nb_cells << endl;
	if (filtering=="acidhc4" || filtering=="acidhc4n") 
	  cout << "nbvarcid=" <<  CtcAcid::nbvarstat << endl;
	cout << "cpu time used=" << s.time << "s."<< endl;

}



