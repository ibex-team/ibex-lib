/*---------------------------------------------------------------------------------
 * 
 * Heap of Cells - For Optimization
 * ---------------------------------------
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

#ifndef _IBEX_CELL_HEAP_FOR_OPTIM_H
#define _IBEX_CELL_HEAP_FOR_OPTIM_H

#include "IbexCellHeap.h"
#include "IbexEvaluator.h"

namespace ibex {

/** \ingroup solvergroup
 *
 * \brief [Optimization] Cell Heap for Optimization.
 * 
 * The heap is organized so that the next box is always
 * the one for which the evaluation of a criterion (a function f)
 * gives an interval with the least lower bound.
 *
 * \see #CellHeap, #CellBuffer
 *
 * \author Gilles Chabert, Bertrand Neveu
 * \date October 2010. Last update: December 2010
 */
class CellHeapForOptim : public CellHeap {

 public:

  /** Build a cell heap that stores (n+1)-dimensional boxes of the following form: <br>
   * ([x]_1,...[x]_n,[y]) <br> 
   * where "y" is a specific variable that is assumed to be the criterion to be minimized.
   * Typically, [y] is the image of a function f calculated on the box [x]=([x]_1,...[x]_n). <br>
   *
   * The heap is built so that:
   *  <ul>
   *  <li> #pop() returns in logarithmic time
   *    the box ([x],[y]) with the minimal lower bound for [y]. 
   *    among all the boxes in the list</li>
   *  <li> #push() is also in logarithmic time.</li>
   *  </ul>
   *
   * \param y - the index of the epr "y" that contains the criterion (typically, f(x)) in each cell's box. */
  CellHeapForOptim(int y);

 protected:
  /** The cost of a cell. */
  REAL cost(const Cell& c) const;

 private:
  const int y;
};

} // end namespace

#endif
