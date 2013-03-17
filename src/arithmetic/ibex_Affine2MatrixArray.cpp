/* ============================================================================
 * I B E X - ibex_Affine2MatrixArray.cpp
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 16, 2013
 * ---------------------------------------------------------------------------- */

#include "ibex_Affine2MatrixArray.h"
#include <cassert>

namespace ibex {

Affine2MatrixArray::Affine2MatrixArray(int n1, int nb_rows1, int nb_cols1) : _n(n1) {
	assert(n1>0);

	_array = new Affine2Matrix[n1];
	for (int i=0; i<n1; i++)
		_array[i].resize(nb_rows1,nb_cols1);
}

Affine2MatrixArray::Affine2MatrixArray(const Affine2MatrixArray& a) : _n(a.size()) {
	_array = new Affine2Matrix[_n];
	for (int i=0; i<_n; i++) {
		_array[i] = Affine2Matrix(a[i]); // array[i] will be resized
	}
}
/*  It is too difficult to know the size of each AF2. So we let the user do what exactly he want to do.
Affine2MatrixArray::Affine2MatrixArray(const IntervalMatrixArray& a, int sizeAF2) : _n(a.size()) {
	_array = new Affine2Matrix[_n];
	for (int i=0; i<_n; i++) {
		_array[i]=Affine2Matrix(a[i],sizeAF2); // array[i] will be resized
	}
}
*/

Affine2MatrixArray& Affine2MatrixArray::operator=(const Affine2MatrixArray& a) {
	if (size()!=a.size()) {
		delete[] _array;
		_array = new Affine2Matrix[a.size()];
		_n = a.size();
	}

	for (int i=0; i<a.size(); i++) {
		_array[i]=a[i]; // array[i] will be resized
	}
	return *this;
}
Affine2MatrixArray& Affine2MatrixArray::operator=(const IntervalMatrixArray& a) {
	if (size()!=a.size()) {
		delete[] _array;
		_array = new Affine2Matrix[a.size()];
		_n = a.size();
	}

	for (int i=0; i<a.size(); i++) {
		_array[i] = a[i]; // array[i] will be resized
	}
	return *this;
}


bool Affine2MatrixArray::operator==(const Affine2MatrixArray& x) const {
	if (_n!=x.size()) return false;
	for (int i=0; i<_n; i++)
		if ((*this)[i]!=(x[i])) return false;
	return true;
}

bool Affine2MatrixArray::operator==(const IntervalMatrixArray& x) const {
	if (_n!=x.size()) return false;
	for (int i=0; i<_n; i++)
		if ((*this)[i]!=(x[i])) return false;
	return true;
}

void Affine2MatrixArray::init(const Affine2& x) {
	for (int i=0; i<size(); i++)
		(*this)[i].init(x);
}
void Affine2MatrixArray::init(const Interval& x) {
	for (int i=0; i<size(); i++)
		(*this)[i].init(x);
}



IntervalMatrixArray Affine2MatrixArray::itv() const {
	assert(!is_empty());
	IntervalMatrixArray intm(size(),nb_rows(),nb_cols());
	for (int k = 0; k < size();k++) {
		for (int i = 0; i < nb_rows(); i++) {
			for (int j =0; j < nb_cols();j++){
				intm[k][i][j] = (*this)[k][i][j].itv();
			}
		}
	}
	return intm;
}

Affine2MatrixArray::~Affine2MatrixArray() {
	delete[] _array;
}


IntervalMatrixArray operator&(const Affine2MatrixArray& x1, const Affine2MatrixArray& x2) {
	assert(x1.size()==x2.size());
	IntervalMatrixArray res (x1.size(),x1.nb_rows(),x1.nb_cols());
	for (int i=0; i<x1.size(); i++) {
		res[i] = x1[i] & x2[i];
	}
	return res;
}
IntervalMatrixArray operator&(const Affine2MatrixArray& x1, const IntervalMatrixArray& x2) {
	assert(x1.size()==x2.size());
	IntervalMatrixArray res (x1.size(),x1.nb_rows(),x1.nb_cols());
	for (int i=0; i<x1.size(); i++) {
		res[i] = x1[i] & x2[i];
	}
	return res;
}


std::ostream& operator<<(std::ostream& os, const Affine2MatrixArray& a) {
	for (int i=0; i<a.size(); i++) {
		os << a[i] << std::endl;
	}
	return os;
}

} // namespace ibex
