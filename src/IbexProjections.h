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


#include "Interval.h"

namespace ibex {

/** ====================================================================================================

  --------------------------------------- PROJECTIONS functions --------------------------------

  All the functions proj_f(const INTERVAL& y, INTERVAL& x) act the same way:

  They contract x -> x' such that all the values v in x satifying f(v)\in y belong to x'

  - They all contract x to the emptyset if y is empty
  - They can all contract x to the emptyset (INTERVAL::EMPTY) if no value in x has its image inside y.

  Similarly for binary operators:
  the functions proj_op(const INTERVAL& z, INTERVAL& x, INTERVAL& y) contract x-->x' and y-->y' such that
  all couple (v,w) in (x,y) satisfying (v op w)\in z belong to (x',y')
			     
==================================================================================================== */


/* project z=x*y over x and y */
bool proj_mult(const INTERVAL& z, INTERVAL& x, INTERVAL& y);

/* ftype is either COS, SIN or TAN (\see IbexToken.h) */
bool proj_trigo(const INTERVAL& y, INTERVAL& x, int ftype);

bool proj_power(const INTERVAL& y, INTERVAL& x, int expon);

bool proj_sqr(const INTERVAL& y, INTERVAL& x);

bool proj_cosh(const INTERVAL& y, INTERVAL& x);

bool proj_exp(const INTERVAL& y, INTERVAL& x);

bool proj_tanh(const INTERVAL& y, INTERVAL& x);

bool proj_arccosh(const INTERVAL& y, INTERVAL& x);

bool proj_max(const INTERVAL& z, INTERVAL& x, INTERVAL& y);

bool proj_min(const INTERVAL& z, INTERVAL& x, INTERVAL& y);

bool proj_sign(const INTERVAL& y, INTERVAL& x);

bool proj_abs(const INTERVAL& y, INTERVAL& x);

} // end namespace
