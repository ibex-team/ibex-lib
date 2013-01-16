#include "ibex.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

 try {
	// Load a system of equations
	// --------------------------
         if (argc<7) {
	   cerr << "usage: solver04 filename ratiopropag filtering xnewton bisection prec  timelimit "  << endl;
	   exit(1);
	 }

	// Load a system of equations
	// --------------------------
	System sys(argv[1]);
	double ratio_propag= atof(argv[2]);
	string filtering= argv[3];   // the main contractor (hc4|acidhc4|3bcidhc4|hc4n|acidhc4n|3bcidhc4n)
	string xnewton = argv[4];    // xn for the additional xnewton contractor
        string bisection= argv[5];   // the bisection heuristics
	double prec= atof(argv[6]);  // the required precision
        double time_limit=atof(argv[7]); // the time limit


	srand(1); // random used in corner choice in X_Newton
	// Build contractor #1:   hc4 acidhc4 or 3bcidhc4
	// --------------------------
	// A "constraint propagation" loop.
	// Each constraint in sys.ctrs is an
	// equation.
	CtcHC4 hc4(sys.ctrs,ratio_propag);
	CtcHC4 hc44cid(sys.ctrs,0.1,true);
	CtcAcid acidhc4(sys,BoolMask(sys.nb_var,1),hc44cid);
	Ctc3BCid c3bcidhc4(BoolMask(sys.nb_var,1),hc44cid,10,1,sys.nb_var);
	// Build contractor #2:
	// --------------------------
	// An interval Newton iteration
	// for solving f()=0 where f is
	// a vector-valued function representing
	// the system.
	CtcNewton* ctcnewton;
	if (filtering == "acidhc4n" || filtering=="hc4n" || filtering=="3bcidhc4n")
	  ctcnewton= new CtcNewton(sys.f,5e8,prec,1.e-4);

	// Build the main contractor:
	// ---------------------------
	// A composition of the  previous contractors
   
	CtcCompo cacid(hc4,acidhc4);
	CtcCompo c3bcid(hc4,c3bcidhc4);
	CtcCompo hc4n(hc4,*ctcnewton);
	CtcCompo cacidn(cacid,*ctcnewton);
	CtcCompo c3bcidn(c3bcid,*ctcnewton);

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
	else {cout << filtering <<  " is not an implemented  contraction  mode "  << endl; return -1;}

	// The X_newton contractor

	// corner selection 
	vector<X_Newton::corner_point> cpoints;
	//	cpoints.push_back(X_Newton::SUP_X);
	//	cpoints.push_back(X_Newton::INF_X);

	cpoints.push_back(X_Newton::RANDOM);
	cpoints.push_back(X_Newton::RANDOM_INV);
	CtcHC4 hc44xn(sys.ctrs,ratio_propag);  // the contractor called in the XNewton loop if the gain is > rfp2

       	X_Newton ctcxnewton (sys, &hc44xn, cpoints, -1,0,0.2,0.2, LR_contractor::ALL_BOX,X_Newton::HANSEN,100,1.e5,1.e4);
	
	// the actual contractor ; composition of ctc ,e.g. acidhc4n,  and Xnewton

	CtcCompo cxn (*ctc, ctcxnewton);

	Ctc* contractor;
        if (xnewton=="xn")
	  contractor= & cxn;
	else
	  contractor=ctc;
	  
	// Build the bisection heuristic
	// --------------------------
	Bsc * bs;
        cout << "file " << argv[1] << endl;
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
	else {cout << bisection << " is not an implemented  bisection mode "  << endl; return -1;} 
	
	// Choose the way the search tree is explored
	// -------------------------------------
	// A "CellStack" means a depth-first search.
	CellStack buff;

	// Build a solver
	// -------------
	// The last parameter (default value 1e-07) is the precision
	// required for the solutions

	Solver s(*contractor,*bs,buff,prec);
	s.time_limit = time_limit;;
	s.trace=1;  // solutions are printed as soon as found when trace=1
	// Get the solutions
	vector<IntervalVector> sols=s.solve(sys.box);

	// Display the number of solutions
	cout << "number of solutions=" << sols.size() << endl;
	//	for (int i=0; i< sols.size() ; i++)
	//	  cout << " sol  "<< i+1 << " : " << sols[i] << endl;
	// Display the number of boxes (called "cells")
	// generated during the search
	cout << "number of cells=" << s.nb_cells << endl;
	//  stats for acid :  Display the average number of "varcided"  variables if acid was called
	//	if (filtering=="acidhc4" || filtering=="acidhc4n") 
	//	  cout << "nbvarcid=" <<  CtcAcid::nbvarstat << endl;
	cout << "cpu time used=" << s.time << "s."<< endl;

 }

 catch(ibex::SyntaxError& e) {
   cout << e << endl;
 }

}


