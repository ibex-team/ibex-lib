//============================================================================
//                                  I B E X                                   
// File        : arith01.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 3, 2012
// Last Update : May 22, 2014
//============================================================================


#include "ibex.h"
#include <fstream>

using namespace ibex;
using namespace std;

ofstream output;

//! [itv-func-compl-vec-C]
void print_compl(const IntervalVector& x) {
	IntervalVector* result;
	int n=x.complementary(result);
	output << "complementary of " << x << " = " << endl;
	for (int i=0; i<n; i++) {
	output << "\t" << result[i] << endl;
	}

	delete[] result; // don't forget to free memory!
}
//! [itv-func-compl-vec-C]

int main() {

	output.open ("doc-arithmetic.txt");

	output << "================= this file is generated ==============" << endl;

	{
	//! [itv-build-C]
	Interval x1;                      // (-oo,+oo)
	Interval x2(2);                   // [2,2]
	Interval x3(x2);                  // [2,2]
	Interval x4=x2;                   // [2,2]

	Interval x5(1,2);                 // [1,2]

	Interval x6(1,POS_INFINITY);      // [1,+oo)
	Interval x7(NEG_INFINITY,-1);     // (-oo,-1]
	Interval x8=Interval::EMPTY_SET;  // empty set

	//! [itv-build-C]
	}

	{
	output << "![itv-constants-O]" << endl;
	//! [itv-constants-C]
	output << " EMPTY_SET =\t " <<  Interval::EMPTY_SET << endl;
	output << " PI =\t\t "      <<  Interval::PI <<  endl;
	output << " 2 PI =\t\t "    <<  Interval::TWO_PI << endl;
	output << " 1/2 PI =\t "    <<  Interval::HALF_PI << endl;
	output << " ONE =\t\t "     <<  Interval::ONE << endl;
	output << " ZERO =\t\t "    <<  Interval::ZERO << endl;
	output << " ALL_REALS =\t " <<  Interval::ALL_REALS << endl;
	output << " POS_REALS =\t " <<  Interval::POS_REALS << endl;
	output << " NEG_REALS =\t " <<  Interval::NEG_REALS << endl;
	//! [itv-constants-C]
	output << "![itv-constants-O]" << endl;

	}

	{
	output << "![itv-div2-O]" << endl;
	//! [itv-div2-C]
	Interval intv(-10,10);
	Interval out2;
	bool result=intv.div2_inter(Interval(2,3), Interval(-1,2), out2);
	output << "the intersection is " << (result? "not":"") << " empty" << endl;
	output << "left part=" << intv << " right part=" << out2 << endl;
	//! [itv-div2-C]
	output << "![itv-div2-O]" << endl;
	}

	{
	//! [itv-build-vec-C]
	// creates ([0,1],[2,3],[4,5]) in several steps.
	IntervalVector x1(3); // so far x1 is ((-oo,oo),(-oo,oo),(-oo,oo))
	x1[0]=Interval(0,1);  // we initialize each component
	x1[1]=Interval(2,3);
	x1[2]=Interval(4,5);

	// creates the same vector in two steps only
	double _x2[3][2]={{0,1},{2,3},{4,5}};
	IntervalVector x2(3,_x2);
	//! [itv-build-vec-C]

	}

	{
	//! [itv-build-vec2-C]
	// creates (0.1,0.2,0.3) in several steps.
	Vector x1(3);  // so far x1 is (0,0,0)
	x1[0]=0.1;     // we initialize each component
	x1[1]=0.2;
	x1[2]=0.3;

	// creates the same vector in two steps only
	double _x2[3]={0.1,0.2,0.3};
	Vector x2(3,_x2);
	//! [itv-build-vec2-C]

	}

	{
	//! [itv-build-vec3-C]
	IntervalVector x2(3,Interval(1,2));         // create ([1,2],[1,2],[1,2])
	IntervalVector x3(3,0.1);                   // create ([0.1,0.1],[0.1,0.1],[0.1,0.1])
	Vector         x4(3,0.1);                   // create (0.1,0.1,0.1)

	IntervalVector x5(x2);                      // create a copy of x2
	Vector x6(x4);                              // create a copy of x4
	//! [itv-build-vec3-C]
	}

	{
	//! [itv-build-vec4-C]
	IntervalVector x1=IntervalVector::empty(3); // create a vector of 3 empty intervals

	Vector x(3,0.1);
	IntervalVector x2(x);                       // create ([0.1,0.1],[0.1,0.1],[0.1,0.1])
	//! [itv-build-vec4-C]
	}

	{
	output << "! [itv-build-mat-O]" << endl;
	//! [itv-build-mat-C]

	// create [(1,2,3);(4,5,6)] in several steps.
	Matrix m1(2,3);   // a 2x3 matrix filled with zeros
	m1[0][0]=1;       // we initialize each component
	m1[0][1]=2;
	m1[0][2]=3;
	m1[1][0]=4;
	m1[1][1]=5;
	m1[1][2]=6;
	output << "m1=" << m1 << endl << endl;

	// create the same matrix in two steps only
	double _m2[2*3]={1,2,3,
			 4,5,6};
	Matrix m2(2,3,_m2);
	output << "m2=" << m2 << endl << endl;

	// create a 2x3 matrix filled with ones
	Matrix m3(2,3,1.0);
	output << "m3=" << m3 << endl << endl;

	// the same matrix using a built-in static function
	Matrix m4=Matrix::ones(2,3);

	// create a copy of m3
	Matrix m5(m3);
	output << "m5=" << m5 << endl;
	//! [itv-build-mat-C]
	output << "! [itv-build-mat-O]" << endl;
	}

	{
	output << "! [itv-build-itv-mat-O]" << endl;
	//! [itv-build-itv-mat-C]
	double eps=1e-02;

	// create in several steps.
	IntervalMatrix m1(3,3);                   // a 3x3 matrix filled with (-oo,oo)
	m1=Matrix::eye(3);                        // set m1 to the identity matrix
	m1+=Interval(-eps,eps)*Matrix::ones(3);   // add [-eps,eps] to each component
	output << "m1=" << m1 << endl << endl;

	// create the same matrix with a matrix of double
	// warning: the matrix of double has a number of rows
	// equal to the total number of components (9) and
	// 2 columns (left bound, right bound).
	double _m2[3*3][2]={{1-eps,1+eps},{-eps,eps},{-eps,eps},
	                   {-eps,eps},{1-eps,1+eps},{-eps,eps},
	                   {-eps,eps},{-eps,eps},{1-eps,1+eps}};

	IntervalMatrix m2(3,3,_m2);
	output << "m2=" << m2 << endl << endl;

	// create a 3x3 matrix filled with [-eps,eps]
	IntervalMatrix m3(3,3,Interval(-eps,eps));
	m3+=Matrix::eye(3);
	output << "m3=" << m3 << endl << endl;

	// create a copy of m3
	IntervalMatrix m4(m3);
	output << "m4=" << m4 << endl;
	//! [itv-build-itv-mat-C]
	output << "! [itv-build-itv-mat-O]" << endl;
	}

	{
	output << "! [itv-compl-O]" << endl;
	//! [itv-compl-C]
	Interval x(0,1);
	Interval c1,c2; // to store the result

	int n=x.complementary(c1,c2);
	output << "complementary of " << x << " = " << c1;
	if (n>1) output << " and " << c2;
	output << endl;
	//! [itv-compl-C]
	output << "! [itv-compl-O]" << endl;
	}


	{
	output << "! [itv-compl-vec-O]" << endl;
	//! [itv-compl-vec-C]
	print_compl(IntervalVector::empty(3));

	print_compl(IntervalVector(3));

	print_compl(IntervalVector(3,Interval(0,1)));
	//! [itv-compl-vec-C]
	output << "! [itv-compl-vec-O]" << endl;
	}

	{
	output << "! [itv-diff-O]" << endl;
	//! [itv-diff-C]

	// set difference between two intervals
	Interval x(0,3);
	Interval y(1,2);
	Interval c1,c2; // to store the result

	int n=x.diff(y,c1,c2);
	output << x << " \\ " << y << " = " << c1;
	if (n>1) output << " and " << c2;
	output << endl;

	//! [itv-diff-C]
	output << "! [itv-diff-O]" << endl;
	}

	{
	output << "! [itv-diff-vec-O]" << endl;
	//! [itv-diff-vec-C]
	// set difference between two boxes
	IntervalVector x(2,Interval(0,3));
	IntervalVector y(2,Interval(1,2));
	IntervalVector* result; // to store the result

	int n=x.diff(y,result);
	output << x << " \\ " << y << " = " << endl;
	for (int i=0; i<n; i++) {
		output << "\t" << result[i] << endl;
	}
	delete[] result; // don't forget to free memory!

	//! [itv-diff-vec-C]
	output << "! [itv-diff-vec-O]" << endl;
	}


	{
	output << "! [itv-bisect-O]" << endl;
	//! [itv-bisect-C]

	IntervalVector x(3,Interval(0,1)); // [0,1]x[0,1]x[0,1]

	std::pair<IntervalVector,IntervalVector> p = x.bisect(1,0.4); // bisect the second component with ratio 0.4

	output << "first box=" << p.first << endl;
	output << "second box=" << p.second << endl;

	//! [itv-bisect-C]
	output << "! [itv-bisect-O]" << endl;
	}

	{
	// Example #4
	// ------------------------------------------------
	// Basic "projection"/"backward arithmetic"
	//
	// (Contraction of x w.r.t. to f(x)=y)
	//
	// > create three intervals x,y and z with z=x+y
	// > project sin(z)=-1 onto z (contracts z)
	// > project x+y onto x and y (contracts x and y)
	// ------------------------------------------------

	Interval c=Interval(1.0);
	Interval x(1,2);
	Interval y(3,4);
	Interval z=x+y;

	output << "x before =" << x << endl;
	output << "y before =" << y << endl;
	output << "z before =" << z << endl << endl;
	bwd_sin(-1.0,z);
	output << "z after =" << z << endl;
	bwd_add(z,x,y);
	output << "x after =" << x << endl;
	output << "y after =" << y << endl;
	// ------------------------------------------------
	}

	{
	// Example #5
	// ------------------------------------------------
	// Example of projection leading to an empty set
	//
	// Same example as Example #3 except that the
	// projection of sin(z)=1 onto z leads to an empty set.
	// ------------------------------------------------
	Interval x(1,2);
	Interval y(3,4);
	Interval z=x+y;
	output << "x before =" << x << endl;
	output << "y before =" << y << endl;
	output << "z before =" << z << endl << endl;
	bwd_sin(1.0,z);
	output << "z after =" << z << endl;
	bwd_add(z,x,y);
	output << "x after =" << x << endl;
	output << "y after =" << y << endl;
	// ------------------------------------------------
	}

	{
	// Example #6
	// ------------------------------------------------
	// Vector/Matrix projection
	// > create a matrix M centered on the identity
	// > create two 1-column vectors x and y
	// > set artificially one entry of M to a large interval
	// > contract M with respect to M*x=y and observe
	//   that the uncertainty on this entry has been reduced
	// ------------------------------------------------
	double _x[][2]={{1,1},{1,1},{1,1}};
	IntervalVector x(3,_x);
	IntervalMatrix M=Matrix::eye(3) + Interval(-0.1,0.1)*Matrix::ones(3);
	output << "x before=" << x << endl;
	output << "M before=" << M << endl << endl;
	output << "M*x=" << M*x << endl << endl;

	M[2][2]=Interval(0.5,1.1);
	IntervalVector y=Vector::ones(3);
	output << "M modified=" << M << endl << endl;

	bwd_mul(y,M,x,1e-04);
	output << "x after=" << x << endl;
	output << "M after=" << M << endl << endl;
	// ------------------------------------------------
	}

	output.close();

	return 0;
}

