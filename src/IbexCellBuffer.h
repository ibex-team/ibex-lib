/*---------------------------------------------------------------------------------
 * 
 * Cell Buffer
 * ---------------------------
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

#ifndef _IBEX_CELL_BUFFER_H
#define _IBEX_CELL_BUFFER_H

/** \defgroup solvergroup Paver  */
/*@{*/

#include "IbexCell.h"

namespace ibex {

/** \ingroup solvergroup
 *
 * \brief Cell Buffer
 * 
 * This class allows to control the order in which cells are treated by
 * the paver, i.e., to control the way domains are explored.
 * For instance, if the container is implemented as a stack, the paver 
 * will perform a depth-first search. If it is a file, the paver will
 * perform a breadth-first search, and so on.
 *
 * \author Gilles Chabert
 * \date October 2010
 */
class CellBuffer {

 public:
  /** Flush the buffer. 
   * All the remaining cells will be *deleted* */
  virtual void flush()=0;

  /** Return the size of the buffer. */
  virtual int size() const=0;
  
  /** Return true if the buffer is empty. */
  virtual bool empty() const=0;

  /** Push a new cell on the stack. */
  virtual void push(Cell* cell)=0;

  /** Pop a cell from the stack and return it.*/
  virtual Cell* pop()=0;

  /** Return the next box (but does not pop it).*/
  virtual Cell* top() const=0;

};

} // end namespace

#endif
