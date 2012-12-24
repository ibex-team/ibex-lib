//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultOptimizer.cpp
// Author      : Gilles Chabert , Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Oct 9, 2012
//============================================================================

#include "ibex_DefaultOptimizer.h"
#include "ibex_SmearFunction.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcCompo.h"
#include "ibex_BoolMask.h"
#include "ibex_CellStack.h"
#include "ibex_Array.h"
#include <vector>

namespace ibex {

using namespace std;


namespace {

// These variable and function are necessary because we need
// to pass the extended system "ext_sys" to the base class
// constructor before it is built as a member of the class

System* tmp_ext_sys=NULL;

System& get_ext_sys(System& sys) {
	if (tmp_ext_sys==NULL)
		tmp_ext_sys=new System(sys,System::EXTEND);
	return *tmp_ext_sys;
}

}

// the defaultoptimizer constructor  1 point for sample_size
DefaultOptimizer::DefaultOptimizer(System& _sys, double prec, double goal_prec) :
		Optimizer(_sys,
				  *new SmearSumRelative(get_ext_sys(_sys),prec),
				  *new CtcCompo (* (contractor_list(_sys,get_ext_sys(_sys),prec))), // warning: we don't know which argument is evaluated first (tmp_ext_sys may be NULL)
				  prec,
				  goal_prec,
				  goal_prec,
				  1),
		__ext_sys(tmp_ext_sys),
		__ctc(dynamic_cast<CtcCompo*>(&ctc)), __bsc(&bsc)  {
  
	tmp_ext_sys=NULL; // important! re-init for next call to constructor.

	srand(1);}

// the corners for X_Newton : one random orner and its opposite
vector<X_Newton::corner_point>*  DefaultOptimizer::default_corners () {
	vector<X_Newton::corner_point>* x;
	x= new vector<X_Newton::corner_point>;
	x->push_back(X_Newton::RANDOM);
	x->push_back(X_Newton::RANDOM_INV);
	return x;
}

// the contractor list  hc4, acid(hc4), xnewton
Array<Ctc>*  DefaultOptimizer::contractor_list (System& sys, System& ext_sys,double prec) {
	Array<Ctc>* ctc_list;
	ctc_list= new Array<Ctc>(3);
	// first contractor on ext_sys : incremental hc4  ratio propag 0.01
	ctc_list->set_ref(0, *new CtcHC4 (ext_sys.ctrs,0.01,true));
	// second contractor on ext_sys : acid (hc4)   with incremental hc4  ratio propag 0.1
	ctc_list->set_ref(1, *new CtcAcid (ext_sys,BoolMask(ext_sys.nb_var,1),
			             *new CtcHC4 (ext_sys.ctrs,0.1,true)));
	// the last contractor is X_Newton  with rfp=0.2 and rfp2=0.2
	// the limits for calling soplex are 1e5 for the derivatives and 1e5 for the domains : no error found with these bounds
	ctc_list->set_ref(2,*new X_Newton(ext_sys,
			new CtcHC4 (ext_sys.ctrs,0.01),  // called in the X_Newton external loop
			*(default_corners()),
					  0,sys.goal,0.2,0.2,LR_contractor::ALL_BOX,X_Newton::HANSEN,100,1.e6,1.e6));

	return ctc_list;
}


// deletion of all dynamically created objects
DefaultOptimizer::~DefaultOptimizer() {
	delete &((dynamic_cast<CtcAcid*> (&__ctc->list[1]))->ctc);
	delete (dynamic_cast<X_Newton*> (&__ctc->list[2]))->ctc;
	delete &((dynamic_cast<X_Newton*> (&__ctc->list[2]))->cpoints);
	for (int i=0 ; i<__ctc->list.size(); i++)
		delete &__ctc->list[i];
	delete __ctc;
	delete __bsc;
	delete __ext_sys;
}

} // end namespace ibex
