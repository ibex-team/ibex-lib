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

#ifndef __IBEX_DIM_H__
#define __IBEX_DIM_H__

#include <iostream> // for ostream
#include <utility>  // for pair
#include <cassert>
#include "ibex_Array.h"

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Dimensions (of a mathematical value)
 *
 * All mathematical expressions are considered as 3-dimensional vectors.
 * The "value" of the ith dimension means the number of valid expressions obtained by indexing in this dimension.
 * The values of the three dimensions are represented by the fields dim1, dim2 and dim3. Let x be an expression.
 * If x is scalar, dim1=1, dim2=1, dim3=1.
 * If x is a row vector of 2 components, dim1=1, dim2=1, dim3=2.
 * If x is a 2x3 matrix, dim1=1, dim2=2, dim3=3.
 * If x is a column vector of 2 elements, then dim1=1, dim2=2 and dim3=1.
 *  If x is an array of 4 matrices which are 2x3matrices then dim1=4, dim2=2 and dim3=3.
 *
 * <p>
 * A combination like dim1=1 dim2=1 and dim3=2 is invalid (we cannot represent array of vectors. we use matrices
 * instead).
 *
 */
class Dim {
public:

	/** \brief Build the dimension of a scalar (1,1,1) */
	Dim();

	/** \brief Build the dimension of a scalar (1,1,1) */
	static Dim scalar();

	/** \brief Build the dimension of a row vector (1,1,n) */
	static Dim row_vec(int n);

	/** \brief Build the dimension of a col vector (1,n,1) */
	static Dim col_vec(int n);

	/** \brief Build the dimension of a matrix (1,m,n) */
	static Dim matrix(int m, int n);

	/** \brief Build the dimension of a matrix array (k,m,n) */
	static Dim matrix_array(int k, int m, int n);

	/** The 4 different types of "Dim" objects */
	typedef enum { SCALAR, ROW_VECTOR, COL_VECTOR, MATRIX, MATRIX_ARRAY } Type;

	/** \brief Return the type of this object */
	Type type() const;

	/** \brief Return the total number of components */
	inline int size()  const;

	/** \brief True if this is a scalar. */
	bool is_scalar() const;

	/** \brief True if this is either a row or column vector. */
	bool is_vector() const;

	/** \brief True if this is either a matrix. */
	bool is_matrix() const;

	/** \brief If *this is a vector, return the dimension.
	 *
	 * This dimension corresponds to dim2 (in case of column vector)
	 * or dim3 (in case of row vector)
	 */
	int vec_size() const;

	/** \brief True if the three dimensions match */
	bool operator==(const Dim& d) const;

	/** \brief True if one the dimensions does not match */
	bool operator!=(const Dim& d) const;

	/**
	 * \brief Transpose *this.
	 *
	 * Compute the dimension of an expression obtained
	 * by indexing an expression whose dimension is *this.
	 */
	Dim transpose_dim() const;

	/**
	 * \brief Add an index to *this
	 *
	 * Compute the dimension of the transpose expression obtained
	 * by indexing an expression whose dimension is *this.
	 */
	Dim index_dim() const;

	/**
	 * \brief Maximal index.
	 *
	 * Return the greatest i such that indexing by i is valid.
	 */
	int max_index() const;

	/* Compute the starting position of x[i] in a flat array
	 * that starts from \a this_num, and representing all elements
	 * x[i][j][k] (resp. x[i][j], x[i]) if *this is an array-of-matrix
	 * (resp. matrix, vector), in lexicographic order.
	 *
	 *  Perform also various checks (that the index is in the range, etc.). */
	//int index_num(int this_num, int index) const;

	//std::pair<int, struct Dim> index_info(int base_num, int index1, int index2, int index3) const;

	/** The number of i such that x[i][1][1] is a valid expression,
	 * where x is an array-of-matrix expression. */
	int dim1;

	/** The number of i such that x[1][i][1], x[i][1] or x[i] is a valid
	 * expression, where x is resp. an array-of-matrix, a matrix
	 * or a column vector expression. */
	int dim2;

	/** The number of i such that x[1][1][i], x[1][i] or x[i] is a
	 * valid (scalar) expression, where x is resp. an array-of-matrix,
	 * a matrix or a row vector expression. */
	int dim3;

	/** \brief Build the three-dimensional structure. */
	Dim(int dim1, int dim2, int dim3);

	/**
	 * \brief True if this is the dimension of a constant vector.
	 *
	 * The dimension of a constant vector (IntervalVector) is a column
	 * vector by default. This means that it may be automatically
	 * transposed to a row vector if necessary.
	 *
	 * This field is always set to false by default.
	 */
	bool cst_vec;

};


/** \ingroup arithmetic */
/*@{*/

/**
 * Return the dimension of a sum
 *
 * This is not as simple as "l" or "r" because
 * constant vectors (IntervalVector) are always
 * column vector by default. So they have to be
 * transformed into row vectors when added to row
 * vectors (so this function can modify "cst_vec" in
 * l or r).
 *
 * If l and r are both cst_vec (possible with expressions built from the parser)
 * then the result is also a cst_vec.
 */
Dim add_dim(Dim& l, Dim& r);

/**
 * Return the dimension of a product (left*right)
 */
Dim mul_dim(const Dim& l, const Dim& r);

/**
 * Return the dimension of a vector
 */
Dim vec_dim(const Array<const Dim>& comp, bool in_a_row);

/*@}*/

/*================================== inline implementations ========================================*/

inline Dim::Dim() : dim1(1), dim2(1), dim3(1), cst_vec(false) {

}

inline Dim Dim::scalar() {
	return Dim(1,1,1);
}

inline Dim Dim::row_vec(int n) {
	if (n==1) return Dim::scalar();
	else      return Dim(1,1,n);
}

inline Dim Dim::col_vec(int n) {
	if (n==1) return Dim::scalar();
	else      return Dim(1,n,1);
}

inline Dim Dim::matrix(int m, int n) {
	if (m==1)      return Dim::row_vec(n);
	else if (n==1) return Dim::col_vec(m);
	else           return Dim(1,m,n);
}

inline Dim Dim::matrix_array(int k, int m, int n) {
	if (k==1) return Dim::matrix(m,n);
	else      return Dim(k,m,n);
}

inline Dim::Type Dim::type() const {
	if (dim1==1)
		if (dim2==1)
			if (dim3==1) return SCALAR;
			else return ROW_VECTOR;
		else
			if (dim3==1) return COL_VECTOR;
			else return MATRIX;
	else
		return MATRIX_ARRAY;
}

inline int Dim::size()  const {
	return dim1*dim2*dim3;
}

inline bool Dim::is_scalar() const {
	return type()==SCALAR;
}

inline bool Dim::is_vector() const {
	Type t=type();
	return t==COL_VECTOR || t==ROW_VECTOR;
}

inline bool Dim::is_matrix() const {
	Type t=type();
	return t==MATRIX;
}

inline int Dim::vec_size() const {
	assert(is_vector());
	return dim2==1? dim3 : dim2;
}

inline bool Dim::operator==(const Dim& d) const {
	return dim1==d.dim1 && dim2==d.dim2 && dim3==d.dim3;
}

inline bool Dim::operator!=(const Dim& d) const {
	return !(*this==d);
}

/** \ingroup arithmetic
 * \brief Streams out a dimension */
std::ostream& operator<<(std::ostream&, const Dim&);

} // namespace ibex

#endif // __IBEX_DIM_H__
