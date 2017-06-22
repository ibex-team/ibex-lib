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
			myxnewton(NULL)
#ifdef _IBEX_WITH_AFFINE_
			, myart(NULL)
#endif
			{

	switch (lmode) {
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

#ifdef _IBEX_WITH_AFFINE_
	case ART:
	case AFFINE2: {
		myart =new LinearRelaxAffine2(sys1);
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

LinearRelaxCombo::~LinearRelaxCombo() {
	cpoints.clear();
	if (myxnewton!=NULL) delete myxnewton;
}

int LinearRelaxCombo::inlinearization(const IntervalVector& box, LinearSolver& lp_solver) {
	int cont=0;
	switch (lmode) {
	case XNEWTON:
	case TAYLOR:
	case HANSEN: {
		cont= myxnewton->inlinearization(box,lp_solver);
		break;
	}

#ifdef _IBEX_WITH_AFFINE_
	case ART:
	case AFFINE2: {
		cont = myart->inlinearization(box,lp_solver);
		break;
	}
	case COMPO: {
		cont = myxnewton->inlinearization(box,lp_solver);
		if (cont<0) 	cont = myart->inlinearization(box,lp_solver);
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
	return cont;
}

//bool LinearRelaxCombo::goal_linearization(const IntervalVector& box, LinearSolver& lp_solver) {
//	bool cont=false;
//	switch (lmode) {
//	case XNEWTON:
//	case TAYLOR:
//	case HANSEN: {
//		cont= myxnewton->goal_linearization(box,lp_solver);
//		break;
//	}
//
//#ifdef _IBEX_WITH_AFFINE_
//	case ART:
//	case AFFINE2: {
//		cont= myart->goal_linearization(box,lp_solver);
//		break;
//	}
//	case COMPO: {
//		cont = myxnewton->goal_linearization(box,lp_solver);
//		if (!cont)	cont = myart->goal_linearization(box,lp_solver);
//		break;
//	}
//#else
//	case ART:
//		ibex_error("LinearRelaxCombo: ART mode requires \"--with-affine\" option");
//		break;
//	case AFFINE2:
//		ibex_error("LinearRelaxCombo: AFFINE2 mode requires \"--with-affine\" option");
//		break;
//	case COMPO:
//		ibex_error("LinearRelaxCombo: COMPO mode requires \"--with-affine\" option");
//		break;
//#endif
//	}
//	return cont;
//}


/*********generation of the linearized system*********/
int LinearRelaxCombo::linearization(const IntervalVector& box, LinearSolver& lp_solver) {

	int cont = 0;

	switch (lmode) {
	case XNEWTON:
	case TAYLOR:
	case HANSEN:
		cont = myxnewton->linearization(box,lp_solver);
		break;

#ifdef _IBEX_WITH_AFFINE_
	case ART:
	case AFFINE2:
		cont = myart->linearization(box,lp_solver);
		break;
	case COMPO: {
		cont = myxnewton->linearization(box,lp_solver);
		if (cont!=-1) {
			int cont2 = myart->linearization(box,lp_solver);
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
