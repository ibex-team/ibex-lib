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

#ifndef IBEX_INTERVALMATRIXARRAY_H_
#define IBEX_INTERVALMATRIXARRAY_H_

namespace ibex {

#include <iostream>

class IntervalMatrixArray {

public:
	IntervalMatrixArray(int n, int nb_rows, int nb_cols);

	IntervalMatrixArray();
	virtual ~IntervalMatrixArray();

	/**
	 * \brief Return the ith matrix.
	 *
	 */
	IntervalMatrix& operator[](int i);

	/**
	 * \brief Return a const reference to the ith matrix.
	 */
	const IntervalMatrix& operator[](int i) const;

};

std::ostream& operator<<(std::ostream& os, const IntervalMatrixArray&);

} // namespace ibex
#endif // IBEX_INTERVALMATRIXARRAY_H_
