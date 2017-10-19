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
#include <cassert>

#include "ibex_Dim.h"
#include "ibex_DoubleIndex.h"

using std::stringstream;

namespace ibex {

/** Build the three-dimensional structure. */
Dim::Dim(int dim2_, int dim3_) : dim2(dim2_), dim3(dim3_) {
	assert(dim2_>0 && dim3_>0);
}

Dim add_dim(const Dim& l, const Dim& r) {
	if (l==r)
		return l;
	else if (l.is_scalar() || r.is_scalar())
		throw DimException("cannot add a scalar to a vector/matrix");
	else if (l.is_vector() && r.is_vector())
		throw DimException("mismatched dimensions in vector addition/subtraction");
	else if (l.is_vector() || r.is_vector())
		throw DimException("cannot add a vector to a matrix");
	else
		throw DimException("mismatched dimensions in matrix addition/subtraction");
}

Dim mul_dim(const Dim& l, const Dim& r) {
	if (l.type()==Dim::SCALAR) // scalar multiplication.
		return r;
	else {
		if (l.nb_cols()!=r.nb_rows()) {
			throw DimException("mismatched dimensions in matrix multiplication");
		} else {
			if (l.nb_rows()==1)
				if (r.nb_cols()==1) return Dim::scalar();
				else return Dim::row_vec(r.nb_cols()); // vector-matrix product
			else
				if (r.nb_cols()==1) return Dim::col_vec(l.nb_rows());
				else return Dim::matrix(l.nb_rows(),r.nb_cols());
		}
	}
}

Dim vec_dim(const Array<const Dim>& comp, bool in_a_row) {
	int n=comp.size();

	if (n==0) throw DimException("a 0-sized vector has no dimension");

	const Dim& d=comp[0];

	if (in_a_row) {
		int row_dim = d.nb_rows(); // the number of rows is fixed
		int total_cols=0;
		for (int i=0; i<n; i++) {
			// we can concatenate row vectors of different size
			// in a single row vector or matrices/column vectors
			// with the right number of rows
			if (comp[i].nb_rows()!=row_dim) goto error;
			total_cols+=comp[i].nb_cols();
		}
		return Dim::matrix(row_dim,total_cols);
	} else {
		int col_dim = d.nb_cols(); // the number of columns is fixed
		int total_rows=0;
		for (int i=0; i<n; i++) {
			// we can concatenate column vectors of different size
			// in a single column vector or matrices/row vectors
			// with the right number of columns
			if (comp[i].nb_cols()!=col_dim) goto error;
			total_rows+=comp[i].nb_rows();
		}
		return Dim::matrix(total_rows,col_dim);
	}

	error:
	throw DimException("impossible to form a vector with heterogeneous components");
}

//Dim Dim::index_dim() const {
//
//  if (nb_rows()==1 || nb_cols()==1) // vector or scalar
//	  return scalar();
//  else // matrix
//	  return row_vec(nb_cols()); // return a row vector
//}

Dim Dim::index_dim(const DoubleIndex& idx) const {

	if (idx.all_rows()) {
		if (idx.all_cols())
			return *this;
		else if (idx.one_col()) {
			switch(type()) {
			case MATRIX: return col_vec(nb_rows());
			case COL_VECTOR: return *this;
			case ROW_VECTOR:
			case SCALAR: return scalar();
			default: ibex_error("index_dim: the object is not a scalar, nor an vector nor a matrix");
			}
		} else {
			if (is_matrix()) return matrix(nb_rows(),idx.nb_cols());
			else             return row_vec(idx.nb_cols());
		}
	} else if (idx.one_row()) {
		if (idx.all_cols()) {
			switch(type()) {
			case MATRIX: return row_vec(nb_cols());
			case ROW_VECTOR: return *this;
			case COL_VECTOR:
			case SCALAR: return scalar();
			default: ibex_error("index_dim: the object is not a scalar, nor an vector nor a matrix");
			}
		} else if (idx.one_col())
			return scalar();
		else
			return row_vec(idx.nb_cols());
	} else {
		if (idx.all_cols()) {
			if (is_matrix()) return matrix(idx.nb_rows(),nb_cols());
			else             return col_vec(idx.nb_rows());
		}
		if (idx.one_col()) {
			return col_vec(idx.nb_rows());
		} else {
			return matrix(idx.nb_rows(),idx.nb_cols());
		}
	}
}

//int Dim::max_index() const {
//	if (is_scalar()) return 0;
//	else if (is_vector()) return vec_size()-1;
//	else return nb_rows()-1;
//}

Dim Dim::transpose_dim() const {
	switch (type()) {
	case SCALAR:       return scalar();
	case ROW_VECTOR:   return col_vec(vec_size());
	case COL_VECTOR:   return row_vec(vec_size());
	case MATRIX:       return matrix(nb_cols(),nb_rows());
	default: ibex_error("transpose_dim: the object is not a scalar, nor an vector nor a matrix");
	}
}

/*
int Dim::index_num(int this_num, int index) const {

  int num=this_num;

  if (index<=0) throw DimException("Null index (note: indexes start from 1)");

  if (this->nb_cols()==0) throw DimException("Too many subscripts (e.g., a vector symbol cannot be indexed twice)");

  if (this->nb_rows()==0) {
    if (index>this->nb_cols())  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw DimException(s.str());
    }
    num+=(index-1);
  }
  else if (this->dim1==0) {
    if (index>this->nb_rows())  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw DimException(s.str());
    }
    num+=(index-1) * this->nb_cols();
  } else {
    if (index>this->dim1)  {
      stringstream s;
      s << "Index (" << index << ") exceeds bounds";
      throw DimException(s.str());
    }
    num+=(index-1) * this->nb_rows() * this->nb_cols();
  }
  return num;
}
*/

std::ostream& operator<<(std::ostream& os, const Dim& d) {
  return os << d.nb_rows() << "x" << d.nb_cols();
}

std::ostream& operator<< (std::ostream& os, const DimException& e) {
  os << "Dimension mismatch: " << e.message();
  return os;
}
} // namespace ibex
