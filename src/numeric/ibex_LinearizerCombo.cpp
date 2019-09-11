//============================================================================
//                                  I B E X                                   
// File        : ibex_LinearizerCombo.cpp
// Author      : Ignacio Araya, Bertrand Neveu , Gilles Trombettoni, Jordan Ninin
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Jul 15, 2017
//============================================================================

#include "ibex_LinearizerCombo.h"

namespace ibex {

LinearizerCombo::LinearizerCombo(const System& sys, linear_mode lmode1) :
			Linearizer(sys.nb_var),
			lmode(lmode1),
			myxnewton(NULL),
			myp_taylor(NULL)
#ifdef _IBEX_WITH_AFFINE_
			, myart(NULL)
#endif
			{

	switch (lmode) {
	case XNEWTON: {
		// the default corner of XNewton linear relaxation
		myxnewton = new LinearizerXTaylor(sys, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::HANSEN);
		break;
	}
	case TAYLOR:{
		// the default corner of XNewton linear relaxation
		myxnewton = new LinearizerXTaylor(sys, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::TAYLOR);
		break;
	}
	case HANSEN: {
		// the default corner of XNewton linear relaxation
		myxnewton = new LinearizerXTaylor(sys, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::HANSEN);
		break;
	}

	case PARALLEL: {
		myp_taylor = new LinearizerParallelTaylor(sys, LinearizerParallelTaylor::RELAX, LinearizerParallelTaylor::HANSEN);
		break;
	}

	case TAY_PAR: {
		// the default corner of XNewton linear relaxation
		myxnewton = new LinearizerXTaylor(sys, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::HANSEN);
		myp_taylor = new LinearizerParallelTaylor(sys, LinearizerParallelTaylor::RELAX, LinearizerParallelTaylor::HANSEN);
		break;
	}

#ifdef _IBEX_WITH_AFFINE_
	case ART:
	case AFFINE2: {
		myart = new LinearizerAffine2(sys);
		break;
	}
	case COMPO: {

		myart = new LinearizerAffine2(sys);
		// the default corner of XNewton linear relaxation
		myxnewton = new LinearizerXTaylor(sys, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::HANSEN);
		break;
	}


	case TAY_PAR_AFF: {

		myart = new LinearizerAffine2(sys);
		myp_taylor = new LinearizerParallelTaylor(sys, LinearizerParallelTaylor::RELAX, LinearizerParallelTaylor::HANSEN);
		// the default corner of XNewton linear relaxation
		myxnewton = new LinearizerXTaylor(sys, LinearizerXTaylor::RELAX, LinearizerXTaylor::RANDOM_OPP, LinearizerXTaylor::HANSEN);
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
	if (myp_taylor)  delete myp_taylor;
#ifdef _IBEX_WITH_AFFINE_
	if (myart!=NULL) delete myart;
#endif
}

void LinearizerCombo::add_property(const IntervalVector& init_box, BoxProperties& prop) {
	if (myxnewton!=NULL) myxnewton->add_property(init_box,prop);
	if (myp_taylor) myp_taylor->add_property(init_box,prop);
#ifdef _IBEX_WITH_AFFINE_
	if (myart!=NULL) myart->add_property(init_box,prop);
#endif
}

int LinearizerCombo::linearize(const IntervalVector& box, LPSolver& lp_solver) {
	BoxProperties prop(box);
	return linearize(box, lp_solver, prop);
}

int LinearizerCombo::linearize(const IntervalVector& box, LPSolver& lp_solver, BoxProperties& prop) {

	int cont = 0;
	int cont2 ;
	switch (lmode) {
	case XNEWTON:
	case TAYLOR:
	case HANSEN:
		cont = myxnewton->linearize(box,lp_solver,prop);
		std::cout<< cont << std::endl;
		break;

	case PARALLEL:
		cont = myp_taylor->linearize(box,lp_solver,prop);
		break;

	case TAY_PAR:
		cont2 = myxnewton->linearize(box,lp_solver,prop);
		if (cont2!=-1) cont+=cont2;
		else return -1;
		cont2 += myp_taylor->linearize(box,lp_solver,prop);
		if (cont2!=-1) cont+=cont2;
		else return -1;
		break;

#ifdef _IBEX_WITH_AFFINE_
	case ART:
	case AFFINE2:
		cont = myart->linearize(box,lp_solver);

		break;
	case COMPO: {
		cont2 = myxnewton->linearize(box,lp_solver,prop);
		if (cont2!=-1) cont+=cont2;
		else return -1;
		cont2 = myart->linearize(box,lp_solver);
		if (cont2!=-1) cont+=cont2;
		else return -1;

		break;
		/*
		if (cont!=-1) {
			int cont2 = myart->linearize(box,lp_solver);
			if (cont2==-1) cont=-1;
			else cont+=cont2;
		}*/
	}

	case TAY_PAR_AFF:
		int cont2 = myp_taylor->linearize(box,lp_solver,prop);
		if (cont2!=-1) cont+=cont2;
		else return -1;
		cont2 = myxnewton->linearize(box,lp_solver,prop);
		if (cont2!=-1) cont+=cont2;
		else return -1;
		cont2 = myart->linearize(box,lp_solver);
		if (cont2!=-1) cont+=cont2;
		else return -1;
		break;

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

} // end namespace
