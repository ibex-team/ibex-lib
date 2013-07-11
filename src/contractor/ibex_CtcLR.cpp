//============================================================================
//                                  I B E X                                   
// File        : ibex_LRiter.cpp
// Author      : Ignacio Araya, Bertrand Neveu , Gilles Trombettoni, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Nov 15, 2012
//============================================================================

#include "ibex_CtcLR.h"

namespace ibex {

// the constructor

CtcLR::CtcLR(const System& sys1,ctc_mode cmode1, linear_mode lmode1, bool init_lp) :
			CtcLinearRelaxationIter(sys1,cmode1,LinearSolver::default_max_iter, LinearSolver::default_max_time_out,
					LinearSolver::default_eps, LinearSolver::default_limit_diam_box, init_lp),
			lmode(lmode1),
			myart(NULL),
			myxnewton(NULL)  {

	switch (lmode) {
	case ART:
	case AFFINE2: {
		myart =new CtcART(sys1,cmode1,LinearSolver::default_max_iter, LinearSolver::default_max_time_out,
				LinearSolver::default_eps, LinearSolver::default_limit_diam_box, false);
		break;
	}
	case XNEWTON: {
		// the default corner of XNewton linear relaxation
		cpoints.push_back(CtcXNewton::RANDOM);
		cpoints.push_back(CtcXNewton::RANDOM_INV);
		myxnewton = new CtcXNewton(sys1,cpoints,cmode1,CtcXNewton::HANSEN, LinearSolver::default_max_iter,
				CtcXNewton::default_max_diam_deriv,LinearSolver::default_limit_diam_box, false);
		break;
	}
	case TAYLOR:{
		// the default corner of XNewton linear relaxation
		cpoints.push_back(CtcXNewton::RANDOM);
		cpoints.push_back(CtcXNewton::RANDOM_INV);
		myxnewton = new CtcXNewton(sys1,cpoints,cmode1,CtcXNewton::TAYLOR, LinearSolver::default_max_iter,
				CtcXNewton::default_max_diam_deriv,LinearSolver::default_limit_diam_box, false);
		break;
	}
	case HANSEN: {
		// the default corner of XNewton linear relaxation
		cpoints.push_back(CtcXNewton::RANDOM);
		cpoints.push_back(CtcXNewton::RANDOM_INV);
		myxnewton = new CtcXNewton(sys1,cpoints,cmode1,CtcXNewton::HANSEN,LinearSolver::default_max_iter,
				CtcXNewton::default_max_diam_deriv,LinearSolver::default_limit_diam_box, false);
		break;
	}
	case COMPO: {

		myart =new CtcART(sys1,cmode1,LinearSolver::default_max_iter, LinearSolver::default_max_time_out,
				LinearSolver::default_eps, LinearSolver::default_limit_diam_box, false);

		// the default corner of XNewton linear relaxation
		cpoints.push_back(CtcXNewton::RANDOM);
		cpoints.push_back(CtcXNewton::RANDOM_INV);
		myxnewton = new CtcXNewton(sys1,cpoints,cmode1,CtcXNewton::HANSEN, LinearSolver::default_max_iter,
				CtcXNewton::default_max_diam_deriv,LinearSolver::default_limit_diam_box, false);

		break;
	}
	}

}

CtcLR::~CtcLR() {
	cpoints.clear();
	if (myart!=NULL) delete myart;
	if (myxnewton!=NULL) delete myxnewton;
}



/*********generation of the linearized system*********/
int CtcLR::linearization(IntervalVector & box, LinearSolver *mysolver) {

	int cont = 0;
	// Update the bounds the variables
	mysolver->initBoundVar(box);

	switch (lmode) {
	case ART:
	case AFFINE2: {
		cont = myart->linearization(box,mysolver);
		break;
	}
	case XNEWTON:
	case TAYLOR:
	case HANSEN: {
		cont = myxnewton->linearization(box,mysolver);
		break;
	}
	case COMPO: {
		cont  = myxnewton->linearization(box,mysolver);
		cont += myart->linearization(box,mysolver);
		break;
	}
	}
	return cont;
}




}
