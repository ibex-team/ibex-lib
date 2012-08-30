//============================================================================
//                                  I B E X                                   
// File        : optimizer01.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================


#include "ibex.h"

using namespace std;
using namespace ibex;
int main(int argc, char** argv){

	// Example #19
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


	System ext_sys(sys,System::EXTEND);
	int goal_var= ext_sys.nb_var-1;  // the last variable in ext_sys

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
	  bs = new SmearSum(ext_sys,prec);
	else if (bisection=="smearmax")
	  bs = new SmearMax(ext_sys,prec);
	else if (bisection=="smearsumrel")
	  bs = new SmearSumRelative(ext_sys,prec);
	else if (bisection=="smearmaxrel")
	  bs = new SmearMaxRelative(ext_sys,prec);


	// The contractors

	CtcHC4 hc4(ext_sys.ctrs,0.01,true);
	// hc4 inside acid
	CtcHC4 hc44cid(ext_sys.ctrs,0.1,true);
	// hc4 inside xnewton loop
	CtcHC4 hc44xn (ext_sys.ctrs,0.01,false);


	CtcAcid acidhc4(ext_sys,BoolMask(ext_sys.nb_var,1),hc44cid,10,1,1.e-8,0.005);
	Ctc3BCid c3bcidhc4(BoolMask(ext_sys.nb_var,1),hc44cid,10,1,ext_sys.nb_var);
	CtcCompo hc4acidhc4 (hc4, acidhc4);



	Ctc* ctc;



	if (filtering == "hc4")
	  ctc= &hc4;
	else if
	  (filtering =="acidhc4")
	  ctc = &hc4acidhc4;
	else if 
	  (filtering =="3bcidhc4")
	  ctc= &c3bcidhc4;


	// The X_Newton contractor
	// corner selection 
	vector<X_Newton::corner_point> cpoints;
	//		cpoints.push_back(X_Newton::SUP_X);
	//		cpoints.push_back(X_Newton::INF_X);
	cpoints.push_back(X_Newton::RANDOM);
	cpoints.push_back(X_Newton::RANDOM_INV);

	//	X_Newton ctcxnewton (ext_sys, ctc, cpoints, 0,sys.goal,0.2,0.2, X_Newton::X_NEWTON,X_Newton::HANSEN,100,1.e5,1.e5);
	X_Newton ctcxnewton (ext_sys, &hc44xn, cpoints, 0,sys.goal,0.2,0.2, X_Newton::X_NEWTON,X_Newton::HANSEN,100,1.e5,1.e5);

	//      X_Newton ctcxnewton (ext_sys, ctc , cpoints, 0,sys.goal,0.3,1,X_Newton::X_NEWTON,X_Newton::HANSEN,100,1.e5,1.e5);

	//  the actual contractor  ctc + xnewton	
	CtcCompo  cxn (*ctc, ctcxnewton);


	Optimizer o(sys,*bs,cxn,prec,goalprec,goalprec,1);

        cout << " sys.box " << sys.box << endl;
	Interval val = sys.goal->eval(sys.box);
	// for comparison with v1.20 benchmarks where y is bounded 
	// if (val.lb() < -1.e8) val = Interval(-1.e8,val.ub());

	ext_sys.box[goal_var]= val;
	cout << " y init " << ext_sys.box[goal_var] << endl;

	//o.in_HC4_flag=false;
	//o.mono_analysis_flag=false;
	o.trace=true;
	o.timeout=timelimit;
	o.optimize(sys.box);
     
	o.report();
	// ACID statistics
	if (filtering == "acidhc4"  )
	  cout    << " nbcidvar " <<  acidhc4.nbvar_stat() << endl;


	return 0;
	}


	catch(ibex::SyntaxError& e) {
	  cout << e << endl;
	}
}
