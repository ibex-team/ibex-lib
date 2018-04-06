//============================================================================
//                                  I B E X                                   
// File        : lab1.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 30, 2015
//============================================================================

#include "ibex.h"
#include "vibes.cpp"

using namespace std;
using namespace ibex;

int n=500;

int main() {
	vibes::beginDrawing ();
	vibes::newFigure("lab1");

	Variable x,y;
	Function f(x,y,Return(sin(x+y),cos(x+0.9*y)));

	IntervalVector box(2,Interval(0,6));

	// size of the "slice" on each dimension (x and y)
	double deltax=box[0].diam()/n;
	double deltay=box[1].diam()/n;

	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++) {
			IntervalVector box2(2);
			box2[0]=Interval(box[0].lb()+i*deltax,
					box[0].lb()+(i+1)*deltax);

			box2[1]=Interval(box[1].lb()+j*deltay,
					box[1].lb()+(j+1)*deltay);
			IntervalVector y=f.eval_vector(box2);
			// "b[b]" means that both the border and the interior of the box
			// is painted in blue
			vibes::drawBox(y[0].lb(), y[0].ub(), y[1].lb(), y[1].ub(), "b[b]");
		}

	vibes::endDrawing();
}

