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

using namespace std;
using namespace ibex;

int main() {

	{
//	Variable x("x"),y("y");
//	double d=0.5*sqrt(2);
//	Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-d, sqrt(sqr(x-1.0)+sqr(y-1.0))-d));
//	cout << f << endl;
//
//	double init_box[][2] = { {-10,10},{-10,10} };
//	IntervalVector box(2,init_box);
//
//	//f.proj(Vector::zeros(2),box);
//	//cout << "box after proj=" << box << endl;
//
//	CtcProj c(f);
//	c.contract(box);
//	cout << "box after proj=" << box << endl; // why different from symb01???
//
//	CtcFixPoint fp(c,1e-03);
//
//	fp.contract(box);
//	cout << "box after fixpoint=" << box << endl; // why different from symb01???
	}

	{
		Variable x("x"),y("y");
		double d=1.0;
		Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-d, sqrt(sqr(x-1.0)+sqr(y-1.0))-d));
		cout << f << endl;

		double init_box[][2] = { {0.9,1.1},{-0.1,0.1} };
		IntervalVector box(2,init_box);

		CtcProj c(f);
		c.contract(box);
		cout << "box after proj=" << box << endl; // why different from symb01???

		CtcNewton newton(f);
		newton.contract(box);
		cout << "box after newton=" << box << endl; // why different from symb01???

	}
}


