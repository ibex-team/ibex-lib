//============================================================================
//                                  I B E X                                   
// File        : ibex_UserFriendlyOptimizer.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 4, 2014
//============================================================================

#include "ibex_UserFriendlyOptimizer.h"

#include "ibex_Optimizer.h"
#include "ibex_DefaultStrategy.cpp_"

using namespace std;

namespace ibex {

namespace {

enum { FILENAME, CTC, LIN_RELAX, BSC, PREC, REL_PREC, ABS_PREC, SAMPLE_SIZE, TIME_LIMIT, EQ_EPS };

int _2int(const char* argname, const char* arg) {
	char* endptr;
	int val = strtol(arg,&endptr,10);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "UserFriendlyOptimizer: " << argname << " must be an integer";
		ibex_error(s.str().c_str());
	}
	return val;
}

double _2dbl(const char* argname, const char* arg) {
	char* endptr;
	double val = strtod(arg,&endptr);
	if (endptr!=arg+strlen(arg)*sizeof(char)) {
		stringstream s;
		s << "UserFriendlyOptimizer: " << argname << " must be a real number";
		ibex_error(s.str().c_str());
	}
	return val;
}

ExtendedSystem& get_ext_sys(char* args[10]) {
	if ((*memory())->sys) return (ExtendedSystem&) *((*memory())->sys); // already built and recorded
	else return rec(new ExtendedSystem(rec(new System(args[FILENAME])),_2dbl("equation thickness",args[EQ_EPS])));
}


// The contractors
Ctc& get_ctc(char* args[10]) {

	Ctc* ctc;

	ExtendedSystem& ext_sys = get_ext_sys(args);

	string filtering = args[CTC];

	// the first contractor called
	CtcHC4& hc4=rec(new CtcHC4(ext_sys,0.01,true));

	if (filtering=="hc4") {
		ctc=&hc4;
	} else {

		// hc4 inside acid and 3bcid : incremental propagation beginning with the shaved variable
		CtcHC4& hc44cid=rec(new CtcHC4(ext_sys.ctrs,0.1,true));

		if (filtering=="acidhc4") {
			// The ACID contractor (component of the contractor  when filtering == "acidhc4")
			CtcAcid& acidhc4=rec(new CtcAcid(ext_sys,hc44cid,true));

			// hc4 followed by acidhc4 : the actual contractor used when filtering == "acidhc4"
			CtcCompo& hc4acidhc4=rec(new CtcCompo(hc4, acidhc4));

			ctc=&hc4acidhc4;
		}

		else if (filtering =="3bcidhc4") {
			// The 3BCID contractor on all variables (component of the contractor when filtering == "3bcidhc4")
			Ctc3BCid& c3bcidhc4=rec(new Ctc3BCid(hc44cid));
			// hc4 followed by 3bcidhc4 : the actual contractor used when filtering == "3bcidhc4"
			CtcCompo& hc43bcidhc4=rec(new CtcCompo(hc4, c3bcidhc4));

			ctc=&hc43bcidhc4;
		}
		else  {
			ibex_error("UserFriendlyOptimizer: unknown contractor mode");
		}
	}

	// The CtcXNewton contractor

	// corner selection for linearizations : two corners are selected, a random one and its opposite
	vector<LinearRelaxXTaylor::corner_point> cpoints;
	cpoints.push_back(LinearRelaxXTaylor::RANDOM);
	cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);

	LinearRelax* lr;

	string linearrelaxation = args[LIN_RELAX];

	if (linearrelaxation=="art")
		lr = &rec(new LinearRelaxCombo(ext_sys,LinearRelaxCombo::ART));
	else if  (linearrelaxation=="compo")
		lr = &rec(new LinearRelaxCombo(ext_sys,LinearRelaxCombo::COMPO));
	else if (linearrelaxation=="xn")
		lr = &rec(new LinearRelaxXTaylor(ext_sys,cpoints));
	else
		ibex_error("UserFriendlyOptimizer: unknown liner relaxation mode");

	// fixpoint linear relaxation , hc4  with default fix point ratio 0.2

	if (linearrelaxation=="compo" || linearrelaxation=="art"|| linearrelaxation=="xn") {

		//cxn = new CtcLinearRelaxation (*lr, hc44xn);
		CtcPolytopeHull& cxn_poly = rec(new CtcPolytopeHull(*lr, CtcPolytopeHull::ALL_BOX));

		// hc4 inside xnewton loop
		CtcHC4& hc44xn = rec(new CtcHC4(ext_sys.ctrs,0.01,false));

		CtcCompo& cxn_compo = rec(new CtcCompo(cxn_poly, hc44xn));

		CtcFixPoint& cxn = rec(new CtcFixPoint (*cxn_compo, default_relax_ratio));

		//  the actual contractor  ctc + linear relaxation
		return rec(new CtcCompo  (*ctc, *cxn));

	}
	else
		return ctc;

}

Bsc& get_bsc(char* args[10]) {
	Bsc* bsc;

	ExtendedSystem& ext_sys = get_ext_sys(args);
	string bisection = args[BSC];
	double prec = _2dbl("precision", args[PREC]);

	if (bisection=="roundrobin")
		bsc = &rec(new RoundRobin (prec));
	else if (bisection== "largestfirst")
		bsc=  &rec(new LargestFirst(prec));
	else if (bisection=="smearsum")
		bsc =  &rec(new SmearSum(ext_sys,prec));
	else if (bisection=="smearmax")
		bsc =  &rec(new SmearMax(ext_sys,prec));
	else if (bisection=="smearsumrel")
		bsc =  &rec(new SmearSumRelative(ext_sys,prec));
	else if (bisection=="smearmaxrel")
		bsc =  &rec(new SmearMaxRelative(ext_sys,prec));
	else
		ibex_error("UserFriendlyOptimizer unknown bisection mode");

	return bsc;
}

} // end anonymous namespace

UserFriendlyOptimizer::UserFriendlyOptimizer(char* args[10]) : Optimizer(get_ext_sys(args),get_ctc(args),get_bsc(args),
		_2dbl("precision",args[PREC]),
		_2dbl("goal relative precision",args[REL_PREC]),
		_2dbl("goal absolute precision",args[ABS_PREC]),
		_2dbl("sample size",args[SAMPLE_SIZE]),
		_2dbl("equation thickness",args[EQ_EPS])) {

	srand(1);

	data = *memory(); // keep track of my data

	timeout = _2dbl("time limit",args[TIME_LIMIT]);

	*memory() = NULL; // reset (for next DefaultOptimizer to be created)
}

UserFriendlyOptimizer::~UserFriendlyOptimizer() {
	// delete all objects dynamically created in the constructor
	delete (Memory*) data;
}

} // end namespace ibex
