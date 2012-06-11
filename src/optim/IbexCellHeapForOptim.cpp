/*---------------------------------------------------------------------------------
 * 
 * Heap of Cells - For Optimization
 * ---------------------------------------
 *
 * Copyright (C) 2007-2010 Gilles Chabert
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

#include "IbexCellHeapForOptim.h"
#include <algorithm>

namespace ibex {

CellHeapForOptim::CellHeapForOptim(const int y) : y_num(y) { }

/* Old version that used an evaluator "evl" to calculate (on-the-fly) 
 * the evaluation of the criterion (there were no extra variable) */
/*
REAL CellHeapForOptim::cost(const Cell& c) const {
  // get the space
  Space& space = c.space;

  INTERVAL_VECTOR savebox(space.box);

  (INTERVAL_VECTOR&) space.box = c.get_box();
    
  // evaluate the goal function
  evl.forward(space);
  REAL res = Inf(evl.output());
    
  // restore the box
  (INTERVAL_VECTOR&) space.box = savebox;

  return res;
}
*/


/* New version: the cost is already computed, it is the min of the 
 * last variable corresponding to the objective */
REAL CellHeapForOptim::cost(const Cell& c) const {
  return Inf(c.space.box(y_num+1));
}
  
} // end namespace


