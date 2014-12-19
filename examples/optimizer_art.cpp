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
	// Parameterized Optimizer (with a system loaded from a file, and choice of contractor and bisector)
	// with X_Newton
	// Load a problem to optimize
	// --------------------------
	try {


		System sys(argv[1]);
		double prec= 1.e-8;
		double goalprec= 1.e-8;
		double timelimit = 3600;

		RNG::srand(1);

		// the extended system
		ExtendedSystem ext_sys(sys);


		cout << argv[1] << " & ";

		// Build the bisection heuristic
		// --------------------------

		Bsc * bs = new SmearSumRelative(ext_sys,prec);

		// The contractors

		// the first contractor called
		CtcHC4 hc4(ext_sys.ctrs,0.001,true);
		// hc4 inside acid and 3bcid : incremental propagation beginning with the shaved variable
		CtcHC4 hc44cid(ext_sys.ctrs,0.001,true);
		// hc4 inside xnewton loop
		CtcHC4 hc44xn (ext_sys.ctrs,0.001,false);

		// The 3BCID contractor on all variables (component of the contractor when filtering == "3bcidhc4")
		Ctc3BCid c3bcidhc4(ext_sys.nb_var, hc44cid);
		// hc4 followed by 3bcidhc4 : the actual contractor used when filtering == "3bcidhc4"
		CtcCompo hc43bcidhc4 (hc4, c3bcidhc4);

		// The ACID contractor (component of the contractor  when filtering == "acidhc4")
		CtcAcid acidhc4(ext_sys,hc44cid,true);
		// hc4 followed by acidhc4 : the actual contractor used when filtering == "acidhc4"
		CtcCompo hc4acidhc4 (hc4, acidhc4);



		Ctc* ctc= &hc4acidhc4;

		// The CtcXNewton contractor
		// corner selection for linearizations : two corners are selected, a random one and its opposite
		vector<LinearRelaxXTaylor::corner_point> cpoints;
		cpoints.push_back(LinearRelaxXTaylor::RANDOM);
		cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);

		LinearRelax* lr= new LinearRelaxCombo(ext_sys,LinearRelaxCombo::ART);

		//	else {cout << linearrelaxation  <<  " is not an implemented  linear relaxation mode "  << endl; return -1;}
		// fixpoint linear relaxation , hc4  with default fix point ratio 0.2
		CtcFixPoint* cxn = new CtcFixPoint (*new CtcCompo(*new CtcPolytopeHull(*lr, CtcPolytopeHull::ALL_BOX), hc44xn), default_relax_ratio);
		//  the actual contractor  ctc + linear relaxation
		Ctc* ctcxn = new CtcCompo  (*ctc, *cxn);
		// one point probed when looking for a new feasible point (updating the loup)
		int samplesize=1;

		// the optimizer : the same precision goalprec is used as relative and absolute precision
		Optimizer o(sys,*ctcxn,*bs,prec,goalprec,goalprec,samplesize);

		//	cout << " sys.box " << sys.box << endl;

		// the trace
		o.trace=0;
		o.in_HC4_flag= 1;

		// the allowed time for search
		o.timeout=timelimit;

		// the search itself
		o.optimize(sys.box);

		// printing the results
		o.report_perf();


		//	if (filtering == "acidhc4"  )
		//cout    << " nbcidvar " <<  acidhc4.nbvar_stat() << endl;

		delete bs;
		delete lr;
		delete ctcxn;
		delete cxn;



		return 0;

	}


	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
