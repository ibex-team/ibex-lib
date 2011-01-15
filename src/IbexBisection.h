/*---------------------------------------------------------------------------------
 * 
 * Bisection structure
 * -------------------
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

#ifndef _IBEX_BISECTION_H
#define _IBEX_BISECTION_H

#include "IbexException.h"
#include "IntervalVector.h"

namespace ibex {

/** \ingroup solvergroup
 * \brief Bisection structure
 *
 * An instance of this class contains the result of a bisection (i.e., a call to a bisector).
 * \see #ibex::Bisector.
 * \author Gilles Chabert
 * \date March 2007
 */
class Bisection {
 public:
  
  /** Create a bisection of a box.
   *  \param box - the bisected box.
   *  \param var - the component of \a box which domain is split.
   *  \param ratio - the ratio between the diameters of the left and the right parts of the bisected
   *  inteval. Default value is 0.5, i.e. the inteval is bisected in the midpoint. 
   *
   * If one of the bound of the domain to be split is infinite (say [a,+oo]) the result is
   * [a,MAX_FLOAT] and [MAX_FLOAT,+oo] (whatever is the value of \a ratio).
   *
   * If the domain is [-oo,oo] the result is [-oo,0] and [0,+oo].
   **/
  Bisection(const INTERVAL_VECTOR& box, int var, REAL ratio=0.5);
  
  /** The left part of a bisection. */
  INTERVAL_VECTOR box1;

  /** The right part of a bisection. */
  INTERVAL_VECTOR box2;

  /** The bisected variable. */
  int var;

  /** The ratio applied. */
  REAL ratio;
};

} // end namespace

#endif
