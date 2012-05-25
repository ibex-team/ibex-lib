/* ============================================================================
 * I B E X - Matrix of intervals
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 6, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_INTERVAL_MATRIX_H__
#define __IBEX_INTERVAL_MATRIX_H__

#include "ibex_IntervalVector.h"
#include "ibex_Matrix.h"

#include <iostream>

namespace ibex {

class IntervalMatrixArray; // declared only for friendship

/**
 * \brief Interval matrix.
 */
class IntervalMatrix {

public:
	/**
	 * \brief Create a (nb_rows x nb_cols) matrix.
	 */
	IntervalMatrix(int nb_rows, int nb_cols);

	/**
	 * \brief Create a (nb_rows x nb_cols) matrix and initialize
	 * all the elements with x.
	 */
	IntervalMatrix(int nb_rows, int nb_cols, const Interval& x);

	/**
	 * \brief Duplicate a matrix.
	 */
	IntervalMatrix(const IntervalMatrix& m);

	/**
	 * \brief Create a degenerated interval matrix.
	 */
	IntervalMatrix(const Matrix& m);

	/**
	 * \brief Create an interval matrix from an array of doubles.
	 *
	 * Create the IntervalMatrix: <br>
	 *  [x[0][0],x[0][1]] ; ... ; [x[n-1][0], x[n-1][1]]   <br>
	 *  [x[n][0],x[n][1]] ; ... ; [x[2n-1][0],x[2n-1][1]]  <br>
	 *  ... <br>
	 *  [x[(m-1)n][0],x[n][1]] ; ... ; [x[mn-1][0],x[2n-1][1]]  <br>
	 *
	 * \param bounds an (mxn)x2 array of doubles
	 * \pre m>0, n>0
	 */
	IntervalMatrix(int m, int n, double x[][2]);

	/**
	 * \brief Create a mxn empty matrix
	 *
	 * Create an empty IntervalMatrix of dimension \a n x \a m
	 * (all the components being empty Intervals)
	 *
	 * \pre m>0, n>0
	 */
	static IntervalMatrix empty(int m, int n);

	/**
	 * \brief Set *this to m.
	 */
	IntervalMatrix& operator=(const IntervalMatrix& x);

	/**
	 * \brief Set *this to its intersection with x
	 *
	 * \return a reference to this.
	 * \throws
	 */
	IntervalMatrix& operator&=(const IntervalMatrix& x);

	/**
	 * \brief Set all the elements to 0 (even if empty).
	 */
	void clear();

	/**
	 * \brief True if the bounds of (*this) coincide with m.
	 *
	 * If the two matrices are empty and with the same dimensions
	 * then they are considered as equal.
	 */
	bool operator==(const IntervalMatrix& m) const;

	/**
	 * \brief True if one bound of (*this) differs from m.
	 */
	bool operator!=(const IntervalMatrix& m);

	/**
	 * \brief Resize the matrix
	 */
	void resize(int nb_rows, int nb_cols);

	/**
	 * \brief Return the number of columns.
	 */
	int nb_cols() const;

	/**
	 * \brief Return the number of rows.
	 */
	int nb_rows() const;

	/**
	 * \brief Return the lower bound matrix
	 * \pre (*this) must be nonempty
	 */
	Matrix lb() const;

	/**
	 * \brief Return the upper bound matrix
	 * \pre (*this) must be nonempty
	 */
	Matrix ub() const;

	/**
	 * \brief Return the midmatrix
	 * \pre (*this) must be nonempty
	 */
	Matrix mid() const;

	/**
	 * \brief Return true iff this IntervalMatrix is empty
	 */
	bool is_empty() const;

	/**
	 * \brief Return the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	IntervalVector& operator[](int i);

	/**
	 * \brief Return a const reference to the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	const IntervalVector& operator[](int i) const;

	/**
	 * \brief Set this IntervalMatrix to the empty IntervalMatrix
	 *
	 * The dimensions remain the same.
	 */
	void set_empty();

	/**
	 * \brief Return a submatrix.
	 */
	IntervalMatrix submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index);

	/**
	 * \brief Transpose of *this.
	 */
	IntervalMatrix transpose() const;

	/**
	 * \brief Return the ith row.

	 * Equivalent to (*this)[i.
	 */
	IntervalVector& row(int i);

	/**
	 * \brief Return a const reference to the ith row.

	 * Equivalent to (*this)[i.
	 */
	const IntervalVector& row(int i) const;

	/**
	 * \brief Return a column
	 */
	IntervalVector col(int i) const;

	/**
	 * \brief Return a subset of rows.
	 */
	IntervalMatrix rows(int start_index, int end_index);

	/**
	 * \brief Return a subset of columns.
	 */
	IntervalMatrix cols(int start_index, int end_index);

	/**
	 * \brief Set a row.
	 */
	void set_row(int row, const IntervalVector& v);

	/**
	 * \brief Set a column.
	 */
	void set_col(int col, const IntervalVector& v);

    /**
     * \brief (*this)+=m.
     */
    IntervalMatrix& operator+=(const Matrix& m);

    /**
     * \brief (*this)+=m.
     */
    IntervalMatrix& operator+=(const IntervalMatrix& m);

    /**
     * \brief (*this)-=m.
     */
    IntervalMatrix& operator-=(const Matrix& m);

    /**
     * \brief (*this)-=m.
     */
    IntervalMatrix& operator-=(const IntervalMatrix& m);

    /**
     * \brief (*this)=x * *(this).
     */
    IntervalMatrix& operator*=(double x);

    /**
     * \brief (*this)=x * *(this).
     */
    IntervalMatrix& operator*=(const Interval& x);

    /**
     * \brief (*this)*=m.
     */
    IntervalMatrix& operator*=(const Matrix& m);

    /**
     * \brief (*this)*=m.
     */
    IntervalMatrix& operator*=(const IntervalMatrix& m);

private:
	friend class IntervalMatrixArray;

	IntervalMatrix(); // for IntervalMatrixArray

	int _nb_rows;
	int _nb_cols;
	IntervalVector* M;
};

/**
 * \brief Return -m.
 */
IntervalMatrix operator-(const IntervalMatrix& m);

/**
 * \brief $[m]_1+[m]_2$.
 */
IntervalMatrix operator+(const IntervalMatrix& m1, const Matrix& m2);

/**
 * \brief $[m]_1+[m]_2$.
 */
IntervalMatrix operator+(const Matrix& m1, const IntervalMatrix& m2);

/**
 * \brief $[m]_1+[m]_2$.
 */
IntervalMatrix operator+(const IntervalMatrix& m1, const IntervalMatrix& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
IntervalMatrix operator-(const IntervalMatrix& m1, const Matrix& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
IntervalMatrix operator-(const Matrix& m1, const IntervalMatrix& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
IntervalMatrix operator-(const IntervalMatrix& m1, const IntervalMatrix& m2);

/**
 * \brief Scalar multiplication of a matrix.
 */
IntervalMatrix operator*(double d, const IntervalMatrix& m);

/**
 * \brief Scalar multiplication of a matrix.
 */
IntervalMatrix operator*(const Interval& x, const Matrix& m);

/**
 * \brief Scalar multiplication of a matrix.
 */
IntervalMatrix operator*(const Interval& x, const IntervalMatrix& m);

/*
 * \brief $[m]*[x]$.
 */
IntervalVector operator*(const IntervalMatrix& m, const Vector& x);

/**
 * \brief $[m]*[x]$.
 */
IntervalVector operator*(const Matrix& m, const IntervalVector& x);

/**
 * \brief $[m]*[x]$.
 */
IntervalVector operator*(const IntervalMatrix& m, const IntervalVector& x);

/*
 * \brief $[x]*[m]$.
 */
IntervalVector operator*(const Vector& x, const IntervalMatrix& m);

/**
 * \brief $[x]*[m]$.
 */
IntervalVector operator*(const IntervalVector& x, const Matrix& m);

/**
 * \brief $[x]*[m]$.
 */
IntervalVector operator*(const IntervalVector& x, const IntervalMatrix& m);

/**
 * \brief $[m]_1*[m]_2$.
 */
IntervalMatrix operator*(const IntervalMatrix& m1, const Matrix& m2);

/**
 * \brief $[m]_1*[m]_2$.
 */
IntervalMatrix operator*(const Matrix& m1, const IntervalMatrix& m2);

/**
 * \brief $[m]_1*[m]_2$.
 */
IntervalMatrix operator*(const IntervalMatrix& m1, const IntervalMatrix& m2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
IntervalMatrix outer_product(const IntervalVector& x1, const Vector& x2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
IntervalMatrix outer_product(const Vector& x1, const IntervalVector& x2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
IntervalMatrix outer_product(const IntervalVector& x1, const IntervalVector& x2);

/**
 * \brief |x|.
 */
IntervalMatrix abs(const IntervalMatrix& m);

/**
 * \brief Projection of $y=x_1+x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1+x_2\}$.
 */
bool proj_add(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);

/**
 * \brief Projection of $y=x_1-x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1-x_2\}$.
 */
bool proj_sub(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);

/**
 * \brief Projection of y1=x1*x2 (scalar product of a matrix).
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool proj_mul(const IntervalMatrix& y, Interval& x1, IntervalMatrix& x2);

/**
 * \brief Projection of y1=x1*x2 (product of a matrix by a vector).
 *
 * Uses a fixpoint algorithm with the provided ratio.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool proj_mul(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2, double ratio);

/**
 * \brief Projection of y1=x1*x2 (product of a row vector by a matrix).
 *
 * Uses a fixpoint algorithm with the provided ratio.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool proj_mul(const IntervalVector& y, IntervalVector& x1, IntervalMatrix& x2, double ratio);

/**
 * \brief Projection of y1=x1*x2 (scalar product of a matrix).
 *
 * Uses a fixpoint algorithm with the provided ratio.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool proj_mul(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2, double ratio);

/**
 * \brief Stream out a matrix.
 */
std::ostream& operator<<(std::ostream& os, const IntervalMatrix&);

/*================================== inline implementations ========================================*/

inline IntervalMatrix IntervalMatrix::empty(int m, int n) {
	return IntervalMatrix(m, n, Interval::EMPTY_SET);
}

inline bool IntervalMatrix::operator!=(const IntervalMatrix& m) {
	return !(*this==m);
}

inline int IntervalMatrix::nb_cols() const {
	return _nb_cols;
}

inline int IntervalMatrix::nb_rows() const {
	return _nb_rows;
}

inline IntervalVector& IntervalMatrix::operator[](int i) {
	assert(i>=0 && i<nb_rows());
	return M[i];
}

inline const IntervalVector& IntervalMatrix::operator[](int i) const {
	assert(i>=0 && i<nb_rows());
	return M[i];
}

inline void IntervalMatrix::set_empty() {
	(*this)[0][0]=Interval::EMPTY_SET;
}

inline IntervalVector& IntervalMatrix::row(int i) {
	assert(i>=0 && i<nb_rows());
	return M[i];
}

inline const IntervalVector& IntervalMatrix::row(int i) const {
	assert(i>=0 && i<nb_rows());
	return M[i];
}

inline IntervalMatrix IntervalMatrix::rows(int start_index, int end_index) {
	return submatrix(start_index, end_index, 0, nb_cols()-1);
}

inline IntervalMatrix IntervalMatrix::cols(int start_index, int end_index) {
	return submatrix(0, nb_rows()-1, start_index, end_index);
}

inline void IntervalMatrix::set_row(int row, const IntervalVector& v) {
	assert(row>=0 && row<nb_rows());
	assert(nb_cols()==v.size());
	M[row]=v;
}

inline bool IntervalMatrix::is_empty() const {
	return (*this)[0][0].is_empty();
}

} // namespace ibex
#endif // __IBEX_INTERVAL_MATRIX_H__
