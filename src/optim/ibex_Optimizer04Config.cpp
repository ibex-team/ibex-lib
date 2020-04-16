//============================================================================
//                                  I B E X                                   
// File        : ibex_Optimizer04Config.cpp
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Dec 11, 2014
// Last Update : Oct 15, 2019
//============================================================================

#include "ibex_Optimizer04Config.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_Ctc3BCid.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcNewton.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcKuhnTucker.h"
#include "ibex_OptimLargestFirst.h"
#include "ibex_RoundRobin.h"
#include "ibex_SmearFunction.h"
#include "ibex_LSmear.h"
#include "ibex_Random.h"
#include "ibex_NormalizedSystem.h"
#include "ibex_LinearizerXTaylor.h"
#include "ibex_LinearizerCompo.h"
#include "ibex_CellHeap.h"
#include "ibex_CellDoubleHeap.h"
#include "ibex_CellBeamSearch.h"
#include "ibex_LoupFinderDefault.h"
#include "ibex_SyntaxError.h"

#ifdef _IBEX_WITH_AFFINE_
#include "ibex_LinearizerAffine2.h"
#endif

#ifdef _IBEX_WITH_AMPL_
#include "ibex_AmplInterface.h"
#endif

#include <sstream>
#include <vector>

using namespace std;

namespace ibex {

Optimizer04Config::Optimizer04Config(int argc, char** argv) {

	try {
		if (argc<8) {
			ibex_error("usage: optimizer04 filename filtering linear_relaxation bisection strategy [beamsize] prec goal_prec timelimit randomseed");
		}
#ifdef _IBEX_WITH_AMPL_
		std::size_t found = string(argv[1]).find(".nl");
		if (found!=std::string::npos) {
			AmplInterface interface (argv[1]);
			sys = &rec(new System(interface));
		} else
			sys = &rec(new System(argv[1]));
#else
		sys = &rec(new System(argv[1]));
#endif
	} catch(SyntaxError& e) {
		ibex_error(e.msg.c_str());
	}

	filtering = argv[2];
	linearrelaxation = argv[3];
	bisection = argv[4];
	strategy = argv[5];

	int nbinput=5;

	beamsize = -1;
	if (strategy=="bs" || strategy== "beamsearch") { beamsize=atoi(argv[6]); nbinput++; }

	double prec= atof(argv[nbinput+1]);
	double goalprec= atof (argv[nbinput+2]);
	double timelimit= atof(argv[nbinput+3]);
	int randomseed = atoi(argv[nbinput+4]);

	RNG::srand(randomseed);

	// the extended system
	double eqeps=1e-8;
	ext_sys = &rec(new ExtendedSystem(*sys,eqeps));
	norm_sys = &rec(new NormalizedSystem(*sys,eqeps));

	/*
	cout << "file " << argv[1] << endl;
	cout << " filtering " << filtering;
        cout << " linearrelaxation " << linearrelaxation;
	cout << " bisection " << bisection ;
	cout << " strategy " << strategy ;
	cout << " randomseed " << randomseed << endl;
	 */

	set_eps_x(prec);
	set_rel_eps_f(goalprec);
	set_abs_eps_f(goalprec);

	// the optimizer : the same precision goalprec is used as relative and absolute precision
	//	Optimizer o(sys->nb_var,*ctcxn,*bs,loupfinder,*buffer,ext_sys.goal_var(),1.e-7,goalprec,goalprec);
	// the trace
	set_trace(0);

	// the allowed time for search
	set_timeout(timelimit);
}

unsigned int Optimizer04Config::nb_var() {
	return sys->nb_var;
}


Linearizer* Optimizer04Config::get_linear_relax() {
	Linearizer* lr;

	if (linearrelaxation=="art")
#ifdef _IBEX_WITH_AFFINE_
		lr = &rec(new LinearizerAffine2(*ext_sys));
#else
	ibex_error("[Optimizer04Config]: ART mode requires \"--with-affine\" option");
#endif
	else if (linearrelaxation=="compo")
#ifdef _IBEX_WITH_AFFINE_
		lr = &rec(new LinearizerCompo(
				rec(new LinearizerXTaylor(*ext_sys)),
				rec(new LinearizerAffine2(*ext_sys))));
#else
	ibex_error("[Optimizer04Config]: COMPO mode requires \"--with-affine\" option");
#endif
	else if (linearrelaxation=="xn")
		lr = &rec(new LinearizerXTaylor(*ext_sys));
/*	else {
			stringstream ss;
			ss << "[optimizer04] " << linearrelaxation << " is not an implemented relaxation mode ";
			ibex_error(ss.str().c_str());
		}
*/

	return lr;
}

Ctc& Optimizer04Config::get_ctc() {

	// the first contractor called
	CtcHC4* hc4 = &rec(new CtcHC4(ext_sys->ctrs,0.01,true));
	// hc4 inside acid and 3bcid : incremental propagation beginning with the shaved variable
	CtcHC4* hc44cid = &rec(new CtcHC4(ext_sys->ctrs,0.1,true));
	// hc4 inside xnewton loop
	CtcHC4* hc44xn = &rec(new CtcHC4(ext_sys->ctrs,0.01,false));

	// The 3BCID contractor on all variables (component of the contractor when filtering == "3bcidhc4")
	Ctc3BCid* c3bcidhc4 = &rec(new Ctc3BCid(*hc44cid));
	// hc4 followed by 3bcidhc4 : the actual contractor used when filtering == "3bcidhc4"
	CtcCompo* hc43bcidhc4 = &rec(new CtcCompo(*hc4, *c3bcidhc4));

	// The ACID contractor (component of the contractor  when filtering == "acidhc4")
	CtcAcid* acidhc4 = &rec(new CtcAcid(*ext_sys, *hc44cid, true));
	// hc4 followed by acidhc4 : the actual contractor used when filtering == "acidhc4"
	CtcCompo* hc4acidhc4 = &rec(new CtcCompo(*hc4, *acidhc4));

	Ctc* ctc;
	if (filtering == "hc4")
		ctc = hc4;
	else if (filtering =="acidhc4")
		ctc = hc4acidhc4;
	else if (filtering =="3bcidhc4")
		ctc = hc43bcidhc4;
	else {
		stringstream ss;
		ss << "[optimizer04] " << filtering << " is not an implemented  contraction mode ";
		ibex_error(ss.str().c_str());
	}

	Linearizer* lr = get_linear_relax();
	Ctc* cxn;
	CtcPolytopeHull* cxn_poly;
	CtcCompo* cxn_compo;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn") {
		cxn_poly = &rec(new CtcPolytopeHull(*lr));
		cxn_compo = &rec(new CtcCompo(*cxn_poly, *hc44xn));
	        if (sys->nb_ctr==0)
		  cxn = cxn_poly;
		else
		  cxn = &rec(new CtcFixPoint (*cxn_compo, relax_ratio));
	}
	//  the actual contractor  ctc + linear relaxation
	Ctc* ctcxn;
	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn")
		ctcxn = &rec(new CtcCompo  (*ctc, *cxn));
	else
		ctcxn = ctc;
	Ctc* ctckkt;

	if (sys->nb_ctr == 0){
	  ctckkt = &rec(new CtcKuhnTucker(*norm_sys, true));
	  ctcxn = &rec(new CtcCompo  (*ctcxn, *ctckkt));
	  }

	return *ctcxn;
}

Bsc& Optimizer04Config::get_bsc() {
	Bsc* bs;

	double prec = get_eps_x();

	if (bisection=="roundrobin")
		bs = &rec(new RoundRobin (prec,0.5));
	else if (bisection== "largestfirst")
                bs = &rec(new OptimLargestFirst(ext_sys->goal_var(),true,prec,0.5));
	else if (bisection== "largestfirstnoobj")
                bs = &rec(new OptimLargestFirst(ext_sys->goal_var(),false,prec,0.5));
	else if (bisection=="smearsum")
		bs = &rec(new SmearSum(*ext_sys,prec,
				       rec(new OptimLargestFirst(ext_sys->goal_var(),true,prec,0.5))));
	else if (bisection=="smearmax")
		bs = &rec(new SmearMax(*ext_sys,prec,
				       rec(new OptimLargestFirst(ext_sys->goal_var(),true,prec,0.5))));
	else if (bisection=="smearsumrel")
                bs = &rec(new SmearSumRelative(*ext_sys,prec,
					       rec(new OptimLargestFirst(ext_sys->goal_var(),true,prec,0.5))));
	else if (bisection=="smearmaxrel")
		bs = &rec(new SmearMaxRelative(*ext_sys,prec,
					       rec(new OptimLargestFirst(ext_sys->goal_var(),true,prec,0.5))));
	else if  (bisection=="lsmear")
                bs = &rec (new LSmear(*ext_sys,prec,
				      rec(new OptimLargestFirst(ext_sys->goal_var(),true,prec,0.5)),
				      LSMEAR));
	else if (bisection=="lsmearmg")
	        bs = &rec (new LSmear(*ext_sys,prec,
				      rec(new OptimLargestFirst(ext_sys->goal_var(),true,prec,0.5))));
	else {
		stringstream ss;
		ss << "[optimizer04] " << bisection << " is not an implemented  bisection mode ";
		ibex_error(ss.str().c_str());
	}

	return *bs;
}


LoupFinder& Optimizer04Config::get_loup_finder() {
	return rec(new LoupFinderDefault(*norm_sys, true));
	//LoupFinderDefault loupfinder (norm_sys,false);
}

CellBufferOptim& Optimizer04Config::get_cell_buffer() {
	CellBufferOptim* buffer;

	CellHeap* futurebuffer = &rec(new CellHeap(*ext_sys));
	CellHeap* currentbuffer = &rec(new CellHeap(*ext_sys));

	//cout << "strategy=[" << strategy << "]\n";
	if (strategy=="bfs")
		buffer = &rec(new CellHeap(*ext_sys));
	else if (strategy=="dh")
		buffer = &rec(new CellDoubleHeap(*ext_sys));
	else if (strategy=="bs")
		buffer = &rec(new CellBeamSearch(*currentbuffer, *futurebuffer, *ext_sys, beamsize));

	return *buffer;
}

int Optimizer04Config::goal_var() {
	return ext_sys->goal_var();
}

} // namespace ibex
