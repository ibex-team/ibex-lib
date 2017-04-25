//============================================================================
//                                  I B E X                                   
// File        : optimizer03.cpp
// Author      : Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 12, 2012
// Last Update : Jul 12, 2012
//============================================================================

#include "ibex.h"

#ifndef _IBEX_WITH_OPTIM_
#error "You need the plugin Optim to run this example."
#endif

using namespace std;
using namespace ibex;
int main(int argc, char** argv){

	// Example #19
	// ------------------------------------------------
	// Optimizer (with a system loaded from a file)
	//

	// Load a problem to optimize
	// --------------------------
	try {

		if (argc<2) {
			cerr << "usage: optimizer02 [filename]" << endl;
			exit(1);
		}

		cout << "file " << argv[1] << endl;

		System sys(argv[1]);
		string filtering = argv[2];
		string bisection= argv[3];
		double prec= atof(argv[4]);
		double goalprec= atof (argv[5]);


		ExtendedSystem ext_sys(sys);

		// Build the bisection heuristic
		// --------------------------
		// Round-robin means that the domain
		// of each variable is bisected in turn
		Bsc * bs;

		cout << "bisection " << bisection << endl;
		if (bisection=="roundrobin")
			bs = new RoundRobin (prec);
		else if (bisection== "largestfirst")
			bs= new LargestFirst(prec);
		else if (bisection=="smearsum")
			bs = new SmearSum(ext_sys,prec);
		else if (bisection=="smearmax")
			bs = new SmearMax(ext_sys,prec);
		else if (bisection=="smearsumrel")
			bs = new SmearSumRelative(ext_sys,prec);
		else if (bisection=="smearmaxrel")
			bs = new SmearMaxRelative(ext_sys,prec);

		Ctc *ctc;

		CtcHC4 hc4(ext_sys.ctrs,0.01);
		CtcHC4 hc44cid(ext_sys.ctrs,0.1,true);
		CtcAcid acidhc4(ext_sys,BitSet::all(sys.nb_var),hc44cid);
		Ctc3BCid c3bcidhc4(BitSet::all(sys.nb_var),hc44cid,10,1,ext_sys.nb_var);
		CtcCompo hc4acidhc4 (hc4, acidhc4);

		if (filtering == "hc4")
			ctc= &hc4;
		else if
		(filtering =="acidhc4")
			ctc = &hc4acidhc4;
		else if
		(filtering =="3bcidhc4")
			ctc= &c3bcidhc4;

		Optimizer o(sys,*ctc,*bs,prec,goalprec,goalprec,1);

		ext_sys.box[sys.nb_var]= sys.goal->eval(sys.box);
		cout << " y init " << ext_sys.box[sys.nb_var] << endl;

		//o.in_HC4_flag=false;
		o.mono_analysis_flag=false;
		o.trace=true;
		o.timeout=1000.0;
		o.optimize(sys.box);

		o.report();

		return 0;
	}


	catch(ibex::SyntaxError& e) {
		cout << e << endl;
	}
}
