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

using namespace std;
using namespace ibex;

int main() {
	{
		// Example #1
		// ------------------------------------------------
		// Basic interval arithmetic
		// > create the inteval x=[1,2] and y=[3,4]
		// > calculate the interval sum x+y
		// > calculate interval image of sin(x+1)
		// ------------------------------------------------
		cout << "-------------------------" << endl;
		Interval x(1,2);
		Interval y(3,4);
		cout << "x = " << x << "; y = " << y << endl;
		cout << "x+y=" << x+y << endl;
		cout << "sin(x+1)=" << sin(x+1) << endl;
		cout << "-------------------------" << endl;
		// ------------------------------------------------
	}

	{

                // Example #2
		// ------------------------------------------------
		// Vector/matrix interval arithmetic
		// > create an interval vector x
		// > create an interval matrix M
		// > calculate M*x
		// > calculate M'*x, where M' is the transpose of M
		// ------------------------------------------------
		double _x[3][2]={{0,1},{2,3},{4,5}};
		IntervalVector x(3,_x);

		double _M[9][2]={{0,1},{0,1},{0,1}, // 3*3 matrix of intervals 
				 {0,2},{0,2},{0,2},
				 {0,1},{0,1},{0,1}};

		IntervalMatrix M(3,3,_M);

                cout << "M=" << M << endl;
                cout << "x=" << x << endl;

		cout << "M*x=" << M*x << endl;
		cout << "transpose(M)*x=" << M.transpose()*x << endl;
		cout << "-------------------------" << endl;
		// ------------------------------------------------
	}

	{
		// Example #3
		// ------------------------------------------------
		// Mixing real/interval vector/matrices
		// > calculate the magnitude of an interval matrix (a real matrix)
		// > calculate the midvector of an interval vector (a real vector)
		// > multiply the latters (floating point arithmetic)
		// ------------------------------------------------
	        double _x[][2]={{0,1},{0,1},{0,1}};
		IntervalVector x(3,_x);

		double _M[9][2]={{0,1},{0,1},{0,1},
				{0,2},{0,2},{0,2},
				{0,1},{0,1},{0,1}};
		IntervalMatrix M(3,3,_M);

		Matrix m2=M.mag();
		Vector x2=x.mid();
                cout << "M2 = Magnitude of M = " << m2 << endl;
                cout << "x2 = Middle of x = " << x2 << endl;
		cout << "M2*x2 = " << m2*x2 << endl;
		cout << "-------------------------" << endl;
		// ------------------------------------------------
	}

	{

	IntervalVector x(3,Interval(1,2));
	IntervalVector y=IntervalVector::empty(3);
	}

	return 0;
}

