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

CtcLR::CtcLR(const System& sys1,ctc_mode cmode1, linear_mode lmode1) :
			CtcLinearRelaxationIter(sys1,cmode1),
			lmode(lmode1) {

	switch (lmode) {
	case ART:
	case AFFINE2: {
		myart =new CtcART(sys1,cmode1);
		break;
	}
	case XNEWTON: {
		// the default corner of XNewton linear relaxation
		cpoints.push_back(CtcXNewton::RANDOM);
		cpoints.push_back(CtcXNewton::RANDOM_INV);
		myxnewton = new CtcXNewton(sys1,cpoints,cmode1);
		break;
	}
	case TAYLOR:{
		// the default corner of XNewton linear relaxation
		cpoints.push_back(CtcXNewton::RANDOM);
		cpoints.push_back(CtcXNewton::RANDOM_INV);
		myxnewton = new CtcXNewton(sys1,cpoints,cmode1,CtcXNewton::TAYLOR);
		break;
	}
	case HANSEN: {
		// the default corner of XNewton linear relaxation
		cpoints.push_back(CtcXNewton::RANDOM);
		cpoints.push_back(CtcXNewton::RANDOM_INV);
		myxnewton = new CtcXNewton(sys1,cpoints,cmode1,CtcXNewton::HANSEN);
		break;
	}
	case COMPO: {

		myart =new CtcART(sys1,cmode1);

		// the default corner of XNewton linear relaxation
		cpoints.push_back(CtcXNewton::RANDOM);
		cpoints.push_back(CtcXNewton::RANDOM_INV);
		myxnewton = new CtcXNewton(sys1,cpoints,cmode1);

		break;
	}
	}

}

CtcLR::~CtcLR() {
	switch (lmode) {
	case ART:
	case AFFINE2: {
		delete myart;
		break;
	}
	case XNEWTON:
	case TAYLOR:
	case HANSEN: {
		cpoints.clear();
		delete myxnewton;
		break;
	}
	case COMPO: {
		cpoints.clear();
		delete myxnewton;
		delete myart;
		break;
	}
	}
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
