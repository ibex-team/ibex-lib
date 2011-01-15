/*---------------------------------------------------------------------------------
 * 
 * Propagation queue
 * -----------------
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

#include "IbexQueue.h"

namespace ibex {

std::ostream& operator<<(std::ostream& os, const CoupleQueue& q) {
  if (q.empty()) return os << "(empty)";

  int M = q.nb_var;
  int i = q.first/M;
  int j = q.first%M;
  int tmp;
  
  do {
    os << "(" << i << ", " << j << ") ";
    tmp = q.table[i*M+j];      
    i = tmp/M;
    j = tmp%M;      
  } while (tmp!=q.first);
  
  return os << std::endl;
}

} // end namespace
