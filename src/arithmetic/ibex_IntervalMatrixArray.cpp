/* ============================================================================
 * I B E X - ibex_IntervalMatrixArray.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 21, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_IntervalMatrixArray.h"
#include <cassert>

namespace ibex {

IntervalMatrixArray::IntervalMatrixArray(int n1, int nb_rows1, int nb_cols1) : n(n1) {
	assert(n1>0);

	array = new IntervalMatrix[n1];
	for (int i=0; i<n1; i++)
		array[i].resize(nb_rows1,nb_cols1);
}

IntervalMatrixArray::IntervalMatrixArray(const IntervalMatrixArray& a) : n(a.n) {
	array = new IntervalMatrix[n];
	for (int i=0; i<n; i++) {
		array[i]=a[i]; // array[i] will be resized
	}
}

IntervalMatrixArray& IntervalMatrixArray::operator=(const IntervalMatrixArray& a) {
	if (size()!=a.size()) {
		delete[] array;
		array = new IntervalMatrix[a.size()];
		n = a.size();
	}

	for (int i=0; i<a.size(); i++) {
		array[i]=a[i]; // array[i] will be resized
	}
	return *this;
}

IntervalMatrixArray& IntervalMatrixArray::operator&=(const IntervalMatrixArray& a) {
	for (int i=0; i<a.size(); i++) {
		array[i] &= a[i];
	}
	return *this;
}

bool IntervalMatrixArray::operator==(const IntervalMatrixArray& x) const {
	if (n!=x.size()) return false;
	for (int i=0; i<n; i++)
		if ((*this)[i]!=(x[i])) return false;
	return true;
}

void IntervalMatrixArray::init(const Interval& x) {
	for (int i=0; i<size(); i++)
		(*this)[i].init(x);
}

IntervalMatrixArray::~IntervalMatrixArray() {
	delete[] array;
}

std::ostream& operator<<(std::ostream& os, const IntervalMatrixArray& a) {
	for (int i=0; i<a.size(); i++) {
		os << a[i] << std::endl;
	}
	return os;
}

} // namespace ibex
