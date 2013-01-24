//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeapOptim.h
// Author      : Gilles Chabert, Bertrand Neveu
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#ifndef __IBEX_CELL_HEAP_OPTIM_H__
#define __IBEX_CELL_HEAP_OPTIM_H__

#include "ibex_CellHeap.h"

namespace ibex {

/** \ingroup strategy
 *
 * \brief Cell Heap for Optimization.
 *
 * The heap is organized so that the next box is always
 * the one for which the evaluation of the criterion to minimized (a function f)
 * gives an interval with the least lower bound. This criterion is stored
 * as a variable (it is not re-calculated here).
 *
 * \see #CellHeap, #CellBuffer
 */
class CellHeapOptim : public CellHeap {
public:
	/**
	 * \brief Build a cell heap for optimization.
	 *
	 * Build a cell heap that stores (n+1)-dimensional boxes of the following form: <br>
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
	 * \param y - the index of the variable "y" that contains the criterion (typically, f(x)) in each cell's box.
	 */
	CellHeapOptim(const int y);

	/** Index of the criterion variable. */
	const int y;

 protected:
   /** The cost of a cell. */
  double cost(const Cell& c) const;

};

} // end namespace ibex
#endif // __IBEX_CELL_HEAP_OPTIM_H__
