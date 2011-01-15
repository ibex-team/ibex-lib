/*---------------------------------------------------------------------------------
 * 
 * Stack of Cells - For Depth-first search
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

#ifndef _IBEX_CELL_STACK_H
#define _IBEX_CELL_STACK_H

#include "IbexCellBuffer.h"
#include <stack>

namespace ibex {

/** \ingroup solvergroup
 *
 * \brief Cell Stack.
 * 
 * For depth-first search. \see #CellBuffer
 *
 * \author Gilles Chabert
 * \date October 2010
 */
class CellStack : public CellBuffer {
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

 private:
  /* Stack of cells */
  stack<Cell*> cstack;
};

} // end namespace

#endif
