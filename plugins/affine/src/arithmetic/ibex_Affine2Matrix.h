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

template<class T> class Affine2MainMatrixArray;

/**
 * \ingroup arithmetic
 *
 * \brief Affine2 matrix.
 */


typedef Affine2MainMatrix<AF_Default> Affine2Matrix;
typedef Affine2MainMatrix<AF_Linear> AffineLinMatrix;

template<class T=AF_Default>
class Affine2MainMatrix {

private:
	friend class Affine2MainMatrixArray<T>;

	Affine2MainMatrix(); // for Affine2MainMatrixArray

	int _nb_rows;
	int _nb_cols;
	Affine2MainVector<T>* _M;

public:
	/**
	 * \brief Create a (nb_rows x nb_cols) matrix.
	 */
	Affine2MainMatrix(int nb_rows, int nb_cols);

	/**
	 * \brief Create a (nb_rows x nb_cols) matrix and initialize
	 * all the elements with x.
	 */
	Affine2MainMatrix(int nb_rows, int nb_cols, const Affine2Main<T>& x);

	/** It is too difficult to know the size of each AF2. So we let the user do what exactly he want to do.
	 * \brief Create \a x.size Affine2MainVector<T> of dimension \a x.nb_col*x.nb_row with
	 * the [i][j] component initialized to \a Affine2(m[i][j]).
	 */
	explicit Affine2MainMatrix(const IntervalMatrix& m);

	/**
	 * \brief Create  a copy of x
	 */
	Affine2MainMatrix(const Affine2MainMatrix& x);

	/**
	 * \brief Create a degenerated Affine2 matrix.
	 * the [i][j] component initialized to \a Affine2(m[i][j]).
	 */
	explicit Affine2MainMatrix(const Matrix& m);

	/**
	 * \brief Create an Affine2 matrix from an array of doubles.
	 *
	 * Create the Affine2MainMatrix: <br>
	 *  [x[0][0],x[0][1]] ; ... ; [x[n-1][0], x[n-1][1]]   <br>
	 *  [x[n][0],x[n][1]] ; ... ; [x[2n-1][0],x[2n-1][1]]  <br>
	 *  ... <br>
	 *  [x[(m-1)n][0],x[n][1]] ; ... ; [x[mn-1][0],x[2n-1][1]]  <br>
	 *
	 * \param bounds an (mxn)x2 array of doubles
	 * \pre m>0, n>0
	 */
	Affine2MainMatrix(int m, int n, double x[][2]);

	/**
	 * \brief Delete *this.
	 */
	~Affine2MainMatrix();

	/**
	 * \brief Create a mxn empty matrix
	 *
	 * Create an empty Affine2MainMatrix of dimension \a n x \a m
	 * (all the components being empty Affine2s)
	 *
	 * \pre m>0, n>0
	 */
	static Affine2MainMatrix empty(int m, int n);

	/**
	 * \brief Set *this to m.
	 */
	Affine2MainMatrix& operator=(const Affine2MainMatrix& x);
	Affine2MainMatrix& operator=(const IntervalMatrix& x);


	/**
	 * \brief Set all the elements to 0 (even if empty).
	 */
	void clear();

	/**
	 * \brief Set all the elements to x (even if empty).
	 *
	 * \note Emptiness is "overridden".
	 */
	void init(const Affine2Main<T>& x);
	void init(const Interval& x);

	/**
	 * \brief True if the bounds of (*this) coincide with m.
	 *
	 * If the two matrices are empty and with the same dimensions
	 * then they are considered as equal.
	 */
	bool operator==(const Affine2MainMatrix& m) const;
	bool operator==(const IntervalMatrix& m) const;

	/**
	 * \brief True if one bound of (*this) differs from m.
	 */
	bool operator!=(const Affine2MainMatrix& m) const;
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
	 * \brief Return true iff this Affine2MainMatrix is empty
	 */
	bool is_empty() const;

	/**
	 * \brief Return the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	Affine2MainVector<T>& operator[](int i);

	/**
	 * \brief Return a const reference to the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	const Affine2MainVector<T>& operator[](int i) const;

	/**
	 * \brief Set this Affine2MainMatrix to the empty Affine2MainMatrix
	 *
	 * The dimensions remain the same.
	 */
	void set_empty();

	/**
	 * \brief Return a submatrix.
	 */
	Affine2MainMatrix submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index);

	/**
	 * \brief Transpose of *this.
	 */
	Affine2MainMatrix transpose() const;

	/**
	 * \brief Return the ith row.

	 * Equivalent to (*this)[i.
	 */
	Affine2MainVector<T>& row(int i);

	/**
	 * \brief Return a const reference to the ith row.

	 * Equivalent to (*this)[i.
	 */
	const Affine2MainVector<T>& row(int i) const;

	/**
	 * \brief Return a column
	 */
	Affine2MainVector<T> col(int i) const;

	/**
	 * \brief Return a subset of rows.
	 */
	Affine2MainMatrix rows(int start_index, int end_index);

	/**
	 * \brief Return a subset of columns.
	 */
	Affine2MainMatrix cols(int start_index, int end_index);

	/**
	 * \brief Set a row.
	 */
	void set_row(int row, const Affine2MainVector<T>& v);


	/**
	 * \brief Set a column.
	 */
	void set_col(int col, const Affine2MainVector<T>& v);


    /**
     * \brief (*this)+=m.
     */
    Affine2MainMatrix& operator+=(const Matrix& m);

    /**
     * \brief (*this)+=m.
     */
    Affine2MainMatrix& operator+=(const Affine2MainMatrix& m);
    Affine2MainMatrix& operator+=(const IntervalMatrix& m);

    /**
     * \brief (*this)-=m.
     */
    Affine2MainMatrix& operator-=(const Matrix& m);

    /**Affine2
     * \brief (*this)-=m.
     */
    Affine2MainMatrix& operator-=(const Affine2MainMatrix& m);
    Affine2MainMatrix& operator-=(const IntervalMatrix& m);

	/**
	 * \brief Add [-rad,+rad] to all the components of *this.
	 *
	 * \return *this.
	 */
	Affine2MainMatrix& inflate(double rad);

    /**
     * \brief (*this)=x * *(this).
     */
    Affine2MainMatrix& operator*=(double x);

    /**
     * \brief (*this)=x * *(this).
     */
    Affine2MainMatrix& operator*=(const Affine2Main<T>& x);
    Affine2MainMatrix& operator*=(const Interval& x);

    /**
     * \brief (*this)*=m.
     */
    Affine2MainMatrix& operator*=(const Matrix& m);

    /**
     * \brief (*this)*=m.
     */
    Affine2MainMatrix& operator*=(const Affine2MainMatrix& m);
    Affine2MainMatrix& operator*=(const IntervalMatrix& m);


};

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief Return the intersection of x and y.
 */
template<class T>
IntervalMatrix operator&(const Affine2MainMatrix<T>& x, const Affine2MainMatrix<T>& y);
template<class T>
IntervalMatrix operator&(const IntervalMatrix& x, const Affine2MainMatrix<T>& y);
template<class T>
IntervalMatrix operator&(const Affine2MainMatrix<T>& x, const IntervalMatrix& y);

/**
 * \brief Return the hull of x & y.
 */
template<class T>
IntervalMatrix operator|(const Affine2MainMatrix<T>& x, const Affine2MainMatrix<T>& y);
template<class T>
IntervalMatrix operator|(const IntervalMatrix& x, const Affine2MainMatrix<T>& y);
template<class T>
IntervalMatrix operator|(const Affine2MainMatrix<T>& x, const IntervalMatrix& y);

/**
 * \brief Return -m.
 */
template<class T>
Affine2MainMatrix<T> operator-(const Affine2MainMatrix<T>& m);

/**
 * \brief $[m]_1+[m]_2$.
 */
template<class T>
Affine2MainMatrix<T> operator+(const Affine2MainMatrix<T>& m1, const Matrix& m2);

/**
 * \brief $[m]_1+[m]_2$.
 */
template<class T>
Affine2MainMatrix<T> operator+(const Matrix& m1, const Affine2MainMatrix<T>& m2);

/**
 * \brief $[m]_1+[m]_2$.
 */
template<class T>
Affine2MainMatrix<T> operator+(const Affine2MainMatrix<T>& m1, const Affine2MainMatrix<T>& m2);
template<class T>
Affine2MainMatrix<T> operator+(const Affine2MainMatrix<T>& m1, const IntervalMatrix& m2);
template<class T>
Affine2MainMatrix<T> operator+(const IntervalMatrix& m1, const Affine2MainMatrix<T>& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
template<class T>
Affine2MainMatrix<T> operator-(const Affine2MainMatrix<T>& m1, const Matrix& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
template<class T>
Affine2MainMatrix<T> operator-(const Matrix& m1, const Affine2MainMatrix<T>& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
template<class T>
Affine2MainMatrix<T> operator-(const Affine2MainMatrix<T>& m1, const Affine2MainMatrix<T>& m2);
template<class T>
Affine2MainMatrix<T> operator-(const Affine2MainMatrix<T>& m1, const IntervalMatrix& m2);
template<class T>
Affine2MainMatrix<T> operator-(const IntervalMatrix& m1, const Affine2MainMatrix<T>& m2);

/**
 * \brief Scalar multiplication of a matrix.
 */
template<class T>
Affine2MainMatrix<T> operator*(double d, const Affine2MainMatrix<T>& m);

/**
 * \brief Scalar multiplication of a matrix.
 */
template<class T>
Affine2MainMatrix<T> operator*(const Affine2Main<T>& x, const Matrix& m);

/**
 * \brief Scalar multiplication of a matrix.
 */
template<class T>
Affine2MainMatrix<T> operator*(const Affine2Main<T>& x, const Affine2MainMatrix<T>& m);
template<class T>
Affine2MainMatrix<T> operator*(const Interval& x, const Affine2MainMatrix<T>& m);
template<class T>
Affine2MainMatrix<T> operator*(const Affine2Main<T>& x, const IntervalMatrix& m);


/*
 * \brief $[m]*[x]$.
 */
template<class T>
Affine2MainVector<T> operator*(const Affine2MainMatrix<T>& m, const Vector& x);

/**
 * \brief $[m]*[x]$.
 */
template<class T>
Affine2MainVector<T> operator*(const Matrix& m, const Affine2MainVector<T>& x);

/**
 * \brief $[m]*[x]$.
 */
template<class T>
Affine2MainVector<T> operator*(const Affine2MainMatrix<T>& m, const Affine2MainVector<T>& x);
template<class T>
Affine2MainVector<T> operator*(const Affine2MainMatrix<T>& m, const IntervalVector& x);
template<class T>
Affine2MainVector<T> operator*(const IntervalMatrix& m, const Affine2MainVector<T>& x);

/*
 * \brief $[x]*[m]$.
 */
template<class T>
Affine2MainVector<T> operator*(const Vector& x, const Affine2MainMatrix<T>& m);

/**
 * \brief $[x]*[m]$.
 */
template<class T>
Affine2MainVector<T> operator*(const Affine2MainVector<T>& x, const Matrix& m);

/**
 * \brief $[x]*[m]$.
 */
template<class T>
Affine2MainVector<T> operator*(const Affine2MainVector<T>& x, const Affine2MainMatrix<T>& m);
template<class T>
Affine2MainVector<T> operator*(const Affine2MainVector<T>& x, const IntervalMatrix& m);
template<class T>
Affine2MainVector<T> operator*(const IntervalVector& x, const Affine2MainMatrix<T>& m);

/**
 * \brief $[m]_1*[m]_2$.
 */
template<class T>
Affine2MainMatrix<T> operator*(const Affine2MainMatrix<T>& m1, const Matrix& m2);

/**
 * \brief $[m]_1*[m]_2$.
 */
template<class T>
Affine2MainMatrix<T> operator*(const Matrix& m1, const Affine2MainMatrix<T>& m2);

/**
 * \brief $[m]_1*[m]_2$.
 */
template<class T>
Affine2MainMatrix<T> operator*(const Affine2MainMatrix<T>& m1, const Affine2MainMatrix<T>& m2);
template<class T>
Affine2MainMatrix<T> operator*(const Affine2MainMatrix<T>& m1, const IntervalMatrix& m2);
template<class T>
Affine2MainMatrix<T> operator*(const IntervalMatrix& m1, const Affine2MainMatrix<T>& m2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
template<class T>
Affine2MainMatrix<T> outer_product(const Affine2MainVector<T>& x1, const Vector& x2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
template<class T>
Affine2MainMatrix<T> outer_product(const Vector& x1, const Affine2MainVector<T>& x2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
template<class T>
Affine2MainMatrix<T> outer_product(const Affine2MainVector<T>& x1, const Affine2MainVector<T>& x2);
template<class T>
Affine2MainMatrix<T> outer_product(const Affine2MainVector<T>& x1, const IntervalVector& x2);
template<class T>
Affine2MainMatrix<T> outer_product(const IntervalVector& x1, const Affine2MainVector<T>& x2);

/**
 * \brief |x|.
 */
template<class T>
Affine2MainMatrix<T> abs(const Affine2MainMatrix<T>& m);


/**
 * \brief Stream out a matrix.
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const Affine2MainMatrix<T>&);

/*@}*/



/*================================== inline implementations ========================================*/

// the following functions are
// introduced to allow genericity
template<class T> inline bool ___is_empty(const Affine2MainMatrix<T>& m) { return m.is_empty(); }
template<class T> inline void ___set_empty(Affine2MainMatrix<T>& m)      { m.set_empty(); }

} // end namespace ibex

#include "ibex_LinearArith.h_"
#include "ibex_Affine2Matrix.h_"

namespace ibex {

template<class T>
inline Affine2MainMatrix<T> Affine2MainMatrix<T>::empty(int m, int n) {
	Affine2MainMatrix<T>  res(m, n);
	res.set_empty();
	return res;
}

template<class T>
inline bool Affine2MainMatrix<T>::operator!=(const Affine2MainMatrix<T>& m) const {
	return !(*this==m);
}
template<class T>
inline bool Affine2MainMatrix<T>::operator!=(const IntervalMatrix& m) const {
	return !(*this==m);
}

template<class T>
inline int Affine2MainMatrix<T>::nb_cols() const {
	return _nb_cols;
}

template<class T>
inline int Affine2MainMatrix<T>::nb_rows() const {
	return _nb_rows;
}

template<class T>
inline Affine2MainVector<T>& Affine2MainMatrix<T>::operator[](int i) {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

template<class T>
inline const Affine2MainVector<T>& Affine2MainMatrix<T>::operator[](int i) const {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

template<class T>
inline void Affine2MainMatrix<T>::clear() {
	init(0);
}

template<class T>
inline void Affine2MainMatrix<T>::set_empty() {
	(*this)[0].set_empty();
}

template<class T>
inline Affine2MainVector<T>& Affine2MainMatrix<T>::row(int i) {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

template<class T>
inline const Affine2MainVector<T>& Affine2MainMatrix<T>::row(int i) const {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

template<class T>
inline Affine2MainMatrix<T> Affine2MainMatrix<T>::rows(int start_index, int end_index) {
	return submatrix(start_index, end_index, 0, nb_cols()-1);
}

template<class T>
inline Affine2MainMatrix<T> Affine2MainMatrix<T>::cols(int start_index, int end_index) {
	return submatrix(0, nb_rows()-1, start_index, end_index);
}

template<class T>
inline void Affine2MainMatrix<T>::set_row(int row1, const Affine2MainVector<T>& v1) {
	assert(row1>=0 && row1<nb_rows());
	assert(nb_cols()==v1.size());
	_M[row1]=v1;
}


template<class T>
inline bool Affine2MainMatrix<T>::is_empty() const {
	return (*this)[0].is_empty();
}



template<class T>
inline IntervalMatrix operator|(const IntervalMatrix& x, const Affine2MainMatrix<T>& y) {
 	 return   (y | x );
}

template<class T>
inline IntervalMatrix operator&(const IntervalMatrix& x, const Affine2MainMatrix<T>& y) {
 	 return   (y & x);
}


template<class T>
inline Affine2MainMatrix<T> operator+(const Affine2MainMatrix<T>& m1, const Matrix& m2) {
	return Affine2MainMatrix<T>(m1)+=m2;
}

template<class T>
inline Affine2MainMatrix<T> operator+(const Matrix& m1, const Affine2MainMatrix<T>& m2) {
	 return Affine2MainMatrix<T>(m2)+=m1;
}

template<class T>
inline Affine2MainMatrix<T> operator+(const Affine2MainMatrix<T>& m1, const Affine2MainMatrix<T>& m2){
 	 return Affine2MainMatrix<T>(m1)+=m2;
}
template<class T>
inline Affine2MainMatrix<T> operator+(const Affine2MainMatrix<T>& m1, const IntervalMatrix& m2){
 	 return Affine2MainMatrix<T>(m1)+=m2;
}
template<class T>
inline Affine2MainMatrix<T> operator+(const IntervalMatrix& m1, const Affine2MainMatrix<T>& m2){
 	 return Affine2MainMatrix<T>(m2)+=m1;
}

template<class T>
inline Affine2MainMatrix<T> operator-(const Matrix& m1, const Affine2MainMatrix<T>& m2){
	 Affine2MainMatrix<T> res(m2.nb_rows(),m2.nb_cols());
	 res = (-m2);
 	 return res+=m1;
}

template<class T>
inline Affine2MainMatrix<T> operator-(const Affine2MainMatrix<T>& m1, const Affine2MainMatrix<T>& m2){
	 return Affine2MainMatrix<T>(m1) += (-m2);
}
template<class T>
inline Affine2MainMatrix<T> operator-(const Affine2MainMatrix<T>& m1, const IntervalMatrix& m2){
 	 return Affine2MainMatrix<T>(m1)-=m2;
}
template<class T>
inline Affine2MainMatrix<T> operator-(const IntervalMatrix& m1, const Affine2MainMatrix<T>& m2){
	 Affine2MainMatrix<T> res(m2.nb_rows(),m2.nb_cols());
	 res = (-m2);
	 return res+=m1;
}

template<class T>
inline Affine2MainMatrix<T> operator-(const Affine2MainMatrix<T>& m1, const Matrix& m2) {
	return Affine2MainMatrix<T>(m1)-=m2;
}

template<class T>
inline Affine2MainMatrix<T> operator*(double d, const Affine2MainMatrix<T>& m){
 	 return Affine2MainMatrix<T>(m)*=d;
}

template<class T>
inline Affine2MainMatrix<T> operator*(const Affine2Main<T>& x, const Affine2MainMatrix<T>& m){
 	 return Affine2MainMatrix<T>(m)*=x;
}
template<class T>
inline Affine2MainMatrix<T> operator*(const Interval& x, const Affine2MainMatrix<T>& m){
 	 return Affine2MainMatrix<T>(m)*=x;
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator*=(const Matrix& m) {
	return (*this)=(*this)*m;
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator*=(const Affine2MainMatrix<T>& m) {
	return (*this)=(*this)*m;
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator*=(const IntervalMatrix& m) {
	return (*this)=(*this)*m;
}



template<class T>
inline Affine2MainMatrix<T> operator-(const Affine2MainMatrix<T>& m) {
	return minusM(m);
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator+=(const Matrix& m) {
	return set_addM<Affine2MainMatrix<T>,Matrix>(*this,m);
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator+=(const Affine2MainMatrix<T>& m) {
	return set_addM<Affine2MainMatrix<T>,Affine2MainMatrix<T> >(*this,m);
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator+=(const IntervalMatrix& m) {
	return set_addM<Affine2MainMatrix<T>,IntervalMatrix>(*this,m);
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator-=(const Matrix& m) {
	return set_subM<Affine2MainMatrix<T>,Matrix>(*this,m);
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator-=(const Affine2MainMatrix<T>& m) {
	return set_subM<Affine2MainMatrix<T>,Affine2MainMatrix<T> >(*this,m);
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator-=(const IntervalMatrix& m) {
	return set_subM<Affine2MainMatrix<T>,IntervalMatrix>(*this,m);
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator*=(double x) {
	return set_mulSM<double,Affine2MainMatrix<T> >(x,*this);
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator*=(const Affine2Main<T>& x) {
	return set_mulSM<Affine2Main<T>,Affine2MainMatrix<T> >(x,*this);
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrix<T>::operator*=(const Interval& x) {
	return set_mulSM<Interval,Affine2MainMatrix<T> >(x,*this);
}

template<class T>
inline Affine2MainVector<T> operator*(const Affine2MainMatrix<T>& m, const Vector& x) {
	return mulMV<Affine2MainMatrix<T>,Vector,Affine2MainVector<T> >(m,x);
}

template<class T>
inline Affine2MainVector<T> operator*(const Matrix& m, const Affine2MainVector<T>& x) {
	return mulMV<Matrix,Affine2MainVector<T>,Affine2MainVector<T> >(m,x);
}

template<class T>
inline Affine2MainVector<T> operator*(const Affine2MainMatrix<T>& m, const Affine2MainVector<T>& x) {
	return mulMV<Affine2MainMatrix<T>,Affine2MainVector<T>,Affine2MainVector<T> >(m,x);
}

template<class T>
inline Affine2MainVector<T> operator*(const Affine2MainMatrix<T>& m, const IntervalVector& x) {
	return mulMV<Affine2MainMatrix<T>,IntervalVector,Affine2MainVector<T> >(m,x);
}

template<class T>
inline Affine2MainVector<T> operator*(const Vector& x, const Affine2MainMatrix<T>& m) {
	return mulVM<Vector,Affine2MainMatrix<T>,Affine2MainVector<T> >(x,m);
}

template<class T>
inline Affine2MainVector<T> operator*(const Affine2MainVector<T>& x, const Affine2MainMatrix<T>& m) {
	return mulVM<Affine2MainVector<T>,Affine2MainMatrix<T>,Affine2MainVector<T> >(x,m);
}

template<class T>
inline Affine2MainVector<T> operator*(const IntervalVector& x, const Affine2MainMatrix<T>& m) {
	return mulVM<IntervalVector,Affine2MainMatrix<T>,Affine2MainVector<T> >(x,m);
}

template<class T>
inline Affine2MainMatrix<T> operator*(const Affine2MainMatrix<T>& m1, const Matrix& m2) {
	return mulMM<Affine2MainMatrix<T>,Matrix,Affine2MainMatrix<T> >(m1,m2);
}

template<class T>
inline Affine2MainMatrix<T> operator*(const Matrix& m1, const Affine2MainMatrix<T>& m2) {
	return mulMM<Matrix,Affine2MainMatrix<T>,Affine2MainMatrix<T> >(m1,m2);
}

template<class T>
inline Affine2MainMatrix<T> operator*(const Affine2MainMatrix<T>& m1, const Affine2MainMatrix<T>& m2) {
	return mulMM<Affine2MainMatrix<T>,Affine2MainMatrix<T>,Affine2MainMatrix<T> >(m1,m2);
}

template<class T>
inline Affine2MainMatrix<T> operator*(const Affine2MainMatrix<T>& m1, const IntervalMatrix& m2) {
	return mulMM<Affine2MainMatrix<T>,IntervalMatrix,Affine2MainMatrix<T> >(m1,m2);
}

template<class T>
inline Affine2MainMatrix<T> operator*(const IntervalMatrix& m1, const Affine2MainMatrix<T>& m2) {
	return mulMM<IntervalMatrix,Affine2MainMatrix<T>,Affine2MainMatrix<T> >(m1,m2);
}

template<class T>
inline Affine2MainMatrix<T> outer_product(const Affine2MainVector<T>& x1, const Vector& x2) {
	return outer_prod<Affine2MainVector<T>,Vector,Affine2MainMatrix<T> >(x1,x2);
}

template<class T>
inline Affine2MainMatrix<T> outer_product(const Vector& x1, const Affine2MainVector<T>& x2) {
	return outer_prod<Vector,Affine2MainVector<T>,Affine2MainMatrix<T> >(x1,x2);
}

template<class T>
inline Affine2MainMatrix<T> outer_product(const Affine2MainVector<T>& x1, const Affine2MainVector<T>& x2) {
	return outer_prod<Affine2MainVector<T>,Affine2MainVector<T>,Affine2MainMatrix<T> >(x1,x2);
}

template<class T>
inline Affine2MainMatrix<T> outer_product(const Affine2MainVector<T>& x1, const IntervalVector& x2) {
	return outer_prod<Affine2MainVector<T>,IntervalVector,Affine2MainMatrix<T> >(x1,x2);
}

template<class T>
inline Affine2MainMatrix<T> outer_product(const IntervalVector& x1, const Affine2MainVector<T>& x2) {
	return outer_prod<IntervalVector,Affine2MainVector<T>,Affine2MainMatrix<T> >(x1,x2);
}

template<class T>
inline Affine2MainMatrix<T> abs(const Affine2MainMatrix<T>& m) {
	return absM(m);
}



} // namespace ibex
#endif // __IBEX_AFFINE2_MATRIX_H__
