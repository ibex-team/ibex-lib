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



	System sys(argv[1]);
	string filtering = "acidhc4";
	string linearrelaxation= "xn";
	string bisection= "smearsumrel";
	double prec= 1.e-6;
	double goalprec= 1.e-6;
	double timelimit = 1000;
	double eqeps= 1.e-6;

	srand(1);



        cout << "file " << argv[1] << endl;

	// Build the bisection heuristic
	// --------------------------

	Bsc * bs;

	if (bisection=="roundrobin")
	  bs = new RoundRobin (prec);
	else if (bisection== "largestfirst")
          bs= new LargestFirst(prec);
	else if (bisection=="smearsum")
	  bs = new SmearSum(sys,prec);
	else if (bisection=="smearmax")
	  bs = new SmearMax(sys,prec);
	else if (bisection=="smearsumrel")
	  bs = new SmearSumRelative(sys,prec);
	else if (bisection=="smearmaxrel")
	  bs = new SmearMaxRelative(sys,prec);
	else {cout << bisection << " is not an implemented  bisection mode "  << endl; return -1;}

	// The contractors

	// the first contractor called
	CtcHC4 hc4(sys.ctrs,0.01,true);
	// hc4 inside acid and 3bcid : incremental propagation beginning with the shaved variable
	CtcHC4 hc44cid(sys.ctrs,0.1,true);
	// hc4 inside xnewton loop 
	CtcHC4 hc44xn (sys.ctrs,0.01,false);

	// The 3BCID contractor on all variables (component of the contractor when filtering == "3bcidhc4") 
	Ctc3BCid c3bcidhc4(sys.nb_var, hc44cid);
	// hc4 followed by 3bcidhc4 : the actual contractor used when filtering == "3bcidhc4" 
	CtcCompo hc43bcidhc4 (hc4, c3bcidhc4);

	// The ACID contractor (component of the contractor  when filtering == "acidhc4")
	CtcAcid acidhc4(sys,hc44cid,true);
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
	  lr= new LinearRelaxCombo(sys,LinearRelaxCombo::ART);
	else if  (linearrelaxation=="compo")
	  lr= new LinearRelaxCombo(sys,LinearRelaxCombo::COMPO);
	else if (linearrelaxation=="xn")
	  lr= new LinearRelaxXTaylor (sys,cpoints);
	//	else {cout << linearrelaxation  <<  " is not an implemented  linear relaxation mode "  << endl; return -1;}
	// fixpoint linear relaxation , hc4  with default fix point ratio 0.2
	CtcFixPoint* cxn;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn")
	  //cxn = new CtcLinearRelaxation (*lr, hc44xn);
		cxn = new CtcFixPoint (*new CtcCompo(*new CtcPolytopeHull(*lr, CtcPolytopeHull::ALL_BOX), hc44xn), default_relax_ratio);

	//  the actual contractor  ctc + linear relaxation 
	Ctc* ctc_out;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn")
		ctc_out= new CtcCompo  (*ctc, *cxn);
	else
		ctc_out = ctc;



	// ctc_in
	NormalizedSystem norm_sys(sys,eqeps);

	vector<Ctc*> array_in;
	for (int i=0; i<norm_sys.nb_ctr; i++) {
		array_in.push_back(new CtcFwdBwd(norm_sys.ctrs[i].f,GT));

	}
	CtcUnion ctc_in2(array_in);
	Ctc3BCid ctc_in(ctc_in2);

	OptimCtc octc(*ctc_out,ctc_in,*(sys.goal),*bs,prec,goalprec,goalprec);

	cout << " sys.box " << sys.box << endl;

	// the trace 
	octc.trace=1;

	// the allowed time for search
	octc.timeout=timelimit;

	// the search itself 
	octc.optimize(sys.box);

	// printing the results     
	octc.report();


	//	if (filtering == "acidhc4"  )
	//cout    << " nbcidvar " <<  acidhc4.nbvar_stat() << endl;

	delete bs;
	if (linearrelaxation=="xn" ||linearrelaxation=="compo" || linearrelaxation=="art" )
	  {delete lr;
	    delete ctc_out;
	    delete cxn;
	  }



	return 0;
	
	}


	catch(ibex::SyntaxError& e) {
	  cout << e << endl;
	}
}
