//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultOptimizer.cpp
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Jul 25, 2017
//============================================================================

#include "ibex_DefaultOptimizer.h"

#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_CellDoubleHeap.h"
#include "ibex_SmearFunction.h"
#include "ibex_LSmear.h"
#include "ibex_LoupFinderDefault.h"
#include "ibex_LoupFinderCertify.h"
#include "ibex_LinearizerCombo.h"
#include "ibex_Array.h"
#include "ibex_Memory.cpp_"
#include "ibex_Random.h"
#include "ibex_CellBeamSearch.h"
#include "ibex_CellHeap.h"



using namespace std;

namespace ibex {

const double DefaultOptimizer::default_random_seed = 1.0;

namespace {

const double default_relax_ratio = 0.2;

int extended_system = -1;   // index in memory (-1=none)
int normalized_system = -1; // index in memory (-1=none)

// The two next functions are necessary because we need
// the normalized and extended system to build
// arguments of the base class constructor (ctc, bsc, loup finder, etc.)
// and we don't know which argument is evaluated first

NormalizedSystem& get_norm_sys(const System& sys, double eps_h) {
	if (normalized_system>=0)
		return (NormalizedSystem&) *((*memory())->sys[normalized_system]); // already built and recorded
	else {
		normalized_system = (*memory())->sys.size();
		return rec(new NormalizedSystem(sys,eps_h));
	}
}

ExtendedSystem& get_ext_sys(const System& sys, double eps_h) {
	if (extended_system>=0)
		return (ExtendedSystem&) *((*memory())->sys[extended_system]); // already built and recorded
	else {
		extended_system = (*memory())->sys.size();
		return rec(new ExtendedSystem(sys,eps_h));
	}
}

}

DefaultOptimizer::DefaultOptimizer(const System& sys, double rel_eps_f, double abs_eps_f, double eps_h, bool rigor, bool inHC4, double random_seed, double eps_x) :
		Optimizer(sys.nb_var,
			  ctc(get_ext_sys(sys,eps_h)), // warning: we don't know which argument is evaluated first
//			  rec(new SmearSumRelative(get_ext_sys(sys,eps_h),eps_x)),
			  rec(new LSmear(get_ext_sys(sys,eps_h),eps_x)),
			  rec(rigor? (LoupFinder*) new LoupFinderCertify(sys,rec(new LoupFinderDefault(get_norm_sys(sys,eps_h),inHC4))) :
						 (LoupFinder*) new LoupFinderDefault(get_norm_sys(sys,eps_h),inHC4)),
			  (CellBufferOptim&) rec(new CellDoubleHeap(get_ext_sys(sys,eps_h))),
//			  (CellBufferOptim&) rec (new  CellBeamSearch (
//								       (CellHeap&) rec (new CellHeap (get_ext_sys(sys,eps_h))),
//								       (CellHeap&) rec (new CellHeap (get_ext_sys(sys,eps_h))),
//								       get_ext_sys(sys,eps_h))),
			  get_ext_sys(sys,eps_h).goal_var(),
			  eps_x,
			  rel_eps_f,
			  abs_eps_f) {
  
	data = *memory(); // keep track of my data

	RNG::srand(random_seed);

	/* reset (for next DefaultOptimizer to be created) */
	*memory() = NULL;
	extended_system = -1;
	normalized_system = -1;
}

Ctc&  DefaultOptimizer::ctc(const System& ext_sys) {
	Array<Ctc> ctc_list(3);

	// first contractor on ext_sys : incremental HC4 (propag ratio=0.01)
	ctc_list.set_ref(0, rec(new CtcHC4 (ext_sys.ctrs,0.01,true)));
	// second contractor on ext_sys : "Acid" with incremental HC4 (propag ratio=0.1)
	ctc_list.set_ref(1, rec(new CtcAcid (ext_sys,rec(new CtcHC4 (ext_sys.ctrs,0.1,true)),true)));
	// the last contractor is "XNewton"
	if (ext_sys.nb_ctr > 1) {
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
