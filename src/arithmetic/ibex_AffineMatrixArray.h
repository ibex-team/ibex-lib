/* ============================================================================
 * I B E X - Matrix Array of Affine form
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 16, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_AFFINE2_MATRIX_ARRAY_H__
#define __IBEX_AFFINE2_MATRIX_ARRAY_H__

#include <iostream>
#include "ibex_IntervalMatrixArray.h"
#include "ibex_AffineMatrix.h"

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Array of Affine2 matrices.
 */
typedef AffineMainMatrixArray<AF_Default> Affine2MatrixArray;
typedef AffineMainMatrixArray<AF_Other> Affine3MatrixArray;

template<class T=AF_Default>
class AffineMainMatrixArray {


private:
	AffineMainMatrix<T>* _array;
	int _n;

public:
	/**
	 * \brief Create an array of n (nb_rowsxnb_cols) Affine2 matrices.
	 */
	AffineMainMatrixArray(int n, int nb_rows, int nb_cols);

	/**
	 * \brief Copy a.
	 */
	AffineMainMatrixArray(const AffineMainMatrixArray<T>& a);

	//It is too difficult to know the size of each AF2. So we let the user do what exactly he want to do.
	// Affine2MainMatrixArray<T>(const IntervalMatrixArray& a, int sizeAF2);

	/**
	 * \brief Assign a to *this.
	 */
	AffineMainMatrixArray<T>& operator=(const AffineMainMatrixArray<T>& a);
	AffineMainMatrixArray<T>& operator=(const IntervalMatrixArray& a);


	/**
	 * \brief Check equality
	 */
	bool operator==(const AffineMainMatrixArray<T>& m) const;
	bool operator==(const IntervalMatrixArray& m) const;

	/**
	 * \brief Set all the elements to x.
	 *
	 */
	void init(const AffineMain<T>& x);
	void init(const Interval& x);

	/**
	 * \brief Set this Affine2MainMatrixArray<T> to the empty Affine2MainMatrixArray<T>
	 *
	 * The dimensions remain the same.
	 */
	void set_empty();

	/**
	 * \brief Delete this.
	 */
	~AffineMainMatrixArray();

	/**
	 * \brief return a IntervalMatrixArray compose by the Interval of each Affine2 forms.
	 */
	IntervalMatrixArray itv() const;

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
	 * \brief Return true iff this Affine2MainMatrixArray<T> is empty
	 */
	bool is_empty() const;

	/**
	 * \brief Return the ith matrix.
	 *
	 */
	AffineMainMatrix<T>& operator[](int i);

	/**
	 * \brief Return a const reference to the ith matrix.
	 */
	const AffineMainMatrix<T>& operator[](int i) const;

};

/** \ingroup arithmetic */
/*@{*/


/**
 * \brief Set *this to its intersection with x
 *
 * \return a reference to this.
 */
template<class T>
IntervalMatrixArray operator&(const AffineMainMatrixArray<T>& x1, const AffineMainMatrixArray<T>& x2);
template<class T>
IntervalMatrixArray operator&(const AffineMainMatrixArray<T>& x1, const IntervalMatrixArray& x2);
template<class T>
IntervalMatrixArray operator&(const IntervalMatrixArray& x1, const AffineMainMatrixArray<T>& x2);

/**
 * \brief Output an array of Affine2 matrices.
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const AffineMainMatrixArray<T>&);

/*@}*/
}
#include "ibex_AffineMatrixArray.h_"

/*================================== inline implementations ========================================*/

namespace ibex {

template<class T>
inline int AffineMainMatrixArray<T>::size() const {
	return _n;
}

template<class T>
inline int AffineMainMatrixArray<T>::nb_rows() const {
	assert(_n>0);
	return _array[0].nb_rows();
}

template<class T>
inline int AffineMainMatrixArray<T>::nb_cols() const {
	assert(_n>0);
	return _array[0].nb_cols();
}

template<class T>
inline AffineMainMatrix<T>& AffineMainMatrixArray<T>::operator[](int i) {
	return _array[i];
}


template<class T>
inline const AffineMainMatrix<T>& AffineMainMatrixArray<T>::operator[](int i) const {
	return _array[i];
}

template<class T>
inline void AffineMainMatrixArray<T>::set_empty() {
	(*this)[0].set_empty();
}

template<class T>
inline bool AffineMainMatrixArray<T>::is_empty() const {
	return (*this)[0].is_empty();
}

template<class T>
inline IntervalMatrixArray operator&(const IntervalMatrixArray& x1, const AffineMainMatrixArray<T>& x2) {
	return (x2 & x1);
}


} // namespace ibex

#endif // __IBEX_AFFINE2_MATRIX_ARRAY_H__
