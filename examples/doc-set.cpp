//============================================================================
//                                  I B E X
// File        : doc-set.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jan 31, 2015
// Last Update : Jan 31, 2015
//============================================================================


#include "ibex.h"
#include "vibes.cpp"

#include <fstream>

using namespace std;
using namespace ibex;

ofstream output;

/**
 * This file contains examples of the documentation.
 */

//! [set-visit-1-C]
class ToConsole : public SetVisitor {
  /**
   * The function that will be called automatically on every boxes (leaves) of the set.
   */
  void visit_leaf(const IntervalVector& box, BoolInterval status) {

    output << box << " : ";

    switch (status) {
      case YES:    output << "in"; break;
      case NO:     output << "out"; break;
      case MAYBE : output << "?"; break;
    }
    output << endl;
  }
};
//! [set-visit-1-C]


//! [set-visit-3-C]
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
//! [set-visit-3-C]


int main() {

  output.open ("doc-set.txt");

  output << "================= this file is generated ==============" << endl;

  {
  //! [set-init-1-C]

  // create the two-dimensional set (-oo,+oo)x(-oo,+oo)
  Set set1(2);

  // create the two-dimensional set [0,1]x[0,1]
  Set set2(IntervalVector(2,Interval(0,1)));

  //! [set-init-1-C]
  }


  {
  vibes::beginDrawing ();
  vibes::newFigure("set-sep");

  //! [set-init-2-C]

  // Create the constraint ||(x,y)||²<=25
  NumConstraint c("x","y","x^2+y^2<=25");

  // Create the set with a precision of 0.01
  Set set(c,0.01);

  //! [set-init-2-C]

  ToVibes to_vibes(5);
  set.visit(to_vibes);

  vibes::endDrawing();
  }

  {
  Function fx("x","y","(x^2-[64,81],y^2-[64,81])");
  SepFwdBwd sepx(fx,LEQ);
  //SepFwdBwd sepy(fy,LEQ);
  Set set(2);
  sepx.contract(set,8.0);
  //set.inter(sepy,1.0);

  //! [set-save-C]
  set.save("set-example");
  //! [set-save-C]
  }

  {
  //! [set-load-C]
  Set set("set-example");
  //! [set-load-C]
  }

  {
  output << "! [set-visit-2-O]" << endl;
  //! [set-visit-2-C]
  Set set("set-example");

  ToConsole to_console;
  set.visit(to_console);
  //! [set-visit-2-C]
  output << "! [set-visit-2-O]" << endl;
  }

  {
  //! [set-visit-4-C]
  vibes::beginDrawing ();
  vibes::newFigure("visit");

  Set set("set-example");

  ToVibes to_vibes(10);
  set.visit(to_vibes);
  vibes::endDrawing();
  //! [set-visit-4-C]
  }

  {
  vibes::beginDrawing ();
  vibes::newFigure("set-inter");

  //! [set-inter-C]

  // Create a first set ||(x,y)||<=5
  NumConstraint c("x","y","x^2+y^2<=25");
  Set set(c,0.01);

  // Create a second set ||(x,y)||>=4
  NumConstraint c2("x","y","x^2+y^2>=16");
  Set set2(c2,0.01);

  // Intersect the first set with the second one
  set &= set2;

  //! [set-inter-C]

  ToVibes to_vibes(5);
  set.visit(to_vibes);
  vibes::endDrawing();
  }

  {
  vibes::beginDrawing ();
  vibes::newFigure("set-union");

  //! [set-union-C]


  // Create a first set ||(x,y)||<=5
  NumConstraint c("x","y","x^2+y^2<=25");
  Set set(c,0.01);

  // Create a second set ||(x-5,y)||<=5
  NumConstraint c2("x","y","(x-5)^2+y^2<=25");
  Set set2(c2,0.01);

  // Make the union
  set |= set2;

  //! [set-union-C]

  ToVibes to_vibes(10);
  set.visit(to_vibes);
  vibes::endDrawing();
  }

  {
  vibes::beginDrawing ();
  vibes::newFigure("set-contract");

  //! [set-contract-1-C]

  double eps=0.01;

  // create the set with one of the constraints
  NumConstraint c("x","y","x^2+y^2<=25");
  Set set(c,eps);

  // create the second constraint
  NumConstraint c2("x","y","x^2+y^2>=16");
  // create a separator for this constraint
  SepFwdBwd sep(c2);
  // contract the set with the separator
  sep.contract(set,eps);

  //! [set-contract-1-C]

  ToVibes to_vibes(10);
  set.visit(to_vibes);
  vibes::endDrawing();
  }

  {
  vibes::beginDrawing ();
  vibes::newFigure("set-interval");

  //! [set-interval-C]

  double eps=0.001;

  // Create the distance function between (x,y)
  // and the point (cos(alpha),sin(alpha))
  Variable x,y,alpha;
  Function f(x,y,alpha,sqr(x-cos(alpha))+sqr(y-sin(alpha)));

  // Build the initial box
  IntervalVector box(2);
  box[0]=Interval(-2,2);
  box[1]=Interval(-2,2);

  // Create the initial i-set [emptyset,[box]]
  SetInterval set(box);

  NumConstraint ctr(x,y,sqr(x)+sqr(y)<=4);
  // Create a separator for the i-set [emptyset,ctr]
  // Initially, the i-set is [ctr,ctr]
  SepFwdBwd sep(ctr);

  // We set the status of the first contraction to
  // "maybe" so that the i-set associated to the separator becomes [emptyset,ctr]
  // We contract the set with the separator, i.e.,
  // we compute [emptyset,[box]] & [emptyset,ctr]
  sep.contract(set,eps,MAYBE,NO);

  // Number of points
  int n=8;

  for (int i=0; i<n; i++) {
    NumConstraint ctr(x,y,f(x,y,i*2*Interval::PI/n)<=0.04);
    SepFwdBwd sep(ctr);
    // We set the status of the second contraction to
    // "maybe" so that the i-set associated to the separator becomes [ctr,R^n]
    sep.contract(set,eps,YES,MAYBE);
  }
  //! [set-interval-C]

  ToVibes to_vibes(2);
  set.visit(to_vibes);
  vibes::drawEllipse(0,0,1.0,1.0,0,"k");
  vibes::endDrawing();
}
}
