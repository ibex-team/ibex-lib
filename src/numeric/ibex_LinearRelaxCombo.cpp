//============================================================================
//                                  I B E X                                   
// File        : ibex_LRiter.cpp
// Author      : Ignacio Araya, Bertrand Neveu , Gilles Trombettoni, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Nov 15, 2012
//============================================================================

#include "ibex_LinearRelaxCombo.h"

namespace ibex {

// the constructor

LinearRelaxCombo::LinearRelaxCombo(const System& sys1, linear_mode lmode1) :
			LinearRelax(sys1),
			lmode(lmode1),
			myart(NULL),
			myxnewton(NULL)  {

	switch (lmode) {
	case ART:
	case AFFINE2: {
		myart =new LinearRelaxAffine2(sys1);
		break;
	}
	case XNEWTON: {
		// the default corner of XNewton linear relaxation
		cpoints.push_back(LinearRelaxXTaylor::RANDOM);
		cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);
		myxnewton = new LinearRelaxXTaylor(sys1,cpoints,LinearRelaxXTaylor::HANSEN,
										   LinearRelaxXTaylor::default_max_diam_deriv);
		break;
	}
	case TAYLOR:{
		// the default corner of XNewton linear relaxation
		cpoints.push_back(LinearRelaxXTaylor::RANDOM);
		cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);
		myxnewton = new LinearRelaxXTaylor(sys1,cpoints,LinearRelaxXTaylor::TAYLOR,
										   LinearRelaxXTaylor::default_max_diam_deriv);
		break;
	}
	case HANSEN: {
		// the default corner of XNewton linear relaxation
		cpoints.push_back(LinearRelaxXTaylor::RANDOM);
		cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);
		myxnewton = new LinearRelaxXTaylor(sys1,cpoints,LinearRelaxXTaylor::HANSEN,
				LinearRelaxXTaylor::default_max_diam_deriv);
		break;
	}
	case COMPO: {

		myart =new LinearRelaxAffine2(sys1);

		// the default corner of XNewton linear relaxation
		cpoints.push_back(LinearRelaxXTaylor::RANDOM);
		cpoints.push_back(LinearRelaxXTaylor::RANDOM_INV);
		myxnewton = new LinearRelaxXTaylor(sys1,cpoints,LinearRelaxXTaylor::HANSEN,
				LinearRelaxXTaylor::default_max_diam_deriv);

		break;
	}
	}

}

LinearRelaxCombo::~LinearRelaxCombo() {
	cpoints.clear();
	if (myart!=NULL) delete myart;
	if (myxnewton!=NULL) delete myxnewton;
}



/*********generation of the linearized system*********/
int LinearRelaxCombo::linearization(IntervalVector & box, LinearSolver *mysolver) {

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
