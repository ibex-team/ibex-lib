//============================================================================
//                                  I B E X
// File        : lab2.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : June 08, 2015
//============================================================================
#include "ibex.h"
#include "vibes.cpp"

using namespace std;
using namespace ibex;

void contract_and_draw(Ctc& c, IntervalVector& box, const char* color) {

	// Variables used to calculate the "diff" between 2 boxes and store the result.
	IntervalVector savebox=box;
	IntervalVector *result;

	c.contract(box);

	int n=savebox.diff(box,result);

	for (int i=0; i<n; i++) {
		if (!result[i].is_empty())
		vibes::drawBox(result[i][0].lb(), result[i][0].ub(), result[i][1].lb(), result[i][1].ub(), color);
	}

	delete[] result;
}

int main() {
	vibes::beginDrawing ();
	vibes::newFigure("lab2");

	// Create the function we want to apply SIVIA on.
	Variable x,y;
	Function f(x,y,sin(x+y)-0.1*x*y);

	// Build the initial box
	IntervalVector box(2);
	box[0]=Interval(-10,10);
	box[1]=Interval(-10,10);

	// Create a stack (for depth-first search)
	stack<IntervalVector> s;

	// Precision (boxes of size less than eps are not processed)
	double eps=0.1;

	// Push the initial box in the stack
	s.push(box);

	Interval z=Interval(0,2);

	while (!s.empty()) {
		IntervalVector box=s.top();
		s.pop();

		Interval fxy=f.eval(box);

		if (fxy.is_subset(z))
			vibes::drawBox(box[0].lb(), box[0].ub(), box[1].lb(), box[1].ub(), "k[g]");
		else if (!fxy.intersects(z))
			vibes::drawBox(box[0].lb(), box[0].ub(), box[1].lb(), box[1].ub(), "k[r]");
		else if (box.max_diam()>eps) {
			// get the index of the dimension of maximal size (false stands for "max")
			int i=box.extr_diam_index(false);
			pair<IntervalVector,IntervalVector> p=box.bisect(i);
			s.push(p.first);
			s.push(p.second);
		}
	}

	vibes::endDrawing();
}
