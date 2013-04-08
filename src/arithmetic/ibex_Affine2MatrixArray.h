/* ============================================================================
 * I B E X - Matrix Array of Affine2 form
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 16, 2013
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_Affine2_MATRIX_ARRAY_H__
#define __IBEX_Affine2_MATRIX_ARRAY_H__

#include <iostream>
#include "ibex_IntervalMatrixArray.h"
#include "ibex_Affine2Matrix.h"

namespace ibex {

/**
 * \ingroup arithmetic
 *
 * \brief Array of Affine2 matrices.
 */
class Affine2MatrixArray {


private:
	Affine2Matrix* _array;
	int _n;

public:
	/**
	 * \brief Create an array of n (nb_rowsxnb_cols) Affine2 matrices.
	 */
	Affine2MatrixArray(int n, int nb_rows, int nb_cols);

	/**
	 * \brief Copy a.
	 */
	Affine2MatrixArray(const Affine2MatrixArray& a);

	//It is too difficult to know the size of each AF2. So we let the user do what exactly he want to do.
	// Affine2MatrixArray(const IntervalMatrixArray& a, int sizeAF2);

	/**
	 * \brief Assign a to *this.
	 */
	Affine2MatrixArray& operator=(const Affine2MatrixArray& a);
	Affine2MatrixArray& operator=(const IntervalMatrixArray& a);


	/**
	 * \brief Check equality
	 */
	bool operator==(const Affine2MatrixArray& m) const;
	bool operator==(const IntervalMatrixArray& m) const;

	/**
	 * \brief Set all the elements to x.
	 *
	 */
	void init(const Affine2& x);
	void init(const Interval& x);

	/**
	 * \brief Set this Affine2MatrixArray to the empty Affine2MatrixArray
	 *
	 * The dimensions remain the same.
	 */
	void set_empty();

	/**
	 * \brief Delete this.
	 */
	~Affine2MatrixArray();

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
	 * \brief Return true iff this Affine2MatrixArray is empty
	 */
	bool is_empty() const;

	/**
	 * \brief Return the ith matrix.
	 *
	 */
	Affine2Matrix& operator[](int i);

	/**
	 * \brief Return a const reference to the ith matrix.
	 */
	const Affine2Matrix& operator[](int i) const;

};

/** \ingroup arithmetic */
/*@{*/


/**
 * \brief Set *this to its intersection with x
 *
 * \return a reference to this.
 */
IntervalMatrixArray operator&(const Affine2MatrixArray& x1, const Affine2MatrixArray& x2);
IntervalMatrixArray operator&(const Affine2MatrixArray& x1, const IntervalMatrixArray& x2);
IntervalMatrixArray operator&(const IntervalMatrixArray& x1, const Affine2MatrixArray& x2);

/**
 * \brief Output an array of Affine2 matrices.
 */
std::ostream& operator<<(std::ostream& os, const Affine2MatrixArray&);

/*@}*/

/*================================== inline implementations ========================================*/

inline int Affine2MatrixArray::size() const {
	return _n;
}

inline int Affine2MatrixArray::nb_rows() const {
	assert(_n>0);
	return _array[0].nb_rows();
}

inline int Affine2MatrixArray::nb_cols() const {
	assert(_n>0);
	return _array[0].nb_cols();
}

inline Affine2Matrix& Affine2MatrixArray::operator[](int i) {
	return _array[i];
}


inline const Affine2Matrix& Affine2MatrixArray::operator[](int i) const {
	return _array[i];
}

inline void Affine2MatrixArray::set_empty() {
	(*this)[0].set_empty();
}

inline bool Affine2MatrixArray::is_empty() const {
	return (*this)[0].is_empty();
}

inline IntervalMatrixArray operator&(const IntervalMatrixArray& x1, const Affine2MatrixArray& x2) {
	return (x2 & x1);
}


} // namespace ibex

#endif // __IBEX_Affine2_MATRIX_ARRAY_H__
