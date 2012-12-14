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
		// Example #13
		// ------------------------------------------------
		// Contractor
		//
		// > Create the function (x,y)->( ||(x,y)||-d,  ||(x,y)-(1,1)||-d )
		// > Create the contractor "projection of f=0"
		//   (i.e., contraction of the input box w.r.t. f=0)
		// > Embed this contractor in a generic fix-point loop
		// ------------------------------------------------

		Variable x("x"),y("y");
		double d=0.5*sqrt(2);
		Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-d, sqrt(sqr(x-1.0)+sqr(y-1.0))-d));
		cout << f << endl;

		double init_box[][2] = { {-10,10},{-10,10} };
		IntervalVector box(2,init_box);

		CtcFwdBwd c(f);
		c.contract(box);
		cout << "box after proj=" << box << endl;

		CtcFixPoint fp(c,1e-03);

		fp.contract(box);
		cout << "box after fixpoint=" << box << endl;
	}

	{
		// Example #14
		// ------------------------------------------------
		// Combining FwdBwdection and Newton Contractor
		//
		// > Create the projection contractor on the same function
		//   as in the last example
		// > Contract the initial box x to x'
		// > Create the Newton iteration contractor
		// > Contract the box x'
		// ------------------------------------------------
		Variable x("x"),y("y");
		double d=1.0;
		Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-d, sqrt(sqr(x-1.0)+sqr(y-1.0))-d));
		cout << f << endl;

		double init_box[][2] = { {0.9,1.1},{-0.1,0.1} };
		IntervalVector box(2,init_box);

		CtcFwdBwd c(f);
		c.contract(box);
		cout << "box after proj=" << box << endl;

		CtcNewton newton(f);
		newton.contract(box);
		cout << "box after newton=" << box << endl;

	}
}


