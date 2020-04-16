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
  void flush();

  /** Return the size of the buffer. */
  unsigned int size() const;

  /** Return true if the buffer is empty. */
  bool empty() const;

  /** push a new cell on the back of the list. */
  void push(Cell* cell);

  /** Pop a cell from the front of list and return it.*/
  Cell* pop();

  /** Return the next box (but does not pop it).*/
  Cell* top() const;

 private:
  /* List of cells */
  std::list<Cell*> clist;
};

} // end namespace ibex
#endif // __IBEX_CELL_LIST_H__
