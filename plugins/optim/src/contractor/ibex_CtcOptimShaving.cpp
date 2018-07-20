//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimShaving.cpp
// Author      : Ignacio Araya, Gilles Chabert,
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Apr 4, 2014
// Last Update : Jul 20, 2018
//============================================================================

#include "ibex_CtcOptimShaving.h"

namespace ibex {

const int CtcOptimShaving::LimitCIDDichotomy=100;

CtcOptimShaving::CtcOptimShaving(Ctc& ctc, int s3b, int scid, int vhandled, double var_min_width) :
			Ctc3BCid (ctc,s3b,scid,vhandled,var_min_width) {
}

void CtcOptimShaving::contract(IntervalVector& box) {
	CtcContext context;
	contract(box,context);
}

void CtcOptimShaving::contract(IntervalVector& box, CtcContext& context) {
	int	var_obj=start_var;

	// [gch]: impact handling:
	bool was_impacted_var = true; // was var_obj initially in the impact?
	if (context.impact()) {
		was_impacted_var = (*context.impact())[var_obj];
		context.impact()->add(var_obj);
	}

	var3BCID(box, var_obj, context);

	if (!was_impacted_var)
		context.impact()->remove(var_obj);             // [gch]

	if (box.is_empty()) {
		context.set_flag(CtcContext::FIXPOINT);
	}
}


// left shaving only (for optimization)

bool CtcOptimShaving::var3BCID_dicho(IntervalVector& box, int var, double w3b, CtcContext& context) {
	IntervalVector initbox = box;

	// left shaving , after box contains the left slice
	int r0 = shave_bound_dicho(box,var, w3b, true, context);
	if (box[var].ub() == initbox[var].ub())
		return true; // the left slice reaches the right bound : nothing more to do
	IntervalVector leftbox=box;
	box=initbox;
	box[var]= Interval(leftbox[var].lb(),initbox[var].ub());
	return r0;
}

int CtcOptimShaving::limitCIDDichotomy () {
	return LimitCIDDichotomy;
}

// left only (for optimization)

bool CtcOptimShaving::var3BCID_slices(IntervalVector& box, int var, int locs3b, double w_DC, Interval& dom, CtcContext& context) {

	IntervalVector savebox(box);

	// Reduce left bound by shaving:

	bool stopLeft = false;
	//double leftBound = dom.lb();
	//double rightBound = dom.ub();
	//double leftCID;

	int k=0;

	while (k < locs3b && ! stopLeft) {

		// Compute a slice 'dom'
		if (k > 0) box = savebox;
		double inf_k = dom.lb()+k*w_DC;
		double sup_k = dom.lb()+(k+1)*w_DC;
		if (sup_k > dom.ub() || (k == locs3b - 1 && sup_k<dom.ub())) sup_k = dom.ub();
		dom = Interval(inf_k, sup_k);

		// Try to refute this slice
		ctc.contract(box,context);

		if (box.is_empty()) {
			//leftBound = sup_k;
			k++;
			continue;
		}
		//non empty box
		stopLeft = true;
		//leftCID = sup_k;
		//leftBound = dom.lb();
		k++;
	}

	if (!stopLeft) { // all slices give an empty box
		box.set_empty();
		return true; // TODO: Bertrand, check the return value
	} else if (k == locs3b) {
		// Only the last slice gives a non-empty box : box is reduced to this last slice
		return true;
	} else {
		IntervalVector newbox (box); // newbox is initialized with the last slice handled in the previous loop
		box=savebox;
		box[var]= Interval(newbox[var].lb(),savebox[var].ub());
		return true;
	}
}

}

