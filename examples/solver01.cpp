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

	Variable x,y;
	double d=1.0;
	Function f(x,y,Return(sqrt(sqr(x)+sqr(y))-d, sqrt(sqr(x-1.0)+sqr(y-1.0))-d));

	double init_box[][2] = { {-10,10},{-10,10} };
	IntervalVector box(2,init_box);

	CtcProj c(f);
	CtcNewton newton(f);
	RoundRobin rr;
	CellStack buff;

	Solver s(c,rr,buff,1e-07);

	vector<IntervalVector> sols=s.solve(box);

	for (int i=0; i<sols.size(); i++)
		cout << "solution n°" << i << "=\t" << sols[i] << endl;

	cout << "number of cells=" << s.nb_cells << endl;

}


