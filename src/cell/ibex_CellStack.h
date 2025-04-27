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
  virtual void flush() override;

  /** Return the size of the buffer. */
  virtual unsigned int size() const override;

  /** Return true if the buffer is empty. */
  virtual bool empty() const override;

  /** push a new cell on the stack. */
  virtual void push(Cell* cell) override;

  /** Pop a cell from the stack and return it.*/
  virtual Cell* pop() override;

  /** Return the next box (but does not pop it).*/
  virtual Cell* top() const override;

 private:
  /* Stack of cells */
  std::stack<Cell*> cstack;
};

} // end namespace ibex
#endif // __IBEX_CELL_STACK_H__
