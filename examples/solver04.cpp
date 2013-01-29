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
        cout << "file " << argv[1] << endl;

	double ratio_propag= atof(argv[2]); // the contraction ratio used as stopping criterion for the hc4 propagation loop
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
	// the first contractor called
	CtcHC4 hc4(sys.ctrs,ratio_propag);
	// hc4 inside acid and 3bcid : incremental propagation beginning with the shaved variable
	CtcHC4 hc44cid(sys.ctrs,0.1,true);
	// The ACID contractor (component of the contractor  when filtering == "acidhc4" or "acidhc4n")
	CtcAcid acid(sys,hc44cid);
	// hc4 followed by 3bcidhc4 : the actual contractor used when filtering == "3bcidhc4" 
	CtcCompo hc4acid(hc4,acid);


	// The 3BCID contractor (3bcid component of the contractor when filtering == "3bcidhc4") on all variables
	Ctc3BCid c3bcid(hc44cid);
	// hc4 followed by 3bcidhc4 : the actual propagation based contractor used when filtering == "3bcidhc4" or "3bcidhc4n"
	CtcCompo hc43bcid(hc4,c3bcid);

	// Build contractor #2:
	// --------------------------
	// An interval Newton iteration
	// for solving f()=0 where f is
	// a vector-valued function representing
	// the system.
	CtcNewton* ctcnewton= NULL;
	if (filtering == "acidhc4n" || filtering=="hc4n" || filtering=="3bcidhc4n")
	  ctcnewton= new CtcNewton(sys.f,5e8,prec,1.e-4);


        // Build contractor #3
	//-----------------------------
	// A Linear relaxation contractor using a linear relaxation of the constraints and calling a linear programming solver
	// for contracting each domain bound 
	// The X_newton contractor

	// corner selection 
	vector<X_Newton::corner_point> cpoints;
	//	cpoints.push_back(X_Newton::SUP_X);  // selection of the superior corners : not used 
	//	cpoints.push_back(X_Newton::INF_X);  // selection of the inferior corners : not used

	// one random corner and its opposite 
	cpoints.push_back(X_Newton::RANDOM);
	cpoints.push_back(X_Newton::RANDOM_INV);
	// the hc4 contractor called in the XNewton loop if the gain is > rfp2 (here 0.2)
	CtcHC4 hc44xn(sys.ctrs,ratio_propag);  
	// XNewton contractor  (see Xnexton documentation for paramaters)
       	X_Newton ctcxnewton (sys, &hc44xn, cpoints, -1,0,0.2,0.2, LR_contractor::ALL_BOX,X_Newton::HANSEN,100,1.e5,1.e4);


	// Build the main contractor:
	// ---------------------------
	// A composition of the  previous contractors
   
	CtcCompo* hc4n;
	CtcCompo* hc4acidn;
	CtcCompo* hc43bcidn; 

	if (ctcnewton) { hc4n= new CtcCompo (hc4,*ctcnewton);
	  hc4acidn = new CtcCompo (hc4acid, *ctcnewton);
	  hc43bcidn = new CtcCompo (hc43bcid,*ctcnewton);
	}

	Ctc* ctc;
	if (filtering== "hc4")
	  ctc = & hc4;
	else if (filtering== "acidhc4")
	  ctc = & hc4acid;
	else if (filtering== "3bcidhc4")
	  ctc = &c3bcid;
	else if (filtering == "hc4n")
	  ctc = hc4n;
	else if (filtering== "acidhc4n")
	  ctc =  hc4acidn;
	else if (filtering== "3bcidhc4n")
	  ctc =  hc43bcidn;
	else {cout << filtering <<  " is not an implemented  contraction  mode "  << endl; return -1;}

	

	// the actual contractor ; composition of ctc ,e.g. acidhc4n,  and Xnewton
	CtcCompo cxn (*ctc, ctcxnewton);
	Ctc* contractor;

	// xnewton is optional and only used if the xnewton parameter is "xn"
        if (xnewton=="xn" ||xnewton=="xnewton" )
	  contractor= & cxn;
	else
	  contractor=ctc;
	  
	// Build the bisection heuristic
	// --------------------------
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

	// Solve the system and get the solutions
	vector<IntervalVector> sols=s.solve(sys.box);

	// Display the number of solutions
	cout << "number of solutions=" << sols.size() << endl;
	// Display the solutions : they are already displayed when s.trace=1, otherwise the following lines can be used.
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


