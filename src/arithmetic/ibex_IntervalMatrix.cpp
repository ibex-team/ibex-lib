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
#include "ibex_TemplateMatrix.h"

namespace ibex {

IntervalMatrix::IntervalMatrix() : _nb_rows(0), _nb_cols(0), M(NULL) {

}

IntervalMatrix::IntervalMatrix(int nb_rows1, int nb_cols1) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

	M = new IntervalVector[_nb_rows];
	for (int i=0; i<_nb_rows; i++) {
		M[i].resize(_nb_cols);
	}
}

IntervalMatrix::IntervalMatrix(int nb_rows1, int nb_cols1, const Interval& x) : _nb_rows(nb_rows1), _nb_cols(nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);

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
	if (M!=NULL) delete[] M;
}

IntervalMatrix& IntervalMatrix::operator=(const IntervalMatrix& x) {
	resize(x.nb_rows(), x.nb_cols());
	// need to be resized when called from operator*= (dimension can change)
	return _assignM(*this,x);
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
	return _equalsM(*this,m);
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

void IntervalMatrix::resize(int nb_rows1, int nb_cols1) {
	assert(nb_rows1>0);
	assert(nb_cols1>0);
	assert((M==NULL && _nb_rows==0) || (_nb_rows!=0 && M!=NULL));

	if (nb_rows1==_nb_rows && nb_cols1==_nb_cols) return;

	IntervalVector* M2;

	if (nb_rows1!=_nb_rows)
		M2 = new IntervalVector[nb_rows1];
	else
		M2 = M;

	for (int i=0; i<nb_rows1; i++) {
		M2[i].resize(nb_cols1);  // ALL_REALS by default
		if (i<_nb_rows) {
			int min_cols=nb_cols1<_nb_cols?nb_cols1:_nb_cols;
			for (int j=0; j<min_cols; j++)
				M2[i][j]=M[i][j];
		}
	}

	if (M!=NULL && nb_rows1!=_nb_rows) delete[] M; // should not happen (default constructor being private)
	M=M2;
	_nb_rows = nb_rows1;
	_nb_cols = nb_cols1;
}

bool IntervalMatrix::is_zero() const {
	for (int i=0; i<nb_rows(); i++)
		if (!(*this)[i].is_zero()) return false;
	return true;
}

bool IntervalMatrix::is_unbounded() const {
	return _is_unboundedM(*this);
}

IntervalMatrix IntervalMatrix::submatrix(int row_start_index, int row_end_index, int col_start_index, int col_end_index) const {
	return _submatrix(*this, row_start_index, row_end_index, col_start_index, col_end_index);
}

void IntervalMatrix::put(int row_start_index, int col_start_index, const Matrix& sub) {
	_put<IntervalMatrix,Matrix>(*this, row_start_index, col_start_index, sub);
}

void IntervalMatrix::put(int row_start_index, int col_start_index, const IntervalMatrix& sub) {
	_put<IntervalMatrix,IntervalMatrix>(*this, row_start_index, col_start_index, sub);
}

void IntervalMatrix::put(int row_start_index, int col_start_index, const Vector& sub, bool row_vec) {
	_put<IntervalMatrix,Vector>(*this, row_start_index, col_start_index, sub, row_vec);
}

void IntervalMatrix::put(int row_start_index, int col_start_index, const IntervalVector& sub, bool row_vec) {
	_put<IntervalMatrix,IntervalVector>(*this, row_start_index, col_start_index, sub, row_vec);
}

IntervalMatrix IntervalMatrix::transpose() const {
	return _transpose(*this);
}

IntervalVector IntervalMatrix::col(int j) const {
	return _col<IntervalMatrix,IntervalVector>(*this,j);
}

void IntervalMatrix::set_col(int col1, const IntervalVector& v) {
	_set_col(*this,col1,v);
}

IntervalMatrix& IntervalMatrix::inflate(double rad) {
	// see comment in IntervalVector::inflate
	if (is_empty()) return *this;

	Interval r(-rad,rad);

	for (int i=0; i<nb_rows(); i++)
		for (int j=0; j<nb_cols(); j++)
			(*this)[i][j]+=r;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const IntervalMatrix& m) {
	return _displayM(os,m);
}

bool bwd_add(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2) {
	x1 &= y-x2;
	x2 &= y-x1;
	return !x1.is_empty() && !x2.is_empty();
}

bool bwd_sub(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2) {
	x1 &= y+x2;
	x2 &= x1-y;
	return !x1.is_empty() && !x2.is_empty();
}

bool bwd_mul(const IntervalMatrix& y, Interval& x1, IntervalMatrix& x2) {
	int n=(y.nb_rows());
	assert((x2.nb_rows())==n && (x2.nb_cols())==(y.nb_cols()));

	for (int i=0; i<n; i++) {
		if (!bwd_mul(y[i],x1,x2[i])) {
			x2.set_empty();
			return false;
		}
	}
	return true;
}

bool bwd_mul(const IntervalVector& y, IntervalMatrix& x1, IntervalVector& x2, double ratio) {
	assert(x1.nb_rows()==y.size());
	assert(x1.nb_cols()==x2.size());

	int last_row=0;
	int i=0;
	int n=y.size();

	do {
		IntervalVector x2old=x2;
		if (!bwd_mul(y[i],x1[i],x2)) {
			x1.set_empty();
			return false;
		}
		if (x2old.rel_distance(x2)>ratio) last_row=i;
		i=(i+1)%n;
	} while(i!=last_row);

	return true;
}

bool bwd_mul(const IntervalVector& y, IntervalVector& x1, IntervalMatrix& x2, double ratio) {
	IntervalMatrix x2t=x2.transpose();

	bool res=bwd_mul(y,x2t,x1,ratio);

	x2=x2t.transpose();
	return res;
}

bool bwd_mul(const IntervalMatrix& y, IntervalMatrix& x1, IntervalMatrix& x2, double ratio) {
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
		if (!bwd_mul(y[i][j],x1[i],x2j)) {
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

Matrix IntervalMatrix::rad() const {
	Matrix M(nb_rows(),nb_cols());
	for (int i=0; i<nb_rows(); i++)
		for (int j=0; j<nb_cols(); j++)
			M[i][j]=(*this)[i][j].rad();
	return M;
}

Matrix IntervalMatrix::diam() const {
	Matrix M(nb_rows(),nb_cols());
	for (int i=0; i<nb_rows(); i++)
		for (int j=0; j<nb_cols(); j++)
			M[i][j]=(*this)[i][j].diam();
	return M;
}

Matrix IntervalMatrix::random() const {
	assert(!is_empty());

	Matrix mm(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		const IntervalVector& vi=(*this)[i];
		mm[i] = vi.random();
	}
	return mm;
}

Matrix IntervalMatrix::random(int seed) const {
	assert(!is_empty());

	Matrix mm(nb_rows(), nb_cols());
	for (int i=0; i<nb_rows(); i++) {
		const IntervalVector& vi=(*this)[i];
		mm[i] = vi.random(seed);
	}
	return mm;
}

IntervalMatrix abs(const IntervalMatrix& m) {
	return _absM(m);
}

double infinite_norm(const IntervalMatrix& m) {
	return _infinite_normM(m);
}


} // namespace ibex
