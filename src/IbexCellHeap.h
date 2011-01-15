/*---------------------------------------------------------------------------------
 * 
 * Heap of Cells - To order boxes wrt to some criterion
 * ----------------------------------------------------
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

#ifndef _IBEX_CELL_HEAP_H
#define _IBEX_CELL_HEAP_H

#include "IbexCellBuffer.h"
#include <vector>

namespace ibex {

/** \ingroup solvergroup
 *
 * \brief Heap-organized buffer of cells
 * 
 * Allows to organize the cell buffer as a heap. 
 * In this way, the next cell is always the one that minimizes
 * some "cost" (size, evaluation of a function, etc.). 
 *
 * The criterion used to order the cells is the one defined by #cost(const Cell*). 
 *
 * \see #CellBuffer, #CellHeapBySize 
 *
 * \author Gilles Chabert
 * \date October 2010
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

  void contract_heap(REAL loup);  

  /** Return the minimum (the criterion for
   * the first cell) */
  REAL minimum() const { 
    return l.begin()->second;
  }

 protected:
  /** The "cost" of a cell. */
  virtual REAL cost(const Cell&) const=0;

  /** The way to compare two pairs (cells,crit). */
  bool operator()(const pair<Cell*,REAL>& c1, const pair<Cell*,REAL>& c2) const;

  // cells and associated "costs"
  vector<pair<Cell*,REAL> > l;

  friend ostream& operator<<(ostream&, const CellHeap&);


};

/** Display the buffer */
ostream& operator<<(ostream&, const CellHeap&);

} // end namespace

#endif
