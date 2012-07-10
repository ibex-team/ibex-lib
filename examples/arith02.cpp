//============================================================================
//                                  I B E X                                   
// File        : arith01.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 3, 2012
// Last Update : Jun 3, 2012
//============================================================================


#include "ibex.h"

using namespace ibex;

int main() {
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
		Interval x(1,2);
		Interval y(3,4);
		Interval z=x+y;
		cout << "x before =" << x << endl;
		cout << "y before =" << y << endl;
		cout << "z before =" << z << endl << endl;
		proj_sin(-1.0,z);
		cout << "z after =" << z << endl;
		proj_add(z,x,y);
		cout << "x after =" << x << endl;
		cout << "y after =" << y << endl;
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
		cout << "x before =" << x << endl;
		cout << "y before =" << y << endl;
		cout << "z before =" << z << endl << endl;
		proj_sin(1.0,z);
		cout << "z after =" << z << endl;
		proj_add(z,x,y);
		cout << "x after =" << x << endl;
		cout << "y after =" << y << endl;
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
		cout << "x before=" << x << endl;
		cout << "M before=" << M << endl << endl;
		cout << "M*x=" << M*x << endl << endl;

		M[2][2]=Interval(0.5,1.1);
		IntervalVector y=Vector::ones(3);
		cout << "M modified=" << M << endl << endl;

		proj_mul(y,M,x,1e-04);
		cout << "x after=" << x << endl;
		cout << "M after=" << M << endl << endl;
		// ------------------------------------------------
	}

	return 0;
}

