/* ============================================================================
 * I B E X - Array of matrices (of reals)
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : May 14, 2014
 * ---------------------------------------------------------------------------- */

#include "ibex_MatrixArray.h"
#include <cassert>

namespace ibex {

MatrixArray::MatrixArray(int n1, int nb_rows1, int nb_cols1) : n(n1) {
	assert(n1>0);

	array = new Matrix[n1];
	for (int i=0; i<n1; i++)
		array[i].resize(nb_rows1,nb_cols1);
}

MatrixArray::MatrixArray(const MatrixArray& a) : n(a.n) {
	array = new Matrix[n];
	for (int i=0; i<n; i++) {
		array[i]=a[i]; // array[i] will be resized
	}
}

MatrixArray& MatrixArray::operator=(const MatrixArray& a) {
	if (size()!=a.size()) {
		delete[] array;
		array = new Matrix[a.size()];
		n = a.size();
	}

	for (int i=0; i<a.size(); i++) {
		array[i]=a[i]; // array[i] will be resized
	}
	return *this;
}

bool MatrixArray::operator==(const MatrixArray& x) const {
	if (n!=x.size()) return false;
	for (int i=0; i<n; i++)
		if ((*this)[i]!=(x[i])) return false;
	return true;
}

MatrixArray::~MatrixArray() {
	delete[] array;
}

std::ostream& operator<<(std::ostream& os, const MatrixArray& a) {
	for (int i=0; i<a.size(); i++) {
		os << a[i] << std::endl;
	}
	return os;
}

} // namespace ibex
