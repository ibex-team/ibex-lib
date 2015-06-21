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

const int n=10;

//! [lab8-data]
// positions of the robot
double x_rob[n]={2, 1.61803, 0.618034, -0.618034, -1.61803, -2, -1.61803, -0.618034, 0.618034, 1.61803};
double y_rob[n]={0, 1.17557, 1.90211, 1.90211, 1.17557, 0, -1.17557, -1.90211, -1.90211, -1.17557};
// positions of the obstacle point
double x_obs[n]={0.9, 0.728115, 0.278115, -0.278115, -0.728115, -0.9, -0.728115, -0.278115, 0.278115, 0.728115};
double y_obs[n]={0, 0.529007, 0.855951, 0.855951, 0.529007, 0, -0.529007, -0.855951, -0.855951, -0.529007};
//! [lab8-data]

int main() {

	vibes::beginDrawing ();
	vibes::newFigure("lab8");

	double eps=0.01;
	double L=2;

	Variable x,y;
	Function dist(x,y,sqr(x)+sqr(y));

	// Build the initial box
	IntervalVector box(2,Interval(-L,L));

	// Create the initial i-set [emptyset,[box]]
	SetInterval set(box,MAYBE);

	for (int i=0; i<n; i++) {
		NumConstraint ctr1(x,y,sqr(x-x_rob[i])+sqr(y-y_rob[i])>=0.81);
		SepFwdBwd sep1(ctr1);
		sep1.contract(set,eps,MAYBE,NO);

		NumConstraint ctr2(x,y,sqr(x-x_obs[i])+sqr(y-y_obs[i])<=0.01);
		SepFwdBwd sep2(ctr2);
		sep2.contract(set,eps,YES,MAYBE);
	}

	ToVibes to_vibes(3);
	set.visit(to_vibes);

	vibes::endDrawing();
}



// the following piece of code is how we have generated data...

//double pi=Interval::PI.lb();
//cout << "double x_rob[n]={";
//for (int i=0; i<n; i++) {
//	cout << 2*cos(i*2*pi/n);
//	if (i<n-1) cout << ", ";
//}
//cout << "};\n";
//cout << "double y_rob[n]={";
//for (int i=0; i<n; i++) {
//	cout << 2*sin(i*2*pi/n);
//	if (i<n-1) cout << ", ";
//}
//cout << "};\n";
//cout << "double x_obs[n]={";
//for (int i=0; i<n; i++) {
//	cout << 0.9*cos(i*2*pi/n);
//	if (i<n-1) cout << ", ";
//}
//cout << "};\n";
//cout << "double y_obs[n]={";
//for (int i=0; i<n; i++) {
//	cout << 0.9*sin(i*2*pi/n);
//	if (i<n-1) cout << ", ";
//}
//cout << "};\n";
