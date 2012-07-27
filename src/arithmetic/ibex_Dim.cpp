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

	assert(dim1>0 && dim2>0 && dim3>0);
}

Dim mul_dim(const Dim& l, const Dim& r) {
	assert(l.dim1==1 && r.dim1==1);

	if (l.type()==Dim::SCALAR) // scalar multiplication.
		return r;
	else {
		if (l.dim3!=r.dim2) {
			throw NonRecoverableException("Mismatched dimensions in matrix multiplication");
		} else {
			if (l.dim2==1)
				if (r.dim3==1) return Dim::scalar();
				else return Dim::row_vec(r.dim3);
			else
				if (r.dim3==1) return Dim::col_vec(l.dim2);
				else return Dim::matrix(l.dim2,r.dim3);
		}
	}
}

Dim vec_dim(const Array<const Dim>& comp, bool in_a_row) {
	int n=comp.size();
	assert (n>0);
	const Dim& d=comp[0];

	if (d.is_scalar()) {
		if (in_a_row) {
			for (int i=0; i<n; i++)
				// we could allow concatenation of
				// row vectors of different size
				// in a single row vector;
				// (but not implemented yet)
				if (comp[i].type()!=Dim::SCALAR) goto error;
			return Dim::row_vec(n);
		}
		else {
			for (int i=0; i<n; i++)
				// we could allow concatenation of
				// column vectors of different size
				// in a single column vector;
				// (but not implemented yet)
				if (comp[i].type()!=Dim::SCALAR) goto error;
			return Dim::col_vec(n);
		}
	} else if (d.is_vector()) {
		if (in_a_row) {
			for (int i=0; i<n; i++)
				// same comment as above: we could also
				// put matrices with different number of columns
				// in a row. Not implemented. Only column vectors are accepted
				if (comp[i].type()!=Dim::COL_VECTOR || comp[i].dim2!=d.dim2) goto error;
			return Dim::matrix(d.dim2,n);
		} else {
			for (int i=0; i<n; i++) {
				// same comment as above: we could also
				// put matrices with different number of rows
				// in column. Not implemented. Only row vectors are accepted
				if (comp[i].type()!=Dim::ROW_VECTOR || comp[i].dim3!=d.dim3) goto error;
			}
			return Dim::matrix(n,d.dim3);
		}
	}

	// notice: array of matrix expressions are only used
	// so far in unvectorizing (for symbols corresponding to matrix arrays)
	else if (d.type()==Dim::MATRIX) {
		for (int i=0; i<n; i++)
			if (comp[i].type()!=Dim::MATRIX || comp[i].dim2!=d.dim2 || comp[i].dim3!=d.dim3) goto error;
		return Dim::matrix_array(n,d.dim2,d.dim3);
	}

	error:
	throw NonRecoverableException("Impossible to form a vector with these components");
}

Dim Dim::index_dim() const {

  const Dim& dim=*this;

  if (dim.dim2==1 && dim.dim3==1) {
    throw NonRecoverableException("Too many subscripts (e.g., a vector symbol cannot be indexed twice)");
  }
  if (dim.dim1>1) // array of matrices
	  return Dim(1,dim.dim2,dim.dim3);
  else
	  if (dim.dim2==1 || dim.dim3==1) // vector
		  return Dim(1,1,1);
	  else // matrix
		  return Dim(1,1,dim.dim3); // return a row vector
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
