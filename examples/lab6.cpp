//============================================================================
//                                  I B E X                                   
// File        : lab6.cpp
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
  ToVibes(double max) : frame(2,max*Interval(0,1)) {  }

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
	vibes::newFigure("lab6");

	double eps=0.001;

	Function f("p1","p2","t","20*exp(-p1*t)-8*exp(-p2*t)");

	const int n=10;

	double _t[n][2] = {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10}};
	double _y[n][2] = {{4.5,7.5}, {0.67,4.6}, {-1,2.8}, {-1.7,1.7}, {-1.9,0.93}, {-1.8,0.5}, {-1.6,0.24}, {-1.4,0.09}, {-1.2,0.0089}, {-1,-0.031}};

	IntervalVector t(n,_t);
	IntervalVector y(n,_y);

	double tdelta=0.3;
	t.inflate(tdelta);

	// Used to represent which variables are "quantified"
	BitSet vars=BitSet::empty(3);
	// add "p1" as variable
	vars.add(0);
	// add "p2" as variable
	vars.add(1);

	Array<Ctc> _c_out(n);
	Array<Ctc> _c_in(n);

	for (int i=0; i<n; i++) {
		IntervalVector ti(1,t[i]);
		_c_out.set_ref(i,*new CtcForAll(*new CtcFwdBwd(f,y[i]),vars,ti,tdelta/5));
		_c_in.set_ref(i,*new CtcExist(*new CtcNotIn(f,y[i]),vars,ti,tdelta/5));
	}

	CtcCompo c_out(_c_out);
	CtcUnion c_in(_c_in);

	SepCtcPair sep(c_in,c_out);

	Set set(IntervalVector(2,Interval(0,1)));

	sep.contract(set,eps);

	ToVibes to_vibes(1);
	set.visit(to_vibes);

	vibes::endDrawing();


}
