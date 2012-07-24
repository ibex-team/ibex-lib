//============================================================================
//                                  I B E X                                   
// File        : ibex_Linear.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Apr 18, 2012
// Last Update : Apr 18, 2012
//============================================================================

#include <math.h>
#include "ibex_Linear.h"
#include "ibex_LinearException.h"
#define TOO_LARGE 1e30
#define TOO_SMALL 1e-10

namespace ibex {

void real_LU(const Matrix& A, Matrix& LU, int* p) {

	int n = (A.nb_rows());
	assert(n == (A.nb_cols())); // throw NotSquareMatrixException();
	assert(n == (LU.nb_rows()) && n == (LU.nb_cols()));

	// check the matrix has no "infinite" values
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			if (A[i][j]>=TOO_LARGE) throw SingularMatrixException();
		}
	}

	LU = A;
	for (int i=0; i<n; i++) p[i]=i;

	// LU computation
	double pivot;

	for (int i=0; i<n; i++) {
		// pivot search
		int swap = i;
		pivot = LU[p[i]][i];
		for (int j=i+1; j<n; j++) {
			volatile double tmp=LU[p[j]][i];
			if (fabs(tmp)>fabs(LU[p[swap]][i])) swap=j;
		}
		int tmp = p[i];
		p[i] = p[swap];
		p[swap] = tmp;
		// --------------------------------------------------

		pivot = LU[p[i]][i];
		if (pivot==0.0) throw SingularMatrixException();
		if (fabs(1/pivot)>=TOO_LARGE) throw SingularMatrixException();

		for (int j=i+1; j<n; j++) {
			for (int k=i+1; k<n; k++) {
				LU[p[j]][k]-=(LU[p[j]][i]/pivot)*LU[p[i]][k];
			}
			LU[p[j]][i] = LU[p[j]][i]/pivot;
		}
	}
}

namespace {

void real_LU_solve(const Matrix& LU, const int* p, const Vector& b, Vector& x) {
	//cout << "LU=" << LU << " b=" << b << endl;

	int n = (LU.nb_rows());
	assert(n == (LU.nb_cols())); // throw NotSquareMatrixException();
	assert(n == (b.size()) && n == (x.size()));

	// solve Lx=b
	x[0] = b[p[0]];
	for (int i=1; i<n; i++) {
		x[i] = b[p[i]];
		for (int j=0; j<i; j++) {
			x[i] -= LU[p[i]][j]*x[j];
		}
	}
	//cout << " x1=" << x << endl;

	// solve Uy=x
	if (fabs(LU[p[n-1]][n-1])<=TOO_SMALL) throw SingularMatrixException();
	x[n-1] /= LU[p[n-1]][n-1];

	for (int i=n-2; i>=0; i--) {
		for (int j=i+1; j<n; j++) {
			x[i] -= LU[p[i]][j]*x[j];
		}
		if (fabs(LU[p[i]][i])<=TOO_SMALL) throw SingularMatrixException();
		x[i] /= LU[p[i]][i];
	}
	//cout << " x2=" << x << endl;
}

}

void real_inverse(const Matrix& A, Matrix& invA) {
	int n = (A.nb_rows());

	Matrix LU(n,n);
	int* p= new int[n];

	try {
		real_LU(A, LU, p);

		Vector b(n,0.0);
		Vector x(n);

		for (int i=0; i<n; i++) {
			b[i]=1;
			real_LU_solve(LU, p, b, x);
			invA.set_col(i, x);
			b[i]=0;
		}
	}
	catch(SingularMatrixException& e) {
		delete[] p;
		throw e;
	}
	delete[] p;
}

void precond(IntervalMatrix& A, IntervalVector& b) {
	int n=(A.nb_rows());
	assert(n == A.nb_cols()); //throw NotSquareMatrixException();  // not well-constraint problem
	assert(n == b.size());

	Matrix C(n,n);
	try { real_inverse(A.mid(), C); }
	catch (SingularMatrixException&) {
		try { real_inverse(A.lb(), C); }
		catch (SingularMatrixException&) {
			real_inverse(A.ub(), C);
		}
	}

	//   cout << "A=" << (A.nb_cols()) << "x" << (A.nb_rows()) << "  " << "b=" << (b.size()) << "  " << "C="
	//        << (C.nb_cols()) << "x" << (C.nb_rows()) << endl;
	//cout << "C=" << C << endl;
	A = C*A;
	b = C*b;
}

// static void lu_interval(IntervalMatrix& A, int i, int n, Interval& det) {

//   if (i==n) {
//     if (A[n][n].contains(0)) throw NullPivotException();
//     det=A[n][n];
//     return;
//   }

//   Interval pivot = A[i][i];

//   if (pivot.contains(0)) throw NullPivotException();

//   for (int j=i+1; j<=n; j++) {
//     for (int k=i+1; k<=n; k++) A[j][k]-=(A[j][i]/pivot)*A[i][k];
//     A[j][i] = A[j][i]/pivot;
//   }

//   lu_interval(A, i+1, n, det);
//   det = A[i][i]*det;
// }

void gauss_seidel(const IntervalMatrix& A, const IntervalVector& b, IntervalVector& x, double ratio) {
	int n=(A.nb_rows());
	assert(n == (A.nb_cols())); // throw NotSquareMatrixException();
	assert(n == (x.size()) && n == (b.size()));

	double red;
	Interval old, proj, tmp;

	do {
		red = 0;
		for (int i=0; i<n; i++) {
			old = x[i];
			proj = b[i];

			for (int j=0; j<n; j++)	if (j!=i) proj -= A[i][j]*x[j];
			tmp=A[i][i];

			proj_mul(proj,tmp,x[i]);

			if (x[i].is_empty()) { x.set_empty(); return; }

			double gain=old.rel_distance(x[i]);
			if (gain>red) red=gain;
		}
	} while (red >= ratio);
}


// void PrecGaussSeidel(IntervalMatrix& A, IntervalVector& b, IntervalVector& x) throw(LinearException) {
//   Precond(A, b);
//   GaussSeidel(A, b, x);
// }

void hansen_bliek(const IntervalMatrix& A, const IntervalVector& B, IntervalVector& x) {
	int n=A.nb_rows();
	assert(n == A.nb_cols()); // throw NotSquareMatrixException();
	assert(n == x.size() && n == B.size());

	Matrix Id(n,n);
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++) {
			Id[i][j]   = i==j;
		}

	IntervalMatrix radA(A-Id);
	Matrix InfA(Id-abs(radA.lb()));
	Matrix M(n,n);

	real_inverse(InfA, M);  // may throw SingularMatrixException...

	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			if (! (M[i][j]>=0.0)) throw NotInversePositiveMatrixException();

	Vector b(B.mid());
	Vector delta = (B.ub())-b;

	Vector xstar = M * (abs(b)+delta);

	double xtildek, xutildek, nuk, max, min;

	for (int k=0; k<n; k++) {
		xtildek = (b[k]>=0) ? xstar[k] : xstar[k] + 2*M[k][k]*b[k];
		xutildek = (b[k]<=0) ? -xstar[k] : -xstar[k] + 2*M[k][k]*b[k];

		nuk = 1/(2*M[k][k]-1);
		max = nuk*xutildek;
		if (max < 0) max = 0;

		min = nuk*xtildek;
		if (min > 0) min = 0;

		/* compute bounds of x(k) */
		if (xtildek >= max) {
			if (xutildek <= min) x[k] = Interval(xutildek,xtildek);
			else x[k] = Interval(max,xtildek);
		} else {
			if (xutildek <= min) x[k] = Interval(xutildek,min);
			else { x.set_empty(); return; }
		}
	}
}

} // end namespace

