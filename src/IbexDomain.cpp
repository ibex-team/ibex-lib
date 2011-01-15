/*---------------------------------------------------------------------------------
 * 
 * Domains
 * -------
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

#include "IbexDomain.h"
#include "IbexException.h"

namespace ibex {

Dim index_dim(const Dim& base_dim) {

  Dim dim=base_dim;
  
  if (dim.dim3==0) {
    throw NonRecoverableException("Too many subscripts (e.g., a vector symbol cannot be indexed twice)");
  } 
  else if (dim.dim2==0) dim.dim3=0;
  else if (dim.dim1==0) dim.dim2=0;
  else dim.dim1=0;
  
  return dim;
}
  
int index_num(const Dim& base_dim, int base_num, int index) {
    
  int num=base_num;
    
  if (index<=0) throw NonRecoverableException("Null index (note: indexes start from 1)");
  
  if (base_dim.dim3==0) throw NonRecoverableException("Too many subscripts (e.g., a vector symbol cannot be indexed twice)");
  
  if (base_dim.dim2==0) {
    if (index>base_dim.dim3)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw NonRecoverableException(s.str());
    }
    num+=(index-1);
  }
  else if (base_dim.dim1==0) {
    if (index>base_dim.dim2)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw NonRecoverableException(s.str());
    }
    num+=(index-1) * base_dim.dim3;
  } else {
    if (index>base_dim.dim1)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw NonRecoverableException(s.str());
    }
    num+=(index-1) * base_dim.dim2 * base_dim.dim3;
  }
  return num;
}

ostream& operator<<(ostream& os, const struct Dim& d) {
  return os << d.dim1 << ", " << d.dim2 << ", " << d.dim3;
}


} // end namespace
