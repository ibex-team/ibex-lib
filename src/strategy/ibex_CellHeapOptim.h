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
 * in a cell as a #ibex::OptimCrit object (it is not re-calculated here).
 *
 * \see #CellHeap, #CellBuffer
 */
class CellHeapOptim : public CellHeap {

 protected:
  /** The cost of a cell. */
  double cost(const Cell& c) const;

};

} // end namespace ibex
#endif // __IBEX_CELL_HEAP_OPTIM_H__
