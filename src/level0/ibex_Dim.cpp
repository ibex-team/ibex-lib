/* ============================================================================
 * I B E X - Dimension of expressions (including symbols)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Dec 13, 2011
 * ---------------------------------------------------------------------------- */

#include <sstream>
#include "ibex_Dim.h"
#include "ibex_NonRecoverableException.h"
#include <cassert>

using std::stringstream;

namespace ibex {

/** Build the three-dimensional structure. */
Dim::Dim(int dim1, int dim2, int dim3) : dim1(dim1), dim2(dim2), dim3(dim3) {

	assert(!(dim1>0 && (dim2==0 || dim3==0)));
}

Dim Dim::index_dim() const {

  const Dim& dim=*this;

  if (dim.dim2==0 && dim.dim3==0) {
    throw NonRecoverableException("Too many subscripts (e.g., a vector symbol cannot be indexed twice)");
  }
  if (dim.dim1>0) // array of matrices
	  return Dim(0,dim.dim2,dim.dim3);
  else
	  if (dim.dim2==0) // column vector
		  return Dim(0,0,0);
	  else
		  if (dim.dim3==0)// row vector
			  return Dim(0,0,0);
		  else	// matrix
			  return Dim(0,dim.dim3,0); // return a row vector
}

/*
int Dim::index_num(int this_num, int index) const {

  int num=this_num;

  if (index<=0) throw NonRecoverableException("Null index (note: indexes start from 1)");

  if (this->dim3==0) throw NonRecoverableException("Too many subscripts (e.g., a vector symbol cannot be indexed twice)");

  if (this->dim2==0) {
    if (index>this->dim3)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw NonRecoverableException(s.str());
    }
    num+=(index-1);
  }
  else if (this->dim1==0) {
    if (index>this->dim2)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw NonRecoverableException(s.str());
    }
    num+=(index-1) * this->dim3;
  } else {
    if (index>this->dim1)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw NonRecoverableException(s.str());
    }
    num+=(index-1) * this->dim2 * this->dim3;
  }
  return num;
}
*/

std::ostream& operator<<(std::ostream& os, const Dim& d) {
  return os << d.dim1 << ", " << d.dim2 << ", " << d.dim3;
}

} // namespace ibex
