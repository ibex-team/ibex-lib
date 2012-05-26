//============================================================================
//                                  I B E X                                   
// File        : basic01.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 26, 2012
// Last Update : May 26, 2012
//============================================================================
#include "ibex.h"

using namespace ibex;

int main() {
	Variable x,y;
	Function f(x,y,x+y);

	double _box[][2]= {{1,2},{3,4}};
	IntervalVector box(2,_box);

	cout << f.eval_scalar(box) << endl;

	return 0;
}



