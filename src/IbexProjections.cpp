/*---------------------------------------------------------------------------------
 * 
 * Implementation of projections
 * -----------------------------
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

#include "IbexProjections.h"
#include "Functions.h"
#include "IbexToken.h"

namespace ibex {

/* project x=y*z over y and z */
bool proj_mult(const INTERVAL& x, INTERVAL& y, INTERVAL& z) {
  if (x.contains(0)) {
    if (!z.contains(0))                           // if x and z contains 0, y can be any real number.
      if (!y.div_intersect(x,z)) return false;    // otherwise x=y*z => y=x/z
    return y.contains(0) || z.div_intersect(x,y);
  } else {
      if (!y.div_intersect(x,z)) return false;
      return z.div_intersect(x,y);
  }
}

bool proj_trigo(const INTERVAL& f_evl, INTERVAL& f_exp_evl, int ftype) { //, bool& continuity, bool& monotonicity) {
  
  if (Inf(f_exp_evl)==BiasNegInf || Sup(f_exp_evl)==BiasPosInf) return true; // infinity of periods
  
  INTERVAL period_0, nb_period;

  switch (ftype) {
  case COS :
    period_0 = ArcCos(f_evl);
    nb_period = f_exp_evl / INTERVAL::Pi;
    break;
  case SIN :
    period_0 = ArcSin(f_evl);
    nb_period = (f_exp_evl+(INTERVAL::Pi/2.0)) / INTERVAL::Pi;
    break;
  case TAN :
  default :
    period_0 = ArcTan(f_evl);
    nb_period = (f_exp_evl+(INTERVAL::Pi/2.0)) / INTERVAL::Pi;
    break;
  }
  if (period_0.empty()) return false;
  
  int p1 = ((int) Inf(nb_period))-1;
  int p2 = ((int) Sup(nb_period));
  INTERVAL tmp1, tmp2;

  bool found = false;
  int i = p1-1;

  switch(ftype) {
  case COS :
    // should find in at most 2 turns.. but consider rounding !
    while (++i<=p2 && !found) found = Intersection(tmp1, i%2==0? period_0 + i*INTERVAL::Pi : (i+1)*INTERVAL::Pi - period_0, f_exp_evl);
    break;
  case SIN :
    while (++i<=p2 && !found) found = Intersection(tmp1, i%2==0? period_0 + i*INTERVAL::Pi : i*INTERVAL::Pi - period_0, f_exp_evl);
    break;
  case TAN :
    while (++i<=p2 && !found) found = Intersection(tmp1, period_0 + i*INTERVAL::Pi, f_exp_evl);
    break;    
  }

  if (!found) return false;
  found = false;
  i=p2+1;
  
  switch(ftype) {
  case COS :
    while (--i>=p1 && !found) found = Intersection(tmp2, i%2==0? period_0 + i*INTERVAL::Pi : (i+1)*INTERVAL::Pi - period_0, f_exp_evl);
    break;
  case SIN :
    while (--i>=p1 && !found) found = Intersection(tmp2, i%2==0? period_0 + i*INTERVAL::Pi : i*INTERVAL::Pi - period_0, f_exp_evl);
    break;
  case TAN :
    while (--i>=p1 && !found) found = Intersection(tmp2, period_0 + i*INTERVAL::Pi, f_exp_evl);
    break;    
  }

  if (!found) return false;

//   if (Sup(tmp1)<Inf(tmp2)) continuity = false;
//   if (tmp1!=tmp2) monotonicity = false; // not in the same period
  
  f_exp_evl = Hull(tmp1, tmp2);

  return true;
}

bool proj_power(const INTERVAL& p_evl, INTERVAL& p_exp_evl, int expon) { //, bool& continuity, bool& monotonicity) {
  
  if (expon % 2 ==0) {

    INTERVAL proj=Root(p_evl, expon);
    
    if (proj.empty()) return false;
    
    INTERVAL tmp1, tmp2;      
    bool pos_proj=Intersection(tmp1, proj, p_exp_evl);
    bool neg_proj=Intersection(tmp2, -proj, p_exp_evl);
    
    if (pos_proj && neg_proj) {
//       monotonicity = false;
//       if (Inf(p_evl)>0) continuity = false;
      p_exp_evl = Hull(tmp1, tmp2);
    } 
    else if (pos_proj) p_exp_evl = tmp1;
    else if (neg_proj) p_exp_evl = tmp2;
    
    return (pos_proj | neg_proj);
  
  } else {

    return p_exp_evl &= Root(p_evl, expon);
    
  }
}

bool proj_sqr(const INTERVAL& evl, INTERVAL& exp_evl) { //, bool& continuity, bool& monotonicity) {

  INTERVAL proj=Sqrt(evl);

  if (proj.empty()) return false;
  
  INTERVAL tmp1, tmp2;      
  bool pos_proj=Intersection(tmp1, proj, exp_evl);
  bool neg_proj=Intersection(tmp2, -proj, exp_evl);
    
  if (pos_proj && neg_proj) {
//     monotonicity = false;
//     if (Inf(evl)>0) { 
//       continuity = false; 
//     }
    exp_evl = Hull(tmp1, tmp2);
  } 
  else if (pos_proj)  exp_evl = tmp1;
  else if (neg_proj) exp_evl = tmp2;   

  return (pos_proj | neg_proj);   
}

bool proj_cosh(const INTERVAL& evl, INTERVAL& exp_evl) { //, bool& continuity, bool& monotonicity) {

  INTERVAL proj=ArCosh(evl);
  if (proj.empty()) return false;
  
  INTERVAL tmp1, tmp2;      
  bool pos_proj=Intersection(tmp1, proj, exp_evl);
  bool neg_proj=Intersection(tmp2, -proj, exp_evl);
    
  if (pos_proj && neg_proj) {
//     monotonicity = false;
//     if (Inf(evl)>1.0) continuity = false;
    exp_evl = Hull(tmp1, tmp2);
  } 
  else if (pos_proj)  exp_evl = tmp1;
  else if (neg_proj) exp_evl = tmp2;   
  
  return (pos_proj | neg_proj);   
}

bool proj_exp(const INTERVAL& evl, INTERVAL& exp_evl) {

  INTERVAL result = Log(evl);
  if (result.empty()) return false; // ? possible in 2B?
  else return exp_evl &= result;
}

bool proj_tanh(const INTERVAL& evl, INTERVAL& exp_evl) {

  INTERVAL result = ArTanh(evl);
  if (result.empty()) return false; // ? possible in 2B?
  else return exp_evl &= result;
}

bool proj_arccosh(const INTERVAL& evl, INTERVAL& exp_evl) {
  if (Sup(evl)<0.0) return false;
  else return exp_evl &= Cosh(INTERVAL(Inf(evl)<0?0:Inf(evl),Sup(evl)));
}

bool proj_max(const INTERVAL& evl, INTERVAL& x, INTERVAL& y) {

  /* ---- Disjoint intervals ---- */
  if (Inf(y)>Sup(x) || Inf(evl)>Sup(x)) {
    /* then, max(x,y) is necessarily y */
    y &= evl; return !y.empty();
  } else if (Inf(x)>Sup(y) || Inf(evl)>Sup(y)) {
    x &= evl; return !x.empty();
  }
  /*------------------------------*/

  if (Sup(evl)<Inf(x) || Sup(evl)<Inf(y)) {
    return false; // inconsistency
  }

  /* At this point, x, y and evl all mutually intersect. */
  if (Sup(x)>Sup(evl)) 
    x=INTERVAL(Inf(x),Sup(evl));
  if (Sup(y)>Sup(evl)) 
    y=INTERVAL(Inf(y),Sup(evl));
  
  return true;
}

bool proj_min(const INTERVAL& evl, INTERVAL& x, INTERVAL& y) {

  INTERVAL x2=-x;
  INTERVAL y2=-y;
  
  if (!proj_max(-evl,x2,y2)) return false;

  x=-x2;
  y=-y2;
  return true;
}

bool proj_sign(const INTERVAL& evl, INTERVAL& x) {

  if (evl.contains(0)) return true;

  if(Inf(evl)<0) return x &= INTERVAL(BiasNegInf,0);

  /* At this point, Sup(evl)>0. */ 
  return x &= INTERVAL(0,BiasPosInf);
}


bool proj_abs(const INTERVAL& evl, INTERVAL& x) {
  INTERVAL x1 = x & evl;
  INTERVAL x2 = x & (-evl);
  
  if (x1.empty() && x2.empty()) {
    return false;
  } else {
    x = Hull(x1,x2);
    return true;
  }
}

}
