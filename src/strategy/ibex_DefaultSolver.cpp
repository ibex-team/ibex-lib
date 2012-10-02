//============================================================================
//                                  I B E X                                   
// File        : ibex_DefaultSolver.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Aug 27, 2012
// Last Update : Aug 27, 2012
//============================================================================

#include "ibex_DefaultSolver.h"
#include "ibex_SmearFunction.h"
#include "ibex_CtcHC4.h"
#include "ibex_CtcAcid.h"
#include "ibex_CtcNewton.h"
#include "ibex_XNewton.h"
#include "ibex_CtcCompo.h"
#include "ibex_BoolMask.h"
#include "ibex_CellStack.h"
#include "ibex_Array.h"
#include <vector>

namespace ibex {

static System* tmp;

  // the corners for  Xnewton
 std::vector<X_Newton::corner_point>*  DefaultSolver::default_corners ()
 { std::vector<X_Newton::corner_point>* x;
   x= new std::vector<X_Newton::corner_point>;
   x->push_back(X_Newton::RANDOM);
   x->push_back(X_Newton::RANDOM_INV);
   return x;}

  // the contractor list  hc4, acid(hc4), newton (if the system is square), xnewton 
  Array<Ctc>*  DefaultSolver::contractor_list (System& sys, double prec)
  {Array<Ctc>* ctc_list;
    ctc_list= new Array<Ctc>(4);
    // first contractor : non incremental hc4
    ctc_list->set_ref(0, *new CtcHC4 (sys.ctrs,0.01));
    // second contractor : acid (hc4)
    ctc_list->set_ref(1, *new CtcAcid (sys,BoolMask(sys.nb_var,1),
				      *new CtcHC4 (sys.ctrs,0.1,true)));
    int index=2;
    // if the system is square, the third contractor is Newton 
    if (sys.nb_var==sys.nb_ctr)
      {ctc_list->set_ref(index,*new CtcNewton(sys.f,5e8,prec,1.e-4));
	index++;}
    // the last contractor is XNewton
    ctc_list->set_ref(index,*new X_Newton(sys, 
					 new CtcHC4 (sys.ctrs,0.01),
					 *(default_corners()),
					 -1,0,0.2,0.2, X_Newton::X_NEWTON,X_Newton::HANSEN,100,1.e5,1.e4));
    ctc_list->resize(index+1);
    return ctc_list;}
		    


  DefaultSolver::DefaultSolver(System& sys, double prec) : Solver(*new CtcCompo (* (contractor_list(sys,prec))),
								  *new SmearSumRelative(sys,prec), 
								  *new CellStack(), prec),
							   sys(sys), __ctc(dynamic_cast<CtcCompo*>(&ctc)), __bsc(&bsc),__buffer(&buffer) {}

  // delete all objects dynamically created in the constructor  TO UPDATE if the constructor is changed

  DefaultSolver::~DefaultSolver() {
    int ind_xnewton=2;
    if (sys.nb_var==sys.nb_ctr) ind_xnewton=3;
    delete &((dynamic_cast<CtcAcid*> (&__ctc->list[1]))->ctc);
    delete (dynamic_cast<X_Newton*> (&__ctc->list[ind_xnewton]))->ctc;
    delete &((dynamic_cast<X_Newton*> (&__ctc->list[ind_xnewton]))->cpoints);
    for (int i=0 ; i<__ctc->list.size(); i++)
      delete &__ctc->list[i];
    delete __ctc;
    delete __bsc;
    delete __buffer;
  }




} // end namespace ibex
