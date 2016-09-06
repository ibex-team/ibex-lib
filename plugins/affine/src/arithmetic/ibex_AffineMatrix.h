/* ============================================================================
 * I B E X - Matrix of Affine forms
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 16, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE_MATRIX_H__
#define __IBEX_AFFINE_MATRIX_H__

#include "ibex_IntervalMatrix.h"
#include "ibex_Matrix.h"
#include <iostream>
#include "ibex_AffineVector.h"

namespace ibex {


/**
 * \ingroup arithmetic
 *
 * \brief Affine2 matrix.
 */


typedef AffineMainMatrix<AF_Default> Affine2Matrix;
typedef AffineMainMatrix<AF_Other> 	 Affine3Matrix;

template<class T=AF_Default>
class AffineMainMatrix {

private:

	AffineMainMatrix(); // forbidden

	int _nb_rows;
	int _nb_cols;
	AffineMainVector<T>* _M;

public:
	/**
	 * \brief Create a (nb_rows x nb_cols) matrix.
	 */
	AffineMainMatrix(int nb_rows, int nb_cols);

	/**
	 * \brief Create a (nb_rows x nb_cols) matrix and initialize
	 * all the elements with x.
	 */
	AffineMainMatrix(int nb_rows, int nb_cols, const AffineMain<T>& x);

	/** It is too difficult to know the size of each AF2. So we let the user do what exactly he want to do.
	 * \brief Create \a x.size Affine2MainVector<T> of dimension \a x.nb_col*x.nb_row with
	 * the [i][j] component initialized to \a Affine2(m[i][j]).
	 */
	explicit AffineMainMatrix(const IntervalMatrix& m);

	/**
	 * \brief Create  a copy of x
	 */
	AffineMainMatrix(const AffineMainMatrix& x);

	/**
	 * \brief Create a degenerated Affine2 matrix.
	 * the [i][j] component initialized to \a Affine2(m[i][j]).
	 */
	explicit AffineMainMatrix(const Matrix& m);

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
	AffineMainMatrix(int m, int n, double x[][2]);

	/**
	 * \brief Delete *this.
	 */
	~AffineMainMatrix();

	/**
	 * \brief Create a mxn empty matrix
	 *
	 * Create an empty Affine2MainMatrix of dimension \a n x \a m
	 * (all the components being empty Affine2s)
	 *
	 * \pre m>0, n>0
	 */
	static AffineMainMatrix empty(int m, int n);

	/**
	 * \brief Set *this to m.
	 */
	AffineMainMatrix& operator=(const AffineMainMatrix& x);
	AffineMainMatrix& operator=(const IntervalMatrix& x);


	/**
	 * \brief Set all the elements to 0 (even if empty).
	 */
	void clear();

	/**
	 * \brief Set all the elements to x (even if empty).
	 *
	 * \note Emptiness is "overridden".
	 */
	void init(const AffineMain<T>& x);
	void init(const Interval& x);

	/**
	 * \brief True if the bounds of (*this) coincide with m.
	 *
	 * If the two matrices are empty and with the same dimensions
	 * then they are considered as equal.
	 */
	bool operator==(const AffineMainMatrix& m) const;
	bool operator==(const IntervalMatrix& m) const;

	/**
	 * \brief True if one bound of (*this) differs from m.
	 */
	bool operator!=(const AffineMainMatrix& m) const;
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
	AffineMainVector<T>& operator[](int i);

	/**
	 * \brief Return a const reference to the ith row.
	 *
	 * Use (*this)[i][j] to get a reference to the element M(i,j).
	 */
	const AffineMainVector<T>& operator[](int i) const;

	/**
	 * \brief Set this Affine2MainMatrix to the empty Affine2MainMatrix
	 *
	 * The dimensions remain the same.
	 */
	void set_empty();

	/**
	 * \brief Return a submatrix.
	 */
	AffineMainMatrix submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index);

	/**
	 * \brief Transpose of *this.
	 */
	AffineMainMatrix transpose() const;

	/**
	 * \brief Return the ith row.

	 * Equivalent to (*this)[i.
	 */
	AffineMainVector<T>& row(int i);

	/**
	 * \brief Return a const reference to the ith row.

	 * Equivalent to (*this)[i.
	 */
	const AffineMainVector<T>& row(int i) const;

	/**
	 * \brief Return a column
	 */
	AffineMainVector<T> col(int i) const;

	/**
	 * \brief Return a subset of rows.
	 */
	AffineMainMatrix rows(int start_index, int end_index);

	/**
	 * \brief Return a subset of columns.
	 */
	AffineMainMatrix cols(int start_index, int end_index);

	/**
	 * \brief Set a row.
	 */
	void set_row(int row, const AffineMainVector<T>& v);
	void set_row(int row, const IntervalVector& v);


	/**
	 * \brief Set a column.
	 */
	void set_col(int col, const AffineMainVector<T>& v);


    /**
     * \brief (*this)+=m.
     */
    AffineMainMatrix& operator+=(const Matrix& m);

    /**
     * \brief (*this)+=m.
     */
    AffineMainMatrix& operator+=(const AffineMainMatrix& m);
    AffineMainMatrix& operator+=(const IntervalMatrix& m);

    /**
     * \brief (*this)-=m.
     */
    AffineMainMatrix& operator-=(const Matrix& m);

    /**Affine2
     * \brief (*this)-=m.
     */
    AffineMainMatrix& operator-=(const AffineMainMatrix& m);
    AffineMainMatrix& operator-=(const IntervalMatrix& m);

	/**
	 * \brief Add [-rad,+rad] to all the components of *this.
	 *
	 * \return *this.
	 */
	AffineMainMatrix& inflate(double rad);

    /**
     * \brief (*this)=x * *(this).
     */
    AffineMainMatrix& operator*=(double x);

    /**
     * \brief (*this)=x * *(this).
     */
    AffineMainMatrix& operator*=(const AffineMain<T>& x);
    AffineMainMatrix& operator*=(const Interval& x);

    /**
     * \brief (*this)*=m.
     */
    AffineMainMatrix& operator*=(const Matrix& m);

    /**
     * \brief (*this)*=m.
     */
    AffineMainMatrix& operator*=(const AffineMainMatrix& m);
    AffineMainMatrix& operator*=(const IntervalMatrix& m);


};

/** \ingroup arithmetic */
/*@{*/

/**
 * \brief Return the intersection of x and y.
 */
template<class T>
IntervalMatrix operator&(const AffineMainMatrix<T>& x, const AffineMainMatrix<T>& y);
template<class T>
IntervalMatrix operator&(const IntervalMatrix& x, const AffineMainMatrix<T>& y);
template<class T>
IntervalMatrix operator&(const AffineMainMatrix<T>& x, const IntervalMatrix& y);

/**
 * \brief Return the hull of x & y.
 */
template<class T>
IntervalMatrix operator|(const AffineMainMatrix<T>& x, const AffineMainMatrix<T>& y);
template<class T>
IntervalMatrix operator|(const IntervalMatrix& x, const AffineMainMatrix<T>& y);
template<class T>
IntervalMatrix operator|(const AffineMainMatrix<T>& x, const IntervalMatrix& y);

/**
 * \brief Return -m.
 */
template<class T>
AffineMainMatrix<T> operator-(const AffineMainMatrix<T>& m);

/**
 * \brief $[m]_1+[m]_2$.
 */
template<class T>
AffineMainMatrix<T> operator+(const AffineMainMatrix<T>& m1, const Matrix& m2);

/**
 * \brief $[m]_1+[m]_2$.
 */
template<class T>
AffineMainMatrix<T> operator+(const Matrix& m1, const AffineMainMatrix<T>& m2);

/**
 * \brief $[m]_1+[m]_2$.
 */
template<class T>
AffineMainMatrix<T> operator+(const AffineMainMatrix<T>& m1, const AffineMainMatrix<T>& m2);
template<class T>
AffineMainMatrix<T> operator+(const AffineMainMatrix<T>& m1, const IntervalMatrix& m2);
template<class T>
AffineMainMatrix<T> operator+(const IntervalMatrix& m1, const AffineMainMatrix<T>& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
template<class T>
AffineMainMatrix<T> operator-(const AffineMainMatrix<T>& m1, const Matrix& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
template<class T>
AffineMainMatrix<T> operator-(const Matrix& m1, const AffineMainMatrix<T>& m2);

/**
 * \brief $[m]_1-[m]_2$.
 */
template<class T>
AffineMainMatrix<T> operator-(const AffineMainMatrix<T>& m1, const AffineMainMatrix<T>& m2);
template<class T>
AffineMainMatrix<T> operator-(const AffineMainMatrix<T>& m1, const IntervalMatrix& m2);
template<class T>
AffineMainMatrix<T> operator-(const IntervalMatrix& m1, const AffineMainMatrix<T>& m2);

/**
 * \brief Scalar multiplication of a matrix.
 */
template<class T>
AffineMainMatrix<T> operator*(double d, const AffineMainMatrix<T>& m);

/**
 * \brief Scalar multiplication of a matrix.
 */
template<class T>
AffineMainMatrix<T> operator*(const AffineMain<T>& x, const Matrix& m);

/**
 * \brief Scalar multiplication of a matrix.
 */
template<class T>
AffineMainMatrix<T> operator*(const AffineMain<T>& x, const AffineMainMatrix<T>& m);
template<class T>
AffineMainMatrix<T> operator*(const Interval& x, const AffineMainMatrix<T>& m);
template<class T>
AffineMainMatrix<T> operator*(const AffineMain<T>& x, const IntervalMatrix& m);


/*
 * \brief $[m]*[x]$.
 */
template<class T>
AffineMainVector<T> operator*(const AffineMainMatrix<T>& m, const Vector& x);

/**
 * \brief $[m]*[x]$.
 */
template<class T>
AffineMainVector<T> operator*(const Matrix& m, const AffineMainVector<T>& x);

/**
 * \brief $[m]*[x]$.
 */
template<class T>
AffineMainVector<T> operator*(const AffineMainMatrix<T>& m, const AffineMainVector<T>& x);
template<class T>
AffineMainVector<T> operator*(const AffineMainMatrix<T>& m, const IntervalVector& x);
template<class T>
AffineMainVector<T> operator*(const IntervalMatrix& m, const AffineMainVector<T>& x);

/*
 * \brief $[x]*[m]$.
 */
template<class T>
AffineMainVector<T> operator*(const Vector& x, const AffineMainMatrix<T>& m);

/**
 * \brief $[x]*[m]$.
 */
template<class T>
AffineMainVector<T> operator*(const AffineMainVector<T>& x, const Matrix& m);

/**
 * \brief $[x]*[m]$.
 */
template<class T>
AffineMainVector<T> operator*(const AffineMainVector<T>& x, const AffineMainMatrix<T>& m);
template<class T>
AffineMainVector<T> operator*(const AffineMainVector<T>& x, const IntervalMatrix& m);
template<class T>
AffineMainVector<T> operator*(const IntervalVector& x, const AffineMainMatrix<T>& m);

/**
 * \brief $[m]_1*[m]_2$.
 */
template<class T>
AffineMainMatrix<T> operator*(const AffineMainMatrix<T>& m1, const Matrix& m2);

/**
 * \brief $[m]_1*[m]_2$.
 */
template<class T>
AffineMainMatrix<T> operator*(const Matrix& m1, const AffineMainMatrix<T>& m2);

/**
 * \brief $[m]_1*[m]_2$.
 */
template<class T>
AffineMainMatrix<T> operator*(const AffineMainMatrix<T>& m1, const AffineMainMatrix<T>& m2);
template<class T>
AffineMainMatrix<T> operator*(const AffineMainMatrix<T>& m1, const IntervalMatrix& m2);
template<class T>
AffineMainMatrix<T> operator*(const IntervalMatrix& m1, const AffineMainMatrix<T>& m2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
template<class T>
AffineMainMatrix<T> outer_product(const AffineMainVector<T>& x1, const Vector& x2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
template<class T>
AffineMainMatrix<T> outer_product(const Vector& x1, const AffineMainVector<T>& x2);

/**
 * \brief Outer product (multiplication of a column vector by a row vector).
 */
template<class T>
AffineMainMatrix<T> outer_product(const AffineMainVector<T>& x1, const AffineMainVector<T>& x2);
template<class T>
AffineMainMatrix<T> outer_product(const AffineMainVector<T>& x1, const IntervalVector& x2);
template<class T>
AffineMainMatrix<T> outer_product(const IntervalVector& x1, const AffineMainVector<T>& x2);

/**
 * \brief |x|.
 */
template<class T>
AffineMainMatrix<T> abs(const AffineMainMatrix<T>& m);


/**
 * \brief Stream out a matrix.
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const AffineMainMatrix<T>&);




/*@}*/



/*================================== inline implementations ========================================*/

// the following functions are
// introduced to allow genericity
template<class T> inline bool ___is_empty(const AffineMainMatrix<T>& m) { return m.is_empty(); }
template<class T> inline void ___set_empty(AffineMainMatrix<T>& m)      { m.set_empty(); }

} // end namespace ibex

#include "ibex_LinearArith.h"
#include "ibex_AffineMatrix.h_"

namespace ibex {

template<class T>
inline AffineMainMatrix<T> AffineMainMatrix<T>::empty(int m, int n) {
	AffineMainMatrix<T>  res(m, n);
	res.set_empty();
	return res;
}

template<class T>
inline bool AffineMainMatrix<T>::operator!=(const AffineMainMatrix<T>& m) const {
	return !(*this==m);
}
template<class T>
inline bool AffineMainMatrix<T>::operator!=(const IntervalMatrix& m) const {
	return !(*this==m);
}

template<class T>
inline int AffineMainMatrix<T>::nb_cols() const {
	return _nb_cols;
}

template<class T>
inline int AffineMainMatrix<T>::nb_rows() const {
	return _nb_rows;
}

template<class T>
inline AffineMainVector<T>& AffineMainMatrix<T>::operator[](int i) {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

template<class T>
inline const AffineMainVector<T>& AffineMainMatrix<T>::operator[](int i) const {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

template<class T>
inline void AffineMainMatrix<T>::clear() {
	init(Interval::ZERO);
}

template<class T>
inline void AffineMainMatrix<T>::set_empty() {
	(*this)[0].set_empty();
}

template<class T>
inline AffineMainVector<T>& AffineMainMatrix<T>::row(int i) {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

template<class T>
inline const AffineMainVector<T>& AffineMainMatrix<T>::row(int i) const {
	assert(i>=0 && i<nb_rows());
	return _M[i];
}

template<class T>
inline AffineMainMatrix<T> AffineMainMatrix<T>::rows(int start_index, int end_index) {
	return submatrix(start_index, end_index, 0, nb_cols()-1);
}

template<class T>
inline AffineMainMatrix<T> AffineMainMatrix<T>::cols(int start_index, int end_index) {
	return submatrix(0, nb_rows()-1, start_index, end_index);
}

template<class T>
inline void AffineMainMatrix<T>::set_row(int row1, const AffineMainVector<T>& v1) {
	assert(row1>=0 && row1<nb_rows());
	assert(nb_cols()==v1.size());
	_M[row1]=v1;
}

template<class T>
inline void AffineMainMatrix<T>::set_row(int row1, const IntervalVector& v1) {
	assert(row1>=0 && row1<nb_rows());
	assert(nb_cols()==v1.size());
	_M[row1]=v1;
}


template<class T>
inline bool AffineMainMatrix<T>::is_empty() const {
	return (*this)[0].is_empty();
}



template<class T>
inline IntervalMatrix operator|(const IntervalMatrix& x, const AffineMainMatrix<T>& y) {
 	 return   (y | x );
}

template<class T>
inline IntervalMatrix operator&(const IntervalMatrix& x, const AffineMainMatrix<T>& y) {
 	 return   (y & x);
}


template<class T>
inline AffineMainMatrix<T> operator+(const AffineMainMatrix<T>& m1, const Matrix& m2) {
	return AffineMainMatrix<T>(m1)+=m2;
}

template<class T>
inline AffineMainMatrix<T> operator+(const Matrix& m1, const AffineMainMatrix<T>& m2) {
	 return AffineMainMatrix<T>(m2)+=m1;
}

template<class T>
inline AffineMainMatrix<T> operator+(const AffineMainMatrix<T>& m1, const AffineMainMatrix<T>& m2){
 	 return AffineMainMatrix<T>(m1)+=m2;
}
template<class T>
inline AffineMainMatrix<T> operator+(const AffineMainMatrix<T>& m1, const IntervalMatrix& m2){
 	 return AffineMainMatrix<T>(m1)+=m2;
}
template<class T>
inline AffineMainMatrix<T> operator+(const IntervalMatrix& m1, const AffineMainMatrix<T>& m2){
 	 return AffineMainMatrix<T>(m2)+=m1;
}

template<class T>
inline AffineMainMatrix<T> operator-(const Matrix& m1, const AffineMainMatrix<T>& m2){
	 AffineMainMatrix<T> res(m2.nb_rows(),m2.nb_cols());
	 res = (-m2);
 	 return res+=m1;
}

template<class T>
inline AffineMainMatrix<T> operator-(const AffineMainMatrix<T>& m1, const AffineMainMatrix<T>& m2){
	 return AffineMainMatrix<T>(m1) += (-m2);
}
template<class T>
inline AffineMainMatrix<T> operator-(const AffineMainMatrix<T>& m1, const IntervalMatrix& m2){
 	 return AffineMainMatrix<T>(m1)-=m2;
}
template<class T>
inline AffineMainMatrix<T> operator-(const IntervalMatrix& m1, const AffineMainMatrix<T>& m2){
	 AffineMainMatrix<T> res(m2.nb_rows(),m2.nb_cols());
	 res = (-m2);
	 return res+=m1;
}

template<class T>
inline AffineMainMatrix<T> operator-(const AffineMainMatrix<T>& m1, const Matrix& m2) {
	return AffineMainMatrix<T>(m1)-=m2;
}

template<class T>
inline AffineMainMatrix<T> operator*(double d, const AffineMainMatrix<T>& m){
 	 return AffineMainMatrix<T>(m)*=d;
}

template<class T>
inline AffineMainMatrix<T> operator*(const AffineMain<T>& x, const AffineMainMatrix<T>& m){
 	 return AffineMainMatrix<T>(m)*=x;
}
template<class T>
inline AffineMainMatrix<T> operator*(const Interval& x, const AffineMainMatrix<T>& m){
 	 return AffineMainMatrix<T>(m)*=x;
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator*=(const Matrix& m) {
	return (*this)=(*this)*m;
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator*=(const AffineMainMatrix<T>& m) {
	return (*this)=(*this)*m;
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator*=(const IntervalMatrix& m) {
	return (*this)=(*this)*m;
}



template<class T>
inline AffineMainMatrix<T> operator-(const AffineMainMatrix<T>& m) {
	return minusM(m);
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator+=(const Matrix& m) {
	return set_addM<AffineMainMatrix<T>,Matrix>(*this,m);
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator+=(const AffineMainMatrix<T>& m) {
	return set_addM<AffineMainMatrix<T>,AffineMainMatrix<T> >(*this,m);
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator+=(const IntervalMatrix& m) {
	return set_addM<AffineMainMatrix<T>,IntervalMatrix>(*this,m);
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator-=(const Matrix& m) {
	return set_subM<AffineMainMatrix<T>,Matrix>(*this,m);
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator-=(const AffineMainMatrix<T>& m) {
	return set_subM<AffineMainMatrix<T>,AffineMainMatrix<T> >(*this,m);
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator-=(const IntervalMatrix& m) {
	return set_subM<AffineMainMatrix<T>,IntervalMatrix>(*this,m);
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator*=(double x) {
	return set_mulSM<double,AffineMainMatrix<T> >(x,*this);
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator*=(const AffineMain<T>& x) {
	return set_mulSM<AffineMain<T>,AffineMainMatrix<T> >(x,*this);
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrix<T>::operator*=(const Interval& x) {
	return set_mulSM<Interval,AffineMainMatrix<T> >(x,*this);
}

template<class T>
inline AffineMainVector<T> operator*(const AffineMainMatrix<T>& m, const Vector& x) {
	return mulMV<AffineMainMatrix<T>,Vector,AffineMainVector<T> >(m,x);
}

template<class T>
inline AffineMainVector<T> operator*(const Matrix& m, const AffineMainVector<T>& x) {
	return mulMV<Matrix,AffineMainVector<T>,AffineMainVector<T> >(m,x);
}

template<class T>
inline AffineMainVector<T> operator*(const AffineMainMatrix<T>& m, const AffineMainVector<T>& x) {
	return mulMV<AffineMainMatrix<T>,AffineMainVector<T>,AffineMainVector<T> >(m,x);
}

template<class T>
inline AffineMainVector<T> operator*(const AffineMainMatrix<T>& m, const IntervalVector& x) {
	return mulMV<AffineMainMatrix<T>,IntervalVector,AffineMainVector<T> >(m,x);
}

template<class T>
inline AffineMainVector<T> operator*(const Vector& x, const AffineMainMatrix<T>& m) {
	return mulVM<Vector,AffineMainMatrix<T>,AffineMainVector<T> >(x,m);
}

template<class T>
inline AffineMainVector<T> operator*(const AffineMainVector<T>& x, const AffineMainMatrix<T>& m) {
	return mulVM<AffineMainVector<T>,AffineMainMatrix<T>,AffineMainVector<T> >(x,m);
}

template<class T>
inline AffineMainVector<T> operator*(const IntervalVector& x, const AffineMainMatrix<T>& m) {
	return mulVM<IntervalVector,AffineMainMatrix<T>,AffineMainVector<T> >(x,m);
}

template<class T>
inline AffineMainMatrix<T> operator*(const AffineMainMatrix<T>& m1, const Matrix& m2) {
	return mulMM<AffineMainMatrix<T>,Matrix,AffineMainMatrix<T> >(m1,m2);
}

template<class T>
inline AffineMainMatrix<T> operator*(const Matrix& m1, const AffineMainMatrix<T>& m2) {
	return mulMM<Matrix,AffineMainMatrix<T>,AffineMainMatrix<T> >(m1,m2);
}

template<class T>
inline AffineMainMatrix<T> operator*(const AffineMainMatrix<T>& m1, const AffineMainMatrix<T>& m2) {
	return mulMM<AffineMainMatrix<T>,AffineMainMatrix<T>,AffineMainMatrix<T> >(m1,m2);
}

template<class T>
inline AffineMainMatrix<T> operator*(const AffineMainMatrix<T>& m1, const IntervalMatrix& m2) {
	return mulMM<AffineMainMatrix<T>,IntervalMatrix,AffineMainMatrix<T> >(m1,m2);
}

template<class T>
inline AffineMainMatrix<T> operator*(const IntervalMatrix& m1, const AffineMainMatrix<T>& m2) {
	return mulMM<IntervalMatrix,AffineMainMatrix<T>,AffineMainMatrix<T> >(m1,m2);
}

template<class T>
inline AffineMainMatrix<T> outer_product(const AffineMainVector<T>& x1, const Vector& x2) {
	return outer_prod<AffineMainVector<T>,Vector,AffineMainMatrix<T> >(x1,x2);
}

template<class T>
inline AffineMainMatrix<T> outer_product(const Vector& x1, const AffineMainVector<T>& x2) {
	return outer_prod<Vector,AffineMainVector<T>,AffineMainMatrix<T> >(x1,x2);
}

template<class T>
inline AffineMainMatrix<T> outer_product(const AffineMainVector<T>& x1, const AffineMainVector<T>& x2) {
	return outer_prod<AffineMainVector<T>,AffineMainVector<T>,AffineMainMatrix<T> >(x1,x2);
}

template<class T>
inline AffineMainMatrix<T> outer_product(const AffineMainVector<T>& x1, const IntervalVector& x2) {
	return outer_prod<AffineMainVector<T>,IntervalVector,AffineMainMatrix<T> >(x1,x2);
}

template<class T>
inline AffineMainMatrix<T> outer_product(const IntervalVector& x1, const AffineMainVector<T>& x2) {
	return outer_prod<IntervalVector,AffineMainVector<T>,AffineMainMatrix<T> >(x1,x2);
}

template<class T>
inline AffineMainMatrix<T> abs(const AffineMainMatrix<T>& m) {
	return absM(m);
}



} // namespace ibex
#endif // __IBEX_AFFINE_MATRIX_H__
