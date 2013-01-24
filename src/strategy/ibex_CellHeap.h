//============================================================================
//                                  I B E X                                   
// File        : ibex_CellHeap.h
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 12, 2012
// Last Update : May 12, 2012
//============================================================================

#ifndef __IBEX_CELL_HEAP_H__
#define __IBEX_CELL_HEAP_H__

#include "ibex_CellBuffer.h"
#include <utility>
#include <vector>

namespace ibex {

/** \ingroup strategy
 *
 * \brief Heap-organized buffer of cells
 *
 * Allows to organize the cell buffer as a heap.
 * In this way, the next cell is always the one that minimizes
 * some "cost" (size, evaluation of a function, etc.).
 *
 * The criterion used to order the cells is the one defined by #cost(const Cell*).
 *
 * The heap is built so that:
 *  <ul>
 *  <li> #pop() returns in logarithmic time
 *    the cell with the minimal criterion.
 *  <li> #push() is also in logarithmic time.</li>
 *  </ul>
 *
 * \see #CellBuffer, #CellHeapBySize
 */
class CellHeap : public CellBuffer {

 public:
  /** Flush the buffer.
   * All the remaining cells will be *deleted* */
  void flush();

  /** Return the size of the buffer. */
  int size() const;

  /** Return true if the buffer is empty. */
  bool empty() const;

  /** push a new cell on the stack. */
  void push(Cell* cell);

  /** Pop a cell from the stack and return it.*/
  Cell* pop();

  /** Return the next box (but does not pop it).*/
  Cell* top() const;

  /**
   * Removes (and deletes) from the heap all the cells
   * with a cost greater than \a loup.
   */
  void contract_heap(double loup);

  /** Return the minimum (the criterion for
   * the first cell) */
  double minimum() const {
    return l.begin()->second;
  }

 protected:
  /** The "cost" of a cell. */
  virtual double cost(const Cell&) const=0;

  /** The way to compare two pairs (cells,crit). */
  bool operator()(const std::pair<Cell*,double>& c1, const std::pair<Cell*,double>& c2) const;

  // cells and associated "costs"
  std::vector<std::pair<Cell*,double> > l;

  friend std::ostream& operator<<(std::ostream&, const CellHeap&);


};

/** Display the buffer */
std::ostream& operator<<(std::ostream&, const CellHeap&);

} // end namespace ibex
#endif // __IBEX_CELL_HEAP_H__
