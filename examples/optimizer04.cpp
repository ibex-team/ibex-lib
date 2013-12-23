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

const double default_relax_ratio = 0.2;

using namespace std;
using namespace ibex;
int main(int argc, char** argv){


	// ------------------------------------------------
	// Parameterized Optimizer (with a system loaded from a file, and choice of contractor, linearization  and bisector)
        // Load a problem to optimize
	// --------------------------
	try {

	if (argc<8) {
		cerr << "usage: optimizer04 filename filtering linear_relaxation bisection prec goal_prec timelimit "  << endl;
		exit(1);
	}

	System sys(argv[1]);
	string filtering = argv[2];
	string linearrelaxation= argv[3];
	string bisection= argv[4];
	double prec= atof(argv[5]);
	double goalprec= atof (argv[6]);
	double timelimit = atof(argv[7]);
	
	srand(1);

	// the extended system 
	ExtendedSystem ext_sys(sys,goalprec);


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

	// The CtcXNewton contractor
	// corner selection for linearizations : two corners are selected, a random one and its opposite
	vector<LinearRelaxXTaylor::corner_point> cpoints;
	cpoints.push_back(LinearRelaxXTaylor::RANDOM);
	cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);

	LinearRelax* lr;
	if (linearrelaxation=="art")
	  lr= new LinearRelaxCombo(ext_sys,LinearRelaxCombo::ART);
	else if  (linearrelaxation=="compo")
	  lr= new LinearRelaxCombo(ext_sys,LinearRelaxCombo::COMPO);
	else if (linearrelaxation=="xn")
	  lr= new LinearRelaxXTaylor (ext_sys,cpoints);
	//	else {cout << linearrelaxation  <<  " is not an implemented  linear relaxation mode "  << endl; return -1;}
	// fixpoint linear relaxation , hc4  with default fix point ratio 0.2
	CtcFixPoint* cxn;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn")
	  //cxn = new CtcLinearRelaxation (*lr, hc44xn);
		cxn = new CtcFixPoint (*new CtcCompo(*new CtcPolytopeHull(*lr, CtcPolytopeHull::ALL_BOX), hc44xn), default_relax_ratio);
	//  the actual contractor  ctc + linear relaxation 
	Ctc* ctcxn;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn")
          ctcxn= new CtcCompo  (*ctc, *cxn); 
	else
	  ctcxn = ctc;
	// one point probed when looking for a new feasible point (updating the loup)
	int samplesize=1;

	// the optimizer : the same precision goalprec is used as relative and absolute precision
	Optimizer o(sys,*bs,*ctcxn,prec,goalprec,goalprec,samplesize);

	cout << " sys.box " << sys.box << endl;

	// the trace 
	o.trace=1;

	// the allowed time for search
	o.timeout=timelimit;

	// the search itself 
	o.optimize(sys.box);

	// printing the results     
	o.report();


	//	if (filtering == "acidhc4"  )
	//cout    << " nbcidvar " <<  acidhc4.nbvar_stat() << endl;

	delete bs;
	if (linearrelaxation=="xn" ||linearrelaxation=="compo" || linearrelaxation=="art" )
	  {delete lr;
	    delete ctcxn;
	    delete cxn;}



	return 0;
	
	}


	catch(ibex::SyntaxError& e) {
	  cout << e << endl;
	}
}
