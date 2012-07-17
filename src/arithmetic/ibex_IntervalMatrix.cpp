/* ============================================================================
 * I B E X - ibex_IntervalMatrix.cpp
 * ============================================================================
 * Copyright   : Ecole des Mines de Nantes (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Gilles Chabert
 * Created     : Jan 6, 2012
 * ---------------------------------------------------------------------------- */

#include "ibex_IntervalMatrix.h"
#include "ibex_Agenda.h"

namespace ibex {

IntervalMatrix::IntervalMatrix() : _nb_rows(0), _nb_cols(0), M(NULL) {

}

IntervalMatrix::IntervalMatrix(int nb_rows, int nb_cols) : _nb_rows(nb_rows), _nb_cols(nb_cols) {
	assert(nb_rows>0);
	assert(nb_cols>0);

	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
	}
}

IntervalMatrix::IntervalMatrix(int nb_rows, int nb_cols, const Interval& x) : _nb_rows(nb_rows), _nb_cols(nb_cols) {
	assert(nb_rows>0);
	assert(nb_cols>0);

	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) M[i].vec[j]=x;
	}
}

IntervalMatrix::IntervalMatrix(int m, int n, double bounds[][2]) : _nb_rows(m), _nb_cols(n) {
	assert(m>0);
	assert(n>0);

	int b=0; // counter for "bounds"
	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) {
			M[i].vec[j]=Interval(bounds[b][0],bounds[b][1]);
			b++;
		}
	}
}

IntervalMatrix::IntervalMatrix(const IntervalMatrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) M[i].vec[j]=m[i][j];
	}
}


IntervalMatrix::IntervalMatrix(const Matrix& m) : _nb_rows(m.nb_rows()), _nb_cols(m.nb_cols()){
	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
		for (int j=0; j<_nb_cols; j++) M[i].vec[j]=m[i][j];
	}
}

IntervalMatrix::~IntervalMatrix() {
	delete[] M;
}

IntervalMatrix& IntervalMatrix::operator=(const IntervalMatrix& x) {
	resize(x.nb_rows(), x.nb_cols());
	for (int i=0; i<nb_rows(); i++)
		(*this)[i]=x[i];
	return *this;
}

IntervalMatrix& IntervalMatrix::operator&=(const IntervalMatrix& m) {
	assert(nb_rows()==m.nb_rows());
	assert(nb_cols()==m.nb_cols());

	if (is_empty()) return *this;
	if (m.is_empty()) { set_empty(); return *this; }

	for (int i=0; i<_nb_rows; i++) {
		if ((row(i) &= m.row(i)).is_empty()) { set_empty(); return *this; }
	}
	return *this;
}

void IntervalMatrix::init(const Interval& x) {
	for (int i=0; i<nb_rows(); i++)
		(*this)[i].init(x);
}

bool IntervalMatrix::operator==(const IntervalMatrix& m) const {
	if (m.nb_rows()!=nb_rows()) return false;
	if (m.nb_cols()!=nb_cols()) return false;

	if (is_empty()) return m.is_empty();
	if (m.is_empty()) return is_empty();

	for (int i=0; i<_nb_rows; i++) {
		if (row(i)!=m.row(i)) return false;
	}
	return true;
}

Matrix IntervalMatrix::lb() const {
	assert(!is_empty());

	Matrix l(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		l[i]=(*this)[i].lb();
	}
	return l;
}

Matrix IntervalMatrix::ub() const {
	assert(!is_empty());

	Matrix u(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		u[i]=(*this)[i].ub();
	}
	return u;
}

Matrix IntervalMatrix::mid() const {
	assert(!is_empty());

	Matrix mV(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		mV[i]=(*this)[i].mid();
	}
	return mV;
}

Matrix IntervalMatrix::mig() const {
	assert(!is_empty());

	Matrix res(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		res[i]=(*this)[i].mig();
	}
	return res;
}

Matrix IntervalMatrix::mag() const {
	assert(!is_empty());

	Matrix res(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		res[i]=(*this)[i].mag();
	}
	return res;
}

void IntervalMatrix::resize(int nb_rows, int nb_cols) {
	assert(nb_rows>0);
	assert(nb_cols>0);
	assert((M==NULL && _nb_rows==0) || (_nb_rows!=0 && M!=NULL));

	if (nb_rows==_nb_rows && nb_cols==_nb_cols) return;

	IntervalVector* M2;

	if (nb_rows!=_nb_rows)
		M2 = new IntervalVector[nb_rows];
	else
		M2 = M;

	for (int i=0; i<nb_rows; i++) {
		M2[i].resize(nb_cols);  // ALL_REALS by default
		if (i<_nb_rows) {
			int min_cols=nb_cols<_nb_cols?nb_cols:_nb_cols;
			for (int j=0; j<min_cols; j++)
				M2[i][j]=M[i][j];
		}
	}

	if (M!=NULL && nb_rows!=_nb_rows) delete[] M; // M=NULL only in IntervalMatrixArray
	M=M2;
	_nb_rows = nb_rows;
	_nb_cols = nb_cols;
}

bool IntervalMatrix::is_zero() const {
	for (int i=0; i<nb_rows(); i++)
		if (!(*this)[i].is_zero()) return false;
	return true;
}

IntervalMatrix IntervalMatrix::submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index) {
	assert(row_start_index>=0 && row_start_index<nb_rows());
	assert(row_end_index>=0 && row_end_index<nb_rows());
	assert(row_start_index<=row_end_index);
	assert(col_start_index>=0 && col_start_index<nb_cols());
	assert(col_end_index>=0 && col_end_index<nb_cols());
	assert(col_start_index<=col_end_index);

	IntervalMatrix sub(row_end_index-row_start_index+1, col_end_index-col_start_index+1);
	//cout << "m=" << (row_end_index-row_start_index+1) << "n=" << (col_end_index-col_start_index+1) << endl;
	//cout << sub << endl;
	int i2=0;
	for (int i=row_start_index; i<=row_end_index; i++, i2++) {
		int j2=0;
		for (int j=col_start_index; j<=col_end_index; j++,j2++)
			sub[i2][j2] = M[i][j];
	}
	return sub;
}

IntervalMatrix IntervalMatrix::transpose() const {
	IntervalMatrix m(nb_cols(), nb_rows());

	for (int i=0; i<nb_rows(); i++) {
		for (int j=0; j<nb_cols(); j++) {
			m[j][i]=(*this)[i][j];
		}
	}
	return m;
}

IntervalVector IntervalMatrix::col(int j) const {
	assert(j>=0 && j<nb_cols());

	IntervalVector res(nb_rows());
	for (int i=0; i<nb_rows(); i++)
		res[i]=(*this)[i][j];
	return res;
}

void IntervalMatrix::set_col(int col, const IntervalVector& v) {
	assert(col>=0 && col<nb_cols());
	assert(nb_rows()==v.size());

	for (int i=0; i<nb_rows(); i++)
		M[i][col]=v[i];
}

bool proj_add(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2) {
	x1 &= y-x2;
	x2 &= y-x1;
	return x1.is_empty() || x2.is_empty();
}

bool proj_sub(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2) {
	x1 &= y+x2;
	x2 &= x1-y;
	return x1.is_empty() || x2.is_empty();
}

bool proj_mul(const IntervalMatrix& y, Interval& x1, IntervalMatrix& x2) {
	int n=(y.nb_rows());
	int m=(y.nb_cols());
	assert((x2.nb_rows())==n && (x2.nb_cols())==m);

	for (int i=0; i<n; i++) {
		if (!proj_mul(y[i],x1,x2[i])) {
			x2.set_empty();
			return false;
		}
	}
	return true;
}

bool proj_mul(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2, double ratio) {
	assert(x1.nb_rows()==y.size());
	assert(x1.nb_cols()==x2.size());

	int last_row=0;
	int i=0;
	int n=y.size();

	do {
		IntervalVector x2old=x2;
		if (!proj_mul(y[i],x1[i],x2)) {
			x1.set_empty();
			return false;
		}
		if (x2old.rel_distance(x2)>ratio) last_row=i;
		i=(i+1)%n;
	} while(i!=last_row);

	return true;
}

bool proj_mul(const IntervalVector& y, IntervalVector& x1, IntervalMatrix& x2, double ratio) {
	IntervalMatrix x2t=x2.transpose();

	bool res=proj_mul(y,x2t,x1,ratio);

	x2=x2t.transpose();
	return res;
}

bool proj_mul(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2, double ratio) {
	int m=y.nb_rows();
	int n=y.nb_cols();
	assert(x1.nb_cols()==x2.nb_rows());
	assert(x1.nb_rows()==m);
	assert(x2.nb_cols()==n);

	// each coefficient (i,j) of y is considered as a binary "dot product" constraint
	// between the ith row of x1 and the jth column of x2
	// (advantage: we have exact projection for the dot product)
	//
	// we propagate these constraints using a simple agenda.
	Agenda a(m*n);

	//init
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++)
			a.push(i*n+j);

	int k;
	while (!a.empty()) {
		a.pop(k);
		int i=k/n;
		int j=k%n;
		IntervalVector x1old=x1[i];
		IntervalVector x2j=x2.col(j);
		IntervalVector x2old=x2j;
		if (!proj_mul(y[i][j],x1[i],x2j)) {
			x1.set_empty();
			x2.set_empty();
			return false;
		} else {
			if (x1old.rel_distance(x1[i])>=ratio) {
				for (int j2=0; j2<n; j2++)
					if (j2!=j) a.push(i*n+j2);
			}
			if (x2old.rel_distance(x2j)>=ratio) {
				for (int i2=0; i2<m; i2++)
					if (i2!=i) a.push(i2*n+j);
			}
			x2.set_col(j,x2j);
		}
	}
	return true;
}


} // namespace ibex
