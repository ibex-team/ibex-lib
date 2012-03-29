/*---------------------------------------------------------------------------------
 * 3BCID contractor
 * ----------------------------------------------------------------------------
 *
 * Copyright (C) 2007 Gilles Chabert
 * 
 * This file is part of IBEX.
 *
 * IBEX is free software; you can redistribute it and/or modify it under the terms of 
 * the GNU General Public License as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later version.
 *
 * IBEX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with IBEX; 
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02110-1301, USA 
 *
 --------------------------------------------------------------------------------*/


#include "Ibex3BCID.h"

namespace ibex {
  
const int _3BCID::default_s3b = 10;
const int _3BCID::default_scid = 1;
const int _3BCID::default_vhandled = ALL_VARS;
const REAL _3BCID::default_var_min_width = 1.e-11;
const int _3BCID::LimitCIDDichotomy=16;


/* compare the boxes in all dimensions except one (var) */
bool  _3BCID::equalBoxes (int var, INTERVAL_VECTOR &box1, INTERVAL_VECTOR &box2) {
  int nb_var = Dimension(box1);
  
  for(int j=0;j<nb_var;j++){
    if(j!=var && box1(j+1)!=box2(j+1)) return false;
  }
  
  return true;
}


/** Applies 3BCID contraction on vhandled variables in cid_space, starting by start_var */
/* cid_space contains the variables to be shaved , space the variables for the subcontractor  */

void _3BCID::contract() {
  //const Indicators* indic=current_indic();
  int nb_var=Dimension(cid_space.box);
  start_var=nb_var-1;        //  patch pour l'optim  A RETIRER ??

  for (int var=start_var; var<start_var+vhandled; var++) {
    int var_env=cid_space.key(IBEX_VAR, var%nb_var);
    int var_space=space.component(var_env);
    var3BCID(var_space);

    if(space.box.empty())
      throw EmptyBoxException();

  }

  start_var=(start_var+vhandled)%nb_var;   //  en contradiction avec le patch pour l'optim
}




/** Applies 3BCID contraction on the variable var  */
/* computes the actual number of slices and the size of a slice,
   taning into account the size limit var_min_width 
   decides the shaving mode : dichotomy or slices */
bool  _3BCID::var3BCID(int var) {
  //const int n = Dimension(space.box);
  INTERVAL& dom(space.box(var+1));
   
  if (Diam(dom) < var_min_width) return false;  // domain already small enough : nothing to do

  REAL w_DC = Diam(dom) / s3b;
  int locs3b=s3b;
   
  // computes the actual number of slices s3b and the actual width of a slice w_DC,
  //  taking into account the minimum width of the slice and the
  // maximum number of slices 
  if (w_DC < var_min_width)
    {
      locs3b = (int) (Diam(dom)/var_min_width);
      if (locs3b < 2)  locs3b=2;
      w_DC= Diam(dom)/ locs3b;
    }

  // depending of the actual number of slices, calls the 3BCID contractor with a dichotomic or a linear shaving.
  if (locs3b > LimitCIDDichotomy)
    return var3BCID_dicho(var, w_DC);
  else
    return var3BCID_slices(var, locs3b, w_DC, dom);
}



/* Call the right, left (3B) and central (CID) contractions, the right and left shavings
   being done in a dichotomic way : returns true if a contraction is done*/

bool _3BCID::var3BCID_dicho(int var, REAL w3b){
  INTERVAL_VECTOR initbox = space.box;

  int r0= shave_bound_dicho(var, w3b, true);  // left shaving , after space.box contains the left slide
  if (Sup(space.box(var+1)) == Sup(initbox(var+1))) 
    return true; // the left slide reaches the right bound : nothing more to do
  INTERVAL_VECTOR leftbox=space.box;
  space.box=initbox;
  space.box(var+1)= INTERVAL(Sup(leftbox(var+1)),Sup(initbox(var+1)));
  int r1=0;
  try {
    r1= shave_bound_dicho (var,  w3b, false); // may throw Unfeasibility exception
  }
  catch (EmptyBoxException e)
    {space.box=leftbox; return true; // in case of Unfeasibility of the right shaving,
      //the contracted box becomes the left box
    }
  INTERVAL_VECTOR rightbox=space.box;
  space.box=initbox;
  space.box(var+1)= INTERVAL(Sup(leftbox(var+1)),Inf(rightbox(var+1))); // the central part 
  INTERVAL_VECTOR savebox=space.box;
  INTERVAL_VECTOR newbox= Hull (leftbox,rightbox);
  if(varCID(var,savebox,newbox)){
    space.box = newbox; return true;  // the contracted box is in newbox
  }
  else{ // VarCID was useless : one returns the result of only 3B : var is the only contracted variable 
    space.box = initbox;
    space.box(var+1) = INTERVAL(Inf(leftbox(var+1)),Sup(rightbox(var+1)));
    if (r0 | r1)
      return true;
    else
      return false;
  }
}


  // left shaving only (for optimization)
/*
bool _3BCID::var3BCID_dicho(int var, REAL w3b){
  INTERVAL_VECTOR initbox = space.box;

  int r0= shave_bound_dicho(var, w3b, true);  // left shaving , after space.box contains the left slide
  if (Sup(space.box(var+1)) == Sup(initbox(var+1))) 
    return true; // the left slide reaches the right bound : nothing more to do
  INTERVAL_VECTOR leftbox=space.box;
  space.box=initbox;
  space.box(var+1)= INTERVAL(Inf(leftbox(var+1)),Sup(initbox(var+1)));
  return r0;
}
*/
/*3B dicho   applies 3B left or right contraction
  returns in space.box  the left or right non empty slide ; may throw Unfeasiblity */
bool _3BCID::shave_bound_dicho(int var,  REAL wv, bool left) {

  INTERVAL_VECTOR initbox = space.box;
  INTERVAL_VECTOR& box(space.box);
  INTERVAL& x(box(var+1));

  REAL inf = Inf(x);                                // inf bound (to increase)
  REAL sup = Sup(x);                                // sup bound (to decrease)
  REAL inf0 = inf;
  REAL sup0 = sup;
  volatile REAL tmp;

  //cout.precision(20);
  // cout << endl << "Shaving variable " << space.var_name(var) << " bound " << (left?"left":"right") << endl;

  if (left) {

    REAL lb  = Mid(x);                              // left bound of the border 
    REAL rb  = sup;                                 // right bound of the border ((should decrease even when shaving
                                                    //    left, thanks to the "bound" test -not yet-)
    while (1) {
      //      cout << "  inf=" << inf << " lb=" << lb << " rb=" << rb << " sup=" << sup << endl;
      box(var+1) = INTERVAL(inf,lb);

      try {

	ctc.contract(space,Indicators(var,ALL_VARS));
	inf=Inf(box(var+1));
	volatile REAL mid = (inf+lb)/2;             // we must subdivide the current slice (declared volatile to prevent 
	                                            //   the compiler from expanding mid in the next line and using higher 
	                                            //   precision than REAL in the comparisons (messing up the result))
	if (lb-inf<=wv || inf>=mid || lb<=mid)      // the two last conditions prevent from splitting a float in half
	  break; 
	else lb=mid;                                // useless to restore domains (we divide the same slice)
	
      }catch(EmptyBoxException e) {       // the current slice has been cut off
	//	cout << "      slice removed.\n";
	if (inf==lb) {                              // border is degenerated and current=border
	  if (inf==sup)                             // current=border=the whole interval itself:
	    throw e;                                //   in this case the box must remain entirely emptied
	  else break;                               // return anyway (no more to do).
	}
	tmp = inf;                                  // current value of inf is used two lines below, save it
	inf = lb;                                   // increase the inf bound
	// lb = 3*lb-2*tmp;                            // optimistic choice: we double the width of the slice
	lb = 2*lb-tmp;                            // more realistic  choice: we take the width of the slice
	if (lb>rb) lb = rb;                         // the largest possible: lb<-rb => try the whole border interval once
	box = initbox;                              // restore domains (slice has changed)
      }
    }
  } else {

    REAL rb  = Mid(x);                              // right bound of the border
    REAL lb  = inf;                                 // left bound of the border (should increase even when shaving
                                                    //    right, thanks to the "bound" test -not yet-)
    while (1) {
      //      cout << "  inf=" << inf << " lb=" << lb << " rb=" << rb << " sup=" << sup << endl;
      box(var+1) = INTERVAL(rb,sup);

      try {

	ctc.contract(space,Indicators(var,ALL_VARS));
	sup=Sup(box(var+1));
	volatile REAL mid = (rb+sup)/2;             // we must subdivide the current interval (declared volatile to prevent 
	                                            //   the compiler from expanding mid in the next line and using higher 
	                                            //   precision than REAL in the comparisons (messing up the result))
	if (sup-rb<=wv || rb>=mid || sup<=mid)      // the two last conditions prevent from splitting a float in half
	  break; 
	else rb=mid;                                // useless to restore domains (we divide the same slice)

      } catch(EmptyBoxException e) {       // the current slice has been cut off
	//cout << "      slice removed.\n";
	if (sup==rb) {                              // border is degenerated and current=border
	  if (inf==sup)                             // current=border=the whole interval itself:
	    throw e;                                //   in this case the box must remain entirely emptied
	  else break;                               // return anyway (no more to do).
	}
	tmp = sup;                                  // current value of sup is used two lines below, save it
	sup = rb;                                   // decrease the sup bound
	//   	rb = 3*rb-2*tmp;                            // optimistic choice: we double the width of the slice
	rb = 2*rb-tmp;                            // more realistic  choice: we take the width of the slice
   	if (rb<lb)  rb = lb;                        // the largest possible: rb<-lb => try the whole border interval once
	box = initbox;                              // restore domains (slice has changed)
      } 
    }
  }
  if (inf < inf0 +wv && sup > sup0 -wv) return false;
  else return true;
}



/* Achieves the shaving in a "linear way" (left and right) and calls
   the CID central contraction.

   Principle: The "linear" shaving is applied to the left side and
   then to the right side of [var]. For the left (resp. right) side,
   from left to right (resp. from right to left), every sub-interval
   [x_i] (dom in the code below) in the left (resp. right) bound of
   [var] is computed (all [x_i] have the same size depending on
   locs3b); the subcontractor \a ctc is called on the corresponding
   sub-box [B_i] (i.e., the current box space.box in which [var] is
   replaced by [x_i], the other intervals being left unchanged); if
   [B_i] is empty, then it ensures that [x_i] can be safely removed
   from [x] (no solution in this slice) and the process continues
   (in a linear way) on the next rightmost (resp. leftmost) slice;
   if [B_i] is not empty, with [x_i] eventually reduced to [x_i]',
   the left (resp. right) shaving terminates and the corresponding
   sub-box [B_left] (resp. [B_right]) is stored to be handled by the
   CID process. In var3BCID_slices, newbox=Hull([B_left],[B_right])
   is transmitted to the method varCID (var3Bcid_box=newbox).

*/


bool _3BCID::var3BCID_slices(int var, int locs3b, REAL w_DC, INTERVAL& dom) {
   
  INTERVAL_VECTOR savebox(space.box);
   
  // Reduce left bound by shaving:

  bool stopLeft = false;
  REAL leftBound = Inf(dom);
  REAL rightBound = Sup(dom);
  REAL leftCID;
   
  int k=0;

  while (k < locs3b && ! stopLeft) {

    // Compute a slice 'dom'
    if (k > 0) space.box = savebox;
    REAL inf_k = Inf(dom)+k*w_DC;
    REAL sup_k = Inf(dom)+(k+1)*w_DC;
    if (sup_k > Sup(dom) || (k == locs3b - 1 && sup_k<Sup(dom))) sup_k = Sup(dom);
    dom = INTERVAL(inf_k, sup_k);

    // Try to refute this slice
    try{
      ctc.contract(space, Indicators(var,ALL_VARS));
    }catch(EmptyBoxException e) {
      leftBound = sup_k;
      k++;
      continue;
    }  
    //non empty box
    stopLeft = true;
    leftCID = sup_k;
    leftBound = Inf(dom);
    k++;
  }

  if (!stopLeft) { // all slices give an empty box
    space.box.set_empty();	
    throw EmptyBoxException();
  } else if (k == locs3b) {
    // Only the last slice gives a non-empty box : space.box is reduced to this last slice
    return true;
  }else {

    INTERVAL_VECTOR newbox (space.box); // newbox is initialized with the last slice handled in the previous loop
    int kLeft = k-1;

    // Reduce right bound by shaving:

    bool stopRight = false;
    REAL lastInf_k;
 
    int k2 = locs3b - 1;

    while (k2 > kLeft && ! stopRight) {

      // Compute slice
      space.box = savebox;
      REAL inf_k = Inf(dom) + k2 * w_DC;
      REAL sup_k = Inf(dom) + (k2+1) * w_DC;
      if (sup_k > Sup(dom) || (k2 == locs3b - 1 && sup_k < Sup(dom))) sup_k = Sup(dom);

      dom = INTERVAL(inf_k, sup_k);

      // Try to refute the slice
      try{
	ctc.contract(space,Indicators(var,ALL_VARS));
      }catch(EmptyBoxException e) {
	rightBound = sup_k;
	k2--;
	continue;
      }


      stopRight = true;
      lastInf_k = inf_k;
      rightBound = Sup(dom);

      k2--;

    }

    if (!stopRight) { // All the boxes visited in the second loop give an empty box
      space.box = newbox;
      return true;
    }else{

      int kRight = k2 +1;

      if (kLeft + 1 == kRight) { // No slice between the last handled left and right slices
	space.box = Hull(space.box, newbox);
	return true;
      }

      else{ // A varCID operation is performed on the remaining box :

	newbox = Hull (newbox, space.box);
	savebox(var+1)=INTERVAL(leftCID, lastInf_k);

	if(varCID(var,savebox,newbox)){ // Call to the central CID contraction
	  space.box = newbox;
	}else{ // VarCID was useless : one returns the result of only 3B
	  space.box = savebox;
	  space.box(var+1) = INTERVAL(leftBound, rightBound);
	}

      }

    }
  }
  return true;
}



// left only (for optimization)
/*
bool _3BCID::var3BCID_slices(int var, int locs3b, REAL w_DC, INTERVAL& dom) {
   
  INTERVAL_VECTOR savebox(space.box);
   
  // Reduce left bound by shaving:

  bool stopLeft = false;
  REAL leftBound = Inf(dom);
  REAL rightBound = Sup(dom);
  REAL leftCID;
   
  int k=0;

  while (k < locs3b && ! stopLeft) {

    // Compute a slice 'dom'
    if (k > 0) space.box = savebox;
    REAL inf_k = Inf(dom)+k*w_DC;
    REAL sup_k = Inf(dom)+(k+1)*w_DC;
    if (sup_k > Sup(dom) || (k == locs3b - 1 && sup_k<Sup(dom))) sup_k = Sup(dom);
    dom = INTERVAL(inf_k, sup_k);

    // Try to refute this slice
    try{
      ctc.contract(space, Indicators(var,ALL_VARS));
    }catch(EmptyBoxException e) {
      leftBound = sup_k;
      k++;
      continue;
    }  
    //non empty box
    stopLeft = true;
    leftCID = sup_k;
    leftBound = Inf(dom);
    k++;
  }

  if (!stopLeft) { // all slices give an empty box
    space.box.set_empty();	
    throw EmptyBoxException();
  } else if (k == locs3b) {
    // Only the last slice gives a non-empty box : space.box is reduced to this last slice
    return true;
  }else {

    INTERVAL_VECTOR newbox (space.box); // newbox is initialized with the last slice handled in the previous loop
    space.box=savebox;
    space.box(var+1)= INTERVAL(Inf(newbox(var+1)),Sup(savebox(var+1)));
    return true;  
  }
}
*/


/* central CID contraction : applies a constructive disjunction, i.e., divides the varcid_box into scid slices,
   contracts each slice and builds the hull of the contracted slices.
   This hull is made in var3Bcid_box that contains at the beginning the hull of the right and left slices that
   have been already treated by a shaving algorithm.
   During the CID algorithm, one contracts the scid central slices : for each slice, one calls the subcontractor
   with var as impact indicator for incrementality.

   returns true if one other variable, different from var, is contracted */

bool _3BCID::varCID(int var, INTERVAL_VECTOR &varcid_box, INTERVAL_VECTOR &var3Bcid_box){
  if(scid==0 || equalBoxes (var, varcid_box, var3Bcid_box)) return false;

  INTERVAL& dom(space.box(var+1));
  space.box=varcid_box;

   
  REAL w_DC = Diam(dom) / scid;
  for (int k = 0 ; k < scid ; k++) {
    if(k>0) space.box=varcid_box;
    // compute slice:
    REAL inf_k = Inf(dom) + k * w_DC;
    REAL sup_k = Inf(dom) + (k+1) * w_DC;
    if (sup_k > Sup(dom) || (k == scid-1 && sup_k < Sup(dom))) sup_k = Sup(dom);
    dom = INTERVAL(inf_k, sup_k);

    try{
      ctc.contract(space, Indicators(var,ALL_VARS));  // call of the subcontractor
    }catch(EmptyBoxException e) {
      continue;                                     // the current slice is unfeasible : nothing to add to the hull
    }

    var3Bcid_box = Hull(var3Bcid_box, space.box);  // add space.box to the hull 
    if(equalBoxes (var, varcid_box, var3Bcid_box)) return false; //VarCID was useles
  }

  return true;
}



}
