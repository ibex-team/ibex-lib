//============================================================================
//                                  I B E X
// File        : ibex_DefaultOptimizer.cpp
// Author      : Gilles Chabert , Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Oct 9, 2012
//============================================================================

#include "ibex_DefaultOptimizerMohc.h"
#include "ibex_SmearFunction.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcMohc.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_LinearRelaxCombo.h"
#include "ibex_BoolMask.h"
#include "ibex_CellStack.h"
#include "ibex_Array.h"
#include <vector>

namespace ibex {

using namespace std;

namespace {

const double default_relax_ratio = 0.2;

// These variable and function are necessary because we need
// to pass the extended system "ext_sys" to the base class
// constructor before it is built as a member of the class

ExtendedSystem* tmp_ext_sys=NULL;

  ExtendedSystem& get_ext_sys(System& sys,double goal_prec) {
	if (tmp_ext_sys==NULL)
	  tmp_ext_sys=new ExtendedSystem(sys,goal_prec);
	return *tmp_ext_sys;
}

}

// the defaultoptimizer constructor  1 point for sample_size
// the equality constraints are relaxed with goal_prec
DefaultOptimizerMohc::DefaultOptimizerMohc(System& _sys, double prec, double goal_prec
                                           , hc_ctc hcc, shav_ctc shavc, lr_ctc lrc) :
		Optimizer(_sys,
			  *new SmearSumRelative(get_ext_sys(_sys,goal_prec),prec),
			  *new CtcCompo (* (contractor_list(_sys,get_ext_sys(_sys,goal_prec),prec,hcc,shavc,lrc))), // warning: we don't know which argument is evaluated first (tmp_ext_sys may be NULL)
				  prec,
				  goal_prec,
				  goal_prec,
				  1),
		__ext_sys(tmp_ext_sys),
		__ctc(dynamic_cast<CtcCompo*>(&ctc)), __bsc(&bsc)  {

	tmp_ext_sys=NULL; // important! re-init for next call to constructor.

	}

// the corners for CtcXNewtonIter : one random orner and its opposite
/*vector<CtcXNewton::corner_point>*  DefaultOptimizer::default_corners () {
	vector<CtcXNewton::corner_point>* x;
	x= new vector<CtcXNewton::corner_point>;
	x->push_back(CtcXNewton::RANDOM);
	x->push_back(CtcXNewton::RANDOM_INV);
	return x;
}*/

// the contractor list  hc4, acid(hc4), xnewton
Array<Ctc>*  DefaultOptimizerMohc::contractor_list (System& sys2, System& ext_sys, double prec
                                                    , hc_ctc hcc, shav_ctc shavc, lr_ctc lrc) {
	Array<Ctc>* ctc_list;
	int index=0;

	ctc_list= new Array<Ctc>(4);

	  // first contractor : non incremental mohc/hc4/box
    CtcMohc* mohc=NULL;
    if(hcc>=MOHC50 && hcc<=MOHC100){
        double tau_mohc;
        switch(hcc){
            case MOHC50: tau_mohc=0.5; break;
            case MOHC60: tau_mohc=0.6; break;
            case MOHC70: tau_mohc=0.7; break;
            case MOHC80: tau_mohc=0.8; break;
            case MOHC90: tau_mohc=0.9; break;
            case MOHC99: tau_mohc=0.9999; break;
            case MOHC100: tau_mohc=1.1;
        }

        mohc=new CtcMohc (ext_sys.ctrs, 0.01, false, 0.01, CtcMohc::default_univ_newton_min_width, tau_mohc);
        ctc_list->set_ref(index, *mohc);
    }else if(hcc==HC4)
	    ctc_list->set_ref(index, *new CtcHC4 (sys.ctrs,0.01));

    index++;

	// second contractor : 3bcid/acid (mohc)
	if(shavc==ACID){
       if(hcc==HC4)  ctc_list->set_ref(index, *new CtcAcid (ext_sys, *new CtcHC4 (ext_sys.ctrs,0.1,true)));
	   else if(hcc>=MOHC50 && hcc<=MOHC100)
	     ctc_list->set_ref(index, *new CtcAcid (ext_sys,
            *new CtcMohc (ext_sys.ctrs,mohc->active_mono_proc,0.1,true, 0.1, CtcMohc::default_univ_newton_min_width)));

	   index++;
	}else if(shavc==_3BCID){
       if(hcc==HC4) ctc_list->set_ref(index, *new Ctc3BCid (*new CtcHC4 (ext_sys.ctrs,0.1,true),10,1));
	   else if(hcc>=MOHC50 && hcc<=MOHC100)
	     ctc_list->set_ref(index, *new Ctc3BCid (
            *new CtcMohc (sys.ctrs,mohc->active_mono_proc,0.1,true, 0.1, CtcMohc::default_univ_newton_min_width),10,1));
       index++;
	}


	// the last contractor is CtcXNewtonIter  with rfp=0.2 and rfp2=0.2
	// the limits for calling soplex are the default values 1e6 for the derivatives and 1e6 for the domains : no error found with these bounds
	if (lrc>=COMPO && sys.nb_ctr > 0){
        if(lrc==COMPO){
            ctc_list->set_ref(index,*new CtcFixPoint
			    (*new CtcCompo(
			    		*new CtcPolytopeHull(*new LinearRelaxCombo (ext_sys,LinearRelaxCombo::COMPO),
			    				CtcPolytopeHull::ALL_BOX),
			    		*new CtcHC4(ext_sys.ctrs,0.01)), default_relax_ratio));
        }else if(lrc==XN_ONLYY){
            ctc_list->set_ref(index,*new CtcFixPoint
			    (*new CtcCompo(
			    		*new CtcPolytopeHull(*new LinearRelaxCombo (ext_sys,LinearRelaxCombo::XNEWTON),
			    				CtcPolytopeHull::ONLY_Y),
			    		*new CtcHC4(ext_sys.ctrs,0.01)), default_relax_ratio));
        }
	    index++;
    }

	ctc_list->resize(index);
	return ctc_list;
}


// deletion of all dynamically created objects
DefaultOptimizerMohc::~DefaultOptimizerMohc() {
	delete &((dynamic_cast<CtcAcid*> (&__ctc->list[1]))->ctc);
	if (sys.nb_ctr > 0)
	  {CtcCompo* ctccompo= dynamic_cast<CtcCompo*>(&(dynamic_cast<CtcFixPoint*>( &__ctc->list[2])->ctc));
	    delete &(ctccompo->list[0]);
	    delete &(ctccompo->list[1]);
	  }
	for (int i=0 ; i<__ctc->list.size(); i++)
		delete &__ctc->list[i];
	delete __ctc;
	delete __bsc;
	delete __ext_sys;

}

} // end namespace ibex
