#include "ibex.h"

using namespace std;
using namespace ibex;

int main(int argc, char** argv) {

 try {
	// Load a system of equations
	// --------------------------
         if (argc<8) {
	   cerr << "usage: solver04 filename ratiopropag filtering linearrelaxation bisection prec  timelimit "  << endl;
	   exit(1);
	 }

	// Load a system of equations
	// --------------------------
	System sys(argv[1]);
        cout << "file " << argv[1] << endl;

	double ratio_propag= atof(argv[2]); // the contraction ratio used as stopping criterion for the hc4 propagation loop
	string filtering= argv[3];   // the main contractor (hc4|acidhc4|3bcidhc4|hc4n|acidhc4n|3bcidhc4n)
	string linearrelaxation = argv[4];    // the linearrelaxation contractor
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
	// The 3BCID contractor (3bcid component of the contractor when filtering == "3bcidhc4") on all variables
	Ctc3BCid c3bcid(hc44cid);



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
	// The LR contractor


	// The CtcXNewton contractor (if linearrelaxation=="xn")
	// corner selection for linearizations : two corners are selected, a random one and its opposite
	vector<CtcXNewton::corner_point> cpoints;
	cpoints.push_back(CtcXNewton::RANDOM);
	cpoints.push_back(CtcXNewton::RANDOM_INV);


	// the linear relaxation contractor 
	CtcLinearRelaxationIter* ctclriter=NULL;
	if (linearrelaxation=="art")
	  ctclriter = new CtcLR(sys, CtcLinearRelaxationIter::ALL_BOX, CtcLR::ART);
	else if (linearrelaxation=="compo")
	  ctclriter = new CtcLR(sys, CtcLinearRelaxationIter::ALL_BOX, CtcLR::COMPO);
	else   if (linearrelaxation=="xn") 
	  ctclriter = new CtcXNewton (sys,cpoints);

	// linearrelaxation is optional and only used if the linearrelaxation parameter is xn, art or compo
	// we build a fixpoint linear relaxation with ctclr and hc44xn  with default fix point ratio 0.2
	// the hc4 contractor called in the following fixpoint contractor
	CtcHC4 hc44xn(sys.ctrs,ratio_propag);  


	CtcLinearRelaxation* ctclr=NULL;
	if (linearrelaxation=="xn" ||linearrelaxation=="compo" || linearrelaxation=="art" )
	  ctclr= (new CtcLinearRelaxation (*ctclriter, hc44xn));

   

	// Build the main contractor:
	// ---------------------------
	// A composition of the  previous contractors
   

	Ctc* ctc;
	if (filtering== "hc4")
	  ctc = new CtcHC4 (sys.ctrs,ratio_propag);
	else if (filtering== "acidhc4")
	  ctc = new CtcCompo(hc4,acid);
	else if (filtering== "3bcidhc4")
	  ctc = new CtcCompo (hc4,c3bcid);
	else if (filtering == "hc4n")
	  ctc = new CtcCompo (hc4,*ctcnewton);
	else if (filtering== "acidhc4n")
	  ctc =  new CtcCompo (hc4,acid, *ctcnewton);
	else if (filtering== "3bcidhc4n")
	  ctc =  new CtcCompo (hc4,c3bcid,*ctcnewton);
	else {cout << filtering <<  " is not an implemented  contraction  mode "  << endl; return -1;}
        cout << "filtering " << filtering << endl;
	
	
	// The actual contractor
	Ctc* contractor;

	if (linearrelaxation=="xn" ||linearrelaxation=="compo" || linearrelaxation=="art" )
	  contractor = new CtcCompo (*ctc, *ctclr);
	else
	  contractor=ctc;
	  
	// Build the bisection heuristic
	// --------------------------
	Bsc * bs;


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
 
	cout << "bisection " << bisection << endl;	
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
	// Display the cpu time used
	cout << "cpu time used=" << s.time << "s."<< endl;

	//	if (filtering == "acidhc4" || filtering=="acidhc4n" )
	//	  cout    << " nbcidvar " <<  acid.nbvar_stat() << endl;
	delete bs;
	delete ctc;
	if (linearrelaxation=="xn" ||linearrelaxation=="compo" || linearrelaxation=="art" )
	  {delete contractor; 
	    delete ctclriter;
	    delete ctclr;
	  }
	if (ctcnewton) delete ctcnewton;
	


 }

 catch(ibex::SyntaxError& e) {
   cout << e << endl;
 }

}


