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
    if (box.mag().max()<=2) {

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

const int n=10;

double t[n] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

double _y[n][2] = {{4.5,7.5}, {0.67,4.6}, {-1,2.8}, {-1.7,1.7}, {-1.9,0.93}, {-1.8,0.5}, {-1.6,0.24}, {-1.4,0.09}, {-1.2,0.0089}, {-1,-0.031}};

/*
Function (p1,p2,)

class CtcOut : public Ctc {

	public:

	CtcOut()

	void contract(IntervalVector& box) {
		IntervalVector box2(3);
		for (int i=0; i<n; i++) {
			box2.put(0,box);
			box2[2]=t[i];

		}

	}

	CtcCompo compo;
}; */


int main() {
	vibes::beginDrawing ();
	vibes::newFigure("lab5");

	double eps=0.001;

	Array<const ExprNode> y(n);

	Variable p1,p2;

	Function f("p1","p2","t","20*exp(-p1*t)-8*exp(-p2*t)");

	for (int i=0; i<n; i++) {
		y.set_ref(i, f(p1,p2,Interval(t[i])));
	}

	Function fn(p1,p2,ExprVector::new_(y,false));

	cout << fn << endl;

	SepFwdBwd sep(fn,IntervalVector(n,_y)); //(0,2));

	// Build the initial box
	IntervalVector box(2);
	box[0]=Interval(0,1);
	box[1]=Interval(0,1);

	Set set(box);

	sep.contract(set,eps);

	ToVibes to_vibes;
	set.visit(to_vibes);


	vibes::endDrawing();

}


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
 *
 */
