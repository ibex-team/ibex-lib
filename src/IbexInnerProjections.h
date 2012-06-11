/*---------------------------------------------------------------------------------
 * 
 * Projections (Backward interval evaluation)
 * -----------------------------------------
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

#ifndef _IBEX_INNERPROJECTIONS_H
#define _IBEX_INNERPROJECTIONS_H

#include "Interval.h"

namespace ibex {

/** ====================================================================================================

  --------------------------------------- INNERPROJECTION functions --------------------------------

  All the functions innerproj_f(const INTERVAL& y, INTERVAL& x) act the same way:

  They contract x -> x' such that all the values v in x' satify f(v)\in y


  Similarly for binary operators:
  the function inner_projection[_nonmonotone](const INTERVAL& x, INTERVAL& y, INTERVAL& z, int op) contract x-->x' and y-->y' such that all couple (v,w) in (x',y') satisfies (v op w)\in z
			     
==================================================================================================== */


//--------------------------  InnerProjections   -------------------------//


  /** Contract x under y=x^2, returns one interval before performing the hull. **/
  bool innerproj_sqr(const INTERVAL& evl, INTERVAL& exp_evl);
  /** Contract x under y=log(x) **/
  bool innerproj_log(const INTERVAL& evl, INTERVAL& exp_evl);
  /** Contract x under y=x^expon **/
  bool innerproj_power(const INTERVAL& p_evl, INTERVAL& p_exp_evl, int expon);
  /** Contract x under y=exp(x) **/
  bool innerproj_exp(const INTERVAL& evl, INTERVAL& exp_evl);
  /** Contract x under y=sqrt(x) **/
  bool innerproj_sqrt(const INTERVAL& evl, INTERVAL& exp_evl);
  /** Finds a box [x]x[y], such that [op]([x],[y]) in [y]. op in {+,-,*,/} **/
  bool inner_projection(INTERVAL& x, INTERVAL& y, INTERVAL z, int op);

  bool inner_projection_mult(INTERVAL& x, INTERVAL& y, INTERVAL z);
  
  
  // This method returns a box [x]x[y] such that every point in it satisfies the
  // constraint op(x, y) >= z_inf
  // the operator MUST BE monotonic wrt x and y.
  // inc_var1 is true if the operator is increasing wrt x, is false if it is decreasing
  // inc_var2 is true if the operator is increasing wrt y, is false if it is decreasing
  bool geq_inner_projection(INTERVAL& x, INTERVAL& y, REAL z_inf, int op, bool inc_var1, bool inc_var2);


  //This method returns a box [x]x[y] such that every point in it satisfies the
  // constraint op(x, y) <= z_sup
  // the operator MUST BE monotonic wrt x and y.
  // inc_var1 is true if the operator is increasing wrt x, is false if it is decreasing
  // inc_var2 is true if the operator is increasing wrt y, is false if it is decreasing
  bool leq_inner_projection(INTERVAL& x, INTERVAL& y, REAL z_sup, int op, bool inc_var1, bool inc_var2);



//Considerations to use the expand* methods:
// - xin and yin are contained in x and y respectively.
// - [op]([xin],[yin]) is contained in [z]


/** returns a box containing [xin]x[yin] and contained in [x]x[y] such that
every point in the box satisfies the constraint: op(x,y) in [z]. op in (/,+,-) **/
  void expand2(const INTERVAL &xin, const INTERVAL& yin, INTERVAL& x, INTERVAL& y, INTERVAL z, int op);

/** returns a box containing [xin]x[yin] and contained in [x]x[y] such that
every point in the box satisfies the constraint: x*y in [z] **/
  void expand2_mult(const INTERVAL &xin, const INTERVAL& yin, INTERVAL& x, INTERVAL& y, INTERVAL z);

/** returns an interval containing [xin] and contained in [x] such that
every point in the box satisfies the constraint: x^2 in [z]. **/
  void expand_sqr(const INTERVAL& xin,  const INTERVAL& z, INTERVAL& x) ;

/** returns an interval containing [xin] and contained in [x] such that
every point in the box satisfies the constraint: x^expon in [z]. **/
  void expand_power(const INTERVAL &xin, const INTERVAL& z, INTERVAL& x, int expon);

/*
  INTERVAL eval(const INTERVAL& x, const INTERVAL& y, int op);
  REAL projx(REAL z, REAL y, int op, bool round_up);
  REAL projy(REAL z, REAL x, int op, bool round_up);*/


 


  
//-------------------------------------------------------------------------//

} // end namespace

#endif
