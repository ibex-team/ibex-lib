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

using namespace std;
using namespace ibex;
int main(int argc, char** argv){


	// ------------------------------------------------
	// Parameterized Optimizer (with a system loaded from a file, and choice of contractor and bisectors)
	// with X_Newton
        // Load a problem to optimize
	// --------------------------
	try {

	if (argc<7) {
		cerr << "usage: optimizer04 filename filtering bisection prec goal_prec timelimit "  << endl;
		exit(1);
	}

	System sys(argv[1]);
        string filtering = argv[2];
	string bisection= argv[3];
	double prec= atof(argv[4]);
	double goalprec= atof (argv[5]);
	double timelimit = atof(argv[6]);

	srand(1);
	// the extended system 
	System ext_sys(sys,System::EXTEND);

        cout << "file " << argv[1] << endl;

	// Build the bisection heuristic
	// --------------------------

	Bsc * bs;

	if (bisection=="roundrobin")
	  bs = new RoundRobin (prec);
	else if (bisection== "largestfirst")
          bs= new LargestFirst();
	else if (bisection=="smearsum")
	  bs = new SmearSum(ext_sys,prec);
	else if (bisection=="smearmax")
	  bs = new SmearMax(ext_sys,prec);
	else if (bisection=="smearsumrel")
	  bs = new SmearSumRelative(ext_sys,prec);
	else if (bisection=="smearmaxrel")
	  bs = new SmearMaxRelative(ext_sys,prec);
	else {cout << bisection << " is not an implemented  bisection mode "  << endl; return -1;}

	// The contractors

	// the first contractor called
	CtcHC4 hc4(ext_sys.ctrs,0.01,true);
	// hc4 inside acid and 3bcid : incremental propagation beginning with the shaved variable
	CtcHC4 hc44cid(ext_sys.ctrs,0.1,true);
	// hc4 inside xnewton loop 
	CtcHC4 hc44xn (ext_sys.ctrs,0.01,false);

	// The 3BCID contractor on all variables (component of the contractor when filtering == "3bcidhc4") 
	Ctc3BCid c3bcidhc4(hc44cid);
	// hc4 followed by 3bcidhc4 : the actual contractor used when filtering == "3bcidhc4" 
	CtcCompo hc43bcidhc4 (hc4, c3bcidhc4);

	// The ACID contractor (component of the contractor  when filtering == "acidhc4")
	CtcAcid acidhc4(ext_sys,hc44cid,true);
	// hc4 followed by acidhc4 : the actual contractor used when filtering == "acidhc4" 
	CtcCompo hc4acidhc4 (hc4, acidhc4);



	Ctc* ctc;
	if (filtering == "hc4")
	  ctc= &hc4;
	else if
	  (filtering =="acidhc4")   
	  ctc= &hc4acidhc4;
	else if 
	  (filtering =="3bcidhc4")
	  ctc= &hc43bcidhc4;
	else {cout << filtering <<  " is not an implemented  contraction  mode "  << endl; return -1;}

	// The CtcXNewtonIter contractor
	// corner selection for linearizations : two corners are slected, a random one and its opposite
	vector<CtcXNewtonIter::corner_point> cpoints;
	cpoints.push_back(CtcXNewtonIter::RANDOM);
	cpoints.push_back(CtcXNewtonIter::RANDOM_INV);

        // the linear relaxation contractor 
	CtcXNewtonIter ctcxnewton (ext_sys,cpoints, 0,sys.goal, CtcLinearRelaxation::ALL_BOX,CtcXNewtonIter::HANSEN,100,1.e6,1.e6);

	// fixpoint linear relaxation , hc4  with default fix point ratio 0.2
	CtcXNewton cxn (ctcxnewton, hc44xn);
	//  the actual contractor  ctc + xnewton	
        CtcCompo ctcxn (*ctc, cxn);
	// one point probed when looking for a new feasible point (updating the loup)
	int samplesize=1;

	// the optimizer : the same precision goalprec is used as relative and absolute precision
	Optimizer o(sys,*bs,ctcxn,prec,goalprec,goalprec,samplesize);

	cout << " sys.box " << sys.box << endl;

	// the trace 
	o.trace=1;
	// the allowed time for search
	o.timeout=timelimit;

	// the search itself 
	o.optimize(sys.box);

	// printing the results     
	o.report();
	return 0;
	}


	catch(ibex::SyntaxError& e) {
	  cout << e << endl;
	}
}
