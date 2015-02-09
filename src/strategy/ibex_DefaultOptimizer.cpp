//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultOptimizer.cpp
// Author      : Gilles Chabert , Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Jul 06, 2014
//============================================================================

#include "ibex_DefaultOptimizer.h"
#include "ibex_SmearFunction.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_LinearRelaxCombo.h"
#include "ibex_CellStack.h"
#include "ibex_Array.h"
#include "ibex_Random.h"
#include <vector>
#include "ibex_DefaultStrategy.cpp_"

namespace ibex {

using namespace std;

namespace {

const double default_relax_ratio = 0.2;

// This function is necessary because we need
// the extended system "ext_sys" to build
// two arguments of the base class constructor (ctc and bsc)
// and we don't know which argument is evaluated first
ExtendedSystem& get_ext_sys(System& sys, double eq_prec) {
	if (!(*memory())->sys.empty()) return (ExtendedSystem&) *((*memory())->sys.back()); // already built and recorded
	else return rec(new ExtendedSystem(sys,eq_prec));
}

}

// the defaultoptimizer constructor  1 point for sample_size
// the equality constraints are relaxed with goal_prec
DefaultOptimizer::DefaultOptimizer(System& _sys, double prec, double goal_prec) :
		Optimizer(_sys,
			  ctc(_sys,get_ext_sys(_sys,default_equ_eps),prec), // warning: we don't know which argument is evaluated first
			  rec(new SmearSumRelative(get_ext_sys(_sys,default_equ_eps),prec)),
			  prec, goal_prec, goal_prec, 1, default_equ_eps) {
  
	RNG::srand(1);

	data = *memory(); // keep track of my data

	*memory() = NULL; // reset (for next DefaultOptimizer to be created)
}

// the corners for CtcXNewtonIter : one random orner and its opposite
/*vector<CtcXNewton::corner_point>*  DefaultOptimizer::default_corners () {
	vector<CtcXNewton::corner_point>* x;
	x= new vector<CtcXNewton::corner_point>;
	x->push_back(CtcXNewton::RANDOM);
	x->push_back(CtcXNewton::RANDOM_INV);
	return x;
}*/

Ctc&  DefaultOptimizer::ctc(System& sys, System& ext_sys, double prec) {
	Array<Ctc> ctc_list(3);

	// first contractor on ext_sys : incremental hc4  ratio propag 0.01
	ctc_list.set_ref(0, rec(new CtcHC4 (ext_sys.ctrs,0.01,true)));
	// second contractor on ext_sys : acid (hc4)   with incremental hc4  ratio propag 0.1
	ctc_list.set_ref(1, rec(new CtcAcid (ext_sys,rec(new CtcHC4 (ext_sys.ctrs,0.1,true)),true)));
	// the last contractor is CtcXNewtonIter  with rfp=0.2 and rfp2=0.2
	// the limits for calling soplex are the default values 1e6 for the derivatives and 1e6 for the domains : no error found with these bounds
	int index=2;
	if (sys.nb_ctr > 0) {
		ctc_list.set_ref(2,rec(new CtcFixPoint
				(rec(new CtcCompo(
						rec(new CtcPolytopeHull(rec(new LinearRelaxCombo (ext_sys,LinearRelaxCombo::XNEWTON)),
								CtcPolytopeHull::ALL_BOX)),
								rec(new CtcHC4(ext_sys.ctrs,0.01)))), default_relax_ratio)));
		index++;
	}
	ctc_list.resize(index);
	return rec(new CtcCompo(ctc_list));
}


// deletion of all dynamically created objects
DefaultOptimizer::~DefaultOptimizer() {
	// delete all objects dynamically created in the constructor
	delete (Memory*) data;
}

} // end namespace ibex
