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

#include "ibex_TemplateMatrix.h"
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

namespace ibex {

template<class T>
AffineMainMatrix<T>::AffineMainMatrix() : _nb_rows(0), _nb_cols(0), _M(NULL) {

}

template<class T>
AffineMainMatrix<T>::AffineMainMatrix(int nb_rows1, int nb_cols1) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

	_M = new AffineMainVector<T>[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
	}
}

template<class T>
AffineMainMatrix<T>::AffineMainMatrix(int nb_rows1, int nb_cols1, const AffineMain<T>& x) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

	_M = new AffineMainVector<T>[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=x;
	}
}



template<class T>
AffineMainMatrix<T>::AffineMainMatrix(int m, int n, double bounds[][2]) : _nb_rows(m), _nb_cols(n) {
	assert(m>0);
	assert(n>0);
		int k=0; // counter for "bounds"
		_M = new AffineMainVector<T>[_nb_rows];
		for (int i=0; i<_nb_rows; i++) {
			_M[i].resize(_nb_cols);
			for (int j=0; j<_nb_cols; j++) {
				_M[i]._vec[j]=AffineMain<T>(Interval(bounds[k][0],bounds[k][1]));
				k++;
			}
		}
}

template<class T>
AffineMainMatrix<T>::AffineMainMatrix(const AffineMainMatrix<T>& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new AffineMainVector<T>[_nb_rows];

	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=AffineMain<T>(m[i][j]);
	}

}


template<class T>
AffineMainMatrix<T>::AffineMainMatrix(const IntervalMatrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new AffineMainVector<T>[_nb_rows];
	int k=0; // counter for "bounds"
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++)  {
			_M[i]._vec[j]=AffineMain<T>(m[i][j]);
			k++;
		}
	}
}


template<class T>
AffineMainMatrix<T>::AffineMainMatrix(const Matrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new AffineMainVector<T>[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			_M[i]._vec[j]=AffineMain<T>(m[i][j]);
		}
	}
}

template<class T>
AffineMainMatrix<T>::~AffineMainMatrix() {
	if (_M!=NULL) delete[] _M;
}

template<class T>
AffineMainMatrix<T>& AffineMainMatrix<T>::operator=(const AffineMainMatrix<T>& x) {
	//resize(x.nb_rows(), x.nb_cols());
	return _assignM(*this,x);
}

template<class T>
AffineMainMatrix<T>& AffineMainMatrix<T>::operator=(const IntervalMatrix& x) {
	//resize(x.nb_rows(), x.nb_cols());
	return _assignM(*this,x);
}

template<class T>
IntervalMatrix AffineMainMatrix<T>::itv() const {
	assert(!is_empty());
	IntervalMatrix intm(nb_rows(),nb_cols());
	for (int i = 0; i < nb_rows(); i++) {
		for (int j =0; j < nb_cols();j++){
			intm[i][j] = (*this)[i][j].itv();
		}
	}
	return intm;
}


template<class T>
void AffineMainMatrix<T>::init(const AffineMain<T>& x) {
	for (int i=0; i<nb_rows(); i++)
		(*this)[i].init(x);
}

template<class T>
void AffineMainMatrix<T>::init(const Interval& x) {
	for (int i=0; i<nb_rows(); i++) {
		for (int j = 0; i < nb_cols(); i++) {
			(*this)[i][j] = AffineMain<T>(x);
		}
	}
}

template<class T>
bool AffineMainMatrix<T>::operator==(const AffineMainMatrix<T>& m) const {
	return _equalsM(*this,m);
}

template<class T>
bool AffineMainMatrix<T>::operator==(const IntervalMatrix& m) const {
	return _equalsM(*this,m);
}

template<class T>
void AffineMainMatrix<T>::resize(int nb_rows1, int nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);
	assert((_M==NULL && _nb_rows==0) || (_nb_rows!=0 && _M!=NULL));

	if (nb_rows1==_nb_rows && nb_cols1==_nb_cols) return;

	AffineMainVector<T>* M2;

	if (nb_rows1!=_nb_rows)
		M2 = new AffineMainVector<T>[nb_rows1];
	else
		M2 = _M;

	for (int i=0; i<nb_rows1; i++) {
		M2[i].resize(nb_cols1);  // ALL_REALS by default
		if (i<_nb_rows) {
			int min_cols=nb_cols1<_nb_cols?nb_cols1:_nb_cols;
			for (int j=0; j<min_cols; j++)
				M2[i][j]=_M[i][j];
		}
	}

	if (_M!=NULL && nb_rows1!=_nb_rows) delete[] _M;

	_M = M2;
	_nb_rows = nb_rows1;
	_nb_cols = nb_cols1;
}

template<class T>
AffineMainMatrix<T> AffineMainMatrix<T>::submatrix(int row_start_index, int row_end_index,	int col_start_index, int col_end_index) {
	return _submatrix(*this,row_start_index,row_end_index,col_start_index,col_end_index);
}

template<class T>
AffineMainMatrix<T> AffineMainMatrix<T>::transpose() const {
	return _transpose(*this);
}

template<class T>
AffineMainVector<T> AffineMainMatrix<T>::col(int j) const {
//	return _col<Affine2MainMatrix<T>,Affine2MainVector<T> >(*this,j);
	{
		assert(!is_empty());
		assert(j>=0 && j<nb_cols());

		AffineMainVector<T> res(nb_rows());
		for (int i=0; i<nb_rows(); i++)
			res[i]=_M[i][j];
		return res;
	}
}

template<class T>
void AffineMainMatrix<T>::set_col(int col1, const AffineMainVector<T>& v) {
	_set_col(*this,col1,v);
}

template<class T>
AffineMainMatrix<T>& AffineMainMatrix<T>::inflate(double rad) {
	// see comment in Affine2MainVector<T>::inflate
	if (is_empty())
		return *this;

	Interval r(-rad, rad);
	for (int i = 0; i < nb_rows(); i++)
		for (int j = 0; j < nb_cols(); j++)
			(*this)[i][j] += r;
	return *this;
}

template<class T>
IntervalMatrix operator&(const AffineMainMatrix<T>& x, const AffineMainMatrix<T>& y) {
	assert(x.nb_rows()==y.nb_rows());
	assert(x.nb_cols()==y.nb_cols());

	if (y.is_empty()||x.is_empty())
		return IntervalMatrix::empty(x.nb_rows(),x.nb_cols());

	IntervalMatrix res(x.nb_rows(),x.nb_cols());
	for (int i=0; i<x.nb_rows(); i++) {
		res [i] = x[i] & y[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}

template<class T>
IntervalMatrix operator&(const AffineMainMatrix<T>& x, const IntervalMatrix& y) {
	assert(x.nb_rows()==y.nb_rows());
	assert(x.nb_cols()==y.nb_cols());

	if (y.is_empty()||x.is_empty())
		return IntervalMatrix::empty(x.nb_rows(),x.nb_cols());

	IntervalMatrix res(x.nb_rows(),x.nb_cols());
	for (int i=0; i<x.nb_rows(); i++) {
		res [i] = x[i] & y[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}

template<class T>
IntervalMatrix operator|(const AffineMainMatrix<T>& x,	const AffineMainMatrix<T>& y) {
	assert(x.nb_rows()==y.nb_rows());
	assert(x.nb_cols()==y.nb_cols());

	if (y.is_empty()&&x.is_empty())
		return IntervalMatrix::empty(x.nb_rows(),x.nb_cols());

	IntervalMatrix res(x.nb_rows(),x.nb_cols());
	for (int i=0; i<x.nb_rows(); i++) {
		res [i] = x[i] | y[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}

template<class T>
IntervalMatrix operator|(const AffineMainMatrix<T>& x,	const IntervalMatrix& y) {
	assert(x.nb_rows()==y.nb_rows());
	assert(x.nb_cols()==y.nb_cols());

	if (y.is_empty()&&x.is_empty())
		return IntervalMatrix::empty(x.nb_rows(),x.nb_cols());

	IntervalMatrix res(x.nb_rows(),x.nb_cols());
	for (int i=0; i<x.nb_rows(); i++) {
		res [i] = x[i] | y[i];
		if (res[i].is_empty()) {
			res.set_empty();
			return res;
		}
	}
	return res;
}


template<class T>
AffineMainMatrix<T> operator*(const AffineMain<T>& x, const Matrix& m) {
	if (x.is_empty()) {
		return AffineMainMatrix<T>::empty(m.nb_rows(),m.nb_cols());
	}
	AffineMainMatrix<T>  res(m.nb_rows(),m.nb_cols(),x);
	for (int i=0; i<m.nb_rows(); i++) {
		for (int j= 0;j<m.nb_cols();j++){
		res[i][j] *= m[i][j];
		}
	}
	return res;
}

template<class T>
AffineMainMatrix<T> operator*(const AffineMain<T>& x, const IntervalMatrix& m) {
	if (x.is_empty()||m.is_empty()) {
		return AffineMainMatrix<T>::empty(m.nb_rows(),m.nb_cols());
	}
	AffineMainMatrix<T>  res(m.nb_rows(),m.nb_cols(),x);
	for (int i=0; i<m.nb_rows(); i++) {
		for (int j= 0;j<m.nb_cols();j++){
		res[i][j] *= m[i][j];
		}
	}
	return res;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const AffineMainMatrix<T>& m) {
	return _displayM(os, m);
}


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
	init(Interval::zero());
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
