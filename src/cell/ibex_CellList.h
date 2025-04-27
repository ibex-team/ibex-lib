//============================================================================
//                                  I B E X                                   
// File        : ibex_CellList.h
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct, 05 2017
//============================================================================

#ifndef __IBEX_CELL_LIST_H__
#define __IBEX_CELL_LIST_H__

#include "ibex_CellBuffer.h"
#include <list>

namespace ibex {

/** \ingroup strategy
 *
 * \brief Cell List.
 *
 * For breadth-first search. \see #CellBuffer
 */
class CellList : public CellBuffer {
 public:
  /** Flush the buffer.
   * All the remaining cells will be *deleted* */
  virtual void flush() override; 

  /** Return the size of the buffer. */
  virtual unsigned int size() const override;

  /** Return true if the buffer is empty. */
  virtual bool empty() const override;

  /** push a new cell on the back of the list. */
  virtual void push(Cell* cell) override;

  /** Pop a cell from the front of list and return it.*/
  virtual Cell* pop() override;

  /** Return the next box (but does not pop it).*/
  virtual Cell* top() const override;

 private:
  /* List of cells */
  std::list<Cell*> clist;
};

} // end namespace ibex
#endif // __IBEX_CELL_LIST_H__
