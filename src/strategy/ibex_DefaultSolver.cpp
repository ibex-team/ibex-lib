//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultSolver.cpp
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : March 21, 2013
//============================================================================

#include "ibex_DefaultSolver.h"
#include "ibex_SmearFunction.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcNewton.h"
#include "ibex_CtcXNewton.h"
#include "ibex_CtcXNewtonIter.h"
#include "ibex_CtcCompo.h"
#include "ibex_CellStack.h"
#include "ibex_Array.h"

namespace ibex {


// the corners for  Xnewton
std::vector<CtcXNewtonIter::corner_point>*  DefaultSolver::default_corners () {
	std::vector<CtcXNewtonIter::corner_point>* x;
	x= new std::vector<CtcXNewtonIter::corner_point>;
	x->push_back(CtcXNewtonIter::RANDOM);
	x->push_back(CtcXNewtonIter::RANDOM_INV);
	return x;
}

// the contractor list  hc4, acid(hc4), newton (if the system is square), xnewton
Array<Ctc>*  DefaultSolver::contractor_list (System& sys, double prec) {
	Array<Ctc>* ctc_list;
	ctc_list= new Array<Ctc>(4);
	// first contractor : non incremental hc4
	ctc_list->set_ref(0, *new CtcHC4 (sys.ctrs,0.01));
	// second contractor : acid (hc4)
	ctc_list->set_ref(1, *new CtcAcid (sys, *new CtcHC4 (sys.ctrs,0.1,true)));
	int index=2;
	// if the system is square, the third contractor is Newton
	if (sys.nb_var==sys.nb_ctr) {
		ctc_list->set_ref(index,*new CtcNewton(sys.f,5e8,prec,1.e-4));
		index++;
	}
	// the last contractor is XNewton
	//	ctc_list->set_ref(index,*new CtcXNewtonIter(sys,
	//                                          new CtcHC4 (sys.ctrs,0.01),
	//*(default_corners())));

	ctc_list->set_ref(index,*new CtcXNewton (*new CtcXNewtonIter (sys,*(default_corners())),
						 *new CtcHC4 (sys.ctrs,0.01)));

	ctc_list->resize(index+1);
	return ctc_list;
}


DefaultSolver::DefaultSolver(System& sys, double prec) : Solver(*new CtcCompo (* (contractor_list(sys,prec))),
		*new SmearSumRelative(sys,prec),
		*new CellStack(), prec),
		sys(sys), __ctc(dynamic_cast<CtcCompo*>(&ctc)), __bsc(&bsc),__buffer(&buffer) {

	srand(1);
}

// delete all objects dynamically created in the constructor  TO UPDATE if the constructor is changed

DefaultSolver::~DefaultSolver() {
	int ind_xnewton=2;
	if (sys.nb_var==sys.nb_ctr) ind_xnewton=3;
	delete &((dynamic_cast<CtcAcid*> (&__ctc->list[1]))->ctc);
	CtcCompo* ctccompo= dynamic_cast<CtcCompo*>(&(dynamic_cast<CtcXNewton*>( &__ctc->list[ind_xnewton])->ctc));
	delete &((dynamic_cast<CtcXNewtonIter*> (&(ctccompo->list[0]))->cpoints));
	delete &(ctccompo->list[0]);
	delete &(ctccompo->list[1]);
	for (int i=0 ; i<__ctc->list.size(); i++)
		delete &__ctc->list[i];
	delete __ctc;
	delete __bsc;
	delete __buffer;
}




} // end namespace ibex
