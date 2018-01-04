//============================================================================
//                                  I B E X                                   
// File        : TestLinear.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Oct 03, 2013
// Last Update : Dec 28, 2017
//============================================================================

#include "TestLinear.h"
#include "ibex_Linear.h"

using namespace std;

namespace ibex {

void TestLinear::lu_partial_underctr() {
	double _M[3*4]={1,-2,3,4,
				    2,-3,4,5,
				    3,4,-5,6};
	double _expected[]={
			0.3333, 0.5882, 0.3529, 1.4118,
			0.6667, -5.6667, 7.3333, 1.0000,
			3.0000, 4.0000, -5.0000, 6.0000
	};

	Matrix M(3,4,_M);
	Matrix LU_actual(3,4);
	Matrix LU_expected(3,4,_expected);
	int p[3];
	real_LU(M,LU_actual,p);
	CPPUNIT_ASSERT(p[0]==2);
	CPPUNIT_ASSERT(p[1]==1);
	CPPUNIT_ASSERT(p[2]==0);
	// note: 1e-04 corresponds to matlab's display precision
	CPPUNIT_ASSERT(almost_eq(LU_actual.row(0),LU_expected.row(0),1e-04));
	CPPUNIT_ASSERT(almost_eq(LU_actual.row(1),LU_expected.row(1),1e-04));
	CPPUNIT_ASSERT(almost_eq(LU_actual.row(2),LU_expected.row(2),1e-04));
}

void TestLinear::lu_complete_underctr() {

}

void TestLinear::gauss_seidel01() {
	int n=4;
	double eps=0.01;

	double _A[] = {
			0,0,1,0,
		    0,1,0,0,
			1,0,0,0,
			0,0,0,1
		};
	IntervalMatrix A=Matrix(n,n,_A)+Interval(-eps,eps)*Matrix::ones(n);
	Vector b=Vector::ones(n);
	IntervalVector x(n,Interval(-10,10));
	gauss_seidel(A,b,x,0.001);

	// the domain [l,u] of x2 and x4 satisfies the fixpoint equation:
	// l = 1/(1+eps)*(1-(n-2)*10*eps-eps*u)
	// u=  1/(1-eps)*(1+(n-2)*10*eps+eps*u)
	// so that
	// u=((1+(n-2)*10*eps)/(1-eps))/(1-(1+eps)/(1-eps))
	// and
	// l is then found with the first equation
	double u=((1+(n-2)*10*eps)/(1-eps)) / (1-(eps/(1-eps)));
	double l=1.0/(1+eps)*(1-(n-2)*10*eps-eps*u);
	Interval fxpt(l,u);

	CPPUNIT_ASSERT(x[0]==Interval(-10,10));
	CPPUNIT_ASSERT(almost_eq(x[1], fxpt, 1e-6));
	CPPUNIT_ASSERT(x[2]==Interval(-10,10));
	CPPUNIT_ASSERT(almost_eq(x[3], fxpt, 1e-6));
}

void TestLinear::gauss_seidel02() {
	int m=4;
	int n=8;
	double eps=0.01;

	double _A[] = {
			0,0,1,0,0,0,0,0,
		    0,1,0,0,0,0,0,0,
			1,0,0,0,0,0,0,0,
			0,0,0,1,0,0,0,0
		};

	IntervalMatrix A=Matrix(m,n,_A)+Interval(-eps,eps)*Matrix::ones(m,n);
	Vector b=Vector::ones(m);
	IntervalVector x(n,Interval(-10,10));

	gauss_seidel(A,b,x,0.001);

	// see comment in gauss_seidel01
	double u=((1+(n-2)*10*eps)/(1-eps)) / (1-(eps/(1-eps)));
	double l=1.0/(1+eps)*(1-(n-2)*10*eps-eps*u);
	Interval fxpt(l,u);

	CPPUNIT_ASSERT(x[0]==Interval(-10,10));
	CPPUNIT_ASSERT(almost_eq(x[1], fxpt, 1e-6));
	CPPUNIT_ASSERT(x[2]==Interval(-10,10));
	CPPUNIT_ASSERT(almost_eq(x[3], fxpt, 1e-6));
}

void TestLinear::gauss_seidel03() {
	int m=8;
	int n=3;
	double eps=0.01;

	double _A[] = {
			0,0,1,
		    1,0,0,
			0,0,1,
			1,0,0,
		    0,0,1,
			0,1,0,
			0,0,1,
		    0,1,0,
		    };

	IntervalMatrix A=Matrix(m,n,_A)+Interval(-eps,eps)*Matrix::ones(m,n);
	Vector b=Vector::ones(m);
	IntervalVector x(n,Interval(-10,10));

	gauss_seidel(A,b,x,0.001);

	// see comment in gauss_seidel01
	double u=(1/(1-eps)) / (1-((n-1)*eps/(1-eps)));
	double l=1.0/(1+eps)*(1-(n-1)*eps*u);
	Interval fxpt(l,u);

	CPPUNIT_ASSERT(almost_eq(x[0],fxpt, 1e-6));
	CPPUNIT_ASSERT(almost_eq(x[1],fxpt, 1e-6));
	CPPUNIT_ASSERT(almost_eq(x[2],fxpt, 1e-6));
}


void TestLinear::inflating_gauss_seidel01() {
	int n=4;
	Matrix A=(n+1)*Matrix::eye(n)-Matrix::ones(n); // diagonally dominant matrix
	Vector b(n);
	for (int i=1; i<=n; i++) b[i-1]=::pow(-1.0,i)*i; // just an arbitrary example

	Matrix invA(n,n);
	real_inverse(A,invA);
	IntervalVector sol=invA*b; // with n=4: x=(0.2000;0.8000;-0.2000;1.2000)

	IntervalVector x=Vector::zeros(n);

	bool ret=inflating_gauss_seidel(A,b,x);
	CPPUNIT_ASSERT(ret);
	CPPUNIT_ASSERT(sol.rel_distance(x)<0.01);
}

void TestLinear::inflating_gauss_seidel02() {
	int n=4;
	Matrix A=(n+1)*Matrix::eye(n)-Matrix::ones(n); // diagonally dominant matrix
	Vector b(n);
	for (int i=1; i<=n; i++) b[i-1]=::pow(-1.0,i)*i; // just an arbitrary example

	Matrix invA(n,n);
	real_inverse(A,invA);
	IntervalVector sol=invA*b; // with n=4: x=(0.2000;0.8000;-0.2000;1.2000)

	IntervalVector x=0.1*Interval(-1,1)*Vector::ones(n);

	bool ret=inflating_gauss_seidel(A,b,x);
	CPPUNIT_ASSERT(ret);
	CPPUNIT_ASSERT(sol.rel_distance(x)<0.01);
}

void TestLinear::inflating_gauss_seidel03() {
	int n=4;
	Matrix A=(n-1)*Matrix::eye(n)-Matrix::ones(n); // diagonally dominant matrix
	Vector b(n);
	for (int i=1; i<=n; i++) b[i-1]=::pow(-1.0,i)*i; // just an arbitrary example

	Matrix invA(n,n);
	real_inverse(A,invA);
	IntervalVector sol=invA*b; // with n=4: x=(0.2000;0.8000;-0.2000;1.2000)

	IntervalVector x=0.1*Interval(-1,1)*Vector::ones(n);

	bool ret=inflating_gauss_seidel(A,b,x);
	CPPUNIT_ASSERT(!ret);
}

void TestLinear::det01() {
    double _tab[] = { 1, 3, 2, 9, 4, 5, 6, 8, 7 };
    Matrix M1(3,3,_tab);
    CPPUNIT_ASSERT(almost_eq(det(M1),-15,ERROR));
}

void TestLinear::det02() {
	CPPUNIT_ASSERT_THROW(det(Matrix(1,2)), NotSquareMatrixException);
}

namespace {

// Return an orthogonal matrix and its inverse
pair<IntervalMatrix,IntervalMatrix> orthogonal_matrix() {

	// Create an orthogonal 3x3 matrix
	// with 3 rotations
	IntervalMatrix R1(3,3,Interval::ZERO);
	R1[0][0]=cos(Interval::PI/4.0);
	R1[0][1]=-sin(Interval::PI/4.0);
	R1[1][0]=sin(Interval::PI/4.0);
	R1[1][1]=cos(Interval::PI/4.0);
	R1[2][2]=Interval::ONE;

	IntervalMatrix R2(3,3,Interval::ZERO);
	R2[1][1]=cos(Interval::PI/3.0);
	R2[1][2]=-sin(Interval::PI/3.0);
	R2[2][1]=sin(Interval::PI/3.0);
	R2[2][2]=cos(Interval::PI/3.0);
	R2[0][0]=Interval::ONE;

	IntervalMatrix R3(3,3,Interval::ZERO);
	R3[2][2]=cos(Interval::PI/5.0);
	R3[2][0]=-sin(Interval::PI/5.0);
	R3[0][2]=sin(Interval::PI/5.0);
	R3[0][0]=cos(Interval::PI/5.0);
	R3[1][1]=Interval::ONE;

	IntervalMatrix R=R1*R2*R3;
	IntervalMatrix Rinv=R3.transpose()*R2.transpose()*R1.transpose();
	return make_pair(R,Rinv);
}

}

void TestLinear::is_posdef_sylvester01() {

	pair<IntervalMatrix,IntervalMatrix> R=orthogonal_matrix();

	double d[9]={2, 0, 0, 0, 1, 0, 0, 0, 3};
	Matrix D(3,3,d);

	CPPUNIT_ASSERT(is_posdef_sylvester(R.first*D*R.second));

	D[1][1]=0.1;
	CPPUNIT_ASSERT(is_posdef_sylvester(R.first*D*R.second));

	D[1][1]=-0.1;
	CPPUNIT_ASSERT(!is_posdef_sylvester(R.first*D*R.second));

}

void TestLinear::is_posdef_rohn01() {

	pair<IntervalMatrix,IntervalMatrix> R=orthogonal_matrix();

	double d[9]={2, 0, 0, 0, 1, 0, 0, 0, 3};
	Matrix D(3,3,d);

	CPPUNIT_ASSERT(is_posdef_rohn(R.first*D*R.second));

	D[1][1]=0.1;
	CPPUNIT_ASSERT(is_posdef_sylvester(R.first*D*R.second));

	D[1][1]=-0.1;
	CPPUNIT_ASSERT(!is_posdef_sylvester(R.first*D*R.second));
}

void TestLinear::is_diagonal_dominant01() {
	int n=10;
	IntervalMatrix M=Matrix::eye(n);
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			if (i<j) M[i][j]=-1.0/((double) n);
			else if (i>j) M[i][j]=1.0/((double) n);
		}
	}
	CPPUNIT_ASSERT(is_diagonal_dominant(M));

	M[n-1][n-2]=Interval(0,1.99/((double) n));
	CPPUNIT_ASSERT(is_diagonal_dominant(M));

	M[n-1][n-2]=Interval(0,2.00/((double) n));
	CPPUNIT_ASSERT(!is_diagonal_dominant(M));
}

} // end namespace ibex
