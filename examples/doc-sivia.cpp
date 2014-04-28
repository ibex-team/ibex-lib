/*
 * sivia.cpp
 *
 *  Created on: Mar 31, 2014
 *  Author: Gilles Chabert
 */
#include "ibex.h"

using namespace std;
using namespace ibex;

int main() {
	// Create the function we want to apply SIVIA on.
	Variable x,y;
	Function f(x,y,sin(x+y)-0.1*x*y);

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

	// Create the precision contractor
	// Boxes of size less than eps=1e-2 are contracted
	PdcDiameterLT prec(0.1);
	CtcEmpty boundary(prec);

	// Push all the contractors in an array
	Array<Ctc> ctc(inside,outside,boundary);

	// Build the way boxes will be bisected.
	// "LargestFirst" means that the dimension bisected
	// is always the largest one.
	LargestFirst lf(0.1);

	// Create a buffer
	CellStack stack;

	// Create the paver
	Paver p(ctc, lf, stack);

	// Discart log messages
	p.trace = 0;

	// Set the timeout to 10s
	p.timeout = 10;

	// Desactivate fine-grained propagation
	// to avoid a very large number of (thin) boxes
	// in the resulting paving
	p.ctc_loop = false;

	// Build the initial box
	IntervalVector box(2);
	box[0]=Interval(-10,10);
	box[1]=Interval(-10,10);

	// Run the paver
	SubPaving* paving=p.pave(box);

	// Display the paving
	// each box ([lx,ux],[ly,uy]) contracted by contractor n°i is displayed like this:
	// lx ux ly uy i
	for (int i=0; i<3; i++) {
		for (std::vector<std::pair<IntervalVector,IntervalVector> >::const_iterator it=paving[i].traces.begin(); it!=paving[i].traces.end(); it++) {
			IntervalVector *boxes;
			int size=it->first.diff(it->second,boxes);
			for (int j=0; j<size; j++) {
				cout << boxes[j][0].lb() << " " << boxes[j][0].ub() << " " <<
						boxes[j][1].lb() << " " << boxes[j][1].ub() << " " << i << endl;
			}
		}
	}


}



