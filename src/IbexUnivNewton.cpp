/*---------------------------------------------------------------------------------
 * 
 * Univariate Newton contractor/solving
 * ------------------------------------
 *
 * Copyright (C) 2006 Gilles Chabert
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

#include <vector>
#include "IbexUnivNewton.h"

namespace ibex {

/*! Default precision for univariate Newton iteration. */
#define __IBEX_DEFAULT_U_NEWTON_PRECISION  1.e-7

/* Executes one step of Newton iteration and returns delta 
 * \param p - expansion point in [X]
 * \param F_p - interval evaluation of F at p (F(p))
 * \warning [X] and DF=F'[X] must be initialized */

REAL UnivNewton::newton_step(const REAL& p, const INTERVAL& F_p) {
//   cout << "--- newton step ---" << endl;
//   cout << " X = " << X <<  "(diam=" << Diam(X) << " DF =  " << DF << endl;
//   cout << "expansion point is " << p << " (F(p) is " << F_p << ")" << endl;

  INTERVAL Y = (X - p);
  INTERVAL Y1 = Y;
  X = p;                                   // temporary assignment  
  INTERVAL Y_other;

  if (solve_mode) {
    if (!Y.div_intersect(-F_p, DF, &Y_other)) throw EmptyBoxException();

    if (!Y_other.empty()) {
//       cout << " ZERO found: X1=" << (Y+p) << " and X2=" << (Y_other+p) << endl;
      stack->push_back(Y_other+p);
    }
  } else 
    if (!Y.div_intersect(-F_p, DF)) throw EmptyBoxException();  

//   cout << "X after = " << (Y+p) << " (delta = " << Y1.delta(Y) << ")" << endl;
  X = Y + p;
  return Y1.delta(Y);
}


/* Executed when 0 is not in DF=F'[X] and 0 is in F(p).
 * Performs all remaining steps of Newton iteration.
 * \param p - expansion point in [X].
 * \see section 9.4 in Hansen's book. 
 * \warning [X] and DF=F'[X] must be initialized. */
void UnivNewton::zero_not_in_DF(REAL p) {
  REAL a,b;
  INTERVAL F_a, F_b;  
  bool flagA = false;
  bool flagB = false;
  int n = 0;
  
  do {
    if (!flagA) {
      a = Inf(X);
      F_a = eval_pt(a);
      if (F_a.contains(0)) flagA = true;
      else newton_step(a, F_a); // this step cannot create 2 intervals (namely, at most 1)	
    }
 
    if (!flagB) {
      b = Sup(X);
      F_b = eval_pt(b);
      if (F_b.contains(0)) flagB = true;
      else {
	if (X.contains(p))
	  newton_step(b, F_b); 	// this step can not create 2 intervals
	else {
	  newton_step(Mid(X), eval_pt(Mid(X)));	  
	}
      }      
    }
    n++;
  } while ((n<4) && !(flagA && flagB));
}

/* Executed when 0 is in both F(p) and F'[X].
 * Performs just one step of Newton iteration.
 * \param p - expansion point in [X].
 * \see section 9.4 in Hansen's book. 
 * \warning [X] and DF=F'[X] must be initialized. */
REAL UnivNewton::zero_in_DF() {
  
  REAL a = Inf(X);
  INTERVAL F_a = eval_pt(a);
  //cout << "a=" << a << " and F(a)=" << F_a << endl;
  if (!F_a.contains(0)) return newton_step(a, F_a);
  
  REAL b = Sup(X);
  INTERVAL F_b = eval_pt(b);
  if (!F_b.contains(0)) return newton_step(b, F_b);
    
  REAL x1 = (3*a+b)/4;
  INTERVAL F_x1 = eval_pt(x1);
  REAL x2 = (a+3*b)/4;
  INTERVAL F_x2 = eval_pt(x2);

  if (((!F_x1.contains(0)) || (!F_x2.contains(0))) && solve_mode) {
    
    stack->push_back(INTERVAL((a+b)/2, b));
    
    X = INTERVAL(a, (a+b)/2);

    return newton_step(x1, F_x1);
  } else {      // else we leave the box "like this"   */
    return 0.0; // no reduction enforced
  }
}

/* Applies all necessary steps of Newton iteration   
 * on interval (with possibly thick parameters) [X].
 * Stops when reduction enforced is less than w. */
void UnivNewton::newton_iterate() {
  newton_iterate(w);
}


/* Called with a precision (when it is called by BoxNarrow, 
 * this precision is >= w) */
void UnivNewton::newton_iterate(REAL prec) {
  
  REAL delta=prec;
  REAL xm;
  INTERVAL F_xm;
  int bderiv=0;  // boolean indicating that the derivative is already computed
  do {
    try {
      DF = deriv(); // forward EmptyBoxException

    }
    catch (NotDifferentiableException e) { return; }
    catch (UnboundedResultException e) { return; }
    
    if (!DF.contains(0)) {bderiv=1;break;}             // try better method in case of monotonicity
    
    xm = Mid(X);  F_xm = eval_pt(xm);
    
    delta = F_xm.contains(0)? zero_in_DF() : newton_step(xm, F_xm);
        
  } while (delta>=prec);

  if (delta>=prec) {   // <=> Monotonicity

    //cout << " monotonicity" << endl;
    do {      
      try {
	if (bderiv==0) DF = deriv();  // forward EmptyBoxException

	
      }
      catch (NotDifferentiableException e) { return; }
      catch (UnboundedResultException e) { return; }
      
      bderiv=0;   
      xm = Mid(X);  F_xm = eval_pt(xm);
      
      if (F_xm.contains(0)) break;
      
      delta = newton_step(xm, F_xm);

    } while (delta>=prec); // this condition is useless in theory
    // but in practice an infinite loop may occur with xm very close to
    // a root, due to interval rounding (narrowing is not effective)
    
    zero_not_in_DF(xm);
  }
}

/* Monotonone iterate - called by Octum  -   
   Gradient already computed by Octum - monotonicity true*/
void UnivNewton::newton_monotone_iterate(REAL prec) {
  
  if (prec < w) prec=w;
  REAL delta=prec;
  REAL xm;
  INTERVAL F_xm;

    do { 
      xm = Mid(X);  F_xm = eval_pt(xm);
      
      if (F_xm.contains(0)) break;
      
      delta = newton_step(xm, F_xm);

    } while (delta>=prec); // this condition is useless in theory
    // but in practice an infinite loop may occur with xm very close to
    // a root, due to interval rounding (narrowing is not effective)
    
    zero_not_in_DF(xm);
}


/** Weak variant. Only contract the initial interval with Newton iteration (splitting is not allowed).
 */
bool UnivNewton::contract(int var) {

  this->var = var;

  if (space.box.empty()) return false; // return INTERVAL::EMPTY;

  X = space.box(var+1);

  //  if (env.adj(key).find(env.key(IBEX_VAR,var))=env.adj(key).end()) return; // return X;

  solve_mode = false;

  //if (Diam(X) < w) return;   // interval too small

  newton_iterate();  
  
  space.box(var+1) = X; 
  
  return true;
}


/* modif BNE : added  a precision parameter transmitted to univnewton  : called by BoxNarrow*/
bool UnivNewton::contract(int var, REAL prec) {

  this->var = var;

  if (space.box.empty()) return false; // return INTERVAL::EMPTY;

  X = space.box(var+1);

  //  if (env.adj(key).find(env.key(IBEX_VAR,var))=env.adj(key).end()) return; // return X;

  solve_mode = false;

  //if (Diam(X) < w) return;   // interval too small

  newton_iterate(prec);  
  
  space.box(var+1) = X; 
  
  return true;
}


/* for Octum : the gradient is already computed in Gv and the monotonicity condition is true */
bool UnivNewton::monotone_contract(int var, INTERVAL & Gv, REAL prec) {

  this->var = var;

  if (space.box.empty()) return false; // return INTERVAL::EMPTY;

  X = space.box(var+1);

  //  if (env.adj(key).find(env.key(IBEX_VAR,var))=env.adj(key).end()) return; // return X;

  solve_mode = false;

  //if (Diam(X) < w) return;   // interval too small

  DF=Gv;

  newton_monotone_iterate(prec);  
  
  space.box(var+1) = X; 
  
  return true;
}


void UnivNewton::contract() {
  const Indicators* indic=current_indic();

  if (indic==NULL || indic->scope==ALL_VARS) {
    for (int i=0; i<space.nb_var(); i++)
      contract(i); // (impact is ignored anyway).
  } else
    contract(indic->scope);
}
  
/** Strong variant. Try to find all the roots on the initial interval (splitting is allowed).
 * Splitting only occurs with a division by 0.
 */
INTERVAL_VECTOR UnivNewton::solve(int var) {  

  this->var = var;

  if (space.box.empty()) return INTERVAL_VECTOR(0);
  
  INTERVAL save = space.box(var+1);

  vector<INTERVAL> roots; // vector of roots found

//  if (env.adj(key).find(env.key(IBEX_VAR,var))=env.adj(key).end()) 
//     INTERVAL_VECTOR res(1);
//     res(1)=space.box(var+1);
//     return res;
//   }   // no change
    
  stack = new vector<INTERVAL>();

  stack->push_back(save);

  solve_mode = true;

  while (!stack->empty()) {

    X = stack->back();
    stack->pop_back();
    
    if (Diam(X) >= w) 
      try {
	newton_iterate();  
	roots.push_back(X);
      } catch(EmptyBoxException) {}
  }
  delete stack;

  INTERVAL_VECTOR res(roots.size());
  for (unsigned int i=0; i<roots.size(); i++) res(i+1) = roots[i];
 
  space.box(var+1) = save;
  return res;
}

const REAL UnivNewton::default_precision = __IBEX_DEFAULT_U_NEWTON_PRECISION;

} // end namespace
