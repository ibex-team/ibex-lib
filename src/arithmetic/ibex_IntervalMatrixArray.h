/* ============================================================================
 * I B E X - ibex_IntervalMatrixArray.h
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 21, 2012
 * ---------------------------------------------------------------------------- */

#ifndef __IBEX_INTERVAL_MATRIX_ARRAY_H__
#define __IBEX_INTERVAL_MATRIX_ARRAY_H__

#include <iostream>
#include "ibex_IntervalMatrix.h"

namespace ibex {

class IntervalMatrixArray {

public:
	/**
	 * \brief Create an array of n (nb_rowsxnb_cols) interval matrices.
	 */
	IntervalMatrixArray(int n, int nb_rows, int nb_cols);

	/**
	 * \brief Copy a.
	 */
	IntervalMatrixArray(const IntervalMatrixArray& a);

	/**
	 * \brief Assign a to *this.
	 */
	IntervalMatrixArray& operator=(const IntervalMatrixArray& a);

	/**
	 * \brief Delete this.
	 */
	~IntervalMatrixArray();

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
	 * \brief Return the ith matrix.
	 *
	 */
	IntervalMatrix& operator[](int i);

	/**
	 * \brief Return a const reference to the ith matrix.
	 */
	const IntervalMatrix& operator[](int i) const;

private:
	IntervalMatrix* array;
	int n;
};

std::ostream& operator<<(std::ostream& os, const IntervalMatrixArray&);


/*================================== inline implementations ========================================*/

inline int IntervalMatrixArray::size() const {
	return n;
}

inline int IntervalMatrixArray::nb_rows() const {
	assert(n>0);
	return array[0].nb_rows();
}

inline int IntervalMatrixArray::nb_cols() const {
	assert(n>0);
	return array[0].nb_cols();
}

inline IntervalMatrix& IntervalMatrixArray::operator[](int i) {
	return array[i];
}

inline const IntervalMatrix& IntervalMatrixArray::operator[](int i) const {
	return array[i];
}

} // namespace ibex

#endif // __IBEX_INTERVAL_MATRIX_ARRAY_H__
