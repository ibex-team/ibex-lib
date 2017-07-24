//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultOptimizer.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Jul 21, 2017
//============================================================================

#include "ibex_DefaultOptimizer.h"

#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_CellDoubleHeap.h"
#include "ibex_SmearFunction.h"
#include "ibex_LoupFinderDefault.h"
#include "ibex_LoupFinderCertify.h"
#include "ibex_LinearizerCombo.h"
#include "ibex_Array.h"
#include "ibex_Random.h"
#include "ibex_DefaultStrategy.cpp_"

using namespace std;

namespace ibex {

const double DefaultOptimizer::default_random_seed = 1.0;

namespace {

const double default_relax_ratio = 0.2;

// This function is necessary because we need
// the extended system "ext_sys" to build
// two arguments of the base class constructor (ctc and bsc)
// and we don't know which argument is evaluated first
ExtendedSystem& get_ext_sys(const System& sys, double eq_prec) {
	if (!(*memory())->sys.empty()) return (ExtendedSystem&) *((*memory())->sys.back()); // already built and recorded
	else return rec(new ExtendedSystem(sys,eq_prec));
}

}

DefaultOptimizer::DefaultOptimizer(const System& _sys, double eps_x, double rel_eps_f, double abs_eps_f, double eps_h, bool rigor, double random_seed) :
		Optimizer(_sys.nb_var,
			  ctc(_sys,get_ext_sys(_sys,NormalizedSystem::default_eps_h),eps_x), // warning: we don't know which argument is evaluated first
			  rec(new SmearSumRelative(get_ext_sys(_sys,NormalizedSystem::default_eps_h),eps_x)),
			  rec(
					  rigor?
							  (LoupFinder*) new LoupFinderCertify(_sys,*new LoupFinderDefault(*new NormalizedSystem(_sys,eps_h))) :

							  (LoupFinder*) new LoupFinderDefault(*new NormalizedSystem(_sys,eps_h))
			  ), //get_ext_sys(_sys,default_eps_h))),
			  *new CellDoubleHeap(_sys),
			  get_ext_sys(_sys,NormalizedSystem::default_eps_h).goal_var(),
			  eps_x, rel_eps_f, abs_eps_f) {
  
	data = *memory(); // keep track of my data

	RNG::srand(random_seed);

	*memory() = NULL; // reset (for next DefaultOptimizer to be created)
}

Ctc&  DefaultOptimizer::ctc(const System& sys, const System& ext_sys, double prec) {
	Array<Ctc> ctc_list(3);

	// first contractor on ext_sys : incremental hc4  ratio propag 0.01
	ctc_list.set_ref(0, rec(new CtcHC4 (ext_sys.ctrs,0.01,true)));
	// second contractor on ext_sys : acid (hc4)   with incremental hc4  ratio propag 0.1
	ctc_list.set_ref(1, rec(new CtcAcid (ext_sys,rec(new CtcHC4 (ext_sys.ctrs,0.1,true)),true)));
	// the last contractor is CtcXNewtonIter  with rfp=0.2 and rfp2=0.2
	// the limits for calling soplex are the default values 1e6 for the derivatives and 1e6 for the domains : no error found with these bounds
	if (sys.nb_ctr > 0) {
		ctc_list.set_ref(2,rec(new CtcFixPoint
				(rec(new CtcCompo(
						rec(new CtcPolytopeHull(rec(new LinearizerCombo (ext_sys,LinearizerCombo::XNEWTON)))),
								rec(new CtcHC4(ext_sys.ctrs,0.01)))), default_relax_ratio)));
	} else {
		ctc_list.set_ref(2,rec(new CtcPolytopeHull(rec(new LinearizerCombo (ext_sys,LinearizerCombo::XNEWTON)))));
	}
	return rec(new CtcCompo(ctc_list));
}

DefaultOptimizer::~DefaultOptimizer() {
	// delete all objects dynamically created in the constructor
	delete (Memory*) data;
}

} // end namespace ibex
