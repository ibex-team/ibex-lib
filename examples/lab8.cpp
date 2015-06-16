//============================================================================
//                                  I B E X                                   
// File        : lab8.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jun 8, 2015
//============================================================================

#include "ibex.h"
#include "vibes.cpp"

using namespace std;
using namespace ibex;

class ToVibes : public SetVisitor {

public:

  /**
   * Plot a  box within the frame [-max,max]x[-max,max]
   *
   * The frame avoids, in particular, to plot unbounded OUT boxes.
   */
  ToVibes(double max) : frame(2,max*Interval(-1,1)) {  }

  /**
   * Function that will be called automatically on every boxes (leaves) of the set.
   */
  void visit_leaf(const IntervalVector& box, BoolInterval status) {

    // Intersect the box with the frame
    IntervalVector framebox=box & frame;

    //  Associate a color to the box.
    //  - YES (means "inside") is in green
    //  - NO (means "outside") is in red
    //  - MAYBE (means "boundary") is in blue.
    const char* color;

    switch (status) {
    case YES:  color="g"; break;
    case NO:   color="r"; break;
    case MAYBE : color="b"; break;
    }

    // Plot the box with Vibes
    vibes::drawBox(framebox[0].lb(), framebox[0].ub(), framebox[1].lb(), framebox[1].ub(), color);
  }

   IntervalVector frame;
};

int main() {

	vibes::beginDrawing ();
	vibes::newFigure("lab8");

	double eps=0.001;
	int n=10;
	double L=2;

	Variable x,y;
	Function dist(x,y,sqr(x)+sqr(y));

	// Build the initial box
	IntervalVector box(2);
	box[0]=Interval(-L,L);
	box[1]=Interval(-L,L);

	// Create the initial i-set [emptyset,[box]]
	SetInterval set(box,MAYBE);

	double pi=Interval::PI.lb();
	for (int i=0; i<n; i++) {
		double x0=cos(i*2*pi/n);
		double y0=sin(i*2*pi/n);
		NumConstraint ctr1(x,y,sqr(x-2*x0)+sqr(y-2*y0)>=0.9);
		SepFwdBwd sep1(ctr1);
		sep1.contract(set,eps,MAYBE,NO);

		NumConstraint ctr2(x,y,sqr(x-0.9*x0)+sqr(y-0.9*y0)<=0.01);
		SepFwdBwd sep2(ctr2);
		sep2.contract(set,eps,YES,MAYBE);
		//cout << x << " " << y << endl;
	}

	ToVibes to_vibes(3);
	set.visit(to_vibes);

	vibes::endDrawing();
}
