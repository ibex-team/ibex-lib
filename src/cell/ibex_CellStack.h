//============================================================================
//                                  I B E X                                   
// File        : ibex_CellStack.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_CELL_STACK_H__
#define __IBEX_CELL_STACK_H__

#include "ibex_CellBuffer.h"
#include <stack>

namespace ibex {

/** \ingroup strategy
 *
 * \brief Cell Stack.
 *
 * For depth-first search. \see #CellBuffer
 */
class CellStack : public CellBuffer {
 public:
  /** Flush the buffer.
   * All the remaining cells will be *deleted* */
  void flush();

  /** Return the size of the buffer. */
  unsigned int size() const;

  /** Return true if the buffer is empty. */
  bool empty() const;

  /** push a new cell on the stack. */
  void push(Cell* cell);

  /** Pop a cell from the stack and return it.*/
  Cell* pop();

  /** Return the next box (but does not pop it).*/
  Cell* top() const;

 private:
  /* Stack of cells */
  std::stack<Cell*> cstack;
};

} // end namespace ibex
#endif // __IBEX_CELL_STACK_H__
