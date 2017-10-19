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
#include "ibex_Exception.h"

namespace ibex {

class DoubleIndex;

/**
 * \ingroup arithmetic
 *
 * \brief Dimensions (of a mathematical value/expression)
 *
 * The dimension of all mathematical expressions are 2-dimensional vectors.
 * The "value" of the ith dimension means the number of valid expressions obtained by indexing in this dimension.
 * The values of the two dimensions are represented by nb_rows() and nb_cols().
 *
 * Examples:
 *   Let x be an expression.
 *   If x is a scalar,                      x.nb_rows()=1, x.nb_cols()=1.
 *   If x is a row vector of 2 elements,    x.nb_rows()=1, x.nb_cols()=2.
 *   If x is a 2x3 matrix,                  x.nb_rows()=2, x.nb_cols()=3.
 *   If x is a column vector of 2 elements, x.nb_rows()=2, x.nb_cols()=1.
 *
 */
class Dim {
public:

	/** \brief Build the dimension of a scalar. */
	Dim();

	/** \brief Build the dimension of a scalar. */
	static Dim scalar();

	/** \brief Build the dimension of a row vector of size n. */
	static Dim row_vec(int n);

	/** \brief Build the dimension of a column vector of size n. */
	static Dim col_vec(int n);

	/** \brief Build the dimension of a matrix m*n. */
	static Dim matrix(int m, int n);

	/** The 4 different types of "Dim" objects. */
	typedef enum { SCALAR, ROW_VECTOR, COL_VECTOR, MATRIX } Type;

	/** \brief Return the type of this object. */
	Type type() const;

	/** \brief Return the total number of components. */
	inline int size() const;

	/** \brief True if *this is a scalar. */
	bool is_scalar() const;

	/** \brief True if *this is either a row or column vector. */
	bool is_vector() const;

	/** \brief True if *this is a matrix. */
	bool is_matrix() const;

	/**
	 * \brief If *this is a vector, return the dimension.
	 *
	 * This dimension corresponds to dim2 (in case of column vector)
	 * or dim3 (in case of row vector).
	 */
	int vec_size() const;

	/**
	 * \brief The number of rows
	 */
	int nb_rows() const;

	/**
	 * \brief The number of columns
	 */
	int nb_cols() const;

	/** \brief True if the three dimensions match with d. */
	bool operator==(const Dim& d) const;

	/** \brief True if one of the dimensions does not match with d. */
	bool operator!=(const Dim& d) const;

	/**
	 * \brief Transpose *this.
	 *
	 * Compute the dimension of an expression obtained
	 * by transposing an expression whose dimension is *this.
	 */
	Dim transpose_dim() const;


//	Dim index_dim() const;

	/*
	 * \brief Add an index to *this
	 *
	 * Compute the dimension of the expression obtained
	 * by indexing an expression whose dimension is *this.
	 */
	Dim index_dim(const DoubleIndex& idx) const;

	/*
	 * \brief Maximal index.
	 *
	 * Return the greatest i such that indexing by i is valid.
	 */
//	int max_index() const;

	/*
	 * Compute the starting position of x[i] in a flat array
	 * that starts from \a this_num, and representing all elements
	 * x[i][j][k] (resp. x[i][j], x[i]) if *this is an array-of-matrix
	 * (resp. matrix, vector), in lexicographic order.
	 *
	 *  Perform also various checks (that the index is in the range, etc.). */
	//int index_num(int this_num, int index) const;

	//std::pair<int, struct Dim> index_info(int base_num, int index1, int index2, int index3) const;


	/** \brief Build the three-dimensional structure. */
	Dim(int dim2, int dim3);

private:
	/**
	 * The number of i such that x[i][0] or x[i] is a valid
	 * expression, where x is resp. a matrix
	 * or a column vector expression.
	 */
	int dim2;

	/**
	 * The number of i such that x[i][0] or x[i] is a
	 * valid (scalar) expression, where x is resp. a matrix
	 * or a row vector expression.
	 */
	int dim3;

};

/**
 * \ingroup arithmetic
 *
 * \brief Thrown when dimensions in matrix/vector operations are not correct
 *
 */
class DimException : public Exception {
public:

	DimException(std::string message1) : msg(message1) { }

	/**
	 * \brief Get the message of this exception
	 */
	const std::string& message() const { return msg; }

private:
	std::string msg;
};


std::ostream& operator<< (std::ostream& os, const DimException& e);

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief Return the dimension of a sum.
 */
Dim add_dim(const Dim& l, const Dim& r);

/**
 * \brief Return the dimension of a product (left*right).
 */
Dim mul_dim(const Dim& l, const Dim& r);

/**
 * \brief Return the dimension of a vector.
 */
Dim vec_dim(const Array<const Dim>& comp, bool in_a_row);

/**
 * \brief Streams out a dimension.
 */
std::ostream& operator<<(std::ostream&, const Dim&);

/*@}*/

/*================================== inline implementations ========================================*/

inline Dim::Dim() : dim2(1), dim3(1) {

}

inline Dim Dim::scalar() {
	return Dim(1,1);
}

inline Dim Dim::row_vec(int n) {
	if (n==1) return Dim::scalar();
	else      return Dim(1,n);
}

inline Dim Dim::col_vec(int n) {
	if (n==1) return Dim::scalar();
	else      return Dim(n,1);
}

inline Dim Dim::matrix(int m, int n) {
	if (m==1)      return Dim::row_vec(n);
	else if (n==1) return Dim::col_vec(m);
	else           return Dim(m,n);
}

inline Dim::Type Dim::type() const {
	if (dim2==1)
		if (dim3==1) return SCALAR;
		else return ROW_VECTOR;
	else
		if (dim3==1) return COL_VECTOR;
		else return MATRIX;
}

inline int Dim::size()  const {
	return dim2*dim3;
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
	return dim2==1? dim3 : dim2;
}

inline int Dim::nb_rows() const {
	return dim2;
}

inline int Dim::nb_cols() const {
	return dim3;
}

inline bool Dim::operator==(const Dim& d) const {
	return dim2==d.dim2 && dim3==d.dim3;
}

inline bool Dim::operator!=(const Dim& d) const {
	return !(*this==d);
}


} // namespace ibex

#endif // __IBEX_DIM_H__
