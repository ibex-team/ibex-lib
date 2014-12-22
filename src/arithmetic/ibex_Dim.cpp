/* ============================================================================
 * I B E X - Dimension of expressions (symbols, etc.)
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
#include "ibex_DimException.h"
#include <cassert>

using std::stringstream;

namespace ibex {

/** Build the three-dimensional structure. */
Dim::Dim(int dim1_, int dim2_, int dim3_) : dim1(dim1_), dim2(dim2_), dim3(dim3_), cst_vec(false) {

	assert(dim1_>0 && dim2_>0 && dim3_>0);
}

Dim add_dim(Dim& l, Dim& r) {
	if (l.dim1!=1 || r.dim1!=1)
		throw DimException("cannot add/subtract a matrix array");

	if (l==r) {
		if (!l.cst_vec || !r.cst_vec) { // either l or r "fixes" the dimension of the other
			l.cst_vec=false;
			r.cst_vec=false;
		}

		return l; // so the result has cst_vec==true if l and r are cst_vec.
	}

	if (l.is_scalar())
		throw DimException("cannot add a scalar to a vector/matrix");
	else if (l.is_vector()) {
		if (l.dim2==1 && r.cst_vec && l.dim3==r.dim2) {  // row vector + constant vector
			assert(r.is_vector());
			r = l;                     // transpose
			r.cst_vec=false;
			return l;
		} else if (r.dim2==1 && l.cst_vec && r.dim3==l.dim2) {
			assert(l.is_vector());
			l = r;                     // transpose
			r.cst_vec=false;
			return l;
		} else
			throw DimException("mismatched dimensions in vector addition/subtraction");
	} else {
		assert(l.is_matrix());
		throw DimException("mismatched dimensions in matrix addition/subtraction");
	}
}

Dim mul_dim(const Dim& l, const Dim& r) {
	if (l.dim1!=1 || r.dim1!=1)
		throw DimException("cannot multiply a matrix array");

	if (l.type()==Dim::SCALAR) // scalar multiplication.
		return r; // cst_vec is maintained.
	else {
		if (l.dim3!=r.dim2) {
			if (l.dim2==r.dim2) {
				if (r.dim3==1) // dot product
					return Dim::scalar();
				else // vector-matrix product
					return Dim::row_vec(r.dim3);
			}
			throw DimException("mismatched dimensions in matrix multiplication");
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

	if (n==0) throw DimException("a 0-sized vector has no dimension");

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
	throw DimException("impossible to form a vector with heterogeneous components");
}

Dim Dim::index_dim() const {

  const Dim& dim=*this;

  if (dim.dim2==1 && dim.dim3==1) {
	  return Dim::scalar();
	  // we allow x[1] for x scalar
	  // old error message -> "Too many subscripts (e.g., a vector symbol cannot be indexed twice)";
  }
  if (dim.dim1>1) // array of matrices
	  return Dim(1,dim.dim2,dim.dim3);
  else
	  if (dim.dim2==1 || dim.dim3==1) // vector
		  return Dim(1,1,1);
	  else // matrix
		  return Dim(1,1,dim.dim3); // return a row vector
}

int Dim::max_index() const {
	if (is_scalar()) return 0;
	else if (is_vector()) return vec_size()-1;
	else if (type()==MATRIX) return dim2-1;
	else return dim1-1;
}

Dim Dim::transpose_dim() const {
	switch (type()) {
	case SCALAR:       return *this;
	case ROW_VECTOR:   return col_vec(vec_size());
	case COL_VECTOR:   return row_vec(vec_size());
	case MATRIX:       return matrix(dim3,dim2);
	case MATRIX_ARRAY:
	default:           throw DimException("cannot transpose an array of matrices");
	                   return *this;
	}
}

/*
int Dim::index_num(int this_num, int index) const {

  int num=this_num;

  if (index<=0) throw DimException("Null index (note: indexes start from 1)");

  if (this->dim3==0) throw DimException("Too many subscripts (e.g., a vector symbol cannot be indexed twice)");

  if (this->dim2==0) {
    if (index>this->dim3)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw DimException(s.str());
    }
    num+=(index-1);
  }
  else if (this->dim1==0) {
    if (index>this->dim2)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw DimException(s.str());
    }
    num+=(index-1) * this->dim3;
  } else {
    if (index>this->dim1)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw DimException(s.str());
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
