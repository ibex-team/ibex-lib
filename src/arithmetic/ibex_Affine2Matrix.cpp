/* ============================================================================
 * I B E X - ibex_Affine2Matrix.cpp
 * ============================================================================
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Jordan Ninin
 * Created     : March 16, 2013
 * ---------------------------------------------------------------------------- */

#include "ibex_Affine2Matrix.h"
#include "ibex_Agenda.h"
#include "ibex_TemplateMatrix.cpp_"

namespace ibex {

IntervalMatrix& IntervalMatrix::operator=(const Affine2Matrix& x) {
	resize(x.nb_rows(), x.nb_cols());
	for (int i=0; i<nb_rows(); i++)
		(*this)[i] = x[i];
	return *this;
}

Affine2Matrix::Affine2Matrix() : _nb_rows(0), _nb_cols(0), _M(NULL) {

}

Affine2Matrix::Affine2Matrix(int nb_rows1, int nb_cols1) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
	}
}

Affine2Matrix::Affine2Matrix(int nb_rows1, int nb_cols1, const Affine2& x) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=x;
	}
}



Affine2Matrix::Affine2Matrix(int m, int n, double bounds[][2]) : _nb_rows(m), _nb_cols(n) {
	assert(m>0);
	assert(n>0);
		int k=0; // counter for "bounds"
		_M = new Affine2Vector[_nb_rows];
		for (int i=0; i<_nb_rows; i++) {
			_M[i].resize(_nb_cols);
			for (int j=0; j<_nb_cols; j++) {
				_M[i]._vec[j]=Affine2(Interval(bounds[k][0],bounds[k][1]));
				k++;
			}
		}
}

Affine2Matrix::Affine2Matrix(const Affine2Matrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new Affine2Vector[_nb_rows];

	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) _M[i]._vec[j]=Affine2(m[i][j]);
	}

}


Affine2Matrix::Affine2Matrix(const IntervalMatrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new Affine2Vector[_nb_rows];
	int k=0; // counter for "bounds"
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++)  {
			_M[i]._vec[j]=Affine2(m[i][j]);
			k++;
		}
	}
}


Affine2Matrix::Affine2Matrix(const Matrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	_M = new Affine2Vector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		_M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			_M[i]._vec[j]=Affine2(m[i][j]);
		}
	}
}

Affine2Matrix::~Affine2Matrix() {
	if (_M!=NULL) delete[] _M;
}

Affine2Matrix& Affine2Matrix::operator=(const Affine2Matrix& x) {
	//resize(x.nb_rows(), x.nb_cols());
	return _assign(*this,x);
}

Affine2Matrix& Affine2Matrix::operator=(const IntervalMatrix& x) {
	//resize(x.nb_rows(), x.nb_cols());
	return _assign(*this,x);
}

IntervalMatrix Affine2Matrix::itv() const {
	assert(!is_empty());
	IntervalMatrix intm(nb_rows(),nb_cols());
	for (int i = 0; i < nb_rows(); i++) {
		for (int j =0; j < nb_cols();j++){
			intm[i][j] = (*this)[i][j].itv();
		}
	}
	return intm;
}


void Affine2Matrix::init(const Affine2& x) {
	for (int i=0; i<nb_rows(); i++)
		(*this)[i].init(x);
}

void Affine2Matrix::init(const Interval& x) {
	for (int i=0; i<nb_rows(); i++) {
		for (int j = 0; i < nb_cols(); i++) {
			(*this)[i][j] = Affine2(x);
		}
	}
}

bool Affine2Matrix::operator==(const Affine2Matrix& m) const {
	return _equals(*this,m);
}

bool Affine2Matrix::operator==(const IntervalMatrix& m) const {
	return _equals(*this,m);
}

void Affine2Matrix::resize(int nb_rows1, int nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);
	assert((_M==NULL && _nb_rows==0) || (_nb_rows!=0 && _M!=NULL));

	if (nb_rows1==_nb_rows && nb_cols1==_nb_cols) return;

	Affine2Vector* M2;

	if (nb_rows1!=_nb_rows)
		M2 = new Affine2Vector[nb_rows1];
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

Affine2Matrix Affine2Matrix::submatrix(int row_start_index, int row_end_index,	int col_start_index, int col_end_index) {
	return _submatrix(*this,row_start_index,row_end_index,col_start_index,col_end_index);
}

Affine2Matrix Affine2Matrix::transpose() const {
	return _transpose(*this);
}

Affine2Vector Affine2Matrix::col(int j) const {
	return _col<Affine2Matrix,Affine2Vector>(*this,j);
}

void Affine2Matrix::set_col(int col1, const Affine2Vector& v) {
	_set_col(*this,col1,v);
}

Affine2Matrix& Affine2Matrix::inflate(double rad) {
	// see comment in Affine2Vector::inflate
	if (is_empty())
		return *this;

	Interval r(-rad, rad);
	for (int i = 0; i < nb_rows(); i++)
		for (int j = 0; j < nb_cols(); j++)
			(*this)[i][j] += r;
	return *this;
}

IntervalMatrix operator&(const Affine2Matrix& x, const Affine2Matrix& y) {
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

IntervalMatrix operator&(const Affine2Matrix& x, const IntervalMatrix& y) {
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

IntervalMatrix operator|(const Affine2Matrix& x,	const Affine2Matrix& y) {
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

IntervalMatrix operator|(const Affine2Matrix& x,	const IntervalMatrix& y) {
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


Affine2Matrix operator*(const Affine2& x, const Matrix& m) {
	if (x.is_empty()) {
		return Affine2Matrix::empty(m.nb_rows(),m.nb_cols());
	}
	Affine2Matrix  res(m.nb_rows(),m.nb_cols(),x);
	for (int i=0; i<m.nb_rows(); i++) {
		for (int j= 0;j<m.nb_cols();j++){
		res[i][j] *= m[i][j];
		}
	}
	return res;
}

Affine2Matrix operator*(const Affine2& x, const IntervalMatrix& m) {
	if (x.is_empty()||m.is_empty()) {
		return Affine2Matrix::empty(m.nb_rows(),m.nb_cols());
	}
	Affine2Matrix  res(m.nb_rows(),m.nb_cols(),x);
	for (int i=0; i<m.nb_rows(); i++) {
		for (int j= 0;j<m.nb_cols();j++){
		res[i][j] *= m[i][j];
		}
	}
	return res;
}

std::ostream& operator<<(std::ostream& os, const Affine2Matrix& m) {
	return display(os, m);
}


} // namespace ibex
