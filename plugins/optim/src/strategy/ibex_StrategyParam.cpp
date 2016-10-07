//============================================================================
//                                  I B E X                                   
// File        : ibex_StrategyParam.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Dec 11, 2014
//============================================================================

#include "ibex_StrategyParam.h"
#include "ibex_DefaultStrategy.cpp_"
#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_Ctc3BCid.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcNewton.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_LinearRelaxXTaylor.h"
#include "ibex_LinearRelaxCombo.h"
#include "ibex_SmearFunction.h"
#include "ibex_LargestFirst.h"
#include "ibex_Random.h"

#include <sstream>
#include <vector>

using namespace std;

const double NEWTON_CEIL=5e8;
const double GAUSS_SEIDEL_RATIO=1e-04;

namespace ibex {

System& StrategyParam::get_sys() {
	if (!(*memory())->sys.empty()) {
		return *((*memory())->sys.front()); // already built and recorded
	}
	else return rec(new System(filename.c_str()));
}

System& StrategyParam::get_ext_sys() {
	return get_sys();
}

System& OptimizerParam::get_ext_sys() {
	if ((*memory())->sys.size()==2) {
		return (ExtendedSystem&) *((*memory())->sys.back()); // already built and recorded
	}
	else return rec(new ExtendedSystem(get_sys(),eq_eps));
}

/*
 *  Build the contractors
 */
Ctc& StrategyParam::get_ctc() {

	Ctc* ctc;

	System& ext_sys = get_ext_sys(); // <=> original system in case of a solver

	// the first contractor called
	Ctc& hc4=rec(new CtcHC4(ext_sys,ratio_propag,hc4_incremental));

	// Build contractor #2:
	// --------------------------
	// An interval Newton iteration
	// for solving f()=0 where f is
	// a vector-valued function representing
	// the system.
	Ctc* ctcnewton= NULL;

	if (filtering == "acidhc4n" || filtering=="hc4n" || filtering=="3bcidhc4n")
	  ctcnewton= &rec(new CtcNewton(get_sys().f, NEWTON_CEIL, prec, GAUSS_SEIDEL_RATIO));

	if (filtering=="hc4" || filtering=="hc4n") {
		ctc = (!ctcnewton)?
				&hc4 :
				&rec(new CtcCompo(hc4,ctcnewton));
	} else {

		// hc4 inside acid and 3bcid : incremental propagation beginning with the shaved variable
		Ctc& hc44cid=rec(new CtcHC4(ext_sys.ctrs,0.1,true));

		if (filtering=="acidhc4" || filtering=="acidhc4n") {
			// The ACID contractor (component of the contractor  when filtering == "acidhc4")
			Ctc& acidhc4=rec(new CtcAcid(ext_sys,hc44cid,optim));

			// hc4 followed by acidhc4 : the actual contractor used when filtering == "acidhc4"
			ctc=(!ctcnewton)?
					&rec(new CtcCompo(hc4, acidhc4)) :
					&rec(new CtcCompo(hc4, acidhc4, *ctcnewton));
		}

		else if (filtering =="3bcidhc4" || filtering =="3bcidhc4n") {
			// The 3BCID contractor on all variables (component of the contractor when filtering == "3bcidhc4")
			Ctc& c3bcidhc4=rec(new Ctc3BCid(hc44cid));
			// hc4 followed by 3bcidhc4 : the actual contractor used when filtering == "3bcidhc4"

			ctc=(!ctcnewton)?
					&rec(new CtcCompo(hc4, c3bcidhc4)) :
					&rec(new CtcCompo(hc4, c3bcidhc4, *ctcnewton));
		}
		else  {
			ibex_error("StrategyParam: unknown contractor mode");
		}
	}

	// The CtcXNewton contractor

	// corner selection for linearizations : two corners are selected, a random one and its opposite
	vector<LinearRelaxXTaylor::corner_point> cpoints;
	cpoints.push_back(LinearRelaxXTaylor::RANDOM);
	cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);

	LinearRelax* lr;

	if (lin_relax=="art")
		lr = &rec(new LinearRelaxCombo(ext_sys,LinearRelaxCombo::ART));
	else if  (lin_relax=="compo")
		lr = &rec(new LinearRelaxCombo(ext_sys,LinearRelaxCombo::COMPO));
	else if (lin_relax=="xn")
		lr = &rec(new LinearRelaxXTaylor(ext_sys,cpoints));
	else
		ibex_error("StrategyParam: unknown liner relaxation mode");

	// fixpoint linear relaxation , hc4  with default fix point ratio 0.2

	if (lin_relax=="compo" || lin_relax=="art"|| lin_relax=="xn") {

		//cxn = new CtcLinearRelaxation (*lr, hc44xn);
		Ctc& cxn_poly = rec(new CtcPolytopeHull(*lr, CtcPolytopeHull::ALL_BOX));

		// hc4 inside xnewton loop
		Ctc& hc44xn = rec(new CtcHC4(ext_sys.ctrs,ratio_propag,false));

		Ctc& cxn_compo = rec(new CtcCompo(cxn_poly, hc44xn));

		Ctc& cxn = rec(new CtcFixPoint (cxn_compo, fixpoint_ratio));

		//  the actual contractor  ctc + linear relaxation
		return rec(new CtcCompo  (*ctc, cxn));

	}
	else
		return *ctc;

}

/*
 * Build the bisector
 */
Bsc&  StrategyParam::get_bsc() {
	Bsc* bsc;

	System& ext_sys = get_ext_sys(); // <=> original system in case of a solver

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
		ibex_error("StrategyParam: unknown bisection mode");

	return *bsc;
}

StrategyParam::StrategyParam(const char* filename, const char* filtering, const char* lin_relax,
		const char* bisection, double prec, double time_limit, bool hc4_incremental,
		double ratio_propag, double fixpoint_ratio, bool optim) :
		prec(prec), time_limit(time_limit), filename(filename), filtering(filtering), lin_relax(lin_relax),
		bisection(bisection), hc4_incremental(hc4_incremental), ratio_propag(ratio_propag),
		fixpoint_ratio(fixpoint_ratio), optim(optim) {

	data = *memory(); // keep track of my data

	//	cout << "system                   " << get_sys(args) << endl;
	//	cout << "precision:               " << prec << endl;
	//	cout << "goal relative precision: " << goal_rel_prec << endl;
	//	cout << "goal absolute precision: " << goal_abs_prec << endl;
	//	cout << "sample size:             "  << sample_size << endl;
	//	cout << "equation thickness:      " << args[EQ_EPS] << endl;
	//	cout << "time out:                " << timeout << endl;

	*memory() = NULL; // reset (for next DefaultOptimizer to be created)

}


StrategyParam::~StrategyParam() {
	// delete all objects dynamically created in the constructor
	delete (Memory*) data;
}

OptimizerParam::OptimizerParam(const char* filename, const char* filtering, const char* lin_relax,
		const char* bisection, double prec, double time_limit, bool hc4_incremental, double ratio_propag,
		double fixpoint_ratio, double goal_rel_prec, double goal_abs_prec, int sample_size, double eq_eps) :

						StrategyParam(filename,filtering,lin_relax,bisection,prec,time_limit,hc4_incremental,ratio_propag,fixpoint_ratio,true),
						goal_rel_prec(goal_rel_prec), goal_abs_prec(goal_abs_prec), sample_size(sample_size), eq_eps(eq_eps) {


	RNG::srand(1);

}

} // namespace ibex
