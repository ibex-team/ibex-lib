//============================================================================
//                                  I B E X
// File        : ibex_DefaultSolver.cpp
// Author      : Bertrand Neveu, Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : March 21, 2013
//============================================================================

#include "ibex_DefaultSolverMohc.h"
#include "ibex_SmearFunction.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcMohc.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcNewton.h"
#include "ibex_CtcPolytopeHull.h"
#include "ibex_CtcCompo.h"
#include "ibex_CtcFixPoint.h"
#include "ibex_CellStack.h"
#include "ibex_LinearRelaxCombo.h"
#include "ibex_Array.h"

namespace ibex {

/* patch */
bool square_eq_sys2(const System& sys) {
	if (sys.nb_var!=sys.nb_ctr) return false;
	for (int i=0; i<sys.nb_ctr; i++)
		if (sys.ctrs[i].op!=EQ) return false;
	return true;
}

// the contractor list  hc4, acid(hc4), newton (if the system is square), xnewton
Array<Ctc>*  DefaultSolverMohc::contractor_list (System& sys, double prec,
            hc_ctc hcc, shav_ctc shavc, lr_ctc lrc) {
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

        mohc=new CtcMohc (sys.ctrs, 0.01, false, 0.01, CtcMohc::default_univ_newton_min_width, tau_mohc);
        ctc_list->set_ref(index, *mohc);
    }else if(hcc==HC4)
	    ctc_list->set_ref(index, *new CtcHC4 (sys.ctrs,0.01));

    index++;

	// second contractor : 3bcid/acid (mohc)
	if(shavc==ACID){
       if(hcc==HC4)  ctc_list->set_ref(index, *new CtcAcid (sys, *new CtcHC4 (sys.ctrs,0.1,true)));
	   else if(hcc>=MOHC50 && hcc<=MOHC100)
	     ctc_list->set_ref(index, *new CtcAcid (sys,
            *new CtcMohc (sys.ctrs,mohc->active_mono_proc,0.1,true, 0.1, CtcMohc::default_univ_newton_min_width)));

	   index++;
	}else if(shavc==_3BCID){
       if(hcc==HC4) ctc_list->set_ref(index, *new Ctc3BCid (*new CtcHC4 (sys.ctrs,0.1,true),10,1));
	   else if(hcc>=MOHC50 && hcc<=MOHC100)
	     ctc_list->set_ref(index, *new Ctc3BCid (
            *new CtcMohc (sys.ctrs,mohc->active_mono_proc,0.1,true, 0.1, CtcMohc::default_univ_newton_min_width),10,1));
       index++;
	}

	// if the system is square, the third contractor is Newton
	if (square_eq_sys2(sys)) {
		ctc_list->set_ref(index,*new CtcNewton(sys.f,5e8,prec,1.e-4));
		index++;
	}

	if(lrc==COMPO){
	  ctc_list->set_ref(index,*new CtcFixPoint(*new CtcCompo(
			*new CtcPolytopeHull(*new LinearRelaxCombo(sys,LinearRelaxCombo::COMPO),CtcPolytopeHull::ALL_BOX),
			*new CtcHC4 (sys.ctrs,0.01))));
	}

	ctc_list->resize(index);
	return ctc_list;
}


DefaultSolverMohc::DefaultSolverMohc(System& sys, double prec,
                    hc_ctc hcc, shav_ctc shavc, lr_ctc lrc) :
        Solver(*new CtcCompo (* (contractor_list(sys,prec,hcc,shavc,lrc))),
		*new SmearSumRelative(sys,prec),
		*new CellStack(), prec) ,sys(sys) , __bsc(&bsc),__buffer(&buffer){
//		, __ctc(dynamic_cast<CtcCompo*>(&ctc)), __bsc(&bsc),__buffer(&buffer) {

	srand(1);
}

// delete all objects dynamically created in the constructor  TO UPDATE if the constructor is changed

DefaultSolverMohc::~DefaultSolverMohc() {
	//int ind_xnewton=2;
	//if (square_eq_sys2(sys)) ind_xnewton=3;
	//delete &((dynamic_cast<CtcAcid*> (&__ctc->list[1]))->ctc);
	//CtcCompo* ctccompo= dynamic_cast<CtcCompo*>(&(dynamic_cast<CtcFixPoint*>( &__ctc->list[ind_xnewton])->ctc));
	//delete &(ctccompo->list[0]);
	//delete &(ctccompo->list[1]);
	//for (int i=0 ; i<__ctc->list.size(); i++)
		//delete &__ctc->list[i];
    //delete __ctc;
	delete __bsc;
	//delete __buffer;
}




} // end namespace ibex
