//============================================================================
//                  I B E X
// File    : lab4.cpp
// Author    : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License   : See the LICENSE file
// Created   : Jun 8, 2015
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
	vibes::newFigure("lab4");


    //! [doc-lab-4-C]

	Function f("x","y","sin(x+y)-0.1*x*y");

	// Create a separator for 0<=f(x,y)<=2
	SepFwdBwd sep(f,Interval(0,2));

	// Build the initial set [-10,10]x[-10,10]
	Set set(IntervalVector(2,Interval(-10,10)));

	// Contract the set with the separator
	sep.contract(set,0.1);

	ToVibes to_vibes(10);
	set.visit(to_vibes);

	//! [doc-lab-4-C]

	vibes::endDrawing();
}
