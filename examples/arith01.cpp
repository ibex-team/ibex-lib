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
		Interval x(1,2);
		Interval y(3,4);
		cout << "x+y=" << x+y << endl;
		cout << "sin(x+1)=" << sin(x+1) << endl;
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
		double _x[][2]={{0,1},{0,1},{0,1}};
		IntervalVector x(3,_x);

		double _M[][2]={{0,1},{0,1},{0,1},
				{0,2},{0,2},{0,2}};
		IntervalMatrix M(3,3,_M);

		cout << "M*x=" << M*x << endl;
		cout << "M'*x=" << M.transpose()*x << endl;
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
		double _M[][2]={{0,1},{0,1},{0,1},
				{0,2},{0,2},{0,2}};
		IntervalMatrix M(3,3,_M);
		Matrix m2=M.mag();
		Vector x2=x.mid();
		cout << m2*x2 << endl;
		// ------------------------------------------------
	}

	return 0;
}

