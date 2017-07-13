//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearRelaxCombo.cpp
// Author      : Ignacio Araya, Bertrand Neveu , Gilles Trombettoni, Jordan Ninin
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Nov 15, 2012
//============================================================================

#include "ibex_LinearizerCombo.h"

namespace ibex {

// the constructor

LinearizerCombo::LinearizerCombo(const System& sys1, linear_mode lmode1) :
			Linearizer(sys1.nb_var),
			lmode(lmode1),
			myxnewton(NULL)
#ifdef _IBEX_WITH_AFFINE_
			, myart(NULL)
#endif
			{

	switch (lmode) {
	case XNEWTON: {
		// the default corner of XNewton linear relaxation
		myxnewton = new LinearizerXTaylor(sys1, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::HANSEN,
										   LinearizerXTaylor::default_max_diam_deriv);
		break;
	}
	case TAYLOR:{
		// the default corner of XNewton linear relaxation
		myxnewton = new LinearizerXTaylor(sys1, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::TAYLOR,
										   LinearizerXTaylor::default_max_diam_deriv);
		break;
	}
	case HANSEN: {
		// the default corner of XNewton linear relaxation
		myxnewton = new LinearizerXTaylor(sys1, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::HANSEN,
				LinearizerXTaylor::default_max_diam_deriv);
		break;
	}

#ifdef _IBEX_WITH_AFFINE_
	case ART:
	case AFFINE2: {
		myart =new LinearRelaxAffine2(sys1);
		break;
	}
	case COMPO: {

		myart =new LinearRelaxAffine2(sys1);
		// the default corner of XNewton linear relaxation
		cpoints.push_back(LinearizerXTaylor::RANDOM);
		cpoints.push_back(LinearizerXTaylor::RANDOM_INV);
		myxnewton = new LinearizerXTaylor(sys1, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::HANSEN,
				LinearizerXTaylor::default_max_diam_deriv);

		break;
	}
#else
	case ART:
		ibex_error("LinearRelaxCombo: ART mode requires \"--with-affine\" option");
		break;
	case AFFINE2:
		ibex_error("LinearRelaxCombo: AFFINE2 mode requires \"--with-affine\" option");
		break;
	case COMPO:
		ibex_error("LinearRelaxCombo: COMPO mode requires \"--with-affine\" option");
		break;
#endif
	}

}

LinearizerCombo::~LinearizerCombo() {
	if (myxnewton!=NULL) delete myxnewton;
}

/*********generation of the linearized system*********/
int LinearizerCombo::linearize(const IntervalVector& box, LinearSolver& lp_solver) {

	int cont = 0;

	switch (lmode) {
	case XNEWTON:
	case TAYLOR:
	case HANSEN:
		cont = myxnewton->linearize(box,lp_solver);
		break;

#ifdef _IBEX_WITH_AFFINE_
	case ART:
	case AFFINE2:
		cont = myart->linearize(box,lp_solver);
		break;
	case COMPO: {
		cont = myxnewton->linearize(box,lp_solver);
		if (cont!=-1) {
			int cont2 = myart->linearize(box,lp_solver);
			if (cont2==-1) cont=-1;
			else cont+=cont2;
		}
	}
#else
	case ART:
		ibex_error("LinearRelaxCombo: ART mode requires \"--with-affine\" option");
		break;
	case AFFINE2:
		ibex_error("LinearRelaxCombo: AFFINE2 mode requires \"--with-affine\" option");
		break;
	case COMPO:
		ibex_error("LinearRelaxCombo: COMPO mode requires \"--with-affine\" option");
		break;
#endif
	}
	return cont;
}




}
