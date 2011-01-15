/*---------------------------------------------------------------------------------
 * 
 * Generic Shaving-based contractor (3B consistency, Bound consistency, etc.)
 * ----------------------------------------------------------------------------
 *
 * Copyright (C) 2007-2009 Gilles Chabert
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

#include "IbexShaving.h"
#include "IbexPropagation.h" // just for sub-contractor default initialization 

namespace ibex {

/*! Default criterion used for shaving one variable bound. */
#define __IBEX_DEFAULT_SHAVE_VAR_RATIO     0.1

/*! Default criterion used for shaving main loop. */
#define __IBEX_DEFAULT_SHAVE_RATIO     BiasPosInf


Shaving::Shaving(const Contractor& ctc, REAL ratio, REAL var_ratio) 
  : Operator(ctc.space), ctc(*ctc.copy()), ratio(ratio), var_ratio(var_ratio)
{
  this->ctc.contract_floor=0;
}


/** Duplicate this instance (space is passed by reference). */
Shaving::Shaving(const Shaving& s) 
  : Operator(s.space), Contractor(s), ctc(*s.ctc.copy()), ratio(s.ratio), var_ratio(s.var_ratio) 
{ 

}


void Shaving::shave_bound(int var, REAL var_ratio, bool left) {
  
  INTERVAL_VECTOR& box(space.box);
  INTERVAL_VECTOR initbox = box;
  INTERVAL& x(initbox(var+1));
  REAL wv  = var_ratio*Diam(x);
  REAL inf = Inf(x);                                // inf bound (to increase)
  REAL sup = Sup(x);                                // sup bound (to decrease)
  volatile REAL tmp;

  //cout.precision(20);
  //cout << endl << "Shaving variable " << var_name(var) << " bound " << (left?"left":"right") << endl;

  if (left) {

    REAL lb  = Mid(x);                              // left bound of the border 
    REAL rb  = sup;                                 // right bound of the border ((should decrease even when shaving
                                                    //    left, thanks to the "bound" test -not yet-)
    while (1) {
      //cout << "  inf=" << inf << " lb=" << lb << " rb=" << rb << " sup=" << sup << endl;
      box(var+1) = INTERVAL(inf,lb);

      try {
	ctc.contract(space, Indicators(var,ALL_VARS));

	volatile REAL mid = (inf+lb)/2;             // we must subdivide the current slice (declared volatile to prevent 
	                                            //   the compiler from expanding mid in the next line and using higher 
	                                            //   precision than REAL in the comparisons (messing up the result))
	if (lb-inf<=wv || inf>=mid || lb<=mid)      // the two last conditions prevent from splitting a float in half
	  break; 
	else lb=mid;                                // useless to restore domains (we divide the same slice)
	
      } catch(EmptyBoxException e) {       // the current slice has been cut off
	//cout << "      slice removed.\n";
	if (inf==lb) {                              // border is degenerated and current=border
	  if (inf==sup)                             // current=border=the whole interval itself:
	    throw e;                                //   in this case the box must remain entirely emptied
	  else break;                               // return anyway (no more to do).
	}
	tmp = inf;                                  // current value of inf is used two lines below, save it
	inf = lb;                                   // increase the inf bound
	lb = 3*lb-2*tmp;                            // optimistic choice: we double the width of the slice
	if (lb>rb) lb = rb;                         // the largest possible: lb<-rb => try the whole border interval once
	box = initbox;                              // restore domains (slice has changed)
      }
    }
  } else {

    REAL rb  = Mid(x);                              // right bound of the border
    REAL lb  = inf;                                 // left bound of the border (should increase even when shaving
                                                    //    right, thanks to the "bound" test -not yet-)
    while (1) {
      //cout << "  inf=" << inf << " lb=" << lb << " rb=" << rb << " sup=" << sup << endl;
      box(var+1) = INTERVAL(rb,sup);

      try {
	ctc.contract(space, Indicators(var,ALL_VARS));

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
   	rb = 3*rb-2*tmp;                            // optimistic choice: we double the width of the slice
   	if (rb<lb)  rb = lb;                        // the largest possible: rb<-lb => try the whole border interval once
	box = initbox;                              // restore domains (slice has changed)
      } 
    }
  }

  box = initbox;                                    // restore domains
  //cout << "result : [" << inf << "," << sup << "]\n";
  box(var+1) = INTERVAL(inf,sup);                   // set shaved domain to the final result
}

void Shaving:: shave(int var) {

  int nb_var = Dimension(space.box);

  //bool result=
  ctc.contract(space); // may throw unfeasibility 
  
  bool loop;
  do {
    //  result |= loop;      
    loop=false;

    for (int v=0; v<nb_var; v++) {
      if (var!=ALL_VARS && v!=var) continue;

        INTERVAL tmp=space.box(v+1);
        shave_bound(v, var_ratio, true); // may throw unfeasibility
        shave_bound(v, var_ratio, false);
        loop |= (tmp.ratiodelta(space.box(v+1))>=ratio);
      }
  } while (loop);
  
  //return result;

}
  
void Shaving::contract() {
  const Indicators* indic=current_indic();
  
  shave(indic==NULL? ALL_VARS : indic->scope);
}

const REAL Shaving::default_ratio = __IBEX_DEFAULT_SHAVE_RATIO;
  
const REAL Shaving::default_var_ratio = __IBEX_DEFAULT_SHAVE_VAR_RATIO;
  
/*--------------------------------------------------------------------------------*/
/*
class ExtraVarSpaceFac : public NewSpaceFactory {
public:
  
  ExtraVarSpaceFac(const Space& initspace, int key) : NewSpaceFactory(initspace.env), initspace(initspace), key(key) { }

  Space* build_space(Space* space) const {
    
    for (int i=0; i<initspace.env.nb_keys(); i++) {
      if (initspace.used(i)) {
	const Entity& e=initspace.entity(i);
	((ExtraVarSpaceFac&) *this).NewSpaceFactory::set_entity(e.key, i==key? IBEX_VAR : e.type);	
      }
    }
    return NewSpaceFactory::build_space(space);
  }

  int key;
  const Space& initspace;
};

class BasicBisector : public Bisector {
public:
  BasicBisector(Space& space, int var) : Bisector(space), var(var) {}

  // Duplicate this instance (space is passed by reference). 
  BasicBisector(const BasicBisector& b) : Bisector(b.space), var(b.var) {}

  // Overrides the interface of the generic operator copy. 
  virtual BasicBisector* copy() const { return new BasicBisector(*this); }

  void accept_printer(const OperatorPrinter&) const {   }

  // Bisects the current cell. 
  virtual Bisection bisect(Cell& cell) const {
    return Bisection(cell.space.box, var);
  }

  int var;
};*/

Slicer::Slicer(const Contractor& ctc, int key, int nb_slices, bool logic_op) : 
  Operator(ctc.space), ctc(*ctc.copy()), key(key), nb_slices(nb_slices), quantifier(logic_op) { }

 // ctc.space.entity(key).type==IBEX_UPR) { }

Slicer::Slicer(const Slicer& s) : 
  Operator(s.space), Contractor(s), ctc(*s.ctc.copy()), key(s.key), nb_slices(s.nb_slices), quantifier(s.quantifier) { }																	       
Slicer::~Slicer() {
  delete &ctc;
}

void Slicer::contract() {
  
  INTERVAL_VECTOR init_box = space.box;
  INTERVAL& param = space.Space::domain(key);
  REAL step = Diam(param)/nb_slices;
  
  VECTOR subdiv(nb_slices+1);
  subdiv(1)=Inf(param);
  for (int i=2; i<=nb_slices; i++) subdiv(i)=subdiv(i-1)+step;
  subdiv(nb_slices+1)=Sup(param);
//   cout << "init param domain=" << param << endl;
//   cout << "subdiv=" << subdiv << endl;

  //cout << "init_box=" << init_box << endl;

  /* init */
  INTERVAL_VECTOR result_box(space.nb_var());
  if (quantifier) 
    result_box = space.box;
  else
    result_box.set_empty();

  INTERVAL init_param = param;
  for (int i=1; i<=nb_slices+1; i++) {
    space.box = init_box;

    if (quantifier)
      param = INTERVAL(subdiv(i),subdiv(i));
    else
      if (i==nb_slices+1) continue;
      else param = INTERVAL(subdiv(i),subdiv(i+1));
    
    try {
      ctc.contract();
      if (quantifier) {
	if (! (result_box &= space.box)) throw EmptyBoxException();
      } else
	result_box |= space.box;      
    } catch(EmptyBoxException e) {      
      if (quantifier) {
	param = init_param;
	throw e;
      }
    }
  }
  param = init_param; // restor parameters

  if (result_box.empty()) throw EmptyBoxException();

  //cout << "result_box=" << result_box << endl;
/*  bool reducted=init_box.maxratiodelta(result_box) > 0.02;   

  space.box = reducted? result_box : init_box; 

  return reducted;*/
}

} // end namespace
