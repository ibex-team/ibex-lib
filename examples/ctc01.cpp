//============================================================================
//                                  I B E X                                   
// File        : ctc01.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 3, 2012
// Last Update : Jun 3, 2012
//============================================================================


#include "ibex.h"

using namespace ibex;

int main() {
	Variable x("x"),y("y");
	double d=0.5*sqrt(2);
	Function f(x,y,VectorImage(sqrt(sqr(x)+sqr(y))-d, sqrt(sqr(x-1.0)+sqr(y-1.0))-d));
	cout << f << endl;

	double init_box[][2] = { {0,1},{0,1} };
		IntervalVector box(2,init_box);

	IntervalMatrix M(2,2);
	f.proj(Vector::zeros(2),box);
	f.jacobian(box,M);

	Newton

	cout << M << endl;
}


