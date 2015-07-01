//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcPolytopeHull.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Oct 31, 2013
// Last Update : Oct 31, 2013
//============================================================================

#include "ibex_CtcPolytopeHullOne.h"


namespace ibex {


CtcPolytopeHullOne::CtcPolytopeHullOne(LinearRelax& lr, ctc_mode cmode, int max_iter, int time_out, double eps, Interval limit_diam) :
		CtcPolytopeHull(lr,cmode, max_iter, time_out, eps, limit_diam) {
}

CtcPolytopeHullOne::CtcPolytopeHullOne(const Matrix& A, const Vector& b, int max_iter, int time_out, double eps, Interval limit_diam) :
		CtcPolytopeHull( A, b, max_iter, time_out,  eps, limit_diam) {

}


#ifndef  _IBEX_WITH_NOLP_

void CtcPolytopeHullOne::contract(IntervalVector& box) {

	if (!(limit_diam_box.contains(box.max_diam()))) return;
	// is it necessary?  YES (BNE) Soplex can give false infeasible results with large numbers
	//cout << "[polytope-hull] box before LR (linear relaxation): " << box << endl;

	try {
		IntervalVector one(box.size(),Interval(-1,1));

		// Update the bounds the variables
		mylinearsolver->initBoundVar(one);

		//returns the number of constraints in the linearized system
		int cont = lr.linearizationOne(box, *mylinearsolver);

		//cout << "[polytope-hull] end of LR" << endl;

		if (cont==-1) throw PolytopeHullEmptyBoxException();

		if (cont==0) return;

		optimizer(one);

		for (int i=0;i<box.size();i++) {
			box[i] &= box[i].mid() + box[i].rad() *one[i];
		}

		//mylinearsolver->writeFile("LP.lp");
		//system ("cat LP.lp");
		//cout << "[polytope-hull] box after LR: " << box << endl;
		mylinearsolver->cleanConst();
	}
	catch(LPException&) {
		mylinearsolver->cleanConst();
	}
	catch(PolytopeHullEmptyBoxException& ) {
		box.set_empty(); // empty the box before exiting
		mylinearsolver->cleanConst();
	}
}


#else /// end _IBEX_WITH_NOLP_


void CtcPolytopeHullOne::contract(IntervalVector& box) { }


#endif /// end _IBEX_WITH_NOLP_




} // end namespace ibex
