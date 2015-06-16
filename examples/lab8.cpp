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
	vibes::newFigure("lab5");

	double eps=0.001;

	// Create the function corresponding to an
	// hyperplane of angle alpha
	Variable x,y,alpha;
	Function f(x,y,alpha,sqr(x-cos(alpha))+sqr(y-sin(alpha)));

	// Build the initial box
	IntervalVector box(2);
	box[0]=Interval(-2,2);
	box[1]=Interval(-2,2);

	Set set(box,MAYBE);

	int n=8;

	NumConstraint ctr(x,y,sqr(x)+sqr(y)<=4);
	SepFwdBwd sep(ctr);
	sep.status1=MAYBE;
	sep.contract(set,eps);

	for (int i=0; i<n; i++) {
		NumConstraint ctr(x,y,f(x,y,i*2*Interval::PI/n)<=0.04);
		SepFwdBwd sep(ctr);
		sep.status2=MAYBE;
		sep.contract(set,eps);
	}

	ToVibes to_vibes(2);
	set.visit(to_vibes);

	vibes::endDrawing();

}
