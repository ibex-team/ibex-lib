//============================================================================
//                                  I B E X                                   
// File        : lab5.cpp
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
	vibes::newFigure("lab5");

	double eps=0.001;

	Function f("p1","p2","t","20*exp(-p1*t)-8*exp(-p2*t)");

	const int n=10;

	double _t[n][2] = {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}, {7,7}, {8,8}, {9,9}, {10,10}};
	double _y[n][2] = {{4.5,7.5}, {0.67,4.6}, {-1,2.8}, {-1.7,1.7}, {-1.9,0.93}, {-1.8,0.5}, {-1.6,0.24}, {-1.4,0.09}, {-1.2,0.0089}, {-1,-0.031}};

	IntervalVector t(n,_t);
	IntervalVector y(n,_y);


	// =============== 1st variant: building a function f:R²->R^n ====================
//	Variable p1,p2;
//
//	Array<const ExprNode> y(n);
//
//	for (int i=0; i<n; i++)
//		y.set_ref(i, f(p1,p2,Interval(t[i])));
//
//	Function fn(p1,p2,ExprVector::new_(y,ExprVector::COL));
//
//	SepFwdBwd sep(fn,y);
	// ==============================================================================


	// =============== 2nd variant: with constraints ====================
	Variable p1,p2;
	SystemFactory fac;
	fac.add_var(p1);
	fac.add_var(p2);
	for (int i=0; i<n; i++)
		fac.add_ctr(f(p1,p2,t[i])=y[i]);

	// add bound constraints
	fac.add_ctr(p1>=0);
	fac.add_ctr(p1<=1);
	fac.add_ctr(p2>=0);
	fac.add_ctr(p2<=1);

	System sys(fac);

	Set set(sys,0.001);

	ToVibes to_vibes(1);
	set.visit(to_vibes);

	vibes::endDrawing();

}


// the following piece of code is how we have generated data...
/*
	double p1=0.6;
	double p2=0.2;
	int n=10;
	for (double t=1; t<=n; t++) {
		p1=0.4; p2=0.2;
		Interval x=20*::exp(-p1*t)-8*::exp(-p2*t);
		p1=0.6;
		x |= 20*::exp(-p1*t)-8*::exp(-p2*t);
		p2=0.3;
		x |= 20*::exp(-p1*t)-8*::exp(-p2*t);
		p1=0.4;
		x |= 20*::exp(-p1*t)-8*::exp(-p2*t);
		cout << setprecision(2) << "{" << x.lb() << "," << x.ub() << "}, ";
	}
	cout << endl;
 */
