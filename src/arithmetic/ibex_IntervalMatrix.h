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

/**
 * \ingroup arithmetic
 *
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
	 * \brief Delete *this.
	 */
	~IntervalMatrix();

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
	 */
	IntervalMatrix& operator&=(const IntervalMatrix& x);

	/**
	 * \brief Set all the elements to 0 (even if empty).
	 */
	void clear();

	/**
	 * \brief Set all the elements to x (even if empty).
	 *
	 * \note Emptiness is "overridden".
	 */
	void init(const Interval& x);

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
	bool operator!=(const IntervalMatrix& m) const;

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
	 * \brief Return the mignitude matrix.
	 * \pre (*this) must be nonempty
	 */
	Matrix mig() const;

	/**
	 * \brief Return the magnitude matrix.
	 * \pre (*this) must be nonempty
	 */
	Matrix mag() const;

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
	 * \brief True iff *this is a matrix of zeros.
	 */
	bool is_zero() const;

	/**
	 * \brief Matrix of radii.
	 */
	Matrix rad() const;

	/**
	 * \brief Matrix of diameters.
	 */
	Matrix diam() const;

	/**
	 * \brief true iff this interval matrix contains an infinite bound.
	 *
	 * \note An empty interval matrix is always bounded.
	 */
	bool is_unbounded() const;

	/**
	 * \brief True iff this interval matrix is a subset of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this matrix.
	 *
	 * \note Always return true if this interval matrix is empty.
	 *
	 * \sa #ibex::Interval::is_subset(const Interval&) const.
	 */
	bool is_subset(const IntervalMatrix& x) const;

	/**
	 * \brief True iff this interval matrix is inside the interior of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this matrix.
	 *
	 * \note return true if this interval matrix is empty and \a x not.
	 *
	 * \sa #ibex::Interval::is_strict_subset(const Interval&) const.
	 */
	bool is_strict_subset(const IntervalMatrix& x) const;

	/**
	 * \brief True iff this interval matrix is a subset of the interior of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this matrix.
	 *
	 * \note Always return true if this interval matrix is empty.
	 *
	 * \sa #ibex::Interval::is_interior_subset(const Interval&) const.
	 */
	bool is_interior_subset(const IntervalMatrix& x) const;

    /**
     * \brief True iff this interval matrix is in the relative interior of \a x.
     *
     * The relative interior of an interval matrix is defined as the matrix
     * of the relative interiors of its components, where
     *
     *     rel([a,b]) := [a,b] (the closed interval) if a==b ;
     *                   (a,b) (the open interval) otherwise.
     */
    bool is_relative_interior_subset(const IntervalMatrix& x) const;

    /**
	 * \brief True iff this interval matrix is a subset of the interior of \a x and different from x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this matrix.
	 *
	 * \note Always return true if this interval matrix is empty.
	 *
	 * \sa #ibex::Interval::is_interior_subset(const Interval&) const.
	 */
	bool is_strict_interior_subset(const IntervalMatrix& x) const;

	/**
	 * \brief True iff this interval matrix is a superset of \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this matrix.

	 * \note Always return true if \a x is empty.

	 * \sa #ibex::Interval::is_superset(const Interval&) const.
	 */
	bool is_superset(const IntervalMatrix& x) const;

	/**
	 * \brief True iff \a x is inside the interior of (*this).
	 *
	 * \pre Dimension of \a x must be equal to the dimension of this matrix.
	 *
	 * \note return true if x is empty and not (*this).
	 *
	 * \sa #ibex::Interval::is_strict_superset(const Interval&) const.
	 */
	bool is_strict_superset(const IntervalMatrix& x) const;

	/**
	 * \brief True iff this interval matrix contains \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::contains(double) const.
	 */
	bool contains(const Matrix& x) const;

	/**
	 * \brief True iff \a x is in the interior of this interval matrix
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::strictly_contains(double) const.
	 */
	bool interior_contains(const Matrix& x) const;

	/**
	 * \brief True iff this interval matrix intersects \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::intersects(double) const.
	 */
	bool intersects(const IntervalMatrix& x) const;

	/**
	 * \brief True iff this interval matrix intersects \a x and the intersection has a non-null volume
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::strictly_intersects(double) const.
	 */
	bool overlaps(const IntervalMatrix& x) const;

	/**
	 * \brief True iff this interval matrix does not intersect \a x.
	 *
	 * \pre Dimension of \a x must be equal to the dimension of (*this).
	 * \sa #ibex::Interval::is_disjoint(double) const.
	 */
	bool is_disjoint(const IntervalMatrix& x) const;

	/**
	 * \brief Return a submatrix.
	 * \pre (*this) must be nonempty
	 */
	IntervalMatrix submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index) const;

	/**
	 * \brief Transpose of *this.
	 * \pre (*this) must be nonempty
	 */
	IntervalMatrix transpose() const;

	/**
	 * \brief Return the ith row.

	 * Equivalent to (*this)[i.
	 * \pre (*this) must be nonempty
	 */
	IntervalVector& row(int i);

	/**
	 * \brief Return a const reference to the ith row.

	 * Equivalent to (*this)[i.
	 * \pre (*this) must be nonempty
	 */
	const IntervalVector& row(int i) const;

	/**
	 * \brief Return a column
	 * \pre (*this) must be nonempty
	 */
	IntervalVector col(int i) const;

	/**
	 * \brief Return a subset of rows.
	 * \pre (*this) must be nonempty
	 */
	IntervalMatrix rows(int start_index, int end_index);

	/**
	 * \brief Return a subset of columns.
	 * \pre (*this) must be nonempty
	 */
	IntervalMatrix cols(int start_index, int end_index);

	/**
	 * \brief Set a row.
	 * \pre (*this) must be nonempty
	 */
	void set_row(int row, const IntervalVector& v);

	/**
	 * \brief Set a column.
	 * \pre (*this) must be nonempty
	 */
	void set_col(int col, const IntervalVector& v);

	/**
	 * \brief Insert a submatrix at some position
	 * \pre (*this) must be nonempty
	 */
	void put(int row_start_index, int col_start_index, const Matrix& M);

	/**
	 * \brief Insert a submatrix at some position
	 * \pre (*this) must be nonempty
	 */
	void put(int row_start_index, int col_start_index, const IntervalMatrix& M);

	/**
	 * \brief Insert a subvecvtor at some position
	 * \param row_vec true if the vector is a row vector
	 * \pre (*this) must be nonempty
	 */
	void put(int row_start_index, int col_start_index, const Vector& V, bool row_vec);

	/**
	 * \brief Insert a subvecvtor at some position
	 * \param row_vec true if the vector is a row vector
	 * \pre (*this) must be nonempty
	 */
	void put(int row_start_index, int col_start_index, const IntervalVector& V, bool row_vec);

	/**
	 * \brief Return a random matix inside *this.
	 *
	 * \pre (*this) must be nonempty.
	 */
	Matrix random(int seed) const;
	Matrix random() const;

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
	 * \brief Add [-rad,+rad] to all the components of *this.
	 *
	 * \return *this.
	 */
	IntervalMatrix& inflate(double rad);

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

    /**
     * \brief Cast the matrix to an expression
     */
    operator const ExprConstant&() const;

private:

	IntervalMatrix();

	int _nb_rows;
	int _nb_cols;
	IntervalVector* M;
};

/** \ingroup arithmetic */
/*@{*/

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
 * \brief ||x||_oo.
 */
double infinite_norm(const IntervalMatrix& m);

/**
 * \brief Projection of $y=x_1+x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1+x_2\}$.
 */
bool bwd_add(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);

/**
 * \brief Projection of $y=x_1-x_2$.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1-x_2\}$.
 */
bool bwd_sub(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2);

/**
 * \brief Projection of y1=x1*x2 (scalar product of a matrix).
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool bwd_mul(const IntervalMatrix& y, Interval& x1, IntervalMatrix& x2);

/**
 * \brief Projection of y1=x1*x2 (product of a matrix by a vector).
 *
 * Uses a fixpoint algorithm with the provided ratio.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool bwd_mul(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2, double ratio);

/**
 * \brief Projection of y1=x1*x2 (product of a row vector by a matrix).
 *
 * Uses a fixpoint algorithm with the provided ratio.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool bwd_mul(const IntervalVector& y, IntervalVector& x1, IntervalMatrix& x2, double ratio);

/**
 * \brief Projection of y1=x1*x2 (scalar product of a matrix).
 *
 * Uses a fixpoint algorithm with the provided ratio.
 *
 * Set $([x]_1,[x]_2)$ to $([x]_1,[x]_2])\cap\{ (x_1,x_2)\in [x]_1\times[x]_2 \ | \ \exists y\in[y],\ y=x_1*x_2\}$.
 */
bool bwd_mul(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2, double ratio);

/**
 * \brief Stream out a matrix.
 */
std::ostream& operator<<(std::ostream& os, const IntervalMatrix&);

/*@}*/

/*================================== inline implementations ========================================*/

// the following functions are
// introduced to allow genericity
inline bool ___is_empty(const IntervalMatrix& m) { return m.is_empty(); }
inline void ___set_empty(IntervalMatrix& m) { m.set_empty(); }

} // end namespace ibex

#include "ibex_LinearArith.h"

namespace ibex {

inline IntervalMatrix IntervalMatrix::empty(int m, int n) {
	return IntervalMatrix(m, n, Interval::empty_set());
}

inline bool IntervalMatrix::operator!=(const IntervalMatrix& m) const {
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

inline void IntervalMatrix::clear() {
	init(0);
}

inline void IntervalMatrix::set_empty() {
	for (int i=0; i<nb_rows(); i++)
		for (int j=0; j<nb_cols(); j++)
			(*this)[i][j]=Interval::empty_set();
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

inline void IntervalMatrix::set_row(int row1, const IntervalVector& v1) {
	assert(row1>=0 && row1<nb_rows());
	assert(nb_cols()==v1.size());
	M[row1]=v1;
}

inline bool IntervalMatrix::is_empty() const {
	return (*this)[0].is_empty();
}

inline IntervalMatrix& IntervalMatrix::operator+=(const Matrix& m) {
	return set_addM<IntervalMatrix,Matrix>(*this,m);
}

inline IntervalMatrix& IntervalMatrix::operator+=(const IntervalMatrix& m) {
	return set_addM<IntervalMatrix,IntervalMatrix>(*this,m);
}

inline IntervalMatrix& IntervalMatrix::operator-=(const Matrix& m) {
	return set_subM<IntervalMatrix,Matrix>(*this,m);
}

inline IntervalMatrix& IntervalMatrix::operator-=(const IntervalMatrix& m) {
	return set_subM<IntervalMatrix,IntervalMatrix>(*this,m);
}

inline IntervalMatrix& IntervalMatrix::operator*=(double x) {
	return set_mulSM<double,IntervalMatrix>(x,*this);
}

inline IntervalMatrix& IntervalMatrix::operator*=(const Interval& x) {
	return set_mulSM<Interval,IntervalMatrix>(x,*this);
}

inline IntervalMatrix& IntervalMatrix::operator*=(const Matrix& m) {
	return *this=(*this*m);
}

inline IntervalMatrix& IntervalMatrix::operator*=(const IntervalMatrix& m) {
	return *this=(*this*m);
}

inline IntervalMatrix operator-(const IntervalMatrix& m) {
	return minusM(m);
}

inline IntervalMatrix operator+(const IntervalMatrix& m1, const Matrix& m2) {
	return IntervalMatrix(m1)+=m2;
}

inline IntervalMatrix operator+(const Matrix& m1, const IntervalMatrix& m2) {
	return IntervalMatrix(m1)+=m2;
}

inline IntervalMatrix operator+(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	return IntervalMatrix(m1)+=m2;
}

inline IntervalMatrix operator-(const IntervalMatrix& m1, const Matrix& m2) {
	return IntervalMatrix(m1)-=m2;
}

inline IntervalMatrix operator-(const Matrix& m1, const IntervalMatrix& m2) {
	return IntervalMatrix(m1)-=m2;
}

inline IntervalMatrix operator-(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	return IntervalMatrix(m1)-=m2;
}

inline IntervalMatrix operator*(double x, const IntervalMatrix& m) {
	return IntervalMatrix(m)*=x;
}

inline IntervalMatrix operator*(const Interval& x, const Matrix& m) {
	return IntervalMatrix(m)*=x;
}

inline IntervalMatrix operator*(const Interval& x, const IntervalMatrix& m) {
	return IntervalMatrix(m)*=x;
}

inline IntervalMatrix outer_product(const Vector& v1, const IntervalVector& v2) {
	return outer_prod<Vector,IntervalVector,IntervalMatrix>(v1,v2);
}

inline IntervalMatrix outer_product(const IntervalVector& v1, const Vector& v2) {
	return outer_prod<IntervalVector,Vector,IntervalMatrix>(v1,v2);
}

inline IntervalMatrix outer_product(const IntervalVector& v1, const IntervalVector& v2) {
	return outer_prod<IntervalVector,IntervalVector,IntervalMatrix>(v1,v2);
}

inline IntervalVector operator*(const IntervalMatrix& m, const Vector& v) {
	return mulMV<IntervalMatrix,Vector,IntervalVector>(m,v);
}

inline IntervalVector operator*(const IntervalMatrix& m, const IntervalVector& v) {
	return mulMV<IntervalMatrix,IntervalVector,IntervalVector>(m,v);
}

inline IntervalVector operator*(const Vector& v, const IntervalMatrix& m) {
	return mulVM<Vector,IntervalMatrix,IntervalVector>(v,m);
}

inline IntervalVector operator*(const IntervalVector& v, const IntervalMatrix& m) {
	return mulVM<IntervalVector,IntervalMatrix,IntervalVector>(v,m);
}

inline IntervalMatrix operator*(const Matrix& m1, const IntervalMatrix& m2) {
	return mulMM<Matrix,IntervalMatrix,IntervalMatrix>(m1,m2);
}

inline IntervalMatrix operator*(const IntervalMatrix& m1, const Matrix& m2) {
	return mulMM<IntervalMatrix,Matrix,IntervalMatrix>(m1,m2);
}

inline IntervalMatrix operator*(const IntervalMatrix& m1, const IntervalMatrix& m2) {
	return mulMM<IntervalMatrix,IntervalMatrix,IntervalMatrix>(m1,m2);
}

} // namespace ibex
#endif // __IBEX_INTERVAL_MATRIX_H__
