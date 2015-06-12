//============================================================================
//                                  I B E X
// File        : lab3.cpp
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
		vibes::drawBox(result[i][0].lb(), result[i][0].ub(), result[i][1].lb(), result[i][1].ub(), color);
	}

	delete[] result;
}

int main() {
	vibes::beginDrawing ();
	vibes::newFigure("lab3");

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

	NumConstraint c1(x,y,f(x,y)<=2);
	NumConstraint c2(x,y,f(x,y)>=0);
	NumConstraint c3(x,y,f(x,y)>2);
	NumConstraint c4(x,y,f(x,y)<0);

	// Create contractors with respect to each
	// of the previous constraints.
	CtcFwdBwd out1(c1);
	CtcFwdBwd out2(c2);
	CtcFwdBwd in1(c3);
	CtcFwdBwd in2(c4);

	// Create a contractor that removes all the points
	// that do not satisfy either f(x,y)<=2 or f(x,y)>=0.
	// These points are "outside" of the solution set.
	CtcCompo outside(out1,out2);

	// Create a contractor that removes all the points
	// that do not satisfy both f(x,y)>2 or f(x,y)<0.
	// These points are "inside" the solution set.
	CtcUnion inside(in1,in2);

	while (!s.empty()) {
		IntervalVector box=s.top();
		s.pop();
		contract_and_draw(outside, box, "k[r]");
		contract_and_draw(inside, box, "k[g]");
		if (!box.is_empty() && box.max_diam()>eps) {
			int i=box.extr_diam_index(false);
			pair<IntervalVector,IntervalVector> p=box.bisect(i);
			s.push(p.first);
			s.push(p.second);
		}
	}

	vibes::endDrawing();
}
