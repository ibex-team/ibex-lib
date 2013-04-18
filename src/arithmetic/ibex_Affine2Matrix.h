/* ============================================================================
 * I B E X - Matrix of Affine2 forms
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 16, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE2_MATRIX_H__
#define __IBEX_AFFINE2_MATRIX_H__

#include "ibex_IntervalMatrix.h"
#include "ibex_Matrix.h"
#include <iostream>
#include "ibex_Affine2Vector.h"

namespace ibex {

class Affine2MatrixArray; // declared only for friendship

/**
 * \ingroup arithmetic
 *
 * \brief Affine2 matrix.
 */
class Affine2Matrix {

private:
	friend class Affine2MatrixArray;

	Affine2Matrix(); // for Affine2MatrixArray

	int _nb_rows;
	int _nb_cols;
	Affine2Vector* _M;

public:
	/**
	 * \brief Create a (nb_rows x nb_cols) matrix.
	 */
	Affine2Matrix(int nb_rows, int nb_cols);

	/**
	 * \brief Create a (nb_rows x nb_cols) matrix and initialize
	 * all the elements with x.
	 */
	Affine2Matrix(int nb_rows, int nb_cols, const Affine2& x);

	/** It is too difficult to know the size of each AF2. So we let the user do what exactly he want to do.
	 * \brief Create \a x.size Affine2Vector of dimension \a x.nb_col*x.nb_row with
	 * the [i][j] component initialized to \a Affine2(m[i][j]).
	 */
	explicit Affine2Matrix(const IntervalMatrix& m);

	/**
	 * \brief Create  a copy of x
	 */
	Affine2Matrix(const Affine2Matrix& x);

	/**
	 * \brief Create a degenerated Affine2 matrix.
	 * the [i][j] component initialized to \a Affine2(m[i][j]).
	 */
	explicit Affine2Matrix(const Matrix& m);

	/**
	 * \brief Create an Affine2 matrix from an array of doubles.
	 *
	 * Create the Affine2Matrix: <br>
	 *  [x[0][0],x[0][1]] ; ... ; [x[n-1][0], x[n-1][1]]   <br>
	 *  [x[n][0],x[n][1]] ; ... ; [x[2n-1][0],x[2n-1][1]]  <br>
	 *  ... <br>
	 *  [x[(m-1)n][0],x[n][1]] ; ... ; [x[mn-1][0],x[2n-1][1]]  <br>
	 *
	 * \param bounds an (mxn)x2 array of doubles
	 * \pre m>0, n>0
	 */
	Affine2Matrix(int m, int n, double x[][2]);

	/**
	 * \brief Delete *this.
	 */
	~Affine2Matrix();

	/**
	 * \brief Create a mxn empty matrix
	 *
	 * Create an empty Affine2Matrix of dimension \a n x \a m
	 * (all the components being empty Affine2s)
	 *
	 * \pre m>0, n>0
	 */
	static Affine2Matrix empty(int m, int n);

	/**
	 * \brief Set *this to m.
	 */
	Affine2Matrix& operator=(const Affine2Matrix& x);
	Affine2Matrix& operator=(const IntervalMatrix& x);


	/**
	 * \brief Set all the elements to 0 (even if empty).
	 */
	void clear();

	/**
	 * \brief Set all the elements to x (even if empty).
	 *
	 * \note Emptiness is "overridden".
	 */
	void init(const Affine2& x);
	void init(const Interval& x);

	/**
	 * \brief True if the bounds of (*this) coincide with m.
	 *
	 * If the two matrices are empty and with the same dimensions
	 * then they are considered as equal.
	 */
	bool operator==(const Affine2Matrix& m) const;
	bool operator==(const IntervalMatrix& m) const;

	/**
	 * \brief True if one bound of (*this) differs from m.
	 */
	bool operator!=(const Affine2Matrix& m) const;
	bool operator!=(const IntervalMatrix& m) const;


	/**
	 * \brief Return the IntervalMatrix compose by the interval of each Affine2 form
	 * \pre (*this) must be nonempty
	 */
	IntervalMatrix itv() const;

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
	 * \brief Return true iff this Affine2Matrix is empty
	 */
	bool is_empty() const;

	/**
	 * \brief Return the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	Affine2Vector& operator[](int i);

	/**
	 * \brief Return a const reference to the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	const Affine2Vector& operator[](int i) const;

	/**
	 * \brief Set this Affine2Matrix to the empty Affine2Matrix
	 *
	 * The dimensions remain the same.
	 */
	void set_empty();

	/**
	 * \brief Return a submatrix.
	 */
	Affine2Matrix submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index);

	/**
	 * \brief Transpose of *this.
	 */
	Affine2Matrix transpose() const;

	/**
	 * \brief Return the ith row.

	 * Equivalent to (*this)[i.
	 */
	Affine2Vector& row(int i);

	/**
	 * \brief Return a const reference to the ith row.

	 * Equivalent to (*this)[i.
	 */
	const Affine2Vector& row(int i) const;

	/**
	 * \brief Return a column
	 */
	Affine2Vector col(int i) const;

	/**
	 * \brief Return a subset of rows.
	 */
	Affine2Matrix rows(int start_index, int end_index);

	/**
	 * \brief Return a subset of columns.
	 */
	Affine2Matrix cols(int start_index, int end_index);

	/**
	 * \brief Set a row.
	 */
	void set_row(int row, const Affine2Vector& v);


	/**
	 * \brief Set a column.
	 */
	void set_col(int col, const Affine2Vector& v);


    /**
     * \brief (*this)+=m.
     */
    Affine2Matrix& operator+=(const Matrix& m);

    /**
     * \brief (*this)+=m.
     */
    Affine2Matrix& operator+=(const Affine2Matrix& m);
    Affine2Matrix& operator+=(const IntervalMatrix& m);

    /**
     * \brief (*this)-=m.
     */
    Affine2Matrix& operator-=(const Matrix& m);

    /**Affine2
     * \brief (*this)-=m.
     */
    Affine2Matrix& operator-=(const Affine2Matrix& m);
    Affine2Matrix& operator-=(const IntervalMatrix& m);

	/**
	 * \brief Add [-rad,+rad] to all the components of *this.
	 *
	 * \return *this.
	 */
	Affine2Matrix& inflate(double rad);

    /**
     * \brief (*this)=x * *(this).
     */
    Affine2Matrix& operator*=(double x);

    /**
     * \brief (*this)=x * *(this).
     */
    Affine2Matrix& operator*=(const Affine2& x);
    Affine2Matrix& operator*=(const Interval& x);

    /**
     * \brief (*this)*=m.
     */
    Affine2Matrix& operator*=(const Matrix& m);

    /**
     * \brief (*this)*=m.
     */
    Affine2Matrix& operator*=(const Affine2Matrix& m);
    Affine2Matrix& operator*=(const IntervalMatrix& m);


};

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief Return the intersection of x and y.
 */
IntervalMatrix operator&(const Affine2Matrix& x, const Affine2Matrix& y);
IntervalMatrix operator&(const IntervalMatrix& x, const Affine2Matrix& y);
IntervalMatrix operator&(const Affine2Matrix& x, const IntervalMatrix& y);

/**
 * \brief Return the hull of x & y.
 */
IntervalMatrix operator|(const Affine2Matrix& x, const Affine2Matrix& y);
IntervalMatrix operator|(const IntervalMatrix& x, const Affine2Matrix& y);
IntervalMatrix operator|(const Affine2Matrix& x, const IntervalMatrix& y);

/**
 * \brief Return -m.
 */
Affine2Matrix operator-(const Affine2Matrix& m);

/**
 * \brief $[m]_1+[m]_2$.
 */
Affine2Matrix operator+(const Affine2Matrix& m1, const Matrix& m2);

/**
 * \brief $[m]_1+[m]_2$.
 */
Affine2Matrix operator+(const Matrix& m1, const Affine2Matrix& m2);

/**
 * \brief $[m]_1+[m]_2$.
 */
Affine2Matrix operator+(const Affine2Matrix& m1, const Affine2Matrix& m2);
Affine2Matrix operator+(const Affine2Matrix& m1, const IntervalMatrix& m2);
Affine2Matrix operator+(const IntervalMatrix& m1, const Affine2Matrix& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
Affine2Matrix operator-(const Affine2Matrix& m1, const Matrix& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
Affine2Matrix operator-(const Matrix& m1, const Affine2Matrix& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
Affine2Matrix operator-(const Affine2Matrix& m1, const Affine2Matrix& m2);
Affine2Matrix operator-(const Affine2Matrix& m1, const IntervalMatrix& m2);
Affine2Matrix operator-(const IntervalMatrix& m1, const Affine2Matrix& m2);

/**
 * \brief Scalar multiplication of a matrix.
 */
Affine2Matrix operator*(double d, const Affine2Matrix& m);

/**
 * \brief Scalar multiplication of a matrix.
 */
Affine2Matrix operator*(const Affine2& x, const Matrix& m);

/**
 * \brief Scalar multiplication of a matrix.
 */
Affine2Matrix operator*(const Affine2& x, const Affine2Matrix& m);
Affine2Matrix operator*(const Interval& x, const Affine2Matrix& m);
Affine2Matrix operator*(const Affine2& x, const IntervalMatrix& m);


/*
 * \brief $[m]*[x]$.
 */
Affine2Vector operator*(const Affine2Matrix& m, const Vector& x);

/**
 * \brief $[m]*[x]$.
 */
Affine2Vector operator*(const Matrix& m, const Affine2Vector& x);

/**
 * \brief $[m]*[x]$.
 */
Affine2Vector operator*(const Affine2Matrix& m, const Affine2Vector& x);
Affine2Vector operator*(const Affine2Matrix& m, const IntervalVector& x);
Affine2Vector operator*(const IntervalMatrix& m, const Affine2Vector& x);

/*
 * \brief $[x]*[m]$.
 */
Affine2Vector operator*(const Vector& x, const Affine2Matrix& m);

/**
 * \brief $[x]*[m]$.
 */
Affine2Vector operator*(const Affine2Vector& x, const Matrix& m);

/**
 * \brief $[x]*[m]$.
 */
Affine2Vector operator*(const Affine2Vector& x, const Affine2Matrix& m);
Affine2Vector operator*(const Affine2Vector& x, const IntervalMatrix& m);
Affine2Vector operator*(const IntervalVector& x, const Affine2Matrix& m);

/**
 * \brief $[m]_1*[m]_2$.
 */
Affine2Matrix operator*(const Affine2Matrix& m1, const Matrix& m2);

/**
 * \brief $[m]_1*[m]_2$.
 */
Affine2Matrix operator*(const Matrix& m1, const Affine2Matrix& m2);

/**
 * \brief $[m]_1*[m]_2$.
 */
Affine2Matrix operator*(const Affine2Matrix& m1, const Affine2Matrix& m2);
Affine2Matrix operator*(const Affine2Matrix& m1, const IntervalMatrix& m2);
Affine2Matrix operator*(const IntervalMatrix& m1, const Affine2Matrix& m2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
Affine2Matrix outer_product(const Affine2Vector& x1, const Vector& x2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
Affine2Matrix outer_product(const Vector& x1, const Affine2Vector& x2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
Affine2Matrix outer_product(const Affine2Vector& x1, const Affine2Vector& x2);
Affine2Matrix outer_product(const Affine2Vector& x1, const IntervalVector& x2);
Affine2Matrix outer_product(const IntervalVector& x1, const Affine2Vector& x2);

/**
 * \brief |x|.
 */
Affine2Matrix abs(const Affine2Matrix& m);


/**
 * \brief Stream out a matrix.
 */
std::ostream& operator<<(std::ostream& os, const Affine2Matrix&);

/*@}*/

/*================================== inline implementations ========================================*/

inline Affine2Matrix Affine2Matrix::empty(int m, int n) {
	Affine2Matrix  res(m, n);
	res.set_empty();
	return res;
}

inline bool Affine2Matrix::operator!=(const Affine2Matrix& m) const {
	return !(*this==m);
}
inline bool Affine2Matrix::operator!=(const IntervalMatrix& m) const {
	return !(*this==m);
}

inline int Affine2Matrix::nb_cols() const {
	return _nb_cols;
}

inline int Affine2Matrix::nb_rows() const {
	return _nb_rows;
}

inline Affine2Vector& Affine2Matrix::operator[](int i) {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

inline const Affine2Vector& Affine2Matrix::operator[](int i) const {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

inline void Affine2Matrix::clear() {
	init(0);
}

inline void Affine2Matrix::set_empty() {
	(*this)[0].set_empty();
}

inline Affine2Vector& Affine2Matrix::row(int i) {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

inline const Affine2Vector& Affine2Matrix::row(int i) const {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

inline Affine2Matrix Affine2Matrix::rows(int start_index, int end_index) {
	return submatrix(start_index, end_index, 0, nb_cols()-1);
}

inline Affine2Matrix Affine2Matrix::cols(int start_index, int end_index) {
	return submatrix(0, nb_rows()-1, start_index, end_index);
}

inline void Affine2Matrix::set_row(int row1, const Affine2Vector& v1) {
	assert(row1>=0 && row1<nb_rows());
	assert(nb_cols()==v1.size());
	_M[row1]=v1;
}


inline bool Affine2Matrix::is_empty() const {
	return (*this)[0].is_empty();
}



inline IntervalMatrix operator|(const IntervalMatrix& x, const Affine2Matrix& y) {
 	 return   (y | x );
}

inline IntervalMatrix operator&(const IntervalMatrix& x, const Affine2Matrix& y) {
 	 return   (y & x);
}


inline Affine2Matrix operator+(const Affine2Matrix& m1, const Matrix& m2) {
	return Affine2Matrix(m1)+=m2;
}

inline Affine2Matrix operator+(const Matrix& m1, const Affine2Matrix& m2) {
	 return Affine2Matrix(m2)+=m1;
}

inline Affine2Matrix operator+(const Affine2Matrix& m1, const Affine2Matrix& m2){
 	 return Affine2Matrix(m1)+=m2;
}
inline Affine2Matrix operator+(const Affine2Matrix& m1, const IntervalMatrix& m2){
 	 return Affine2Matrix(m1)+=m2;
}
inline Affine2Matrix operator+(const IntervalMatrix& m1, const Affine2Matrix& m2){
 	 return Affine2Matrix(m2)+=m1;
}

inline Affine2Matrix operator-(const Matrix& m1, const Affine2Matrix& m2){
	 Affine2Matrix res(m2.nb_rows(),m2.nb_cols());
	 res = (-m2);
 	 return res+=m1;
}

inline Affine2Matrix operator-(const Affine2Matrix& m1, const Affine2Matrix& m2){
	 return Affine2Matrix(m1) += (-m2);
}
inline Affine2Matrix operator-(const Affine2Matrix& m1, const IntervalMatrix& m2){
 	 return Affine2Matrix(m1)-=m2;
}
inline Affine2Matrix operator-(const IntervalMatrix& m1, const Affine2Matrix& m2){
	 Affine2Matrix res(m2.nb_rows(),m2.nb_cols());
	 res = (-m2);
	 return res+=m1;
}

inline Affine2Matrix operator-(const Affine2Matrix& m1, const Matrix& m2) {
	return Affine2Matrix(m1)-=m2;
}

inline Affine2Matrix operator*(double d, const Affine2Matrix& m){
 	 return Affine2Matrix(m)*=d;
}

inline Affine2Matrix operator*(const Affine2& x, const Affine2Matrix& m){
 	 return Affine2Matrix(m)*=x;
}
inline Affine2Matrix operator*(const Interval& x, const Affine2Matrix& m){
 	 return Affine2Matrix(m)*=x;
}

inline Affine2Matrix& Affine2Matrix::operator*=(const Matrix& m) {
	return (*this)=(*this)*m;
}

inline Affine2Matrix& Affine2Matrix::operator*=(const Affine2Matrix& m) {
	return (*this)=(*this)*m;
}

inline Affine2Matrix& Affine2Matrix::operator*=(const IntervalMatrix& m) {
	return (*this)=(*this)*m;
}




} // namespace ibex
#endif // __IBEX_AFFINE2_MATRIX_H__
