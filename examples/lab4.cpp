//============================================================================
//                                  I B E X                                   
// File        : lab4.cpp
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

  /**
   * Function that will be called automatically on every boxes (leaves) of the set.
   */
  void visit_leaf(const IntervalVector& box, BoolInterval status) {

    // we skip unbounded OUT boxes (to obtain a bounded frame)
    if (box.mag().max()<=10) {

      //  Associate a color to the box.
      //  - YES (means "inside") is in green
      //  - NO (means "outside") is in red
      //  - MAYBE (means "boundary") is in blue.
      const char* color;

      switch (status) {
        case YES:    color="g"; break;
        case NO:     color="r"; break;
        case MAYBE : color="b"; break;
      }

      // Plot the box with Vibes
      vibes::drawBox(box[0].lb(), box[0].ub(), box[1].lb(), box[1].ub(), color);
    }
  }
};

int main() {
	vibes::beginDrawing ();
	vibes::newFigure("lab4");

	// Create the constraint
	Variable x,y;
	Function f(x,y,sin(x+y)-0.1*x*y);
	SepFwdBwd sep(f,Interval(0,2));

	// Build the initial box
	IntervalVector box(2);
	box[0]=Interval(-10,10);
	box[1]=Interval(-10,10);

	Set set(box);
	sep.contract(set,0.1);

	ToVibes to_vibes;
	set.visit(to_vibes);

	vibes::endDrawing();
}
