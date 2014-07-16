//============================================================================
//                                  I B E X                                   
// File        : ibex_OptimShaving.cpp
// Author      : Ignacio Araya, Gilles Chabert,
//               Bertrand Neveu, Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 4, 2014
// Last Update : Apr 4, 2014
//============================================================================

#include "ibex_CtcOptimShaving.h"

namespace ibex {

const int CtcOptimShaving::LimitCIDDichotomy=100;
  
  CtcOptimShaving::CtcOptimShaving(Ctc& ctc, int s3b, int scid, int vhandled, double var_min_width) :
	Ctc3BCid (ctc,s3b,scid,vhandled,var_min_width) {
}


  void CtcOptimShaving::contract(IntervalVector& box) {
    int	var_obj=start_var;
	impact.unset_all();                                // [gch]
	impact.set(var_obj);                              // [gch]
	var3BCID(box,var_obj);
	impact.unset(var_obj);                            // [gch]
	
	  if(box.is_empty()) throw EmptyBoxException();
	}


// left shaving only (for optimization)

  bool CtcOptimShaving::var3BCID_dicho(IntervalVector& box,int var, double w3b){
  IntervalVector initbox = box;

  int r0= shave_bound_dicho(box,var, w3b, true);  // left shaving , after box contains the left slide
  if (box[var].ub() == initbox[var].ub())
    return true; // the left slide reaches the right bound : nothing more to do
  IntervalVector leftbox=box;
  box=initbox;
  box[var]= Interval(leftbox[var].lb(),initbox[var].ub());
  return r0;
}
  int CtcOptimShaving::limitCIDDichotomy () 
  {return LimitCIDDichotomy;}
  

// left only (for optimization)

  bool CtcOptimShaving::var3BCID_slices(IntervalVector& box,int var, int locs3b, double w_DC, Interval& dom) {

  IntervalVector savebox(box);

  // Reduce left bound by shaving:

  bool stopLeft = false;
  //double leftBound = dom.lb();
  //double rightBound = dom.ub();
  //double leftCID;

  int k=0;

  while (k < locs3b && ! stopLeft) {

    // Compute a slice 'dom'
    if (k > 0) box = savebox;
    double inf_k = dom.lb()+k*w_DC;
    double sup_k = dom.lb()+(k+1)*w_DC;
    if (sup_k > dom.ub() || (k == locs3b - 1 && sup_k<dom.ub())) sup_k = dom.ub();
    dom = Interval(inf_k, sup_k);

    // Try to refute this slice
    try{
      ctc.contract(box,impact);
    }catch(EmptyBoxException e) {
      //leftBound = sup_k;
      k++;
      continue;
    }
    //non empty box
    stopLeft = true;
    //leftCID = sup_k;
    //leftBound = dom.lb();
    k++;
  }

  if (!stopLeft) { // all slices give an empty box
    box.set_empty();
    throw EmptyBoxException();
  } else if (k == locs3b) {
    // Only the last slice gives a non-empty box : box is reduced to this last slice
    return true;
  }else {

    IntervalVector newbox (box); // newbox is initialized with the last slice handled in the previous loop
    box=savebox;
    box[var]= Interval(newbox[var].lb(),savebox[var].ub());
    return true;
  }
  }
  
}

