//============================================================================
//                                  I B E X
// File        : doc-separator.cpp
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

  /**
   * Function that will be called automatically on every boxes (leaves) of the set.
   */
  void visit_leaf(const IntervalVector& box, BoolInterval status) {

    // we skip unbounded OUT boxes (to obtain a bounded frame)
    if (box.mag().max()<1e8) {

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
//! [set-visit-3-C]


int main() {

  output.open ("doc-separator.txt");

  output << "================= this file is generated ==============" << endl;

  {
  //! [set-init-C]
  Set set1(2); // create the two-dimensional set (-oo,+oo)x(-oo,+oo)

  IntervalVector box(2,Interval(0,1));
  Set set2(box); // create the set [0,1]x[0,1]

  //! [set-init-C]
  }

  {
  IntervalVector box(2,Interval(-10,10));
  Function fx("x","y","(x^2-[64,81],y^2-[64,81])");
  SepFwdBwd sepx(fx,LEQ);
  //SepFwdBwd sepy(fy,LEQ);
  Set set(box);
  set.inter(sepx,8.0);
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

  ToVibes to_vibes;
  set.visit(to_vibes);
  vibes::endDrawing();
  //! [set-visit-4-C]
  }

  {
  vibes::beginDrawing ();
  vibes::newFigure("set-sep");

  //! [set-sep-C]

  Set set(2); // create R²

  // Create the function f(x)=||x||²-25
  Function f("x","y","x^2+y^2-25");

  // Creates the separator for the constraint f(x)<=0
  SepFwdBwd sep(f,LEQ);

  // Intersect the set with {x, f(x)<=0}:
  //
  // The separator is applied until leaves
  // that belong to the boundary have a
  // maximal diameter less than 0.01
  set.inter(sep,0.01);

  //! [set-sep-C]

  ToVibes to_vibes;
    set.visit(to_vibes);

    vibes::endDrawing();
  }

  {
  vibes::beginDrawing ();
  vibes::newFigure("set-inter");

  //! [set-inter-C]

  // Create a first set from a separator
  Set set(2);
  Function f("x","y","x^2+y^2-25");
  SepFwdBwd sep(f,LEQ);
  set.inter(sep,0.01);

  // Create a second set from a separator
  Set set2(2);
  Function f2("x","y","x^2+y^2-16");
  SepFwdBwd sep2(f2,GEQ);
  set2.inter(sep2,0.01);

  // Intersect the first set with the second one
  set &= set2;

  //! [set-inter-C]

  ToVibes to_vibes;
  set.visit(to_vibes);
  vibes::endDrawing();
  }

  {
   vibes::beginDrawing ();
   vibes::newFigure("set-union");

   //! [set-union-C]

   // Create a first set from a separator
   Set set(IntervalVector(2,Interval(-10,10)));
   Function f("x","y","x^2+y^2-25");
   SepFwdBwd sep(f,LEQ);
   set.inter(sep,0.01);

   // Create a second set from a separator
   Set set2(2); //IntervalVector(2,Interval(-10,10)));
   Function f2("x","y","(x-5)^2+y^2-25");
   SepFwdBwd sep2(f2,LEQ);
   set2.inter(sep2,0.01);

   // Make the union
   set |= set2;

   //! [set-union-C]

   ToVibes to_vibes;
   set.visit(to_vibes);
   vibes::endDrawing();
   }


  {
  output << "! [sep-ctc-pair-O]" << endl;
  //! [sep-ctc-pair-C]

  //! [sep-ctc-pair-C]
  output << "! [sep-ctc-pair-O]" << endl;
  }

  {
  output << "! [sep-fwd-bwd-O]" << endl;
  //! [sep-fwd-bwd-C]

  // Define the function
  Function f("x", "y", "x - y");

  // Build the separator for the set X = {x, y | x - y < 0}
  SepFwdBwd s(f, LT);

  // Note: in a similar way, it is possible to define the separator
  // for the complementary set  :  X = {x, y | x - y >= 0}
  //SepFwdBwd s_compl(f, GEQ);

  double _box[2][2] = {{0,3},{1,2}};
  IntervalVector box(2,_box);
  IntervalVector x_in=box;
  IntervalVector x_out=box;

  s.separate(x_in,x_out);

  output << "result of inner contraction=" << x_in << endl;
  output << "result of outer contraction=" << x_out << endl;

  //! [sep-fwd-bwd-C]
  output << "! [sep-fwd-bwd-O]" << endl;
  }

  {
  output << "! [sep-boundary-O]" << endl;
  //! [sep-boundary-C]

  // Define the function
  Function f("x", "y", "x - y");

  // Build a contractor for the boundary
  CtcFwdBwd c(f,EQ);

  // Build a predicate for the inside
  PdcFwdBwd p(f,LEQ);

  // Build the separator for the set X = {x, y | x - y < 0}
  SepBoundaryCtc s(c,p);

  double _box[2][2] = {{0,3},{1,2}};
  IntervalVector box(2,_box);
  IntervalVector x_in=box;
  IntervalVector x_out=box;

  s.separate(x_in,x_out);

  output << "result of inner contraction=" << x_in << endl;
  output << "result of outer contraction=" << x_out << endl;

  //! [sep-boundary-C]
  output << "! [sep-boundary-O]" << endl;
  }

}
