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

#ifndef _IBEX_DIM_H_
#define _IBEX_DIM_H_

#include <iostream> // for ostream
#include <utility>  // for pair
#include "ibex_NonRecoverableException.h"

namespace ibex {

/**
 * \ingroup level1
 * \brief Dimensions (of an mathematical expression)
 *
 * All expressions are considered here as 3-dimensional vectors, except that the "value" of a dimension can be 0.
 * The "value" of the ith dimension means the number of valid expressions obtained by indexing in this dimension.
 * The values of the three dimensions are represented by the fields dim1, dim2 and dim3. Let x be an expression.
 * If x is scalar, dim1=0, dim2=0, dim3=0. Indeed, x[1] is not a valid symbol. If x is a vector of 2 components,
 * dim1=0, dim2=0, dim3=2 (because x[1] and x[2] are valid symbols but not x[1][1]). If x is a 2x3 matrix, dim1=0,
 * dim2=2, dim3=3. Special case: if x is a *row vector* of 2 elements, then dim1=0, dim2=2 and dim3=0.
 * Last case: if x is an array of 4 matrices which are 2x3matrices then dim1=4, dim2=2 and dim3=3.
 *
 * <p>
 * A combination like dim1=1 dim2=0 and dim3=2 is invalid (we cannot represent array of vectors. Use matrices
 * instead).
 *
 * The "size" of a dimension represents the number of elements in the dimension (this time seen as an array).
 * The size is simply equal to the "value" if it is greater than 0, and 1 otherwise.
 *
 * Hence, a vector with a single component is distinguished from a scalar: the field dim3 is 1 and 0 respectively.
 * But, in both cases, size3() returns 1.
 */
class Dim {
public:

	/** The 4 different types of "Dim" objects */
	typedef enum { SCALAR, COL_VECTOR, ROW_VECTOR, MATRIX, MATRIX_ARRAY } Type;

	/** \brief Build the three-dimensional structure. */
	Dim(int dim1, int dim2, int dim3);

	/** \brief Return the type of this object */
	Type type() const;

	/** \brief Return the size of the first dimension. */
	inline int size1() const;

	/** \brief Return the size of the second dimension. */
	inline int size2() const;

	/** \brief Return the size of the third dimension. */
	inline int size3() const;

	/** \brief Return the total number of components */
	inline int size()  const;

	/** \brief True if this is a scalar. */
	bool is_scalar() const;

	/** \brief True if this is either a row or column vector. */
	bool is_vector() const;

	/** \brief True if the three dimensions match */
	bool operator==(const Dim& d) const;

	/** \brief Compute the dimension of an expression obtained
	 * by indexing an expression whose dimension is *this. */
	Dim index_dim() const;

	/** Compute the starting position of x[i] in a flat array
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

	/** The number of i such that x[1][i][1] or x[i][1] is a valid
	 * expression, where x is resp. an array-of-matrix or a matrix
	 * expression. */
	int dim2;

	/** The number of i such that x[1][1][i] or x[1][i] or x[i] is a
	 * valid (scalar) expression, where x is resp. an array-of-matrix,
	 * a matrix or a vector expression. */
	int dim3;
};

/*================================== inline implementations ========================================*/

inline Dim::Type Dim::type() const {
	if (dim1==0)
		if (dim2==0)
			if (dim3==0) return SCALAR;
			else return COL_VECTOR;
		else
			if (dim3==0) return ROW_VECTOR;
			else return MATRIX;
	else
		return MATRIX_ARRAY;
}

inline int Dim::size1() const {
	return dim1==0?1:dim1;
}

inline int Dim::size2() const {
	return dim2==0?1:dim2;
}

inline int Dim::size3() const {
	return dim3==0?1:dim3;
}

inline int Dim::size()  const {
	return size1()*size2()*size3();
}

inline bool Dim::is_scalar() const {
	return type()==SCALAR;
}

inline bool Dim::is_vector() const {
	Type t=type();
	return t==COL_VECTOR || t==ROW_VECTOR;
}

inline bool Dim::operator==(const Dim& d) const {
	return dim1==d.dim1 && dim2==d.dim2 && dim3==d.dim3;
}

/**
 * \ingroup level1
 * \brief Indices for an expression.
 *
 * In the expression x[1][0][2], the triple of indices (1,0,2)
 * is represented by an object of this class.
 */
class Indices {
public:
	Indices() : index1(-1), start_index2(-1), end_index2(-1), start_index3(-1), end_index3(-1) { }

	void add_index(int index) {
		if (start_index3==-1) start_index3=end_index3=index;
		else if (start_index2==-1) start_index2=end_index2=index;
		else if (index1==-1) index1=index;
		else throw NonRecoverableException("Cannot use 4 levels of indices.");
	}

	void add_index_range(int start_index, int end_index) {
		if (start_index3==-1) {start_index3=start_index; end_index3=end_index; }
		else if (start_index2==-1) {start_index2=start_index; end_index2=end_index; }
		else if (index1==-1) throw NonRecoverableException("Cannot use range of 3rd-level indices.");
		else throw NonRecoverableException("Cannot use 4 levels of indices.");
	}

	int index1;
	int start_index2;
	int end_index2;
	int start_index3;
	int end_index3;
};

/** Streams out a dimension  */
std::ostream& operator<<(std::ostream&, const Dim&);

} // namespace ibex

#endif // _IBEX_DIM_H_
