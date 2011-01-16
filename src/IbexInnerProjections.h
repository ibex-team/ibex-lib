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

  --------------------------------------- INNERPROJECTION functions --------------------------------

  All the functions innerproj_f(const INTERVAL& y, INTERVAL& x) act the same way:

  They contract x -> x' such that all the values v in x' satify f(v)\in y


  Similarly for binary operators:
  the function inner_projection[_nonmonotone](const INTERVAL& x, INTERVAL& y, INTERVAL& z, int op) contract x-->x' and y-->y' such that all couple (v,w) in (x',y') satisfies (v op w)\in z
			     
==================================================================================================== */


//--------------------------  InnerProjections   -------------------------//

  /** Finds a box [x]x[y], such that [op]([x],[y]) in [y]. op MUST BE monotone wrt x and y **/
  bool inner_projection(INTERVAL& x, INTERVAL& y, INTERVAL z, int op); 

  /** Finds a box [x]x[y], such that [op]([x],[y]) in [y]. op is a not monotonice operator (*,/) **/
  bool inner_projection_nonmonotone(INTERVAL& x, INTERVAL& y, INTERVAL z, int op);

  /** Contract x under y=x^2, returns one interval before performing the hull. (It may not be conservative)  **/
  bool innerproj_sqr(const INTERVAL& y, INTERVAL& x);
//-------------------------------------------------------------------------//

} // end namespace
