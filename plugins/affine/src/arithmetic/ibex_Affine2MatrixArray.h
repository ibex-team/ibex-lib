/* ============================================================================
 * I B E X - Matrix Array of Affine2 form
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
#include "ibex_Affine2Matrix.h"

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Array of Affine2 matrices.
 */
typedef Affine2MainMatrixArray<AF_Default> Affine2MatrixArray;
typedef Affine2MainMatrixArray<AF_Linear> AffineLinMatrixArray;

template<class T=AF_Default>
class Affine2MainMatrixArray {


private:
	Affine2MainMatrix<T>* _array;
	int _n;

public:
	/**
	 * \brief Create an array of n (nb_rowsxnb_cols) Affine2 matrices.
	 */
	Affine2MainMatrixArray(int n, int nb_rows, int nb_cols);

	/**
	 * \brief Copy a.
	 */
	Affine2MainMatrixArray(const Affine2MainMatrixArray<T>& a);

	//It is too difficult to know the size of each AF2. So we let the user do what exactly he want to do.
	// Affine2MainMatrixArray<T>(const IntervalMatrixArray& a, int sizeAF2);

	/**
	 * \brief Assign a to *this.
	 */
	Affine2MainMatrixArray<T>& operator=(const Affine2MainMatrixArray<T>& a);
	Affine2MainMatrixArray<T>& operator=(const IntervalMatrixArray& a);


	/**
	 * \brief Check equality
	 */
	bool operator==(const Affine2MainMatrixArray<T>& m) const;
	bool operator==(const IntervalMatrixArray& m) const;

	/**
	 * \brief Set all the elements to x.
	 *
	 */
	void init(const Affine2Main<T>& x);
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
	~Affine2MainMatrixArray();

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
	Affine2MainMatrix<T>& operator[](int i);

	/**
	 * \brief Return a const reference to the ith matrix.
	 */
	const Affine2MainMatrix<T>& operator[](int i) const;

};

/** \ingroup arithmetic */
/*@{*/


/**
 * \brief Set *this to its intersection with x
 *
 * \return a reference to this.
 */
template<class T>
IntervalMatrixArray operator&(const Affine2MainMatrixArray<T>& x1, const Affine2MainMatrixArray<T>& x2);
template<class T>
IntervalMatrixArray operator&(const Affine2MainMatrixArray<T>& x1, const IntervalMatrixArray& x2);
template<class T>
IntervalMatrixArray operator&(const IntervalMatrixArray& x1, const Affine2MainMatrixArray<T>& x2);

/**
 * \brief Output an array of Affine2 matrices.
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const Affine2MainMatrixArray<T>&);

/*@}*/
}
#include "ibex_Affine2MatrixArray.h_"

/*================================== inline implementations ========================================*/

namespace ibex {

template<class T>
inline int Affine2MainMatrixArray<T>::size() const {
	return _n;
}

template<class T>
inline int Affine2MainMatrixArray<T>::nb_rows() const {
	assert(_n>0);
	return _array[0].nb_rows();
}

template<class T>
inline int Affine2MainMatrixArray<T>::nb_cols() const {
	assert(_n>0);
	return _array[0].nb_cols();
}

template<class T>
inline Affine2MainMatrix<T>& Affine2MainMatrixArray<T>::operator[](int i) {
	return _array[i];
}


template<class T>
inline const Affine2MainMatrix<T>& Affine2MainMatrixArray<T>::operator[](int i) const {
	return _array[i];
}

template<class T>
inline void Affine2MainMatrixArray<T>::set_empty() {
	(*this)[0].set_empty();
}

template<class T>
inline bool Affine2MainMatrixArray<T>::is_empty() const {
	return (*this)[0].is_empty();
}

template<class T>
inline IntervalMatrixArray operator&(const IntervalMatrixArray& x1, const Affine2MainMatrixArray<T>& x2) {
	return (x2 & x1);
}


} // namespace ibex

#endif // __IBEX_AFFINE2_MATRIX_ARRAY_H__
