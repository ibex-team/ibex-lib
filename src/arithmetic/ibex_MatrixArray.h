/* ============================================================================
 * I B E X - Array of matrices (of reals)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 14, 2014
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_MATRIX_ARRAY_H__
#define __IBEX_MATRIX_ARRAY_H__

#include "ibex_Matrix.h"
#include <iostream>

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Array of real matrices.
 */
class MatrixArray {

public:
	/**
	 * \brief Create a (size x nb_rows x nb_cols) MatrixArray.
	 */
	MatrixArray(int size, int nb_rows, int nb_cols);

	/**
	 * \brief Create a (size x nb_rows x nb_cols) MatrixArray and initialize
	 * all the elements with x.
	 */
	MatrixArray(int size, int nb_rows, int nb_cols, double x);

	/**
	 * \brief Duplicate a MatrixArray.
	 */
	MatrixArray(const MatrixArray& m);

	/*
	 * \brief Create a MatrixArray from an array of doubles.
	 *
	 * TO
	 *
	 * Create the MatrixArray: <br>
	 *  x[0] ; ... ; x[n-1]   <br>
	 *  x[n] ; ... ; x[2n-1]  <br>
	 *  ... <br>
	 *  x[(m-1)n] ; ... ; x[2n-1]  <br>
	 *
	 * \param x an (mxn) array of doubles
	 * \pre m>0, n>0
	 */
	//MatrixArray(int sze, int nb_rows, int nb_cols, double x[]);

	/**
	 * \brief Delete *this.
	 */
	~MatrixArray();

	/**
	 * \brief Set *this to m.
	 */
	MatrixArray& operator=(const MatrixArray& x);

	/**
	 * \brief True if the entries of (*this) coincide with m.
	 */
	bool operator==(const MatrixArray& m) const;

	/**
	 * \brief True if one entry of (*this) differs from m.
	 */
	bool operator!=(const MatrixArray& m);

	/**
	 * \brief Size of the array.
	 */
	int size() const;

	/**
	 * \brief Number of rows of each matrix
	 */
	int nb_rows() const;

	/**
	 * \brief Number of columns of each matrix
	 */
	int nb_cols() const;

	/**
	 * \brief Return the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	Matrix& operator[](int i);

	/**
	 * \brief Return a const reference to the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	const Matrix& operator[](int i) const;

private:
	Matrix* array;
	int n;
};


/** \ingroup arithmetic */
/*@{*/


/**
 * \brief Output an array of real matrices.
 */
std::ostream& operator<<(std::ostream& os, const MatrixArray&);

/*@}*/

/*================================== inline implementations ========================================*/

inline int MatrixArray::size() const {
	return n;
}

inline int MatrixArray::nb_rows() const {
	assert(n>0);
	return array[0].nb_rows();
}

inline int MatrixArray::nb_cols() const {
	assert(n>0);
	return array[0].nb_cols();
}

inline Matrix& MatrixArray::operator[](int i) {
	return array[i];
}

inline const Matrix& MatrixArray::operator[](int i) const {
	return array[i];
}

} // namespace ibex

#endif // __IBEX_MATRIX_ARRAY_H__
